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
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#ifdef ULTRIX
#include <fcntl.h>
#else
#include <sys/fcntl.h>
#endif
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <limits.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>

#include "include/common.h"
#include "newproto.h"
#include "global.h"
#include "mpeg_shared/routine.h"
#include "mpeg_shared/sendpt.h"
#include "mpeg_shared/filters.h"
#include "mpeg_shared/fileio.h"
#include "mpeg_shared/com.h"
#include "Command_Handler.h"

#define SPEEDHIST_SIZE 20

#define max(a,b) (a>b ? a : b)
#define min(a,b) (a<b ? a : b)

static int usr1_flag = 0;
static int rtplay = 1;
static int cmdSocket = -1;
static int CTRpid = -1, VBpid = -1, VDpid = -1, UIpid = -1;
static int ABpid = -1;
static int videoSocket = -1;
static int audioSocket = -1;
static int hasAudioDevice = 0;
static int cmdBytes = 0;
static char *cmdBuffer = NULL;
static char *vh, *vf, *ah, *af;
static int cmdAcks = 0;
static int speedPtr = 0;
static struct
{
  int frameId;
  int usecPerFrame;
  float frameRateLimit;
  int frames;
  int framesDropped;
} speedHistory[SPEEDHIST_SIZE];

/* following are for live video */
static unsigned startTime;  /* used to record start time for live playback,
			     also used to record FF/FB/PLAY round trip time */
static unsigned startVSA; /* VS advance at the begining of playback of live video */
static int displayedFrames;

/* following static variable is for frameRate feedback. See comments on
 initialization of these variables in function play()*/

static int fbstate = 0;   /* state value of frame-rate feedback */
static double maxfr = 0;  /* frame-rate limit during playback, in fps. This is deduced
			   from frameRateLimit, a constant during a playback, even if
			   play speed changes */
static int minupf = 0;    /* minimum usec/frame, equals to 1000000/maxfr */
static double maxrate = 0; /* current max frame rate, in percent of maxfr, after taken
			      playspeed into consideration. If play speed > maxfr,
			      then this value would be 1.0. Otherwise it would be
			      playspeed/maxfr */
static double frate = 0;  /* current server frame rate, in percentage of maxrate */
static double adjstep = 0; /* step of frame-rate adjustment. In percentage value.
			      This is basically
			      maxrate/#frames-in-feedback-frame-sequence */
static int fb_startup = 0; /* Indicate the first feedback action. The first feedback
			      action is to set the server fps directly according to
			      actully measured display frame-rate. All following
			      feedback action would then adjust server fps linearly
			      adjstep each time */

#define max(a,b) (a>b ? a : b)
#define min(a,b) (a<b ? a : b)

#define NewCmd(val) {shared->cmd = val; shared->cmdsn ++; }

static void compute_sendPattern(void);

int
OurCmdRead(char *buf, int size)
{
  int val;
  if (size == 0) return 0;
  if (cmdBytes > 0)
  {
    memcpy(buf, cmdBuffer, size);
    cmdBytes -= size;
    cmdBuffer += size;
    return 0;
  }
  while ((val = read(cmdSocket, (buf), (size))) <= 0)
    {
    if (val == -1 && errno == EINTR) return 1;
    if (!val) {
      perror("CTR error, EOF reached unexpected within CmdRead()");
    }
    else {
      perror("CTR CmdRead() from UI through CmdSocket");
    }
    exit(1);
  }
  return 0;
}

void CmdRead(char *buf, int size)
{
  int val;
  if (size == 0) return;
  if (cmdBytes > 0)
  {
    memcpy(buf, cmdBuffer, size);
    cmdBytes -= size;
    cmdBuffer += size;
    return;
  }
  while ((val = read(cmdSocket, (buf), (size))) <= 0)
    {
    if (val == -1 && errno == EINTR) continue;
    if (!val) {
      perror("CTR error, EOF reached unexpected within CmdRead()");
    }
    else {
      perror("CTR CmdRead() from UI through CmdSocket");
    }
    exit(1);
  }
  return;
}

static int CmdReadNW(char *buf, int size)
{
  struct fd_set read_mask;
  int nfds = cmdSocket+1;
  int val;
  
  if (size == 0) return 0;
  
  if (cmdBytes > 0)
  {
    memcpy(buf, cmdBuffer, size);
    cmdBytes -= size;
    cmdBuffer += size;
    return 0;
  }
  
  FD_ZERO(&read_mask);
  FD_SET(cmdSocket, &read_mask);
#ifdef _HPUX_SOURCE
  if (select(nfds, (int *)&read_mask, NULL, NULL, NULL) == -1)
#else
  if (select(nfds, &read_mask, NULL, NULL, NULL) == -1)
#endif
  {
    if (errno == EINTR)
      return -1;
    perror("CTR CmdReadNW by select");
    exit(1);
  }
  if ((val = read(cmdSocket, (buf), (size))) < 0)
  {
    if (errno == EINTR)
      return -1;
    
    perror("CTR CmdReadNW by read");
    exit(1);
  }
  if (val == 0)
  {
    fprintf(stderr, "Error: EOF reached unexpectedly within CmdReadNW().");
    exit(1);
  }
  return 0;
}

static void CmdWrite(unsigned char * buf, int size)
{
  if (cmdAcks > 0 && size == 1 && *buf == CmdDONE)
  {
    cmdAcks --;
    return;
  }
    
  while (write(cmdSocket, buf, size) == -1)
  {
    if (errno == EINTR)
      continue;
    perror("CTR writes to UI through cmdSocket");
    exit(1);
  }
}

/* following function can be replaced by ../shared/fileio.c:read_bytes()
   but SocketRead provides more error info */

static void SocketRead(int s, char *buf, int size)
{ int val, remain = size;
  char * ptr = buf;
  fprintf (stderr, "SocketRead: videoSocket = %d\n",
           videoSocket);
  for (;;)
  {
    val = read(s, ptr, remain);
    
    fprintf(stderr, "CTR got from %sSocket %d of %d.\n",
	    s == videoSocket ? "video" : "audio", val, remain);
    
    if (val == -1 && errno == EINTR)
    {
      errno = 0;
      continue;
    }
    if (val == -1)
    {
      fprintf(stderr, "CTR error read %sSocket, ret=%d(size=%d)",
	      s == videoSocket ? "video" : "audio", size-remain, size);
      perror("");
      exit(1);
    }
    if (val == 0)
    {
      fprintf(stderr, "CTR error read %sSocket, EOF met, ret=%d(size=%d).\n",
	      s == videoSocket ? "video" : "audio", size-remain, size);
      exit(1);
    }
    ptr += val;
    remain -= val;
    if (remain < 0)
    {
      fprintf(stderr, "CTR error read %sSocket, read too much, ret=%d(size=%d).\n",
	      s == videoSocket ? "video" : "audio", size-remain, size);
      exit(1);
    }
    if (remain == 0)
      break;
  }
}

#if 0
static void SocketRecv(int s, char *buf, int size)
{ int val, remain = size;
  char * ptr = buf;
  for (;;)
  {
    val = read(s, ptr, remain);
    /*
    fprintf(stderr, "CTR got from %sSocket %d of %d.\n",
	    s == videoSocket ? "video" : "audio", val, remain);
    */
    if (val == -1 && errno == EINTR)
    {
      errno = 0;
      continue;
    }
    if (val == -1)
    {
      fprintf(stderr, "CTR error read %sSocket, ret=%d(size=%d)",
	      s == videoSocket ? "video" : "audio", size-remain, size);
      perror("");
      exit(1);
    }
    if (val == 0)
    {
      fprintf(stderr, "CTR error read %sSocket, EOF met, ret=%d(size=%d).\n",
	      s == videoSocket ? "video" : "audio", size-remain, size);
      exit(1);
    }
    ptr += val;
    remain -= val;
    if (remain < 0)
    {
      fprintf(stderr, "CTR error read %sSocket, read too much, ret=%d(size=%d).\n",
	      s == videoSocket ? "video" : "audio", size-remain, size);
      exit(1);
    }
    if (remain == 0)
      break;
  }
}
#endif

#define VideoRead(buf, size) SocketRead(videoSocket, buf, size)

#define VideoWrite(buf, psize) \
  { int val; while ((val = write(videoSocket, (buf), (psize))) == -1) \
    { if (errno == EINTR) continue; \
      perror("CTR writes to VS through videoSocket");\
      exit(1); \
    } \
    if (val < (int)(psize)) { \
      fprintf(stderr, "CTR bad VideoWrite, size %d, val %d", psize, val); perror(""); }\
  }

#define AudioRead(buf, size) SocketRead(audioSocket, buf, size)

#define AudioWrite(buf, size) \
  { while (write(audioSocket, (buf), (size)) == -1) \
    { if (errno == EINTR) continue; \
      perror("CTR writes to AS through audioSocket"); \
      exit(1); \
    } \
  }

static void start_timer(void);
static void stop_timer(void);
static void timer_speed(void);
static void wait_display(void);
static void stop_playing();

