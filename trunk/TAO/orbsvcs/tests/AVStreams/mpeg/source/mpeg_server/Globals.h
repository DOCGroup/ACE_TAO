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

#ifndef MPEG_GLOBAL_H
#define MPEG_GLOBAL_H

#include "ace/SOCK_CODgram.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Global symbols, these need to become enums eventually.

#define SESSION_NUM     4
#define CONN_INET       1
#define CONN_UNIX       2
#define CONN_ATM        3

#define SERVER_FDTABLE_SIZE     50
#define CLIENT_FDTABLE_SIZE     10

#define STATE_PENDING   1
#define STATE_CONTROL   2
#define STATE_DATA      3
#define STATE_SINGLE    4

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

// Global definitions
#define nextByte  {int val; fileptr ++; \
                   if ((val = getc(VIDEO_SINGLETON::instance ()->fp)) == EOF) \
                   {\
                     perror("Crossed EOF or error while scanning"); \
                     return 1; \
                   } nb = val;}

#define computePicSize \
        if (inpic) \
        { \
          if (pictype == 'I') \
          { \
            VIDEO_SINGLETON::instance ()->maxI = max(VIDEO_SINGLETON::instance ()->maxI, (int)(fileptr - picptr - 4)); \
            VIDEO_SINGLETON::instance ()->minI = min(VIDEO_SINGLETON::instance ()->minI, (int)(fileptr - picptr - 4)); \
          } \
          else if (pictype == 'P') \
          { \
            VIDEO_SINGLETON::instance ()->maxP = max(VIDEO_SINGLETON::instance ()->maxP, (int)(fileptr - picptr - 4)); \
            VIDEO_SINGLETON::instance ()->minP = min(VIDEO_SINGLETON::instance ()->minP, (int)(fileptr - picptr - 4)); \
          } \
          else \
          { \
            VIDEO_SINGLETON::instance ()->maxB = max(VIDEO_SINGLETON::instance ()->maxB, (int)(fileptr - picptr - 4)); \
            VIDEO_SINGLETON::instance ()->minB = min(VIDEO_SINGLETON::instance ()->minB, (int)(fileptr - picptr - 4)); \
          } \
          VIDEO_SINGLETON::instance ()->frameTable[ftptr].type = pictype; \
          VIDEO_SINGLETON::instance ()->frameTable[ftptr++].size = (int)(fileptr - picptr - 4); \
          inpic = 0; \
        }


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

#include "ace/Singleton.h"
#include "include/common.h"
#include "mpeg_shared/routine.h"
#include "mpeg_shared/fileio.h"
#include "mpeg_shared/com.h"
#include "mpeg_shared/sendpt.h"
#include "mpeg_server/server_proto.h"
#include "mpeg_server/Video_Server.h"
#include "mpeg_shared/Video_ControlS.h"
#include "mpeg_shared/Audio_ControlS.h"

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

  ACE_SOCK_CODgram dgram;
  // the UDP data socket

  char *data_host;
  // name of the host to bind to.
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
  Video_Control::FFpara fast_para;
  Video_Control::PLAYpara play_para;
  int fast_preGroup;
  int fast_preHeader;

  // globals functions
  int CmdRead (char *buf, int psize);
  int CmdWrite (char *buf, int size);
  int SendPacket (int shtag,
                  int gop,
                  int frame,
                  int timeToUse,
                  int first_time = 0);
  int FBread (char *buf, int size);
  int PLAYliveVideo (PLAYpara * para);
  void ComputeFirstSendPattern (float limit);
  int FrameToGroup (int * frame);
  int SendReferences (int group, int frame);
  int send_to_network (int timeToUse);
  int first_packet_send_to_network (int timeToUse);
  int GetFeedBack (void);
  int SendPicture (int *frame);
  int ReadInfoFromFile (void);
  void WriteInfoToFile (void);
  int init_MPEG1_video_file (void);
  int play_send (int debug=0);
  int fast_play_send (void);
  int position (void);
  int step_video (void);
  int fast_forward (void);
  int fast_backward (void);
  int stat_stream (void);
  int stat_sent (void);
  int init_play (Video_Control::PLAYpara para,
                 CORBA::Long_out vts);
  CORBA::Boolean init_fast_play (const Video_Control::FFpara &para);
  int init_video (void);
};

typedef ACE_Singleton <Video_Global, ACE_SYNCH_MUTEX> VIDEO_SINGLETON;

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

#define DATABUF_SIZE 500

#define SPEEDUP_SCALE 5
#define MAX_RESEND_REQS 10
#define FBBUF_SIZE (sizeof(AudioFeedBackPara) + \
                    (MAX_RESEND_REQS - 1) * sizeof(APdescriptor))



  class Audio_Global
  {
  public:
    Audio_Global (void);
    // Default constructor.
    int CmdRead(char *buf, int psize);
    void CmdWrite(char *buf, int size);
    int INITaudio(void);
    int send_packet(int firstSample, int samples);
    int SendPacket(void);
    void ResendPacket(int firstsample, int samples);
    int PLAYaudio(void);
    int play_audio (void);
    int send_audio (void);
    void on_exit_routine(void);

  public:

    char *data_host;
    enum audio_state {AUDIO_WAITING = 0,AUDIO_PLAY =1};

    audio_state state;
    // The state of the audio server.

    // playaudio members
    int packets;
    int hasdata;
    struct timeval tval;

    int addSamples;
    unsigned nextTime;
    int upp;  /* micro-seconds per packet */
    int delta_sps ;
    // The members previously in PLAY audio.

    int bytes_sent ;
    time_t start_time;

    int conn_tag;

    int serviceSocket;
    int audioSocket ;

    char audioFile[PATH_SIZE];
    int fd;
    Audio_Control::AudioParameter audioPara;
    int totalSamples;
    int fileSize;
    unsigned char cmd;
    int live_source ;

    int databuf_size;
    int cmdsn;
    int nextsample;
    int sps; /* audio play speed given by the client: samples per second */
    int spslimit;
    int spp; /* samples per packet */
    AudioPacket * pktbuf ;
    AudioFeedBackPara * fbpara ;
  };

typedef ACE_Singleton <Audio_Global,ACE_SYNCH_MUTEX> AUDIO_GLOBAL;


#endif /* define MPEG_GLOBAL_H */

