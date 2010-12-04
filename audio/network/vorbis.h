#ifndef __VORBIS_RTP__

#define __VORBID_RTP__

#define HEADER_SIZE                    22
#define OGG_VERSION                    2
#define OGG_NOT_USED                   0
#define OGG_NOT_FRAGMENTED             0
#define OGG_RAW_VORBIS_PAYLOAD         0
#define OGG_PAYLOAD_TYPE               0x76

#define OGG_CHANNEL_TWO               2
#define OGG_SAMPLE_RATE               44
#define OGG_BITRATE                   7

#define OGG_IDENT                     ((OGG_CHANNEL_TWO << 16) | (OGG_SAMPLE_RATE << 8) | (OGG_BITRATE))

#endif /*__VORBID_RTP__*/