static int InitVideoChannel(char * phostname, char * vf)
{
  int dataSocket = -1;

  if (ComOpenConnPair(phostname, &videoSocket,
		      &dataSocket, &shared->videoMaxPktSize) == -1) {
    return -1;
  }

  /* Initialize with VS */
  {
    INITvideoPara para;
    INITvideoReply reply;
    unsigned char tmp;
    tmp = CmdINITvideo;
    VideoWrite(&tmp, 1);
    
    para.sn = htonl(shared->cmdsn);
    para.version = htonl(VERSION);
    para.nameLength = htonl(strlen(vf)+1);
    
    VideoWrite(&para, sizeof(para));
    VideoWrite(vf, strlen(vf)+1);
    /*
    fprintf(stderr, "InitCommand to VSwritten.\n");
    */
    /*
    VideoRead((char*)&tmp, 1);
    */
    errno = 0;
    if (read(videoSocket, (char*)&tmp, 1) <= 0) {
      if (errno) {
	perror("CTR error read first byte from videoSocket");
      }
      else {
	fprintf(stderr, "CTR error: videoSocket closed by VS.\n");
      }
      ComCloseConn(dataSocket);
      ComCloseConn(videoSocket);
      videoSocket = -1;
      return -1;
    }
    if (tmp == CmdFAIL)
    {
      char errmsg[128];
      read_string(videoSocket, errmsg, 128);
      fprintf(stderr, "CTR failed to connect to VS: %s.\n", errmsg);
      ComCloseConn(dataSocket);
      ComCloseConn(videoSocket);
      videoSocket = -1;
      return -1;
    }
    else
    {
      VideoRead((char*)&reply, sizeof(reply));
#ifdef NeedByteOrderConversion
      reply.live = ntohl(reply.live);
      reply.format = ntohl(reply.format);
      reply.totalHeaders = ntohl(reply.totalHeaders);
      reply.totalGroups = ntohl(reply.totalGroups);
      reply.totalFrames = ntohl(reply.totalFrames);
      reply.horizontalSize = ntohl(reply.horizontalSize);
      reply.verticalSize = ntohl(reply.verticalSize);
      reply.pelAspectRatio = ntohl(reply.pelAspectRatio);
      reply.pictureRate1000 = ntohl(reply.pictureRate1000);
      reply.vbvBufferSize = ntohl(reply.vbvBufferSize);
      reply.sizeIFrame = ntohl(reply.sizeIFrame);
      reply.sizePFrame = ntohl(reply.sizePFrame);
      reply.sizeBFrame = ntohl(reply.sizeBFrame);
      reply.sizeSystemHeader = ntohl(reply.sizeSystemHeader);
      reply.sizeGop = ntohl(reply.sizeGop);
      reply.averageFrameSize = ntohl(reply.averageFrameSize);
      reply.firstGopFrames = ntohl(reply.firstGopFrames);
      reply.patternSize = ntohl(reply.patternSize);
#endif
      shared->live += reply.live;
      shared->videoFormat = reply.format;
      shared->totalHeaders = reply.totalHeaders;
      shared->totalFrames = reply.totalFrames;
      shared->totalGroups = reply.totalGroups;
      shared->averageFrameSize = reply.averageFrameSize;
      shared->horizontalSize = reply.horizontalSize;
      shared->verticalSize = reply.verticalSize;
      shared->pelAspectRatio = reply.pelAspectRatio;
      shared->pictureRate = ((double)reply.pictureRate1000) / 1000.0;
      shared->vbvBufferSize = reply.vbvBufferSize;
      shared->firstGopFrames = reply.firstGopFrames;
      shared->patternSize = strlen(reply.pattern);
      if (shared->patternSize == 0) {
	
	Fprintf(stderr, "CTR warning: patternsize %d\n", shared->patternSize);
	
	shared->patternSize = 1;
	shared->pattern[0]  = 'I';
	shared->pattern[1] = 0;
	shared->IframeGap = 1;
      }
      else if (shared->patternSize < PATTERN_SIZE)
      {
	int i;
	char * ptr = shared->pattern + shared->patternSize;
	strncpy(shared->pattern, reply.pattern, shared->patternSize);
	for (i = 1; i < PATTERN_SIZE / shared->patternSize; i ++) {
	  memcpy(ptr, shared->pattern, shared->patternSize);
	  ptr += shared->patternSize;
	}
	shared->IframeGap = 1;
	while (shared->IframeGap < shared->patternSize)
	{
	  if (shared->pattern[shared->IframeGap] == 'I')
	    break;
	  else
	    shared->IframeGap ++;
	}
      }
      else
      {
	fprintf(stderr, "CTR Error: patternSize %d greater than PATTERN_SIZE %d.\n",
		shared->patternSize, PATTERN_SIZE);
	exit(1);
      }
      fprintf(stderr, "Video: %s, %s\n",
	      shared->videoFormat == VIDEO_SIF ? "SIF" :
	      shared->videoFormat == VIDEO_JPEG ? "JPEG" :
	      shared->videoFormat == VIDEO_MPEG1 ? "MPEG1" :
	      shared->videoFormat == VIDEO_MPEG2 ? "MPEG2" : "UNKOWN format",
	      reply.live ? "live source" : "stored source");
	      
      fprintf(stderr, "Video: numS-%d, numG-%d, numF-%d, aveFrameSize-%d\n",
	      reply.totalHeaders, reply.totalGroups, reply.totalFrames,
	      reply.averageFrameSize);
      fprintf(stderr, "Video: maxS-%d, maxG-%d, maxI-%d, maxP-%d, maxB-%d\n",
              reply.sizeSystemHeader, reply.sizeGop,
	      reply.sizeIFrame, reply.sizePFrame, reply.sizeBFrame);
      fprintf(stderr,
	      "Video: SHinfo: hsize-%d, vsize-%d, pelAspect-%d, rate-%f, vbv-%d.\n",
	      reply.horizontalSize, reply.verticalSize, reply.pelAspectRatio,
	      shared->pictureRate, reply.vbvBufferSize);
      shared->pattern[shared->patternSize] = 0;
      fprintf(stderr, "Video: firstGopFrames %d, pattern: %s, IframeGap %d\n",
	      reply.firstGopFrames, reply.pattern, shared->IframeGap);
      shared->pattern[shared->patternSize] = 'I';
      if (reply.totalFrames > MAX_FRAMES && (!shared->live))
      {
	fprintf(stderr,
		"Error: totalFrames %d > MAX_FRAMES %d, needs change and recompile.\n",
		reply.totalFrames, MAX_FRAMES);
	ComCloseConn(dataSocket);
	ComCloseConn(videoSocket);
	videoSocket = -1;
	return -1;
      }
  
    }

    /* create VB, and put INIT frame to VB*/
    {
      int sp[2];  /* sp[0] is for CTR and sp[1] is for VB */
      
      /* create command socket pair for sending INIT frame to VB, the pipe
       should be discard/non-discard in consistent with videoSocket*/
      if (socketpair(AF_UNIX,
		     shared->videoMaxPktSize >= 0 ? SOCK_STREAM :
		     SOCK_DGRAM, 0, sp) == -1)
      {
	perror("CTR error on open CTR-VB socketpair");
	exit(1);
      }
      
      switch (VBpid = fork())
      {
      case -1:
	perror("CTR error on forking VB process");
	exit(1);
	break;
      case 0:
	if (realTimeFlag) {
	  SetRTpriority("VB", -1);
	}
	free(vh);
	free(vf);
	free(ah);
	free(af);
	close(sp[0]);
	ComCloseFd(videoSocket);
	if (audioSocket >= 0)
	  ComCloseFd(audioSocket);
	ABdeleteBuf();
	VDdeleteBuf();
	if (cmdSocket >= 0)
	  close(cmdSocket);
	if (realTimeFlag >= 2) {
#ifdef __svr4__
	  if (SetRTpriority("VB", 0)) realTimeFlag = 0;
#elif defined(_HPUX_SOURCE)
	  if (SetRTpriority("VB", 1)) realTimeFlag = 0;
#endif
	}
	VBprocess(sp[1], dataSocket);
	break;
      default:
	close(sp[1]);
	ComCloseFd(dataSocket);
	{
	  int bytes, res;
	  /* passing all messages of INIT frame to VB here. */
	  char * buf = (char *)malloc(INET_SOCKET_BUFFER_SIZE);
	  VideoMessage *msg = (VideoMessage *)buf;
	  int pkts = 1, msgo = 0, msgs = 0;
	  
	  if (buf == NULL) {
	    perror("CTR error on malloc() for INIT frame");
	    exit(1);
	  }
	  while (msgo + msgs < pkts) {
	    VideoRead(buf, sizeof(*msg));
	    pkts = ntohl(msg->packetSize);
	    msgo = ntohl(msg->msgOffset);
	    msgs = ntohl(msg->msgSize);
	    if (shared->videoMaxPktSize >= 0) {  /* non-discard mode */
	      write_bytes(sp[0], buf, sizeof(*msg));
	      bytes = msgs;
	      while (bytes > 0) {
		int size = min(bytes, INET_SOCKET_BUFFER_SIZE);
		VideoRead(buf, size);
		write_bytes(sp[0], buf, size);
		bytes -= size;
	      }
	    }
	    else {
	      VideoRead(buf + sizeof(*msg), msgs);
	      bytes = sizeof(*msg) + msgs;
	      while ((res = write(sp[0], buf, bytes)) == -1) {
		if (errno == EINTR || errno == ENOBUFS) continue;
		perror("CTR error on sending INIT frame to VB");
		exit(1);
	      }
	      if (res < bytes) {
		fprintf(stderr, "CTR warn: send() res %dB < bytes %dB\n", res, bytes);
	      }
	      /*
		 Fprintf(stderr,
			 "CTR transferred INIT frame to VB: pkts %d, msgo %d, msgs %d\n",
			 pkts, msgo, msgs);
		*/
	    }
	  }
	  read(sp[0], buf, 1); /* read a garbage byte, to sync with VB */
	  close(sp[0]);
	  free(buf);
	}
	break;
      }
    }
  }
#ifdef STAT
  if (shared->config.collectFrameInfo && (!shared->live))
  {
    int i;
    int count = 0;
    char ch;
    char buf[100];
    FILE *fp;
    
    for (;;)
    {
      sprintf(buf, "struct.%d", count++);
      if (access(buf, 0))
        break;
      if (count > 10000)
      {
	fprintf(stderr, "CTR generating struct file, weired thing happened.\n");
	exit(1);
      }
    }
    fprintf(stderr, "MPEG info collected to %s. . .", buf);
    fp = fopen(buf, "w");
    if (fp == NULL)
    {
      fprintf(stderr, "CTR failed to open %s for write.\n", buf);
      perror("");
      exit(1);
    }
    {
      time_t val = time(NULL);
      get_hostname(buf, 100);
      buf[99] = 0;
      fprintf(fp, "ClientHost: %s\n", buf);
      fprintf(fp, "Date: %s\n", ctime(&val));
    }
    fprintf(fp, "VideoHost: %s\nVideoFile: %s\n", vh, vf);
    fprintf(fp, "AudioHost: %s\nAudioFile: %s\n\n", ah, af);
    fprintf(fp, "TotalFrames: %d\nTotalGroups: %d\n",
	    shared->totalFrames, shared->totalGroups);
    fprintf(fp, "TotalHeaders: %d\n", shared->totalHeaders);
    fprintf(fp, "PictureRate: %f\nPictureSize: %d x %d\n",
	    shared->pictureRate, shared->horizontalSize, shared->verticalSize);
    fprintf(fp, "AverageFrameSize: %d\n", shared->averageFrameSize);
    shared->pattern[shared->patternSize] = 0;
    fprintf(fp, "Pattern(%d frames): %s\n\n", shared->patternSize, shared->pattern);
    shared->pattern[shared->patternSize] = 'I';
    {
      fprintf(fp, "FrameInfo:\n      ");
      for (i = 0; i < 10; i++)
        fprintf(fp, " %-6d", i);
      fprintf(fp, "\n      ----------------------------------------------------");
      ch = CmdSTATstream;
      VideoWrite(&ch, 1);
      for (i = 0; i < shared->totalFrames; i++)
      {
	short size;
	VideoRead(&ch, 1);
	VideoRead((char*)&size, 2);
	size = ntohs(size);
	if (i % 10 == 0)
          fprintf(fp, "\n%4d: ", i / 10);
	fprintf(fp, "%c%-6d", ch, (int)size);
      }
    }
  }
#endif
  return 0;
}

static int InitAudioChannel(char * phostname, char * af)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) InitAudioChannel called\n"));
  int dataSocket = -1;

  if (!hasAudioDevice)
  {
    fprintf(stderr, "CTR warning: Audio device not available, Audio ignored.\n");
    return -1;
  }

  if (ComOpenConnPair(phostname, &audioSocket,
		      &dataSocket, &shared->audioMaxPktSize) == -1) {
    return -1;
  }
  
  /* Initialize with AS */
  {
    INITaudioPara para;
    INITaudioReply reply;
    unsigned char tmp;
    //     tmp = CmdINITaudio;
    //     AudioWrite(&tmp, 1);
    para.sn = htonl(shared->cmdsn);
    para.version = htonl(VERSION);
    para.para.encodeType = htonl(shared->AFPara.encodeType);
    para.para.channels = htonl(shared->AFPara.channels);
    para.para.samplesPerSecond = htonl(shared->AFPara.samplesPerSecond);
    para.para.bytesPerSample = htonl(shared->AFPara.bytesPerSample);
    para.nameLength = htonl(strlen(af)+1);
    /*
    fprintf(stderr, "AF Audio para: encode %d, ch %d, sps %d, bps %d.\n",
	    para.para.encodeType, para.para.channels,
	    para.para.samplesPerSecond, para.para.bytesPerSample);
     */
    AudioWrite(&para, sizeof(para));
    AudioWrite(af, strlen(af)+1);
    /*
    fprintf(stderr, "CTR: InitCommand to AS written.\n");
    */
    /*
    AudioRead((char*)&tmp, 1);
    */
    errno = 0;
    if (read(audioSocket, (char*)&tmp, 1) <= 0) {
      if (errno) {
	perror("CTR error read first byte from audioSocket");
      }
      else {
	fprintf(stderr, "CTR error: audioSocket closed by AS.\n");
      }
      ComCloseConn(audioSocket);
      ComCloseConn(dataSocket);
      videoSocket = -1;
      return -1;
    }
    if (tmp == CmdFAIL)
    {
      char errmsg[128];
      read_string(audioSocket, errmsg, 128);
      fprintf(stderr, "CTR failed to connect to AS: %s\n", errmsg);
      ComCloseConn(audioSocket);
      ComCloseConn(dataSocket);
      audioSocket = -1;
      return -1;
    }
    else
    {
      int flag = 1;
      AudioRead((char*)&reply, sizeof(reply));
      shared->live += ntohl(reply.live);
      shared->audioFormat = ntohl(reply.format);
      shared->audioPara.encodeType = ntohl(reply.para.encodeType);
      shared->audioPara.channels = ntohl(reply.para.channels);
      shared->audioPara.samplesPerSecond = ntohl(reply.para.samplesPerSecond);
      shared->audioPara.bytesPerSample = ntohl(reply.para.bytesPerSample);
      shared->totalSamples = ntohl(reply.totalSamples);
      
      fprintf(stderr, "Audio: samples %d, sps %d, bps %d\n",
	      shared->totalSamples, shared->AFPara.samplesPerSecond,
	      shared->AFPara.bytesPerSample);
      
      SetAudioParameter(&shared->audioPara);
    }
  
    /* create AB */
    {
      switch (ABpid = fork())
      {
      case -1:
	perror("CTR error on forking AB process");
	exit(1);
	break;
      case 0:
	if (realTimeFlag) {
	  SetRTpriority("AB", -1);
	}
	free(vh);
	free(vf);
	free(ah);
	free(af);
	ComCloseFd(audioSocket);
	if (videoSocket >= 0)
	  ComCloseFd(videoSocket);
	VBdeleteBuf();
	VDdeleteBuf();
	if (cmdSocket >= 0)
	  close(cmdSocket);
	if (realTimeFlag >= 2) {
#ifdef __svr4__
	  if (SetRTpriority("AB", 0)) realTimeFlag = 0;
#elif defined(_HPUX_SOURCE)
	  if (SetRTpriority("AB", 1)) realTimeFlag = 0;
#endif
	}
	ABprocess(dataSocket);
	break;
      default:
	ComCloseFd(dataSocket);
	ABflushBuf(0);
	break;
      }
    }
  }
  return 0;
}

