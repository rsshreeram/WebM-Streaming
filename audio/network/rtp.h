/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#define LARGESTFRAMESIZE 1000000
#define PACKET_SIZE 1024

enum
{
    DATAPACKET = 0,
    XORPACKET = 1,
};
enum
{
    NORMAL = 0,
    KEY = 1,
    GOLD = 2,
    ALTREF = 3
};
enum
{
    LOG_PACKET = 1,
    SKIP   = 2,
    REBUILD = 4,
    DISCARD = 8,
    FRAME   = 16,
    ERRORS = 32,
};


#define LOG_MASK  ( ERRORS| SKIP|REBUILD|DISCARD ) // ( ERROR|LOG_PACKET|FRAME|SKIP|REBUILD|DISCARD ) //

typedef struct
{
    int version : 2;
    int pad : 1;
    int extension : 1;
    int csrccount : 4;
    int marker : 1;
    int payloadtype : 7;
    unsigned short seq;
    unsigned int timestamp;

    unsigned int ssrc ;
    unsigned int csrc ;  // repeated up to 15 times


    int ident : 24;
    int fragment_type : 2;
    int vorbis_data_type : 2;
    int nopkts : 4;
    unsigned short len;
    unsigned char data[PACKET_SIZE];

    // this value doesn't actually get written or read
    unsigned int size;

} PACKET;

#define PACKET_HEADER_SIZE offsetof(PACKET,data)

unsigned int get_time(void);
void vpxlog_dbg_no_head(int level, const tc8 *format, ...);
void vpxlog_dbg(int level, const tc8 *format, ...);

#define IP_SIZE  512
#define DEFAULT_SEND_PORT     8546
#define DEFAULT_RECV_PORT     8548


#define AU_BITRATE       128
#define SAMPLE_RATE      44.1

