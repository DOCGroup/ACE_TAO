/* $Id$ */
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
#ifndef _MPEG_COMMON_H
#define _MPEG_COMMON_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

extern int gethostname (char*, size_t);

#define VERSION 200   /* version 2.00 */

#ifdef LITTLE_ENDIAN
#define NeedByteOrderConversion
#elif !defined(BIG_ENDIAN)
#if (defined(vax) || defined(i386) || defined(i486)) && !defined(NeedByteOrderConversion)
#define NeedByteOrderConversion
#endif
#endif

/*following is defined so that debuging information is printed out for only
  specific user (usually software developer), LOG_DIR is also defined */

/*
#define LOG_DIR "/home/shanweic/log/"
#define DEVELOPER_UID   8640
*/

/* %% this is where the logging is done.." */
#define LOG_DIR "./"
#define DEVELOPER_UID   1735

#define SFprintf        if ((!getuid()) || getuid() == DEVELOPER_UID) fprintf
#define Fprintf         if (shared->config.verbose) fprintf

#define VCR_TCP_PORT    7101
#define VCR_UNIX_PORT   "/tmp/vcr_unix_port01"
#define VCR_ATM_PORT    "3005"

#define SPEEDUP_INV_SCALE 5  /* VS fast starts at 5/(5-1) normal speed */

#define AB_BUF_SIZE (64 * 1024)   /* size of client AB buffer */

#if 0
#define VB_BUF_SIZE (256 * 1024)  /* Size of client VB buffer */
#endif

#define VB_BUF_SIZE (1024 * 1024)  /* Size of client VB buffer */

#define VD_BUF_SIZE (1024 * 1024)  /* Size of client VD buffer */

#define PATH_SIZE       256
#define PATTERN_SIZE    100
#define MAX_FRAMES      54000  /* half hour video at 30FPS */
#define MAX_VDQUEUE_SIZE 20
#define MAX_FB_PACKETS  20  /* max number of feedback packet info collected. */
#define POSITION_RANGE  100
#define MSGGAP_MIN      -10  /* these are used for recording stat of
                                msgs received by VB */
#define MSGGAP_MAX      50

#define INET_SOCKET_BUFFER_SIZE 1400

/* audio type constants */

#define AudioTypeMuLaw 0
#define AudioTypeALaw 1
#define AudioTypeRIFF 2
#define AudioTypeLiner16 3
#define AudioTypeLiner8  4
#define AudioTypeLiner8Offset 5

/* Video and audio compression format */

#define VIDEO_MPEG1     0
#define VIDEO_MPEG2     1
#define VIDEO_JPEG      3
#define VIDEO_SIF       4

#define AUDIO_RAW       10
#define AUDIO_MPEG1     11
#define AUDIO_MPEG2     12


/* system configuration default values */

#define DEFAULT_maxFPS 60
#define DEFAULT_maxSPS 16000
#define DEFAULT_ffFPS 150
#define DEFAULT_fbFPS 150
#define DEFAULT_feedBackDelay 2000
#define DEFAULT_audioMask 0x1
#define DEFAULT_encodeType AudioTypeMuLaw
#define DEFAULT_channels 1
#define DEFAULT_samplesPerSecond 8000
#define DEFAULT_bytesPerSample 1
#define DEFAULT_audioTimerDuration 500
#define DEFAULT_audioBufferedIntervals 2
#define DEFAULT_framesPerAudioPlay  4
#define DEFAULT_audioForward   800
#define DEFAULT_filterPara  50
#define DEFAULT_maxSPframes 60
#define DEFAULT_VStimeAdvance 100

#define DEFAULT_volume 0

/* audio parameter data structure */

typedef struct
{
  int   encodeType;
  int   channels;
  int   samplesPerSecond;
  int   bytesPerSample;
} AudioParameter;

/* client processes shared data structure definition */