static void set_speed(void)
{
  int val;
  if (shared->live) {
    shared->speedPosition = 50;
  }
  val = shared->speedPosition;
  if (val == 0)
    val = 1;
  if (shared->cmd == CmdPLAY && audioSocket >= 0 && videoSocket >= 0 && rtplay) {
    /* limit audio signal period to no more than 1.0 seconds, because AudioFile
       will block if one send more than 4 seconds of sample ahead of current
       time. and our timing control is not precise.
       */
    double s = ((1.0 / shared->pictureRate) * 50.0) *
            (double)max(shared->config.framesPerAudioPlay, 1) / 1.0;
    if (s > 50.0) {
      fprintf(stderr, "CTR warning: lower value of parameter framesPerAudioPlay,\n");
      fprintf(stderr, "             otherwise the Player may not work properly.\n");
    }
    if (val < s) {
      if (shared->config.verbose) {
	if (val < s * 1.0 / 2.0) {
	  val = (int)s;
	  fprintf(stderr, "CTR warning: speed too low, set to %d scale val.\n", val);
	}
	else {
	  fprintf(stderr, "CTR warning: audio signal period %3.2f > 1 sec.\n",
		  1.0 * (float)s / (float) val);
	  fprintf(stderr, "    lower framesPerAudioPlay or increase speed.\n");
	}
      }
      else {
	val = (int)s;
      }
    }
  }
  if (videoSocket >= 0)
  {
    float fps;
    if (val <= 50)
      fps = shared->pictureRate * (float)val / 50.0;
    else
    {
      float rate = shared->config.maxFPS - shared->pictureRate;
      fps = shared->pictureRate + (float)rate * (float)(val - 50) / 50.0;
                    /* only B frames are to be dropped */
    }
    Fprintf(stderr, "Play speed video %5.1ffps\n", fps);
    shared->framesPerSecond = (int)fps;
    shared->usecPerFrame = (int) (1000000.0/fps);
    if (audioSocket >= 0)
    {
      float sps = shared->audioPara.samplesPerSecond * fps / shared->pictureRate;
      shared->samplesPerSecond = (int)sps;
      shared->usecPerSample = (int)(1000000.0/sps);
    }
  }
  else if (audioSocket >= 0)
  {
    float sps;
    if (val <= 50)
      sps = (float)shared->audioPara.samplesPerSecond * (float)val / 50.0;
    else
    {
      int rate = shared->config.maxSPS - shared->audioPara.samplesPerSecond;
      sps = shared->audioPara.samplesPerSecond + (float)rate * (float)(val - 50) / 50.0;
    }
    Fprintf(stderr, "Play speed audio %5.1fsps\n", sps);
    shared->samplesPerSecond = (int)sps;
    shared->usecPerSample = (int)(1000000.0/sps);
  }
}

static void init()
{
  int i, j;
 
  /* try to stop and close previous playing */
  if (audioSocket >= 0 || videoSocket >= 0)
  {
    unsigned char tmp = CmdCLOSE;
    stop_playing();
    
    if (audioSocket >= 0)
    {
      if (ABpid > 0) {
        kill(ABpid, SIGUSR1);
        ABpid = -1;
      }
      usleep(10000);
      AudioWrite(&tmp, 1);
      ComCloseConn(audioSocket);
      audioSocket = -1;
    }
    
    if (videoSocket >= 0)
    {
      if (VBpid > 0) {
        kill(VBpid, SIGUSR1);
        VBpid = -1;
      }
      usleep(10000);
      VideoWrite(&tmp, 1);
      ComCloseConn(videoSocket);
      videoSocket = -1;
      while ((!VBbufEmpty()) || !VDbufEmpty()) {
        while (VDpeekMsg() != NULL) {
          VDreclaimMsg(VDgetMsg());
        }
        usleep(10000);
      }
      usleep(10000);
    }
  }
 
  /* read in video/audio files */
  NewCmd(CmdINIT);
  CmdRead((char*)&i, 4);
  CmdRead(vh, i);
  vh[i] = 0;
  CmdRead((char*)&i, 4);
  CmdRead(vf, i);
  vf[i] = 0;
  CmdRead((char*)&i, 4);
  CmdRead(ah, i);
  ah[i] = 0;
  CmdRead((char*)&i, 4);
  CmdRead(af, i);
  af[i] = 0;
  /*
  fprintf(stderr, "INIT: vh-%s, vf-%s, ah-%s, af-%s\n", vh, vf, ah, af);
  */
 
  shared->live = 0;
  shared->audioMaxPktSize = !shared->config.audioConn;
  shared->videoMaxPktSize = !shared->config.videoConn;
  
  if (af[0] != 0)
  {
    if (InitAudioChannel(ah, af))
    {
      audioSocket = -1;
      shared->totalSamples = 0;
    }
    else
    {
      shared->nextSample = 0;
      if (shared->config.maxSPS < shared->audioPara.samplesPerSecond)
        shared->config.maxSPS < shared->audioPara.samplesPerSecond;
    }
  }
  else
  {
    shared->totalSamples = 0;
    audioSocket = -1;
  }
  if (vf[0] != 0)
  {
    if (InitVideoChannel(vh, vf))
    {
      shared->totalFrames = 0;      /* disable video channel */
      videoSocket = -1;
    }
    else
    {
      shared->nextFrame = 1;
      shared->nextGroup = 0;
      shared->currentFrame = shared->currentGroup = shared->currentDisplay = 0;
      if (shared->config.maxFPS < shared->framesPerSecond)
        shared->config.maxFPS = shared->framesPerSecond;
    }
  }
  else
  {
    videoSocket = -1;
    shared->totalFrames = 0;  /* disable video channel */
  }
  if (audioSocket < 0 && videoSocket < 0)  /* none of video/audio channels is setup */
  {
    unsigned char tmp = CmdFAIL;
    CmdWrite(&tmp, 1);
    /*
    fprintf(stderr, "CTR initialization failed.\n");
    */
    return;
  }
  else
  {
    unsigned char tmp = CmdDONE;
    set_speed();
    if (videoSocket >= 0)
      wait_display();
    CmdWrite(&tmp, 1);
    if (videoSocket < 0)
    {
      tmp = CmdVPclearScreen;
      CmdWrite(&tmp, 1);
    }
     return;
   }
}
 
static void stop(void)
{
#ifdef STAT
  unsigned char preCmd = shared->cmd;
#endif
  unsigned char tmp = CmdDONE;
  /*
  fprintf(stderr, "CTR: STOP . . .\n");
  */
  stop_playing();
 
  if (shared->live && videoSocket >= 0) {
    Fprintf(stderr, "CTR live video stat: average disp frame rate: %5.2f fps\n",
            shared->pictureRate * displayedFrames / shared->nextFrame);
  }
  CmdWrite(&tmp, 1);
  
#ifdef STAT
  if (shared->collectStat && preCmd == CmdPLAY && videoSocket >= 0)
  {
    int i;
    int count = 0;
    char ch;
    char buf[100];
    FILE *fp;
    
    for (;;)
    {
      sprintf(buf, "stat.%02d", count++);
      if (access(buf, 0))
        break;
      if (count > 10000)
      {
        fprintf(stderr, "CTR generating stat file, weired thing happened.\n");
        exit(1);
      }
    }
    fprintf(stderr, "Statistics is being collected to file %s. . .", buf);
    fp = fopen(buf, "w");
    if (fp == NULL)
    {
      fprintf(stderr, "CTR failed to open %s for write.\n", buf);
      perror("");
      exit(1);
    }
    {
      time_t val = time(NULL);
      get_hostname(buf, 100);
      buf[99] = 0;
      fprintf(fp, "ClientHost: %s\n", buf);
      fprintf(fp, "Date: %s\n", ctime(&val));
    }
    fprintf(fp, "VideoHost: %s\nVideoFile: %s\n", vh, vf);
    fprintf(fp, "AudioHost: %s\nAudioFile: %s\n\n", ah, af);
    fprintf(fp, "TotalFrames: %d\nTotalGroups: %d\n",
            shared->totalFrames, shared->totalGroups);
    fprintf(fp, "TotalHeaders: %d\n", shared->totalHeaders);
    fprintf(fp, "PictureRate: %f\nPictureSize: %d x %d\n",
            shared->pictureRate, shared->horizontalSize, shared->verticalSize);
    fprintf(fp, "AverageFrameSize: %d\n", shared->averageFrameSize);
    shared->pattern[shared->patternSize] = 0;
    fprintf(fp, "Pattern(%d frames): %s\n", shared->patternSize, shared->pattern);
    shared->pattern[shared->patternSize] = 'I';
 
    fprintf(fp, "\nStartPlayRoundTripDelay: %d (millisec)\n",
            shared->playRoundTripDelay);
    fprintf(fp, "VBmaxBytes: %d\nVBdroppedFrames: %d\n",
            shared->stat.VBmaxBytes, shared->stat.VBdroppedFrames);
    fprintf(fp, "VBemptyTimes: %d\nVDlastFrameDecoded: %d\n",
            shared->stat.VBemptyTimes, shared->stat.VDlastFrameDecoded);
    
    fprintf(fp, "\nVDframesDroppedWithoutReference: %d\n",
            shared->stat.VDnoRef);
    fprintf(fp, "VDframesDroppedAgainstSendPattern: %d\n",
            shared->stat.VDagainstSendPattern);
    fprintf(fp, "VDIframesDroppedTooLate: %d\n",
            shared->stat.VDtooLateI);
    fprintf(fp, "VDPframesDroppedTooLate: %d\n",
            shared->stat.VDtooLateP);
    fprintf(fp, "VDBframesDroppedTooLate: %d\n",
            shared->stat.VDtooLateB);
 
    fprintf(fp, "CTRframesDisplayedOnTime: %d\n", shared->stat.CTRdispOnTime);
    fprintf(fp, "CTRframesDisplayedLate:   %d\n", shared->stat.CTRdispLate);
    fprintf(fp, "CTRframesDroppedOutOrder: %d\n", shared->stat.CTRdropOutOrder);
    fprintf(fp, "CTRframesDroppedLate:     %d\n", shared->stat.CTRdropLate);
    
    fprintf(fp,
"\nSpeedChangeHistory:\n(frameId, UPF, FPS, frameRateLimit, frames, dropped):\n");
    for (i = 0; i < min(speedPtr, SPEEDHIST_SIZE); i ++)
      fprintf(fp, "%-4d %-6d %6.2f %6.2f %-4d %d\n",
              speedHistory[i].frameId, speedHistory[i].usecPerFrame,
              1000000.0 / (double)speedHistory[i].usecPerFrame,
              speedHistory[i].frameRateLimit,
              speedHistory[i].frames, speedHistory[i].framesDropped);
    if (speedPtr > SPEEDHIST_SIZE)
      fprintf(fp, "Actual speed change times: %d (>%d)\n",
              speedPtr, SPEEDHIST_SIZE);
 
    fprintf(fp, "\nVDbufferFillLevel:\n(frames, times):\n");
    for (i = 0; i < MAX_VDQUEUE_SIZE; i ++) {
      if (shared->stat.VDqueue[i]) {
        fprintf(fp, "%-6d %d\n", i, shared->stat.VDqueue[i]);
      }
    }
 
    fprintf(fp, "\nVBmessageGap:\n(width, times):\n");
    {
      for (i = 0; i <= MSGGAP_MAX  - MSGGAP_MIN; i ++) {
        if (shared->stat.VBmsgGaps[i]) {
          fprintf(fp, "%-6d %d\n", i + MSGGAP_MIN, shared->stat.VBmsgGaps[i]);
        }
      }
    }
    
    if (shared->stat.fbPacketNumber > 0)
    {
      fprintf(fp,
"\nFeedbackPackets:\n(fId,addUPF,addf,rateLimit,frames,fdropped,advance):\n");
      for (i = 0; i < min(shared->stat.fbPacketNumber, MAX_FB_PACKETS); i++)
        fprintf(fp, "%-6d %-6d %-6d %6.2f %4d %4d %d\n",
                shared->stat.fbPackets[i].frameId,
                shared->stat.fbPackets[i].addUsecPerFrame,
                shared->stat.fbPackets[i].addFrames,
                shared->stat.fbPackets[i].frameRateLimit,
                shared->stat.fbPackets[i].frames,
                shared->stat.fbPackets[i].framesDropped,
                shared->stat.fbPackets[i].advance);
      if (shared->stat.fbPacketNumber > MAX_FB_PACKETS)
        fprintf(fp, "Actual # of FB packets: %d\n", shared->stat.fbPacketNumber);
    }
 
    ch = CmdSTATsent;
    VideoWrite(&ch, 1);
    fprintf(fp, "\n\nVSFramesSent:\n       ");
    for (i = 0; i < 8; i++)
      fprintf(fp, "%-10d", i * 10);
    fprintf(fp, "\n       -----------------------------------------------------");
    count = 0;
    for (i = 0; i < (shared->totalFrames + 7)/8; i ++)
    {
      int j;
      VideoRead(&ch, 1);
      if (i % 10 == 0)
        fprintf(fp, "\n%5d: ", i * 8);
      for (j = 0; j < 8; j++)
      {
        if (ch & (1 << j))
        {
          count ++;
          fputc('x', fp);
        }
        else
          fputc('-', fp);
      }
    }
    fprintf(fp, "\nVSTotalFramesSent: %d\n", count);
    fprintf(fp, "\nVBFramesReceived:\n       ");
    for (i = 0; i < 8; i++)
      fprintf(fp, "%-10d", i * 10);
    fprintf(fp, "\n       -----------------------------------------------------");
    count = 0;
    for (i = 0; i < (shared->totalFrames + 7)/8; i ++)
    {
      int j;
      if (i % 10 == 0)
        fprintf(fp, "\n%5d: ", i * 8);
      for (j = 0; j < 8; j++)
      {
        if (shared->stat.VBframesReceived[i] & (1 << j))
        {
          count ++;
          fputc('x', fp);
        }
        else
          fputc('-', fp);
      }
    }
    fprintf(fp, "\nVBTotalFramesReceived: %d\n", count);
    fprintf(fp, "\nVDFramesDecoded:\n       ");
    for (i = 0; i < 8; i++)
      fprintf(fp, "%-10d", i * 10);
    fprintf(fp, "\n       -----------------------------------------------------");
    count = 0;
    for (i = 0; i < (shared->totalFrames + 7)/8; i ++)
    {
      int j;
      if (i % 10 == 0)
        fprintf(fp, "\n%5d: ", i * 8);
      for (j = 0; j < 8; j++)
      {
        if (shared->stat.VDframesDecoded[i] & (1 << j))
        {
          count ++;
          fputc('x', fp);
        }
        else
          fputc('-', fp);
      }
    }
    fprintf(fp, "\nVDTotalFramesDecoded: %d\n", count);
    fprintf(fp, "\nVPFramesDisplayed:\n       ");
    for (i = 0; i < 8; i++)
      fprintf(fp, "%-10d", i * 10);
    fprintf(fp, "\n       -----------------------------------------------------");
    count = 0;
    for (i = 0; i < (shared->totalFrames + 7)/8; i ++)
    {
      int j;
      if (i % 10 == 0)
        fprintf(fp, "\n%5d: ", i * 8);
      for (j = 0; j < 8; j++)
      {
        if (shared->stat.VPframesDisplayed[i] & (1 << j))
        {
          count ++;
          fputc('x', fp);
        }
        else
          fputc('-', fp);
      }
    }
    fprintf(fp, "\nVPTotalFramesDisplayed: %d\n", count);
 
    fprintf(fp, "\nVBBufferFillLevelHistory:\n       ");
    for (i = 0; i < 10; i ++)
      fprintf(fp, "%-7d", i);
    fprintf(fp, "\n       -----------------------------------------------------");
    for (i = 0; i < shared->totalFrames; i++)
    {
      if (i % 10 == 0)
        fprintf(fp, "\n%5d: ", i / 10);
      if (shared->stat.VBfillLevel[i] == SHRT_MIN)
        fprintf(fp, "x      ");
      else
        fprintf(fp, "%-7d", shared->stat.VBfillLevel[i]);
    }
    fprintf(fp, "\nHistoryEnd\n");
    fclose(fp);
    fprintf(stderr, "Statistics collecting done.\n");
  }
#endif
  
  return;
}

