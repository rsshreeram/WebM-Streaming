/*
Program to send RTP audio  packets.
Copyright (C) 2010  SHREERAM RAMAMURTHY SWAMINATHAN

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

*/

#include "vpx_network.h"
#include "tctypes.h"
#include "rtp.h"
#include "vorbis.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <time.h>


#define READ                  1024
#define BACKLOG               3
#define FAIL_ON_NONZERO(x) if((x)) { vpxlog_dbg(ERRORS,#x"\n");return -1; };
#define FAIL_ON_ZERO(x) if(!(x)) { vpxlog_dbg(ERRORS,#x"\n");return -1; };
#define FAIL_ON_NEGATIVE(x) if((x)<0) { vpxlog_dbg(ERRORS,#x"\n");return -1; };

typedef unsigned int u_int32;

void packetize(PACKET *packet, char *readbuf);
u_int32 random32(int type);
unsigned int seqno = 0;
unsigned int sourceid;

int main(int argc, char *argv[]) {
  
  char ip[IP_SIZE];
  
  if (argc > 1) {
    strncpy(ip, argv[0], IP_SIZE);
  } else {
    strncpy(ip, "127.0.0.1", IP_SIZE);
  }
  
  
  packetizeandsend(&ip);
  return 0;
}


TCRV packetizeandsend(char *ip) {
  
  //char ip[IP_SIZE];
  //strncpy(ip, "127.0.0.1", IP_SIZE);
  
  struct vpxsocket ogg_sock1, ogg_sock2;
  union vpx_sockaddr_x ogg_address, ogg_address2;
  
  enum network_layer nl = vpx_IPv4;
  enum transport_layer tl = vpx_UDP;
  
  tcu16 default_send_port = DEFAULT_RECV_PORT;
  tcu16 default_recv_port = DEFAULT_SEND_PORT;
  
  int audio_bitrate, sample_rate;

  tc8 one_packet[8000];
  TCRV rc;
  /*
    Initialize the network
  */
  vpx_net_init();
  
  
  /*
    Create socket connections
    Two connections one for sending and one for
    receiving
  */
  FAIL_ON_NONZERO(vpx_net_open(&ogg_sock1, nl, tl))
  FAIL_ON_NONZERO(vpx_net_get_addr_info(ip, default_send_port, nl, 
					tl, &ogg_address))
    
  FAIL_ON_NONZERO(vpx_net_open(&ogg_sock2, nl ,tl))
  
  vpx_net_set_read_timeout(&ogg_sock2, vpx_NET_NO_TIMEOUT);
  vpx_net_set_send_timeout(&ogg_sock1, vpx_NET_NO_TIMEOUT);
  
  /*
    Bind the receiving connection as it needs
    to receive the request from client
  */
  rc = vpx_net_bind(&ogg_sock2, 0, default_recv_port);
    
  /*
    Wait for incoming connection data from a client
    This is a Handshake for RTP and session initiation
  */
  int bytes_read;
  int bytes_sent;
  
  while (!_kbhit())
    {
      char init_packet[PACKET_SIZE] = "initiate call";
      
      rc = vpx_net_sendto(&ogg_sock1, (tc8 *) &init_packet, 
			  PACKET_SIZE , &bytes_sent, ogg_address);
      
      rc = vpx_net_recvfrom(&ogg_sock2, one_packet, 
			    sizeof(one_packet), &bytes_read, &ogg_address2);
      
      if (rc != TC_OK && rc != TC_WOULDBLOCK)
	vpxlog_dbg(LOG_PACKET, "error\n");
      
      if (bytes_read == -1)
	bytes_read = 0;
      
      if (bytes_read)
        {
	  if (strncmp(one_packet, "configuration ", 14) == 0)
            {
	      /*
		Right now the encoder supports only default configuration
		whatever the client sends is not processed. Just a print
	      */
	      sscanf(one_packet + 14, "%d %d", &sample_rate, &audio_bitrate);
	      printf("Dimensions: %dkHz %dkbps \n", sample_rate, audio_bitrate);
	      break;
            }
        }
    }
  
  char init_packet[PACKET_SIZE] = "confirmed";
  rc = vpx_net_sendto(&ogg_sock1, (tc8 *) &init_packet, PACKET_SIZE , &bytes_sent, ogg_address);
  
  /*
    Receive acknowledgement
   */
  rc = vpx_net_recvfrom(&ogg_sock2, one_packet, 
			sizeof(one_packet), &bytes_read, &ogg_address2);
      
  if (rc != TC_OK && rc != TC_WOULDBLOCK)
    vpxlog_dbg(LOG_PACKET, "error\n");

        
  if (bytes_read == -1)
    bytes_read = 0;
  /*
    Read the acknowledgement pkt
   */
  if (bytes_read)
    {
      if (strncmp(one_packet, "configuration ", 14) == 0)
	printf("Sending Audio Data\n");
      else {
	printf("Wrong Ack type\n");
	return TC_ERROR;
      } 
    }     
  
  /*
    start rtp payload
   */  
  char readbuffer[READ];
  PACKET packet;
  sourceid = random32(OGG_VERSION);
  
  while (! feof(stdin) && ! ferror(stdin)) {
    long bytes=fread(readbuffer, 1, READ, stdin);
    
    if (bytes != 0) {
      packetize(&packet, readbuffer);
      rc = vpx_net_sendto(&ogg_sock1, (tc8 *)&packet, sizeof(packet) , &bytes_sent, ogg_address);
      
      if (rc != TC_OK && rc != TC_WOULDBLOCK)
	vpxlog_dbg(LOG_PACKET, "error\n");

    }

  }
  
  return TC_OK;
}


/*
  Packetize the read buffer as an rtp packet
 */
void packetize(PACKET *packet, char *readbuf) {
  unsigned int time = get_time();
  packet->version = OGG_VERSION;
  packet->pad = OGG_NOT_USED;
  packet->extension = OGG_NOT_USED;
  packet->csrccount = 1;
  packet->marker = OGG_NOT_USED;
  packet->payloadtype = OGG_PAYLOAD_TYPE;
  packet->seq = seqno;
  packet->timestamp = time;
  
  packet->ssrc = sourceid;
  packet->csrc = packet->ssrc; 
  
  packet->ident = OGG_IDENT;
  packet->fragment_type = OGG_NOT_FRAGMENTED;
  packet->vorbis_data_type = OGG_RAW_VORBIS_PAYLOAD;
  packet->len = READ;
  memcpy(packet->data, readbuf, packet->len);
  
}

u_int32 random32(int type)
{
  struct {
    int type;
    struct timeval tv;
    clock_t cpu;
    pid_t pid;
    u_long hid;
    uid_t uid;
    gid_t gid;
    struct utsname name;
  } s;

  gettimeofday(&s.tv, 0);
  uname(&s.name);
  s.type = type;
  s.cpu = clock();
  s.pid = getpid();
  s.hid = gethostid();
  s.uid = getuid();
  s.gid = getgid();
  /* also: system uptime */

  unsigned int rand = s.type || (s.cpu && s.pid) || (s.hid && s.uid) || s.gid;
  return rand;
  /* random32 */
}