typedef struct
{
  int VDframeNumber;
  int VBbufId, VDbufId;
  char * VBbufAddr, * VDbufAddr;
  int pixelValid;
  unsigned char pixel[256];  /* for transfering pixel value from VP to VD */

  /* command and status   */
  int cmd;
  int cmdsn;
  int cmdBusy;

  int loopBack;

  int framesPerSecond;   /* Frames per second */
  int usecPerFrame;    /* micro-seconds per frame, actual video speed */
  int samplesPerSecond;   /* Samples per second */
  int usecPerSample;   /* micro-seconds per sample, actual audio speed */

  int volumePosition;
  int balancePosition;
  int speedPosition;
  int locationPosition;
  struct hostent audioHost;
  char audioPath[PATH_SIZE];
  struct hostent videoHost;
  char videoPath[PATH_SIZE];

  int live;  /* non-zero if any of the video/audio streams are live source */
  int videoFormat;
  int audioFormat;

  /*  video channel parameters  */
  int totalFrames;
  int totalGroups;
  int totalHeaders;
  int averageFrameSize;
  double  pictureRate;
  int horizontalSize;
  int verticalSize;
  int pelAspectRatio;
  int vbvBufferSize;
  int firstGopFrames;
  int patternSize;
  char pattern[PATTERN_SIZE];
  int IframeGap;
  int VStimeAdvance;

  int lastIframeDecoded;
  int sendPatternGops;
  char sendPattern[PATTERN_SIZE];
  float frameRateLimit;
  int framesDropped;
  int qosRecomputes;

  /*  video channel status  */
  int currentGroup;
  int currentFrame;
  int currentDisplay;
  int nextFrame;
  int nextGroup;

  /* audio channel parameters  */
  int totalSamples;
  AudioParameter audioPara;

  /* audio channel status  */
  int nextSample;   /* played by AP */

  /* AudioFile device parameter */
  AudioParameter AFPara;

  int currentUPF;
  int rtplay;           /* realtime tag, -- VD needs to drop frames when needed */
  int collectStat;  /* to indicate if statistics should be collected or not */
  int VBheadFrame;      /* ID of last frame put to VBbuffer by VB */
  int needHeader;   /* VD requests resending of systerm Header */
  int playRoundTripDelay; /* play round trip time (from issuing play/FF/FB
                             command to first Decoded frame showup in
                             VD buffer) estimated by CTR start_timer() */
  int audioMaxPktSize;  /* 0 -- reliable byte stream,
                           >0 - non-discard mode packet stream,
                           <0 - discard mode packet stream */
  int videoMaxPktSize;

  /* System configuration parameters */
  struct
  {
    int rt;             /* play as fast as possible without dropping frames,
                           and audio turned off when this tag is set to 0 (zero)
                                   */
    int maxFPS;         /* maximum PLAY frames-per-second, this play speed is used
                           during playback when speed scale is set to 100 */
    int maxSPS;         /* similar to maxFPS, used when audio is played without
                           corresponding video
                           */
    int ffFPS;          /* play speed when Fast forward */
    int fbFPS;          /* play speed when fast rewind */
    int feedBackDelay;  /* msec, delay of feedback when AB, VB received first
                           packet, 0 (zero) default to adaptive */
    int audioMask; /* mask for audio output device */
    AudioParameter audioPara; /* default audio parameter by system */
    int audioTimerDuration; /* for audio-only case, miliseconds */
    int audioBufferedIntervals; /* for audio-only case,
                                   audioTimerDuration * audioBufferedIntervals
                                   milliseconds of audio samples will be buffered
                                   in AF buffer
                                   */
    int framesPerAudioPlay;   /* for audio-video case, each time, samples corresponding
                                 to this number of video frames will be played */
    int audioForward;    /* forward value for audio channel, in AFTime unit
                            (number of samples), audio channel is played ahead of video
                            by this number of samples. This is introduce to compensate
                            the dely to AF in audio channel, and usually should be less
                            than 1 (one) second, but greater than zero.
                          */
    int VStimeAdvance;  /* target VB buffer level to be mentained (micro-seconds). This
                           is sent in INIT message to VS, to decide how many frames to
                           be maintained in VB buffer
                          */
    float frameRateLimit; /* playback frame rate limit (frames per second) */
    int collectStat;    /* tag to collect statistics to file 'stat.n' when set */
    int collectFrameInfo; /* collect MPEG frame info to file 'stat.n' when set*/
    int syncEffective;  /* tag to make sync mechanism effective when set */
    int qosEffective;   /* tag to make QoS control effective when set */
    int audioOffset;    /* offset added to audio channel 'nextSample' when play().
                           usually audio and video are not recorded strictly
                           synchronized. This parameter is to compensate it. The
                           value can be any integer value.
                         */
    int filterPara;     /* 1/R of Median, nsamples of Mean and Average */
    int maxSPframes;    /* max number of frame in a sendPattern */
    int audioConn;      /* tag for connection oriented audio data channel */
    int videoConn;      /* tag for connection oriented video data channel */
    int verbose;
  } config;

#ifdef STAT
  struct
  {
    unsigned VBmaxBytes;        /* max fill level (bytes) in VB */
    unsigned VBdroppedFrames;   /* accumulative total number of frames dropped by VB
                                   because VB buffer full */
    unsigned VBemptyTimes;      /* number of time VD fail to get a frame from VB
                                   immediately */
    unsigned VDnoRef;            /* # of frames without reference */
    unsigned VDagainstSendPattern; /* # of frames against send pattern */
    unsigned VDtooLateI;         /* # of I frames dropped 'cause too late */
    unsigned VDtooLateP;         /* # of P frames dropped 'cause too late */
    unsigned VDtooLateB;         /* # of B frames dropped 'cause too late */
    unsigned VDlastFrameDecoded;        /* ID of last frame decoded by VD */
    unsigned CTRdropLate;
    unsigned CTRdropOutOrder;
    unsigned CTRdispOnTime;
    unsigned CTRdispLate;
    unsigned fbPacketNumber;    /* # packets sent by feedback mechanism */
    struct {
      int frameId;
      int addUsecPerFrame;
      int addFrames;
      int frames;
      int framesDropped;
      float frameRateLimit;
      int advance;
    } fbPackets[MAX_FB_PACKETS];        /* recording FB packet contents */
    unsigned VDqueue[MAX_VDQUEUE_SIZE]; /* record number of frames with VD queue
                                           length of index number */
    int  VBmsgGaps[MSGGAP_MAX + 1 - MSGGAP_MIN]; /* # of ind-sized gaps between msgs */
    char VBframesReceived[(MAX_FRAMES + 7)/8];  /* bit array for frames received by VB */
    char VDframesDecoded[(MAX_FRAMES + 7)/8];   /* bit array for frames decoded by VD */
    char VPframesDisplayed[(MAX_FRAMES + 7)/8]; /* bit array for frames displayed by VP */
    short VBfillLevel[MAX_FRAMES];      /* VB buffer fill level (frames) recorded by CTR */
  } stat;
#endif
} SharedData;