static void ff(void)
{
  unsigned char tmp;
  FFpara para;
  /*
  fprintf(stderr, "CTR: FF . . .\n");
  */
  if (shared->live) {
    beep();
  }
  else {
    stop_playing();
    if (shared->nextGroup < 0)
      shared->nextGroup = 0;
    if (videoSocket >= 0 && shared->nextGroup < shared->totalGroups)
    {
      NewCmd(CmdFF);
      shared->needHeader = 0;
      shared->framesPerSecond = shared->config.ffFPS /
                             shared->patternSize;
      shared->usecPerFrame = (int)(1000000.0 / (float)shared->config.ffFPS) *
                             shared->patternSize;
 
      shared->VStimeAdvance =
          max(shared->config.VStimeAdvance, DEFAULT_VStimeAdvance) * 1000;
      if (shared->VStimeAdvance < shared->usecPerFrame)
        shared->VStimeAdvance = shared->usecPerFrame;
 
      para.VStimeAdvance = htonl(shared->VStimeAdvance);
      para.sn = htonl(shared->cmdsn);
      para.nextGroup = htonl(shared->nextGroup);
      para.usecPerFrame = htonl(shared->usecPerFrame);
      para.framesPerSecond = htonl(shared->framesPerSecond);
      tmp = CmdFF;
      startTime = get_usec();
      VideoWrite(&tmp, 1);
      VideoWrite(&para, sizeof(para));
      start_timer();
    }
  }
  tmp = CmdDONE;
  CmdWrite(&tmp, 1);
  return;
}

static void fb(void)
{
  unsigned char tmp;
  FFpara para;
  /*
  fprintf(stderr, "CTR: FB . . .\n");
  */
  if (shared->live) {
    beep();
  }
  else {
    stop_playing();
    if (shared->nextGroup >= shared->totalGroups)
      shared->nextGroup = shared->totalGroups - 1;
    if (videoSocket >= 0 && shared->nextGroup >= 0)
    {
      NewCmd(CmdFB);
      shared->needHeader = 0;
      shared->framesPerSecond = shared->config.fbFPS /
                             shared->patternSize;
      shared->usecPerFrame = (int)(1000000.0 / (float)shared->config.fbFPS) *
                             shared->patternSize;
 
      shared->VStimeAdvance =
          max(shared->config.VStimeAdvance, DEFAULT_VStimeAdvance) * 1000;
      if (shared->VStimeAdvance < shared->usecPerFrame)
        shared->VStimeAdvance = shared->usecPerFrame;
 
      para.VStimeAdvance = htonl(shared->VStimeAdvance);
      para.sn = htonl(shared->cmdsn);
      para.nextGroup = htonl(shared->nextGroup);
      para.usecPerFrame = htonl(shared->usecPerFrame);
      para.framesPerSecond = htonl(shared->framesPerSecond);
      tmp = CmdFB;
      startTime = get_usec();
      VideoWrite(&tmp, 1);
      VideoWrite(&para, sizeof(para));
      start_timer();
    }
  }
  tmp = CmdDONE;
  CmdWrite(&tmp, 1);
  return;
}
 
static void step()
{
  unsigned char tmp;
  STEPpara para;
  stop_playing();
  NewCmd(CmdSTEP);
  if (videoSocket >= 0 && shared->nextFrame <= shared->totalFrames)
  { /* when shared->nextFrame == shared->totalFrame, it will force VS to send a ENDSEQ,
       to let VD give out the remaining frame in its ring[] buffer */
    para.sn = htonl(shared->cmdsn);
    para.nextFrame = htonl(shared->nextFrame);
    /*
    fprintf(stderr, "CTR: STEP . . . frame-%d\n", para.nextFrame);
    */
    tmp = CmdSTEP;
    VideoWrite(&tmp, 1);
    VideoWrite(&para, sizeof(para));
    wait_display();
  }
  tmp = CmdDONE;
  CmdWrite(&tmp, 1);
  return;
}
 
static void play(int auto_exp)
{
  unsigned char tmp;
  unsigned ats, vts;
  int cmdstarted = 0;
  int stuffsamples = 0;
  /*
  fprintf(stderr, "CTR: PLAY . . .\n");
  */
  stop_playing();
 
  if (!shared->live && !shared->config.rt && videoSocket >= 0) {
    /* rtplay turned off only when video avaible and not want RT play */
    rtplay = 0;
    fprintf(stderr, "VCR is not playing at in realtime mode, audio disabled\n");
  }
  else {
    rtplay = 1;
  }
  
  if (shared->live) {
    rtplay = 1;
    shared->nextFrame = 0;
    shared->nextSample = 0;
  }
 
  shared->rtplay = rtplay;
  
  if (shared->nextFrame < 0)
    shared->nextFrame = 0;
  else if (shared->nextFrame >= shared->totalFrames)
    shared->nextFrame = shared->totalFrames - 1;
 
  if (audioSocket >= 0 && shared->nextSample < shared->totalSamples && rtplay)
  {
    PLAYaudioPara para;
    if (cmdstarted == 0)
    {
      NewCmd(CmdPLAY);
      if (!auto_exp) set_speed();
      cmdstarted = 1;
    }
 
    if (videoSocket >= 0 && rtplay && !shared->live) {
      /* video channel also active, recompute nextSample */
      shared->nextSample =(int) ((double)shared->audioPara.samplesPerSecond *
                           ((double)shared->nextFrame / shared->pictureRate));
      shared->nextSample += shared->config.audioOffset;
      if (shared->nextSample < 0) {
        stuffsamples = (- shared->nextSample);
        shared->nextSample = 0;
      }
      else if (shared->nextSample >= shared->totalSamples)
        shared->nextSample = shared->totalSamples - 1;
    }
 
    ABflushBuf(shared->nextSample);
    
    para.sn = htonl(shared->cmdsn);
    para.nextSample = htonl(shared->nextSample);
    para.samplesPerSecond = htonl(shared->samplesPerSecond);
    para.samplesPerPacket = htonl(1024 / shared->audioPara.bytesPerSample);
    para.ABsamples = htonl(AB_BUF_SIZE / shared->audioPara.bytesPerSample);
    para.spslimit = htonl(32000);
    
    startTime = get_usec();
    tmp = CmdPLAY;
    AudioWrite(&tmp, 1);
    AudioWrite(&para, sizeof(para));
    read_int(audioSocket, (int *)&ats);
  }
 
  if (videoSocket >= 0 && shared->nextFrame < shared->totalFrames)
  {
    PLAYpara para;
    if (cmdstarted == 0)
    {
      NewCmd(CmdPLAY);
      if (!auto_exp) set_speed();
      cmdstarted = 1;
    }
    shared->VBheadFrame = -1;
    shared->needHeader = 0;
    {
      int i = shared->config.maxSPframes;
      i = (int) ((double)i * (1000000.0 / (double)shared->usecPerFrame) /
          shared->pictureRate);
      shared->sendPatternGops = max(min(i, PATTERN_SIZE) / shared->patternSize, 1);
    }
    cmdstarted = 1;
#ifdef STAT
    shared->collectStat = (shared->config.collectStat && (!shared->live));
    if (shared->collectStat)
    {
      int i;
      memset(&(shared->stat), 0, sizeof(shared->stat));
      shared->stat.VDlastFrameDecoded = (unsigned)-1;
      for (i = 0; i < MAX_FRAMES; i++)
        shared->stat.VBfillLevel[i] = SHRT_MIN;
      speedPtr = 0;
    }
#endif
    shared->VStimeAdvance =
        max(shared->config.VStimeAdvance, DEFAULT_VStimeAdvance) * 1000;
    if (shared->VStimeAdvance < shared->usecPerFrame)
      shared->VStimeAdvance = shared->usecPerFrame;
    
    para.VStimeAdvance = htonl(shared->VStimeAdvance);
    para.sn = htonl(shared->cmdsn);
    para.nextFrame = htonl(shared->nextFrame);
    para.usecPerFrame = htonl(shared->usecPerFrame);
    para.framesPerSecond = htonl(shared->framesPerSecond);
    para.collectStat = htonl(shared->collectStat);
    frate = shared->config.frameRateLimit;
    if (frate <= 0.0) {
      frate = 1.0;
    }
    shared->frameRateLimit = frate;
    para.frameRateLimit1000 =
           htonl((long)(shared->frameRateLimit * 1000.0));
    compute_sendPattern();
    para.sendPatternGops = htonl(shared->sendPatternGops);
    memcpy(para.sendPattern, shared->sendPattern, PATTERN_SIZE);
    startTime = get_usec();
    tmp = CmdPLAY;
    VideoWrite(&tmp, 1);
    VideoWrite(&para, sizeof(para));
    read_int(videoSocket, (int *)&vts);
 
    if (shared->config.qosEffective) {
      /*
      fprintf(stderr, "CTR start FeedBack with init frameRateLimit %lf\n",
              frate);
      */
      maxfr = frate;    /* max frame rate all the time during one playback */
      minupf = (int) (1000000.0 / maxfr);  /* min usec-per-frame all the time
                                      during one playback */
      maxrate = (double)minupf / (double)max(shared->usecPerFrame, minupf);
                /* this is current max frame rate in percentage against 'maxfr',
                   current max frame rate is the lower of 'maxfr' and play speed */
      frate = 1.0; /* current sending frame rate, in percentage against 'maxfr'
                    This value is set with init value as 1.0, so that if current
                    speed is lower than frameRateLimit, no frames are dropped,
                    then when play speed increases frame rate will increase
                    accordingly until frames are dropped*/
      adjstep = ((double)minupf / (double)shared->usecPerFrame) /
          (double)max(shared->patternSize * shared->sendPatternGops, 5);
               /* adjust step for current play speed, in percentage against
                  'maxfr' */
      
      fbstate = 1;
      fb_startup = 1;
      
      /*
      fprintf(stderr, "CTR init frate: %lf minupf %d, shared->upf %d\n",
              frate, minupf, shared->usecPerFrame);
       */
    }
  }
  
  if (shared->live && (videoSocket >= 0) && (audioSocket >= 0)) {
    int gap = get_duration(ats, vts);
    if (gap < 0 || gap >= 5000000) {
      Fprintf(stderr, "Error for live source: ats %u, vts %u, gap %d\n",
              ats, vts, gap);
    }
    else {
      int skipped = gap * shared->audioPara.samplesPerSecond / 1000000;
      skipped += shared->config.audioOffset;
      ABskipSamples(skipped);
      Fprintf(stderr, "Live source: skipped %d audio samples\n", skipped);
    }
  }
  else if (stuffsamples) {
    ABskipSamples(-stuffsamples);
  }
  if (cmdstarted)
    start_timer();
  tmp = CmdDONE;
  CmdWrite(&tmp, 1);
  return;
}
 
