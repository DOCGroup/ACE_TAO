/* -*- C++ -*- */
// $Id$
/* Copyright (c) 1995 Oregon Graduate Institute of Science and Technology
 * P.O.Box 91000-1000, Portland, OR 97291, USA;
 * 
 * Permission to use, copy, modify, distribute, and sell this software and its 
 * documentation for any purpose is hereby granted without fee, provided that 
 * the above copyright notice appear in all copies and that both that 
 * copyright notice and this permission notice appear in supporting 
 * documentation, and that the name of O.G.I. not be used in advertising or 
 * publicity pertaining to distribution of the software without specific, 
 * written prior permission.  O.G.I. makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is" 
 * without express or implied warranty.
 * 
 * O.G.I. DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING 
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL 
 * O.G.I. BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY 
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN 
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF 
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Author: Shanwei Cen
 *         Department of Computer Science and Engineering
 *         email: scen@cse.ogi.edu
 */

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

#define CheckFrameRange(pnextFrame) \
{ if ((pnextFrame) < 0 || (pnextFrame) >= VIDEO_SINGLETON::instance ()->numF) \
  { fprintf(stderr, "VS: %d.VIDEO_SINGLETON::instance ()->nextFrame(%d) out of range (%d).\n", VIDEO_SINGLETON::instance ()->cmd, (pnextFrame), VIDEO_SINGLETON::instance ()->numF); \
    return 0; } }

#define CheckGroupRange(pnextGroup) \
{ if ((pnextGroup) < 0 || (pnextGroup) >= VIDEO_SINGLETON::instance ()->numG) \
  { fprintf(stderr, "VS: %d.VIDEO_SINGLETON::instance ()->nextGroup(%d) out of range (%d).\n", VIDEO_SINGLETON::instance ()->cmd, (pnextGroup), VIDEO_SINGLETON::instance ()->numG); \
    return 0; } }

// Global definitions

#include <sys/types.h>
#include <sys/socket.h>
#include "../include/common.h"
#include "ace/Singleton.h"
#include "../mpeg_shared/routine.h"

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
 
  int preGroup;
  int preHeader;
  int preFrame;

  // fast video play locals
  FFpara fast_para;
  int fast_preGroup;
  int fast_preHeader;
  
};

typedef ACE_TSS_Singleton <Video_Global, ACE_SYNCH_MUTEX> VIDEO_SINGLETON;

class Video_Timer_Global
// A class that holds the static timer variables defined in the middle
// of vs.cpp
{
public:
  static int timerHeader;
  static int timerGroup;
  static int timerFrame;
  static int timerOn ;
  static int timerAdjust ;
  static int preTimerVal ;
  static void StartTimer (void);
  static void StopTimer(void);
  static void TimerSpeed(void);
  static void TimerProcessing(void);
  static void timerHandler (int sig);
};
  
#endif /* define MPEG_GLOBAL_H */

