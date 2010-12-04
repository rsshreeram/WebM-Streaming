/*
Program to receive RTP audio  packets.
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


#include "tctypes.h"
#include "vpx_network.h"
#include "rtp.h"
#include "vorbis.h"
#include <stdio.h>
#include <ctype.h>  //for tolower
#include <string.h>
#include <sys/time.h>

#define READ        1024
void depacketize(PACKET *packet, char *readbuf);


int main() {
  recvpacket();
}

TCRV recvpacket() {
    struct vpxsocket ogg_sock, ogg_sock2;
    union vpx_sockaddr_x ogg_address, ogg_address2;

    TCRV rc;
    tc32 bytes_read;
    tc8 one_packet[8000];

    tcu16 default_send_port = DEFAULT_SEND_PORT;
    tcu16 default_recv_port = DEFAULT_RECV_PORT;


    int sample_rate = SAMPLE_RATE;
    int audio_bitrate = AU_BITRATE;
    
    int responded = 0; 
    /*
      Begin with initialisation
    */
    vpx_net_init();
    

    if (TC_OK != vpx_net_open(&ogg_sock, vpx_IPv4, vpx_UDP))
        return -1;

    vpx_net_set_read_timeout(&ogg_sock, vpx_NET_NO_TIMEOUT);
    vpx_net_bind(&ogg_sock, 0, default_recv_port);

    if (TC_OK != vpx_net_open(&ogg_sock2, vpx_IPv4, vpx_UDP))
        return -1;

    int bytes_sent;

    while (!_kbhit())
    {
        char initPacket[PACKET_SIZE];
        sprintf(initPacket, "configuration  %d %d ", sample_rate, audio_bitrate);
        rc = vpx_net_recvfrom(&ogg_sock, one_packet, sizeof(one_packet), &bytes_read, &ogg_address);

        if (rc != TC_OK && rc != TC_WOULDBLOCK && rc != TC_TIMEDOUT)
            vpxlog_dbg(DISCARD, "error\n");

        if (bytes_read == -1)
            bytes_read = 0;

        if (bytes_read)
        {
            if (!responded)
            {
                char add[400];
                sprintf(add, "%d.%d.%d.%d",
                        ((unsigned char *)&ogg_address.sa_in.sin_addr)[0],
                        ((unsigned char *)&ogg_address.sa_in.sin_addr)[1],
                        ((unsigned char *)&ogg_address.sa_in.sin_addr)[2],
                        ((unsigned char *)&ogg_address.sa_in.sin_addr)[3]);

                vpxlog_dbg(LOG_PACKET, "Address of Sender : %s \n", add);
                vpx_net_get_addr_info(add, default_send_port, vpx_IPv4, vpx_UDP, &ogg_address2);
                responded = 1;
            }
	    
            if (strncmp(one_packet, "initiate call", PACKET_SIZE) == 0)
            {
                rc = vpx_net_sendto(&ogg_sock2, (tc8 *) &initPacket, PACKET_SIZE , &bytes_sent, ogg_address2);
            }

            if (strncmp(one_packet, "confirmed", PACKET_SIZE) == 0)
            {
                rc = vpx_net_sendto(&ogg_sock2, (tc8 *) &initPacket, PACKET_SIZE , &bytes_sent, ogg_address2);
                break;
            }
        }
    }
    printf("Receiving Audio Packet");
    
    PACKET packet;

    while(1) {
      rc = vpx_net_recvfrom(&ogg_sock, one_packet, sizeof(one_packet), &bytes_read, &ogg_address);
      
      if (rc != TC_OK && rc != TC_WOULDBLOCK && rc != TC_TIMEDOUT)
            vpxlog_dbg(DISCARD, "error\n");

        if (bytes_read == -1)
            bytes_read = 0;
	
        if (bytes_read) {
	  depacketize(&packet, one_packet);
	}

	fwrite(packet.data, 1, READ, stdout);
    }
    
    return TC_OK;
}



void depacketize(PACKET *packet, char *readbuf) {
  packet->len = readbuf[HEADER_SIZE - 2];
  memcpy(packet->data, readbuf + HEADER_SIZE, packet->len);
}