static void position_action(int operation_tag)
{
  int val;
  unsigned char tmp = CmdDONE;
  CmdRead((char*)&val, 4);
  if (shared->live) {
    beep();
  }
  else {
    shared->locationPosition = val;
    stop_playing();
    NewCmd(CmdPOSITION);
    if (videoSocket >= 0)
    {
      int gop = shared->nextGroup;
      POSITIONpara para;
      shared->nextGroup = ((shared->totalGroups-1) * val) / POSITION_RANGE;
      /*
      fprintf(stderr, "CTR: POSITION%s %d (nextGop %d). . .\n",
              operation_tag ? "_released" : "", val, shared->nextGroup);
      */
      if (gop != shared->nextGroup || operation_tag)
      {
        shared->nextFrame = ((shared->totalFrames-1) * val) / POSITION_RANGE;
        para.sn = htonl(shared->cmdsn);
        para.nextGroup = htonl(shared->nextGroup);
        tmp = CmdPOSITION;
        VideoWrite(&tmp, 1);
        VideoWrite(&para, sizeof(para));
        if (operation_tag)  /* release or LOOPrewind */
          wait_display();
      }
      if (operation_tag && audioSocket >= 0) /* needs to adjust audio position */
      {
        shared->nextSample = (int)((double)shared->audioPara.samplesPerSecond *
                             ((double)shared->nextFrame / shared->pictureRate));
      }
    }
    else if (audioSocket >= 0)
      shared->nextSample  = (shared->totalSamples-1) * val / POSITION_RANGE;
  }
  tmp = CmdDONE;
  CmdWrite(&tmp, 1);
  return;
}
 
static void position()
{
  position_action(0);
}
 
static void position_release()
{
  position_action(1);
}
 
static void volume()
{
  CmdRead((char *)&shared->volumePosition, 4);
  if (audioSocket >= 0) {
    SetAudioGain();
  }
  /*
  unsigned char tmp = CmdDONE;
  tmp = CmdDONE;
  CmdWrite(&tmp, 1);
  */
  return;
}
 
static void balance()
{
  CmdRead((char *)&shared->balancePosition, 4);
  /*
  unsigned char tmp = CmdDONE;
  tmp = CmdDONE;
  CmdWrite(&tmp, 1);
  */
  return;
}
 
static void speed(void)
{
  unsigned char tmp;
  CmdRead((char *)&shared->speedPosition, 4);
  set_speed();
  if (!shared->live && shared->cmd == CmdPLAY)
  {
    if (videoSocket >= 0) {
      SPEEDpara para;
      para.sn = htonl(shared->cmdsn);
      para.usecPerFrame = htonl(shared->usecPerFrame);
      para.framesPerSecond = htonl(shared->framesPerSecond);
      para.frameRateLimit1000 =
                  htonl((long)(shared->frameRateLimit * 1000.0));
      {
        int i = shared->config.maxSPframes;
        i = (int) ((double)i * (1000000.0 / (double)shared->usecPerFrame) /
            shared->pictureRate);
        shared->sendPatternGops = max(min(i, PATTERN_SIZE) / shared->patternSize, 1);
      }
      compute_sendPattern();
      para.sendPatternGops = htonl(shared->sendPatternGops);
      memcpy(para.sendPattern, shared->sendPattern, PATTERN_SIZE);
      tmp = CmdSPEED;
      VideoWrite(&tmp, 1);
      VideoWrite(&para, sizeof(para));
      if (fbstate) {
        maxrate = (double)minupf / (double)max(shared->usecPerFrame, minupf);
        adjstep = ((double)minupf / (double)shared->usecPerFrame) /
            (double)max(shared->patternSize * shared->sendPatternGops, 5);
        fbstate = 1;
      }
    }
    if (audioSocket >= 0) {
      SPEEDaudioPara para;
      para.sn = htonl(shared->cmdsn);
      para.samplesPerSecond = htonl(shared->samplesPerSecond);
      para.samplesPerPacket = htonl(1024 / shared->audioPara.bytesPerSample);
      para.spslimit = htonl(32000);
      tmp = CmdSPEED;
      AudioWrite(&tmp, 1);
      AudioWrite(&para, sizeof(para));
    }
    /*
    timer_speed();
    */
  }
  /*
  tmp = CmdDONE;
  CmdWrite(&tmp, 1);
  */
  return;
}

static int timer_on = 0;     
static int timer_signals_skip = 0;
static int timerCount = 0;   
static int lastCount = 0;    
static int timerUPF;         
static int wait_usr2 = 0;    
static int last_disp_fid = 0;

static void stop_playing(void)
{
  unsigned char precmd = shared->cmd;
  
  if (precmd == CmdFF || precmd == CmdFB || precmd == CmdPLAY)
  {
 
    unsigned char tmp = CmdSTOP;
    NewCmd(CmdSTOP);
    
    /* notify AS and/or VS */
    if (audioSocket >= 0 && precmd == CmdPLAY && rtplay)
    {
      int cmdsn = htonl(shared->cmdsn);
      AudioWrite(&tmp, 1);
      AudioWrite(&cmdsn, 4);
    }
    if (videoSocket >= 0)
    {
      // CORBA call
      int cmdsn = htonl(shared->cmdsn);
      VideoWrite(&tmp, 1);
      VideoWrite(&cmdsn, 4);
    }
    
    /* stop timer and sleep for a while */
    stop_timer();
    usleep(100000);
 
    /* purge VDbuf and audio channel from AS*/
    if (videoSocket >= 0)
    {
      while (VDpeekMsg() != NULL)
        VDreclaimMsg(VDgetMsg());
      /*
      Fprintf(stderr, "CTR: VDbuf purged.\n");
      */
      fbstate = 0;
      
    }
    
    /* adjust some info */
    if (precmd == CmdPLAY && videoSocket >= 0)
      shared->nextFrame = shared->currentFrame+1;
    else
      shared->nextGroup = shared->currentGroup + 1;
  }
}
 
static void loopBack(void)
{
  static char loopBackCmd[7] = {CmdSTOP, CmdPOSITIONrelease,
                                0, 0, 0, 0, CmdPLAY};
  cmdBuffer = loopBackCmd;
  cmdBytes = 7;
  cmdAcks = 3;
}
 
void TimerHandler(int sig)
{
  int currentUPF = shared->currentUPF;
  /*
  Fprintf(stderr, "CTR in TimerHandler.\n");
  */
  if (videoSocket >= 0 && shared->cmd == CmdPLAY && currentUPF != timerUPF)
  {
    struct itimerval val;
    {
      val.it_interval.tv_sec =  val.it_value.tv_sec = currentUPF / 1000000;
      val.it_interval.tv_usec = val.it_value.tv_usec = currentUPF % 1000000;
      setitimer(ITIMER_REAL, &val, NULL);
      /*
      fprintf(stderr, "CTR: timer speed changed to %d upf.\n", shared->currentUPF);
      */
    }
    timerUPF = currentUPF;
#ifdef STAT
    if (shared->collectStat && speedPtr < SPEEDHIST_SIZE)
    {
      speedHistory[speedPtr].frameId = shared->nextFrame;
      speedHistory[speedPtr].usecPerFrame = timerUPF;
      speedHistory[speedPtr].frameRateLimit = shared->frameRateLimit;
      speedHistory[speedPtr].frames = shared->sendPatternGops * shared->patternSize;
      speedHistory[speedPtr].framesDropped = shared->framesDropped;
    }
    speedPtr ++;
#endif
  }
  /*
  fprintf(stderr, "+\n");
  */
  if (!timerCount) {
    int addedVSwat;
    if (shared->cmd == CmdPLAY) {
      addedVSwat = shared->usecPerFrame * (shared->VBheadFrame - shared->nextFrame);
    }
    else if (shared->cmd == CmdFF) {
      addedVSwat = shared->usecPerFrame * (shared->VBheadFrame - shared->nextGroup);
    }
    else { /* shared->cmd == CmdFB */
      addedVSwat = shared->usecPerFrame * (shared->nextGroup - shared->VBheadFrame);
    }
    shared->VStimeAdvance += addedVSwat;
  }
  timerCount += timer_signals_skip + 1;
  timer_signals_skip = 0;
  
  if (shared->live ) {
    if (audioSocket <= 0) {  /* video only */
      unsigned t = shared->VBheadFrame - startVSA;
      if (timerCount < t) {
        /*
        Fprintf(stderr, "CTR: (av) timerCount %d, t %d\n", timerCount, t);
        */
        timerCount = t;
      }
    }
    /* audio involved,  TimerProcessing() will adjust the rate automatically */
  }
}
 