/* command and reply: GUI-CTR, CTR-VS, CTR-AS */

/* GUI to CTR, (and also CTR to VS and/or AS) */

#define CmdINIT 0    /* followed by: videoHost, videoPath, audioHost, audioPath
                        each item is passed as a string (without 0), led with length(int) */
#define CmdINITaudio    1   /* for CTR-AS */
#define CmdINITvideo    2   /* for CTR-VS */
#define CmdSTOP 3
#define CmdFF   4
#define CmdFB   5
#define CmdSTEP 6
#define CmdPLAY 7
#define CmdREF  20              /* this is for sending REFerence frame by VS
                                   for CmdSTEP and CmdPLAY */
#define CmdPOSITION     8         /* followed by position */
#define CmdPOSITIONrelease      9 /* followed by position */
#define CmdVOLUME       10        /* followed by position */
#define CmdBALANCE      11        /* followed by position */
#define CmdSPEED        12        /* followed by position */
#define CmdLOOPenable   13
#define CmdLOOPdisable  14
#define CmdSTATstream   23   /* reply with { | type(byte) | size(int) }+ | */
#define CmdSTATsent     24   /* reply with { | byte }+ | */
#define CmdCLOSE        18
#define CmdFAIL         19   /* followed by a string */

#define CmdDONE         15      /* CTR to GUI:
                                   replay for accptance and execution of Above Cmd */

#define CmdVPinitScreen 30    /* CTR to VP(GUI), audio-only screen clearning */
#define CmdVPdisplayFrame 31  /* CTR to VP(GUI) to display the single frame in buffer */
#define CmdVPclearScreen 32   /* CTR to VP(GUI), to clear all images in VD buffer */
#define CmdVPaudioPosition 33 /* CTR to VP(GUI), in audio-only case, set position bar */

// to terminate the event loop
#define CmdExit  34


// cmd sent by Java GUI to Controller process
#define CmdJINIT 35

#define EXIT 36
#define DECODED 37

/* CTR-VS */

/* CmdINITvideo: followd by parameters of a fixed structure of 3 integers
   reply with CmdINITvideo followed by reply or CmdFAIL followed by a string,
*/
typedef struct
{
  int sn;
  int version;
  int nameLength;    /* the length of the video file */
  /* this structure is followed by a zero-terminated file name as follows:
      char videoFile[];
   */
} INITvideoPara;

