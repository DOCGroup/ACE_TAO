/* -*- C++ -*- */
#if !defined (MPEG_GLOBAL_H)
#define MPEG_GLOBAL_H

// Global symbols, these need to become enums eventually.

#define SESSION_NUM     4
#define CONN_INET	1
#define CONN_UNIX	2
#define CONN_ATM	3

#define SERVER_FDTABLE_SIZE	50
#define CLIENT_FDTABLE_SIZE	10

#define STATE_PENDING	1
#define STATE_CONTROL	2
#define STATE_DATA	3
#define STATE_SINGLE	4

#define min(a, b) ((a) > (b) ? (b) : (a))
#define max(a, b) ((a) > (b) ? (a) : (b))

/* Start codes. */

#define READ_BLOCK_SIZE 512
#define MAX_TIMER_ADJUST (100 * SPEEDUP_INV_SCALE)

#define SEQ_END_CODE 0x000001b7
#define SEQ_START_CODE 0x000001b3
#define GOP_START_CODE 0x000001b8
#define PICTURE_START_CODE 0x00000100
#define SLICE_MIN_START_CODE 0x00000101
#define SLICE_MAX_START_CODE 0x000001af
#define EXT_START_CODE 0x000001b5
#define USER_START_CODE 0x000001b2


#define FileRead(position, buf, size) \
	{ \
	  if (fseek(VIDEO_SINGLETON::instance ()->fp, (position), 0) == -1) \
	  { \
	    perror("VS error on fseek VideoFile"); \
	    return (-1); \
	  } \
	  while (fread((buf), (size), 1, VIDEO_SINGLETON::instance ()->fp) == 0) \
	  { if (errno == EINTR) { errno = 0; continue;}\
             perror("VS error on fread VideoFile"); \
             return (-1); \
	  } \
	}

// Global definitions

#include <sys/types.h>
#include <sys/socket.h>
#include "../include/common.h"
#include "ace/Singleton.h"

class Mpeg_Global
{
public:
  static int parentpid;

  static int listenSocketIn;
  static int listenSocketUn;

  static struct linger linger;

  static int live_audio;
  static int live_video;  /* 0 - no,  1 - to open,  2 - opened */

  static int drift_ppm;  /* clock drift in ppm */

  static int session_limit;
  static int session_num;
  static int rttag;
};

class Video_Global
{
 public:
  Video_Global ();

  int live_source;
  int video_format;

  int pkts_sent;
  time_t start_time;

  int conn_tag;

  int normalExit;

  int serviceSocket;
  int videoSocket;

  char videoFile[PATH_SIZE];
  FILE *fp;
  int needHeader;
  int lastRef[2];
  int lastRefPtr;
  int currentUPF;
  int addedUPF;
  int addedSignals;
  int VStimeAdvance;
  double fps;  /* current frames-per-second: playback speed */
  double frameRateLimit;

  VideoPacket * packet;
  int packetBufSize;
  int msgsn;
  int packetsn;
  int msgsize;

  unsigned char precmd;
  unsigned char cmd;
  int cmdsn;
  int nextFrame;
  int nextGroup;
  int firstPatternSize;
  char *firstSendPattern;
  int sendPatternGops;
  char sendPattern [PATTERN_SIZE];

#ifdef STAT
  char * framesSent;
#endif

  int fileSize;
  int maxS;
  int maxG;  
  int maxI;
  int maxP;
  int maxB;
  int minS;
  int minG;
  int minI;
  int minP;
  int minB;
  int numS;
  int numG;
  int numF;
  int numI;
  int numP;
  int numB;
  int averageFrameSize;
  int horizontalSize;
  int verticalSize;
  int pelAspectRatio;
  int pictureRate;
  int vbvBufferSize;
  int firstGopFrames;
  int patternSize;
  char pattern [PATTERN_SIZE];

  struct SystemHeader
  {
    long offset;
    int size;
  } * systemHeader;

  struct GopTable
  {
    int systemHeader;
    long offset;
    int headerSize;
    int size;
    int totalFrames;
    int previousFrames;
    long firstIoffset;
  } * gopTable;

  struct FrameTable
  {
    char type;
    unsigned short size;
  } * frameTable;
 

};

typedef ACE_TSS_Singleton <Video_Global, ACE_SYNCH_MUTEX> VIDEO_SINGLETON;
 

#endif /* define MPEG_GLOBAL_H */