static void DisplayPicture(void)
{
  int toDisplay = 1;
  int count = timerCount;
  
  if ((shared->cmd != CmdPLAY &&
       shared->cmd != CmdFF &&
       shared->cmd != CmdFB) ||
      lastCount == count)
    return;
  if (timer_on >4)   /* 4 is a magic number */
  {
    stop_timer();
 
    /* tries to rewind and play again */
    if (shared->cmd == CmdPLAY && shared->loopBack)
      loopBack();
 
    return;
  }
  /*
  Fprintf(stderr, "CTR in diplayPicture().\n");
  */
  {
    int i, j;
    FrameBlock *buf, *next;
#if 0
    if (shared->cmd == CmdPLAY && rtplay &&
        (i = shared->nextFrame - shared->firstGopFrames) > 0) {
      j = (i / shared->patternSize) % shared->sendPatternGops;
      i %= shared->patternSize;
      if (shared->pattern[i] != 'B') {
        while (i > 0) {
          if (shared->pattern[--i] != 'B') break;
        }
      }
      toDisplay = (shared->sendPattern + j * shared->patternSize)[i];
    }
    else if (shared->cmd != CmdPLAY) {
      if (last_disp_fid != shared->nextGroup) toDisplay = 1;
      else toDisplay = 0;
    }
    if (toDisplay)
#endif
    {
      if (shared->cmd == CmdPLAY && !rtplay) {  /* if play with best effort */
        while (VDcheckMsg() <= 0)  /* keep sleeping for 10 millisec until a decoded
                                      frame show up in VD buffer */
          usleep(10000);
      }
#ifdef STAT
      if (shared->collectStat)
      {
        shared->stat.VBfillLevel[shared->nextFrame] =
            shared->VBheadFrame - shared->nextFrame;
        i = VDcheckMsg();
        if (i < 0) i = 0;
        else if (i >= MAX_VDQUEUE_SIZE) i = MAX_VDQUEUE_SIZE - 1;
        shared->stat.VDqueue[i] ++;
      }
#endif
      toDisplay = 0;
      for (;;)
      {
        buf = VDpeekMsg();
 
        if (buf == NULL) {
          goto loop_end;
        }
 
        switch (shared->cmd)
        {
          int position;
        case CmdPLAY:
          position = shared->nextFrame;
          /*
          Fprintf(stderr, "CTR PLAY: buf->display %d, position %d\n",
                  buf->display, position);
          */
          if (buf->display == position)
          {  /* display it */
            buf = VDgetMsg();
            shared->nextGroup = buf->gop + 1;
#ifdef STAT
            shared->stat.CTRdispOnTime ++;
#endif
            goto display_picture;
          }
          else if (buf->display > position)
          {  /* too early, wait for future display */
            goto loop_end;
          }
          else  /* this picture too late */
          {
            // this line gets the buffer or dequeues from the shared memory queue
            buf = VDgetMsg();
            if (((next = VDpeekMsg()) == NULL || next->display > position) &&
                buf->display > last_disp_fid) {
#ifdef STAT
              shared->stat.CTRdispLate ++;
#endif
              /* buf is the last one, or next too to early, display it anyway */
              goto display_picture;
            }
            else   /* next not too early, discard buf */
            {
#ifdef STAT
              if (buf->display < last_disp_fid)
                shared->stat.CTRdropOutOrder ++;
              else
                shared->stat.CTRdropLate ++;
#endif
              /*
              Fprintf(stderr, "CTR drops frame display=%d, shared->nextFrame=%d\n",
                      buf->display, shared->nextFrame);
              */
              VDreclaimMsg(buf);
              continue;
            }
          }
          break;
        case CmdFF:
          position = shared->nextGroup;
          if (buf->gop == position)
          {  /* display it */
            buf = VDgetMsg();
            shared->nextFrame = buf->display;
            goto display_picture;
          }
          else if (buf->gop > position)
          {  /* hold it for future display */
            goto loop_end;
          }
          else   /* discard late picture */
          {
            buf = VDgetMsg();
            if ((next = VDpeekMsg()) == NULL || next->gop > position)
              /* buf is the last one, or next too to early, display it anyway */
              goto display_picture;
            else   /* next not too early, discard buf */
            {
              VDreclaimMsg(buf);
              continue;
            }
          }
          break;
        case CmdFB:
          position = shared->nextGroup;
          if (buf->gop == position)
          {  /* display it */
            buf = VDgetMsg();
            shared->nextFrame = buf->display;
            goto display_picture;
          }
          else if (buf->gop < position)
          {  /* hold it for future display */
            goto loop_end;
          }
          else   /* discard late picture */
          {
            buf = VDgetMsg();
            if ((next = VDpeekMsg()) == NULL || next->gop < position)
              /* buf is the last one, or next too to early, display it anyway */
              goto display_picture;
            else   /* next not too early, discard buf */
            {
              VDreclaimMsg(buf);
              continue;
            }
          }
          break;
        default:
          goto loop_end;
        }
      }
    display_picture:
      toDisplay = 1;
      if (shared->cmd == CmdPLAY) last_disp_fid = buf->display;
      else last_disp_fid = buf->gop;
#ifdef STAT
      if (shared->live) displayedFrames ++;
      if (shared->collectStat)
        shared->stat.VPframesDisplayed[buf->display >> 3] |= 1 << (buf->display % 8);
#endif
      {
        unsigned char tmp = CmdVPdisplayFrame;
        CmdWrite(&tmp, 1);
      }
      CmdWrite((unsigned char *)&buf, sizeof(char *));
    loop_end:;
    }
  }
  
  if (shared->cmd == CmdPLAY)
  {
    shared->nextFrame += rtplay ? count - lastCount : 1;
    if (shared->nextFrame >= shared->totalFrames)
    {
      timer_on ++;
      shared->nextFrame = shared->totalFrames;
    }
    
    /* following is frameRate feedback algorithm */
    if (fbstate && toDisplay && rtplay) {
      static Filter *fr = NULL;  /* frame-rate filter */
      static int start;  /* feedback action time, in microseconds */
      static int delay;  /* time to delay after each action, and to charge
                          the filter after action-delay */
      static int pretime; /* time of previous frame, in microseconds */
      static int throw_outlier = 0;
      /* tag to throw away outlier. In case there are outliers, this
         tag is flipped by the algorithm, so that If there are two
         consecutive sample deviating very much from the filtered mean
         value, the second is not considered outlier. This may mean
         that the frame rate has dropped significantly. */
      static double vr; /* filtered frame-rate value, in microseconds/frame */
      double r, nr;
      int t = get_usec();
 
      switch (fbstate) {
      case 3:  /* working, monitoring */
        {
          int interval = get_duration(pretime, t);
          if (throw_outlier) {
            if (interval >> 2 >= vr) {
              /* at least four times the filtered mean value to be
                 considered outlier */
              /* In case an outlier is detected and thrown away, then
                 the following sample will never be classified as an
                 outlier, and the current time is recorded */
              throw_outlier = 0;
              pretime = t;
              Fprintf(stderr, "CTR detected a gap %d (vr = %d) us\n",
                      interval, (int)vr);
              break;
            }
          }
          else {
            throw_outlier = 1;
          }
          vr = DoFilter(fr, (double)interval);
        }
        pretime = t;
 
        r = minupf / vr; /* convert the display fps to percentage of maxfr */
        
        nr = 0; /* This variable contains the newly computed server frame rate */
 
        /* let nr oscillate around 1.5 ~ 2.5 */
        if (min(frate, maxrate) - r >= 3.0 * adjstep) {
          /* pipeline is considered overloaded if server fps is more than 3 adjsteps
           higher than display fps */
          nr = min(frate, maxrate) - adjstep; /* slow down server frame rate
                                                 one step */
          if (fb_startup) {  /* startup feedback action: jump set the server fps
                                to a value close to actually measure display
                                frame rate */
            fb_startup = 0;
            while (nr >= r + 2.5 * adjstep) {
              nr -= adjstep;
            }
          }
        }
        else if (frate - r <= 0.5 * adjstep && frate < maxrate) {
          /* pipeline load is considered too light if the server frame rate
             is less than 0.5fps higher than display frame rate, while the server
             fps is no maximum yet. The server fps then is stepped up. */
          nr = min(frate + adjstep, maxrate);
        }
        if (nr > 0) { /* nr = 0 if not feedback action needs to be taken */
          shared->frameRateLimit = maxfr * nr;
          compute_sendPattern();
          /*
          fprintf(stderr,
            "CTR adjust frameRate to %lf, vr=%lf minupf=%d, r=%lf, frate=%lf, nr=%lf\n",
                  shared->frameRateLimit, vr, minupf, r, frate, nr);
           */
          frate = nr;  /* remember new server frame rate */
          shared->qosRecomputes ++;
          start = t;  /* remember the action time */
          /* delay for some time before restarts, to let feedback take effect */
          delay = shared->usecPerFrame * (shared->VBheadFrame - shared->nextFrame) +
                  shared->playRoundTripDelay;
          if (delay < 0) delay = shared->usecPerFrame;
          fbstate = 4;
        }
        break;
      case 4:  /* delay and reset after action*/
        if (get_duration(start, t) >= delay) {
          /*
          fprintf(stderr,
                  "CTR VB from s2 to s3, vr %lf, frate %lf, maxrate %lf, step %lf\n",
                  vr, frate, maxrate, adjstep);
           */
          fr = ResetFilter(fr, shared->config.filterPara >= 1 ?
                           shared->config.filterPara : 100);
          delay = shared->usecPerFrame *
                  max(shared->sendPatternGops * shared->patternSize,
                      shared->config.filterPara);
                  /* charge filter for time */
          start = pretime = t;
          throw_outlier = 0;
          fbstate = 2;
        }
        break;
      case 2:  /* charge the filter */
        {
          int interval = get_duration(pretime, t);
          if (throw_outlier) {
            if (interval >> 2 >= vr) {  /* at least four times the previous average */
              throw_outlier = 0;
              pretime = t;
              Fprintf(stderr, "CTR detected a gap %d (vr = %d) us\n",
                      interval, (int)vr);
              break;
            }
          }
          else {
            throw_outlier = 1;
          }
          vr = DoFilter(fr, (double)interval);
        }
        pretime = t;
        if (get_duration(start, t) >= delay) {
          /*
          fprintf(stderr,
                  "CTR VB from s2 to s3, vr %lf, frate %lf, maxrate %lf, step %lf\n",
                  vr, frate, maxrate, adjstep);
           */
          fbstate = 3;
        }
        break;
      case 1:  /* start or speed change, wait until speed data consistant */
        if (shared->currentUPF == shared->usecPerFrame) {
          if (fr == NULL) {
            fr = NewFilter(FILTER_LOWPASS, shared->config.filterPara >= 1 ?
                          shared->config.filterPara : 100);
          }
          else {
            fr = ResetFilter(fr, shared->config.filterPara >= 1 ?
                            shared->config.filterPara : 100);
          }
          if (fr == NULL) {
            perror("CTR failed to allocate space for fr filter");
            fbstate = 0;
          }
          vr = DoFilter(fr, (double)shared->usecPerFrame);
          delay = shared->usecPerFrame *
                  shared->sendPatternGops * shared->patternSize;
                  /* charge filter for some time */
          pretime = start = t;
          throw_outlier = 0;
          fbstate = 2;
        }
        break;
      default:
        fprintf(stderr, "CTR error: unknown feedback state: %d\n", fbstate);
        fbstate = 1;
        break;
      }
    }
    /* end of frame rate control algorithm */
    
  }
  else if (shared->cmd == CmdFF)
  {
    shared->nextGroup += count - lastCount;
    if (shared->nextGroup >= shared->totalGroups)
    {
      timer_on ++;
      shared->nextGroup = shared->totalGroups - 1;
    }
  }
  else
  {
    shared->nextGroup -= count - lastCount;
    if (shared->nextGroup < 0)
    {
      timer_on ++;
      shared->nextGroup = 0;
    }
  }
  lastCount = count;
}
 
#define AUDIO_BUFSIZE   2048
 
static int audioCount = 0;
static int audioFirst = 0;
static int needAudioSkip = 0;
static int bufferedSamples;
static int nextASSample, startSample;
static unsigned int nextAFtime;
static int forward, forwardDelta;
static int audioForward, framesPerAudioPlay;
static int AudioBufSize;
static char * rawBuf = NULL, * workBuf = NULL, * convBuf = NULL;
 
static void PlayAudioInit(void)
{
  audioCount = -1;
  audioFirst = 1;
  needAudioSkip = 0;
  startSample = nextASSample = shared->nextSample;
  audioForward = shared->config.audioForward;
  framesPerAudioPlay = max(shared->config.framesPerAudioPlay, 1);
  if (audioForward <= 0)
    audioForward = DEFAULT_audioForward;
  fprintf(stderr, "CTR audioForward: %d samples.\n", audioForward);
  bufferedSamples = (int)(((double)
                     (shared->config.audioTimerDuration *
                      shared->config.audioBufferedIntervals))
                    * (double)shared->audioPara.samplesPerSecond / 1000.0);
  if (rawBuf == NULL)
  {
    if ((rawBuf = (char *)malloc(AUDIO_BUFSIZE)) == NULL)
    {
      perror("CTR fails to allocate rawBuf for audio channel");
      exit(1);
    }
     if ((workBuf = (char *)malloc(AUDIO_BUFSIZE)) == NULL)
    {
      perror("CTR fails to allocate workBuf for audio channel");
      exit(1);
    }
  }
  AudioBufSize = (AUDIO_BUFSIZE / shared->audioPara.bytesPerSample) *
                 shared->audioPara.bytesPerSample;
 
  StartAudioPlaySession();
}
 