typedef struct
{
  int live;
  int format;

  int totalHeaders;
  int totalFrames;
  int totalGroups;
  int horizontalSize;
  int verticalSize;
  int pelAspectRatio;
  int pictureRate1000;
  int vbvBufferSize;
  int sizeIFrame;
  int sizePFrame;
  int sizeBFrame;
  int sizeSystemHeader;
  int sizeGop;
  int averageFrameSize;
  int firstGopFrames;
  int patternSize;
  char pattern[PATTERN_SIZE];
} INITvideoReply;

/* CmdPOSITION, CmdPOSITIONrelease: followed by following parameter */
typedef struct
{
  int sn;
  int nextGroup;
} POSITIONpara;

/* CmdSTEP: followed by following parameter */
typedef struct
{
  int sn;
  int nextFrame;
} STEPpara;

/* CmdFF, CmdFB: followed by following parameter */
typedef struct
{
  int sn;
  int nextGroup;
  int usecPerFrame;
  int framesPerSecond;
  int VStimeAdvance;
} FFpara;

/*CmdPLAY: followed by following parameter */
typedef struct
{
  int sn;
  int collectStat;
  int nextFrame;
  int usecPerFrame;
  int framesPerSecond;
  int frameRateLimit1000;
  int VStimeAdvance;
  int sendPatternGops;
  char sendPattern[PATTERN_SIZE];
} PLAYpara;

/* CmdSPEED: parameter */
typedef struct
{
  int sn;
  int usecPerFrame;
  int framesPerSecond;
  int frameRateLimit1000;
  int sendPatternGops;
  char sendPattern[PATTERN_SIZE];
} SPEEDpara;

/* CmdLOOPenabe, CmdLOOPdisable, CmdSTOP: following by SN */


/* video channel data packet structure */

typedef struct
{
  int cmd;
  int cmdsn;
  int sh;
  int gop;
  int frame;
  int display;  /* display frame number */
  int future;
  int past;
  int currentUPF;
  int dataBytes;
} VideoPacket;


/* video channel data message structure */

typedef struct
{
  int packetsn;
  int packetSize;
  int msgsn;
  int msgOffset;
  int msgSize;
} VideoMessage;


/* Feedback parameter structure */

typedef struct
{
  int cmdsn;
  int needHeader;
  int addUsecPerFrame;
  int addFrames;
  int frameRateLimit1000;
  int sendPatternGops;
  int sendPattern[PATTERN_SIZE];
} VideoFeedBackPara;



/* audio channel commands and parameters */

/* CmdINITaudio parameter of fixed structure of 3 integers and a AudioPara,
   replys with CmdINITaudio followed by reply or CmdFAIL followed by a string */
typedef struct
{
  int sn;
  int version;
  AudioParameter para;
  int nameLength;
  /* char audioFile[] */
} INITaudioPara;

typedef struct
{
  int live;
  int format;

  AudioParameter para;
  int totalSamples;
} INITaudioReply;


/* CmdPLAY parameter */
typedef struct
{
  int sn;
  int nextSample;
#if 0
  int samplesPerSecond; /* <= shared->audioPara.samplesPerSecond   */
#endif
  int samplesPerSecond; /* audio playback speed */
  int samplesPerPacket; /* Number of samples in a packet */
  int ABsamples; /* size of client VB buffer in samples */
  int spslimit;  /* QoS parameter: sps supported by the audio channel */
} PLAYaudioPara;

/* STOP followed only by SN */

/* CmdSPEED parameter */
typedef struct
{
  int sn;
  int samplesPerSecond;
  int samplesPerPacket; /* Number of samples in a packet */
  int spslimit;
} SPEEDaudioPara;

/* audio packet structure */
typedef struct
{
  int cmdsn;
  int resend;
  int samplesPerSecond;
  int firstSample;
  int samples;
  int actualSamples;
  int dataBytes;
} AudioPacket;

typedef struct
{
  int firstSample;  /* Id of the first sample in this packet */
  int samples;   /* number of samples in current packet */
} APdescriptor; /* audio packet decriptor */

/* audio feedback packet */
typedef struct
{
  int cmdsn;
  int type;   /* 0 - feedback command; >=1 - Number of packets to be resent */
  union {
    struct {
      int addsps;
      int addSamples;
    } fb;
    APdescriptor ap;
  } data;
} AudioFeedBackPara;
/* One actual resent request may have multiple APdescriptors depending on
   the value in element type, all execpt the first one follows above structure*/

typedef struct {
  int sh, gop, frame, display, future, past;
  int refcount;                          /* reference count    */
  unsigned char * data;                  /* data buffer for image */
} FrameBlock;


#endif /* _MPEG_COMMON_H */