/* input the number of samples in the raw buffer,
   returns the numberof samples in theconverted buffer */
static int AudioConvert(int len)
{
  int convLen;
  
  if (shared->samplesPerSecond == shared->audioPara.samplesPerSecond)
  {
    convBuf = rawBuf;
    return len;
  }
  
  convBuf = workBuf;
  convLen = len * shared->audioPara.samplesPerSecond / shared->samplesPerSecond;
  AudioInterpolate(rawBuf, len, convBuf, convLen);
  return convLen;
}
 
static void PlayAudioOnly(void)
{
  int maxSize;
  int size, csize;  /* all in samples */
  unsigned int AFtime;
  
  if (audioFirst)
  {
    audioFirst = 0;
    nextAFtime = GetAudioTime() + audioForward;
  }
  else
    if ((int)(nextAFtime - GetAudioTime()) >= bufferedSamples)
      return;
  
  if (timer_on >4)
  {
    stop_timer();
    
    /* tries to rewind and play again */
    if (shared->loopBack)
    {
      /*
      fprintf(stderr, "CTR: trying loopBack().\n");
      */
      loopBack();
    }
 
    return;
  }
  if (nextASSample >= shared->totalSamples)
  {
    timer_on ++;
    return;
  }
  if (shared->samplesPerSecond >= shared->audioPara.samplesPerSecond)
    maxSize = (AudioBufSize/shared->audioPara.bytesPerSample);
  else
    maxSize = ((AudioBufSize/shared->audioPara.bytesPerSample) *
               shared->samplesPerSecond) /
              shared->audioPara.samplesPerSecond;
  for (;;)
  {
    size = ABgetSamples(rawBuf, maxSize);
    csize = AudioConvert(size);
    AFtime = PlayAudioSamples(nextAFtime, convBuf, csize);
    nextASSample += size;
    shared->nextSample += size;
    nextAFtime += csize;
    if ((int)(nextAFtime - AFtime) < 0)
      nextAFtime = AFtime;
    if (nextASSample >= shared->totalSamples)
    {
      timer_on ++;
      break;
    }
    /*
    Fprintf(stderr, "CTR: nextAFtime:%d, AFtime:%d, bufferedSamples:%d\n",
            nextAFtime, AFtime, bufferedSamples);
            */
    if ((int)(nextAFtime - AFtime) >= bufferedSamples)
      break;
  }
  {
    unsigned char tmp = CmdVPaudioPosition;
    CmdWrite(&tmp, 1);
  }
}
 
/* returns: 0 - no forward calculated , 1 - forward calculated */
static int PlayAudio(void)
{
  int timer_count = timerCount;
  int i = timer_count / framesPerAudioPlay;
  if (audioFirst)
  {
    audioFirst = 0;
    nextAFtime = GetAudioTime() + audioForward;;
    audioCount = -1; /* guarantee that PlayAudio do the first step */
  }
  if (audioCount == i)  /* current audio timer signal already processed */
    return 0;
  if (audioCount < i - 1) /* some audio timer signal missing */
  {
    needAudioSkip = 1;
    /*
    Fprintf(stderr, "CTR: some audio timer signal missed.\n");
    */
  }
  if (nextASSample >= shared->totalSamples)
  {
    /*
       timer_on ++;
       */
    return 0;
  }
  
  audioCount = i;
 
  if (needAudioSkip)
  {
    /* return if not enough skip can be done */
    int next_sample = (int)(startSample +
                      ((double)timer_count / shared->pictureRate) *
                      shared->audioPara.samplesPerSecond);
    int skip_samples = next_sample - nextASSample;
    ABskipSamples(skip_samples);
    nextAFtime += (unsigned int)(((double)skip_samples *
                   (double)shared->audioPara.samplesPerSecond) /
                  (double)shared->samplesPerSecond);
    nextASSample += skip_samples;
    needAudioSkip = 0;
  }
  
  /* read current block, convert, put to AF, and update nextAFtime */
  {
    int frame_samples = (int)(startSample +
                        ((double)(timer_count + framesPerAudioPlay) / shared->pictureRate) *
                        (double)shared->audioPara.samplesPerSecond -
                        nextASSample);
    int fsize = frame_samples * shared->audioPara.bytesPerSample;
    int read_samples;
    unsigned int curAFtime;
    unsigned int oldAFtime = nextAFtime;
    char * ptr = rawBuf;
    if (fsize > AudioBufSize)
    {  /* make sure the rawBuf is big enough */
      Fprintf(stderr, "CTR PlayAudio error: BytesPerFrame=%d > AudioBufSize=%d.\n",
              fsize, AudioBufSize);
      Fprintf(stderr,
              "CTR PlayAudio: startSample=%d, timer_count=%d, nextASSmple=%d.\n",
              startSample, timer_count, nextASSample);
      fprintf(stderr, "Internal error: audio resampling buffer not big enough,\n\
  please reduce value of parameter 'Audio timer interval',\n\
  or 'Audio buffered intervals' or Frames per audio play',\n\
  and try again.\n");
      exit(1);
    }
    
    read_samples = ABgetSamples(ptr, frame_samples);
    
    /* convert and play to AF */
    {
      int maxSamples;
      int left_samples = read_samples;
      if (shared->samplesPerSecond >= shared->audioPara.samplesPerSecond)
        maxSamples = (AudioBufSize/shared->audioPara.bytesPerSample);
      else
        maxSamples = ((AudioBufSize/shared->audioPara.bytesPerSample) *
                      shared->samplesPerSecond) /
                     shared->audioPara.samplesPerSecond;
      for (;;)
      {
        int samples = (left_samples > maxSamples) ? maxSamples : left_samples;
        int convSamples = AudioConvert(samples);
        curAFtime = PlayAudioSamples(nextAFtime, convBuf, convSamples);
        nextAFtime += convSamples;
        left_samples -= samples;
        if (left_samples == 0)
          break;
        else
        {
          memcpy(rawBuf, rawBuf + samples * shared->audioPara.bytesPerSample,
                 left_samples * shared->audioPara.bytesPerSample);
        }
      }
    }
    
    /* possible bugs in AF some times cause PlayAudioSample() to return 0 */
    forward = curAFtime ? (oldAFtime - curAFtime) : audioForward;
    /*
    if ((oldAFtime - curAFtime) > (audioForward << 6)) {
      fprintf(stderr, "forward %d, oldAFtime %d, curAFtime %d\n",
              oldAFtime - curAFtime, oldAFtime, curAFtime);
    }
    */
    /* check condition */
    if (read_samples == frame_samples) { /* all bytes for current frame already read */
      nextASSample += frame_samples;
    }
    else {  /* read to the end of audio file */
      /*
      timer_on ++;
      */
      nextASSample += read_samples;
    }
    shared->nextSample = nextASSample;
  }
  return 1;
}
 
 
void TimerProcessing(void)
{
  if (audioSocket >= 0 && shared->cmd == CmdPLAY)
  {
    if (videoSocket < 0)
      PlayAudioOnly();
    else if (rtplay)
    {
      int res = PlayAudio();
      /* and also tries to sync audio and video */
      if (res)
      {
        int jit;
        res = forward - audioForward;
        jit = (res>0 ? res : -res);
        jit = (int)(((double)jit / (double)shared->samplesPerSecond) * 1000000.0);
        if (res < -audioForward/2) /* needs to speedup the clock */
        {
          struct itimerval val;
          getitimer(ITIMER_REAL, &val);
          if ((int)val.it_value.tv_usec > jit)
            val.it_value.tv_usec -= jit;
          else
          {
            timer_signals_skip ++;
            val.it_value.tv_usec = 2;
          }
          setitimer(ITIMER_REAL, &val, NULL);
        }
        else if (res > audioForward) /* needs to slow down the clock */
        {
          struct itimerval val;
          /*
          val.it_interval.tv_sec =  val.it_value.tv_sec = 0;
          val.it_interval.tv_usec = val.it_value.tv_usec = 0;
          */
          getitimer(ITIMER_REAL, &val);
          val.it_value.tv_usec += (jit % 1000000);
          val.it_value.tv_sec += (jit / 1000000);
          setitimer(ITIMER_REAL, &val, NULL);
        }
        /*
        if (res < -audioForward || res > audioForward)
        */
        if (res < -16000 || res > 16000)
        {
          Fprintf(stderr, "Audio forward jit %d samples\n", res);
        }
      }
    }
  }
  if (videoSocket >= 0 &&
      (shared->cmd == CmdPLAY || shared->cmd == CmdFF || shared->cmd == CmdFB)) {
    DisplayPicture();
  }
}
 
#define MAX_WAIT_USEC 10000000
 
static void start_timer(void)
{
  struct itimerval val;
  
  if (audioSocket >= 0 && shared->cmd == CmdPLAY && rtplay)
    PlayAudioInit();
 
  /* sleep for a while to wait for decoding the first picture
     and/or audio stream ready */
  {
    unsigned val1 = startTime;
    if (audioSocket >= 0 && shared->cmd == CmdPLAY && rtplay)
    {
      int samples = videoSocket >= 0 ? 1200 : 1200;
      while (ABcheckSamples() <= samples)
      {
        if (get_duration(val1, get_usec()) >= MAX_WAIT_USEC) {
          fprintf(stderr, "CTR warning: audio is not ready yet.\n");
          break;
        }
        else {
          usleep(10000);
          continue;
        }
      }
    }
    if (videoSocket >= 0)
    {
      while (VDcheckMsg() <= 0) {  /* keep sleeping for 1 millisec until a decoded
                                      frame show up in VD buffer */
        if (get_duration(val1, get_usec()) >= MAX_WAIT_USEC) {
          fprintf(stderr, "CTR warning: video is not ready yet.\n");
          break;
        }
        usleep(10000);
      }
    }
    val1 = get_duration(val1, get_usec()) / 1000;
    shared->playRoundTripDelay = val1;
    
    Fprintf(stderr, "CTR: estimated play round trip delay: %d millisec.\n", val1);
 
    /*
    if (shared->collectStat)
    {
      fprintf(stderr, "CTR: estimated play round trip delay: %d millisec.\n", val1);
    }
    */
  }
  timer_on = 1;
  timer_signals_skip = 0;
  timerCount = 0;
  lastCount = 0;
  
  if (shared->live) {
 
    /* delay for shared->config.VStimeAdvance milliseconds, adjustable delay */
    if (shared->config.VStimeAdvance > 0) {
      Fprintf(stderr, "CTR adds %d millisec of latency\n",
              shared->config.VStimeAdvance);
      usleep(shared->config.VStimeAdvance * 1000);
    }
    displayedFrames = 0;
    startTime = get_usec();
    startVSA = shared->nextFrame - shared->VBheadFrame;
    if (startVSA < 5) startVSA = 5;
    /*
    Fprintf(stderr, "CTR: startVSA %d\n", startVSA);
    */
  }
  
  if (shared->cmd == CmdPLAY && rtplay) {
    wait_usr2 = 0;
  }
  else {
    wait_usr2 = 1;
  }
  last_disp_fid = -1;
  /*
  fprintf(stderr, "CTR start_timer() - %s for usr2\n",
          wait_usr2 ? "wait" : "NOwait");
   */
  if (shared->cmd == CmdPLAY && audioSocket >= 0 && videoSocket < 0)
    timerUPF = shared->config.audioTimerDuration * 1000;
  else {
    timerUPF = shared->usecPerFrame;
  }
#ifdef STAT
  speedPtr = 1;
  speedHistory[0].frameId = shared->nextFrame;
  speedHistory[0].usecPerFrame = timerUPF;
  speedHistory[0].frameRateLimit = shared->frameRateLimit;
  speedHistory[0].frames = shared->sendPatternGops * shared->patternSize;
  speedHistory[0].framesDropped = shared->framesDropped;
#endif
  /*
  fprintf(stderr, "CTR: timer started at %d upf.\n", timerUPF);
  */
  val.it_interval.tv_sec =  val.it_value.tv_sec = timerUPF / 1000000;
  val.it_interval.tv_usec = val.it_value.tv_usec = timerUPF % 1000000;
  //  setsignal(SIGALRM, TimerHandler);
  setitimer(ITIMER_REAL, &val, NULL);
}
 
static void stop_timer(void)
{
  struct itimerval val;
  
  if (!timer_on)
    return;
  
  timer_on = 0;
 
  //  setsignal(SIGALRM, SIG_IGN);
  
  val.it_interval.tv_sec =  val.it_value.tv_sec = 0;
  val.it_interval.tv_usec = val.it_value.tv_usec = 0;
  setitimer(ITIMER_REAL, &val, NULL);
  /*
  fprintf(stderr, "CTR: timer stopped.\n");
  */
  /*
  usleep(200000);
  */
}
 
static void timer_speed()
{
  /*
  struct itimerval val;
  if (timer_on && videoSocket >= 0 &&
      (shared->cmd == CmdPLAY || shared->cmd == CmdFF || shared->cmd == CmdFB))
  {
    fprintf(stderr, "CTR: timer speed changed to %d upf.\n", shared->usecPerFrame);
    val.it_interval.tv_sec =  val.it_value.tv_sec = shared->usecPerFrame / 1000000;
    val.it_interval.tv_usec = val.it_value.tv_usec = shared->usecPerFrame % 1000000;
    setitimer(ITIMER_REAL, &val, NULL);
  }
  */
}
 
static void wait_display(void)
{
  if (!usr1_flag) {
    if (shared->cmd == CmdINIT) usleep(5000000);// XXX hack to make the client wait until the server gets a chance.
    else usleep(800000);
  }
  if (!usr1_flag)
  {
    if (shared->cmd == CmdSTEP) /* to skip undecodable pictures */
      shared->nextFrame ++;
    else if (shared->cmd == CmdINIT) {
      fprintf(stderr, "Warning: initialilzation failed. Please reinitialize the program.\n");
    }
    return;
  }
  usr1_flag = 0;
}
 
static void usr1_handler(int sig)
{
  FrameBlock *buf;
  unsigned char tmp;
  /*
  Fprintf(stderr, "CTR got USR1 for displaying single frame.\n");
  */
  usr1_flag = 1;
  
  if (shared->cmd == CmdINIT) { /* Initialize display window */
    tmp = CmdVPinitScreen;
    CmdWrite(&tmp, 1);
  }
  /*
     fprintf(stderr, "CTR: %d frames in VDbuf.\n", VDcheckMsg());
     */
  while (VDcheckMsg() > 1)
  {
    /*
    Fprintf(stderr, "CTR: VDcheckMsg() = %d, to discard a picture.\n", VDcheckMsg());
    */
    if ((buf = VDgetMsg()) != NULL)
      VDreclaimMsg(buf);
    else
      fprintf(stderr, "CTR error: VDgetMsg is NULL, unexpected.\n");
  }
  if (VDcheckMsg() >0 ) {
    /*
    Fprintf(stderr, "CTR to display a frame in user1_handler()\n");
    */
    buf = VDgetMsg();
  }
  else {
    /*
    Fprintf(stderr, "CTR no frame to display in user_handler()\n");
    */
    buf = NULL;
  }
  
  if (buf != NULL) {
    if (shared->cmd == CmdSTEP)
    {
      shared->nextFrame ++;
      shared->nextGroup = buf->gop + 1;
    }
    else
    {
      shared->nextFrame = buf->display;
      shared->nextGroup = buf->gop;
    }
    tmp = CmdVPdisplayFrame;
    CmdWrite(&tmp, 1);
    CmdWrite((unsigned char *)&buf, sizeof(char *));
  }
  else if (shared->cmd == CmdSTEP) {
    shared->nextFrame ++;
  }
}
 
static void default_usr2_handler(int sig)
{
  Fprintf(stderr, "CTR warning: void SIGUSR2 handler.\n");
}
 
static void compute_sendPattern(void)
{
  char buf[PATTERN_SIZE];
  float limit = shared->frameRateLimit;
  char * pat = shared->pattern;
  int len = shared->patternSize * shared->sendPatternGops;
  int f;
 
  if (limit <= 0)
    limit = 1.0;
  if (rtplay) {
    f = (int)((double)len *
        ((double)limit / (1000000.0 / (double)shared->usecPerFrame)) + 0.5);
    /* rounded to integer, instead of truncated */
  }
  else {
    f = len;
  }
  if (f >= len)
    f = len;
  else if (f <= 1)
    f = 1;
 
  ComputeSendPattern(pat, buf, len, f);
  shared->qosRecomputes ++;
  
  memcpy(shared->sendPattern, buf, PATTERN_SIZE);
 
  f = len - f;
  if (shared->config.verbose) {
    fprintf(stderr, "speed %dupf, %d frames dropped, frames %d:\nsend pattern:",
            shared->usecPerFrame, f, len);
    shared->framesDropped = f;
    {
      int i;
      for (i = 0; i < len; i ++)
        fputc(buf[i] ? pat[i] : '-', stderr);
    }
    fputc('\n', stderr);
  }
}
 
/* about automatic experiment plan by software developers or specific users:
   to be able to conduct experiment plan, the user need to have a uid defined
   by DEVELOPER_UID in "../include/common.h", and he/she needs to prepare a file
   EXP_PLAN_FILE, with following format:
 
   {Delay #seconds |
    Expriment } *
   EndExpriment
 
   An experiment command is followed by one or more of following parameters:
 
   playSpeed 	#float
   frameRateLimit #float
   maxSPframes 	#int
   filterPara	#int
   collectStat 	0/1
   qosEffective 0/1
   syncEffective 0/1
 
   and the parameter is terminated by an empty line.
 
   The automatic experiment plan file is opened after init() and experiment
   setting read just before calling play(). The Player will be terminated
   after all experiments are done.
   
   */
 
static void on_exit_routine(void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) %s:%d\n",
              __FILE__,
              __LINE__));
  unsigned char tmp = CmdCLOSE;
 
  if (getpid() != CTRpid) return;
  
  if (audioSocket >= 0)
  {
    write(audioSocket, &tmp, 1);
    ComCloseConn(audioSocket);
    audioSocket = -1;
  }
  if (videoSocket >= 0)
  {
    write(videoSocket, &tmp, 1);
    ComCloseConn(videoSocket);
    videoSocket = -1;
    if (VBpid > 0) {
      kill(VBpid, SIGUSR1);
      VBpid = -1;
    }
  }
  ComCloseClient();
}
 
#define EXP_PLAN_FILE "experiment_plan"
 
int CTRmain(void)
{
  int sv[2];
  extern void set_exit_routine_tag(int tag);
 
  FILE * fp = NULL;   /* file pointer for experiment plan */
 
  set_exit_routine_tag(0);
  //  setsignal(SIGUSR2, default_usr2_handler);
  
  /* allocate shared data structure and initialize it */
  shared = (SharedData *) creat_shared_mem(sizeof(*shared));
  shared->cmd = 0;
  shared->cmdsn = 0;
  shared->cmdBusy = 0;
  shared->loopBack = 0;
  shared->volumePosition = 0;
  shared->balancePosition = 50;
  shared->locationPosition = 0;
  shared->speedPosition = 0;
  shared->qosRecomputes = 0;
 
  shared->pixelValid = 0;
  
  shared->config.rt = 1;
  shared->config.maxFPS = DEFAULT_maxFPS;
  shared->config.maxSPS = DEFAULT_maxSPS;
  shared->config.ffFPS = DEFAULT_ffFPS;
  shared->config.fbFPS = DEFAULT_fbFPS;
  shared->config.feedBackDelay = DEFAULT_feedBackDelay;
  shared->config.audioMask = DEFAULT_audioMask;
  shared->config.audioPara.encodeType = DEFAULT_encodeType;
  shared->config.audioPara.channels = DEFAULT_channels;
  shared->config.audioPara.samplesPerSecond = DEFAULT_samplesPerSecond;
  shared->config.audioPara.bytesPerSample = DEFAULT_bytesPerSample;
  shared->config.audioTimerDuration = DEFAULT_audioTimerDuration;
  shared->config.audioBufferedIntervals = DEFAULT_audioBufferedIntervals;
  shared->config.framesPerAudioPlay = DEFAULT_framesPerAudioPlay;
  shared->config.audioForward = DEFAULT_audioForward;
  shared->config.VStimeAdvance = DEFAULT_VStimeAdvance;
  shared->config.frameRateLimit = DEFAULT_maxFPS;
  shared->config.collectStat = 0;
  shared->config.collectFrameInfo = 0;
  shared->config.syncEffective = 1;
  shared->config.qosEffective = 1;
  shared->config.audioOffset = 0;
  shared->config.filterPara = DEFAULT_filterPara;
  shared->config.maxSPframes = DEFAULT_maxSPframes;
  shared->config.audioConn = 0;
  shared->config.videoConn = 0;
  shared->config.verbose = (!getuid()) || getuid() == DEVELOPER_UID;
 
  /* create all shared buffers: AB-CTR, VB-VD, VD-VP */
  ABinitBuf(AB_BUF_SIZE);
  VBinitBuf(VB_BUF_SIZE);
  VDinitBuf(VD_BUF_SIZE);
 
  CTRpid = getpid();
  set_exit_routine_tag(1);
 
  /* create command socket pair */
  if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1)
  {
    perror("CTR error on open cmd socketpair");
    exit(1);
  }
  cmdSocket = sv[0];
  
  /* fork processes: VD, GUI */
 
  if ((VDpid = fork()) == -1)
  {
    perror("CTR error on fork VD");
    exit(1);
  } else if (VDpid == 0)
  {
    ABdeleteBuf();
    close(sv[0]);
    close(sv[1]);
    if (realTimeFlag >= 3) {
      if (SetRTpriority("VD", 0)) realTimeFlag = 0;
    }
    VDprocess(CTRpid);
  }
 
  if ((UIpid = fork()) == -1)
  {
    perror("CTR error on fork UI");
    exit(1);
  } else if (UIpid == 0)
  {
    VBdeleteBuf();
    ABdeleteBuf();
    close(sv[0]);
    if (realTimeFlag >= 2) {
#ifdef __svr4__
      if (SetRTpriority("VB", 0)) realTimeFlag = 0;
#elif defined(_HPUX_SOURCE)
      if (SetRTpriority("VB", 2)) realTimeFlag = 0;
#endif
    }
    UIprocess(sv[1]);
  }
  close(sv[1]);
  //  setsignal(SIGUSR1, usr1_handler);
  
  /* initialize Audio device */
  if (InitAudioDevice() == 0)
    hasAudioDevice = 1;
  else
    hasAudioDevice = 0;
 
  ComInitClient(VCR_TCP_PORT, VCR_UNIX_PORT, VCR_ATM_PORT);
 
  if ((vh = (char *)malloc(PATH_SIZE)) == NULL)
  {
    perror("CTR failed to allocate space for vh");
    exit(1);
  }
  if ((vf = (char *)malloc(PATH_SIZE)) == NULL)
  {
    perror("CTR failed to allocate space for vf");
    exit(1);
  }
  if ((ah = (char *)malloc(PATH_SIZE)) == NULL)
  {
    perror("CTR failed to allocate space for ah");
    exit(1);
  }
  if ((af = (char *)malloc(PATH_SIZE)) == NULL)
  {
    perror("CTR failed to allocate space for af");
    exit(1);
  }
  if (realTimeFlag) {
    if (SetRTpriority("CTR", 4)) realTimeFlag = 0;
  }
 
 
  //  atexit(on_exit_routine);
 
 
  // instantiate our command handler
  Command_Handler command_handler (cmdSocket);
//   ACE_NEW_RETURN (command_handler,
//                   Command_Handler (cmdSocket),
//                   -1);
 
  //  if (command_handler->init () == -1)
  if (command_handler.init () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) command_handler: init returned -1"),
                      -1);
 
  // .. and register it with the reactor.
  if (ACE_Reactor::instance ()->register_handler (&command_handler,
                                                  ACE_Event_Handler::READ_MASK) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) register_handler for command_handler failed\n"),
                        -1);
  
  // and now instantiate the sig_handler
  Client_Sig_Handler client_sig_handler (&command_handler);
//   Client_Sig_Handler *client_sig_handler;
//   ACE_NEW_RETURN (client_sig_handler,
//                   Client_Sig_Handler (command_handler),
//                   -1);
 
  // .. and ask it to register itself with the reactor
  if (client_sig_handler.register_handler () < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) register_handler for sig_handler failed\n"),
                      -1);
  
  // and run the event loop
  ACE_Reactor::instance ()->run_event_loop ();
 
  ACE_DEBUG ((LM_DEBUG, 
              "(%P|%t) Exited the client command handler event loop\n"
              "%p\n",
              "run_event_loop"));
  
  return 0;
}

