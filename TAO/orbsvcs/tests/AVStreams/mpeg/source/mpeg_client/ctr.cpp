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

ACE_RCSID(mpeg_client, ctr, "$Id$")

#define SPEEDHIST_SIZE 20

#if defined max
#undef max
#endif
#define max(a,b) (a>b ? a : b)

#if defined min
#undef min
#endif
#define min(a,b) (a<b ? a : b)

static int usr1_flag = 0;
static int rtplay = 1;
static int cmdSocket = -1;

static int writeSocket = -1;
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

AudioBuffer *abuffer;
VideoBuffer *vbuffer;
int asp[2];
int vsp[2];
int vdsp[2];

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
    ACE_OS::memcpy (buf, cmdBuffer, size);
    cmdBytes -= size;
    cmdBuffer += size;
    return 0;
  }
  while ((val = ACE_OS::read (cmdSocket, (buf), (size))) <= 0)
    {
    if (val == -1 && errno == EINTR) return 1;
    if (!val) {
     ACE_OS::perror ("CTR error, EOF reached unexpected within CmdRead()");
    }
    else {
     ACE_OS::perror ("CTR CmdRead() from UI through CmdSocket");
    }
    ACE_OS::exit (1);
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

  while (ACE_OS::write(cmdSocket, buf, size) == -1)
  {
    if (errno == EINTR)
      continue;
    ACE_OS::perror ("CTR writes to UI through cmdSocket");
    ACE_OS::exit (1);
  }
}

/* following function can be replaced by ../shared/fileio.c:read_bytes()
   but SocketRead provides more error info */

static void SocketRead(int s, char *buf, int size)
{ int val, remain = size;
  char * ptr = buf;
  //  fprintf (stderr, "SocketRead: videoSocket = %d\n",videoSocket);
  for (;;)
  {
    val = ACE_OS::read (s, ptr, remain);

    //    fprintf(stderr, "CTR got from %sSocket %d of %d.\n",s == videoSocket ? "video" : "audio", val, remain);

    if (val == -1 && errno == EINTR)
    {
      errno = 0;
      continue;
    }
    if (val == -1)
    {
      fprintf(stderr, "CTR error read %sSocket, ret=%d(size=%d)",
	      s == videoSocket ? "video" : "audio", size-remain, size);
      ACE_OS::perror ("");
      ACE_OS::exit (1);
    }
    if (val == 0)
    {
      fprintf(stderr, "CTR error read %sSocket, EOF met, ret=%d(size=%d).\n",
	      s == videoSocket ? "video" : "audio", size-remain, size);
      ACE_OS::exit (1);
    }
    ptr += val;
    remain -= val;
    if (remain < 0)
    {
      fprintf(stderr, "CTR error read %sSocket, read too much, ret=%d(size=%d).\n",
	      s == videoSocket ? "video" : "audio", size-remain, size);
      ACE_OS::exit (1);
    }
    if (remain == 0)
      break;
  }
}

#define VideoRead(buf, size) SocketRead(videoSocket, buf, size)

#define VideoWrite(buf, psize) \
  { int val; while ((val = ACE_OS::write (videoSocket, (buf), (psize))) == -1) \
    { if (errno == EINTR) continue; \
     ACE_OS::perror ("CTR writes to VS through videoSocket");\
      ACE_OS::exit (1); \
    } \
    if (val < (int)(psize)) { \
      fprintf(stderr, "CTR bad VideoWrite, size %d, val %d", psize, val);ACE_OS::perror (""); }\
  }

#define AudioRead(buf, size) SocketRead(audioSocket, buf, size)

#define AudioWrite(buf, size) \
  { while (write(audioSocket, (buf), (size)) == -1) \
    { if (errno == EINTR) continue; \
     ACE_OS::perror ("CTR writes to AS through audioSocket"); \
      ACE_OS::exit (1); \
    } \
  }

static void start_timer(void);
static void stop_timer(void);
static void timer_speed(void);
static void wait_display(void);
static void stop_playing();

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

static int timer_on = 0;
static int timer_signals_skip = 0;
static int timerCount = 0;
static int lastCount = 0;
static int timerUPF;
static int wait_usr2 = 0;
static int last_disp_fid = 0;

static void loopBack(void)
{
  static char loopBackCmd[7] = {CmdSTOP, CmdPOSITIONrelease,
                                0, 0, 0, 0, CmdPLAY};
  cmdBuffer = loopBackCmd;
  cmdBytes = 7;
  cmdAcks = 3;
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
    if ((rawBuf = (char *)ACE_OS::malloc(AUDIO_BUFSIZE)) == NULL)
    {
     ACE_OS::perror ("CTR fails to allocate rawBuf for audio channel");
      ACE_OS::exit (1);
    }
     if ((workBuf = (char *)ACE_OS::malloc(AUDIO_BUFSIZE)) == NULL)
    {
     ACE_OS::perror ("CTR fails to allocate workBuf for audio channel");
      ACE_OS::exit (1);
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
    abuffer->ABskipSamples(skip_samples);
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
      ACE_OS::exit (1);
    }

    read_samples = abuffer->ABgetSamples(ptr, frame_samples);

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
          ACE_OS::memcpy (rawBuf, rawBuf + samples * shared->audioPara.bytesPerSample,
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



#define MAX_WAIT_USEC 10000000

static void start_timer (void)
{
  struct itimerval val;

  if (audioSocket >= 0 && shared->cmd == CmdPLAY && rtplay)
    {
      //      ACE_DEBUG ((LM_DEBUG,"calling playaudioinit ()\n"));
      PlayAudioInit();
    }

  /* sleep for a while to wait for decoding the first picture
     and/or audio stream ready */
  {
    unsigned val1 = startTime;
    if (audioSocket >= 0 && shared->cmd == CmdPLAY && rtplay)
    {
      if (shared->nextSample < shared->totalSamples)
        {
          int samples = videoSocket >= 0 ? 1200 : 1200;
          while (abuffer->ABcheckSamples() <= samples)
            {
              if (get_duration(val1, get_usec()) >= MAX_WAIT_USEC)
                {
                  fprintf(stderr, "CTR warning: audio is not ready yet.\n");
                  break;
                }
              else
                {
                  usleep(10000);
                  continue;
                }
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


static void wait_display(void)
{
  if (!usr1_flag) {
    if (shared->cmd == CmdINIT) sleep(10);// XXX hack to make the client wait until the server gets a chance.
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

  ACE_OS::memcpy (shared->sendPattern, buf, PATTERN_SIZE);

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


static void on_exit_routine(void)
{
  //  ACE_DEBUG ((LM_DEBUG,
  //          "(%P|%t) %s:%d\n",
  //          __FILE__,
  //          __LINE__));
  unsigned char tmp = CmdCLOSE;

  if (getpid() != CTRpid) return;

  if (audioSocket >= 0)
  {
    char message[BUFSIZ];
    message [0] = EXIT;
    ACE_OS::write (asp[0],&message,BUFSIZ);
    //    ACE_OS::write (audioSocket, &tmp, 1);
    //    ComCloseConn(audioSocket);
    audioSocket = -1;
  }

  if (videoSocket >= 0)
  {
    //   ACE_OS::write (videoSocket, &tmp, 1);
    //    ComCloseConn(videoSocket);
    videoSocket = -1;
    if (VBpid > 0) {
      char message[BUFSIZ];
      message [0] = EXIT;
      ACE_OS::write (vsp[0],&message,BUFSIZ);
      //      ACE_OS::kill (VBpid, SIGUSR1);
      VBpid = -1;
    }
  }
  ComCloseClient();
}

int CTRmain(int argc,
            char **argv)
{
  int sv[2];
  extern void set_exit_routine_tag(int tag);

  FILE * fp = NULL;   /* file pointer for experiment plan */

  set_exit_routine_tag(0);

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
  shared->config.verbose = (!getuid()) || ACE_OS::getuid () == DEVELOPER_UID;

  ACE_NEW_RETURN (vbuffer,
                  VideoBuffer (),
                  -1);

  ACE_NEW_RETURN (abuffer,
                  AudioBuffer (),
                  -1);

  /* create all shared buffers: AB-CTR, VB-VD, VD-VP */
  abuffer->ABinitBuf(AB_BUF_SIZE);
  vbuffer->VBinitBuf(VB_BUF_SIZE);
  VDinitBuf(VD_BUF_SIZE);

  CTRpid =ACE_OS::getpid ();
  set_exit_routine_tag(1);


  // create the notification socket pair.
  if (ACE_OS::socketpair (AF_UNIX,SOCK_STREAM,0,asp) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"Error in opening notification socket:%p",
                       "notification socket"),-1);

  // create the notification socket pair.
  if (ACE_OS::socketpair (AF_UNIX,SOCK_STREAM,0,vsp) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"Error in opening notification socket:%p",
                       "notification socket"),-1);

  if (ACE_OS::socketpair (AF_UNIX,SOCK_STREAM,0,vdsp) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,"Error in opening notification socket:%p",
                       "notification socket"),-1);

  /* create command socket pair */
  if (ACE_OS::socketpair(AF_UNIX, SOCK_STREAM, 0, sv) == -1)
  {
   ACE_OS::perror ("CTR error on open cmd socketpair");
    ACE_OS::exit (1);
  }
  cmdSocket = sv[0];
  /* fork processes: VD, GUI */

  if ((VDpid = ACE_OS::fork ()) == -1)
  {
   ACE_OS::perror ("CTR error on fork VD");
    ACE_OS::exit (1);
  } else if (VDpid == 0)
  {
    abuffer->ABdeleteBuf();
    ACE_OS::close (sv[0]);
    ACE_OS::close (sv[1]);
    if (realTimeFlag >= 3) {
      if (SetRTpriority("VD", 0)) realTimeFlag = 0;
    }
    VDprocess(CTRpid);
  }

  if ((UIpid = ACE_OS::fork ()) == -1)
  {
   ACE_OS::perror ("CTR error on fork UI");
    ACE_OS::exit (1);
  } else if (UIpid == 0)
  {
    vbuffer->VBdeleteBuf();
    abuffer->ABdeleteBuf();
    ACE_OS::close (sv[0]);
    if (realTimeFlag >= 2) {
#ifdef __svr4__
      if (SetRTpriority("VB", 0)) realTimeFlag = 0;
#elif defined(_HPUX_SOURCE)
      if (SetRTpriority("VB", 2)) realTimeFlag = 0;
#endif
    }
    UIprocess(sv[1]);
  }
  ACE_OS::close (sv[1]);

  /* initialize Audio device */
  if (InitAudioDevice() == 0)
    hasAudioDevice = 1;
  else
    hasAudioDevice = 0;

  //  ComInitClient(VCR_TCP_PORT, VCR_UNIX_PORT, VCR_ATM_PORT);

  if ((vh = (char *)ACE_OS::malloc(PATH_SIZE)) == NULL)
  {
   ACE_OS::perror ("CTR failed to allocate space for vh");
    ACE_OS::exit (1);
  }
  if ((vf = (char *)ACE_OS::malloc(PATH_SIZE)) == NULL)
  {
   ACE_OS::perror ("CTR failed to allocate space for vf");
    ACE_OS::exit (1);
  }
  if ((ah = (char *)ACE_OS::malloc(PATH_SIZE)) == NULL)
  {
   ACE_OS::perror ("CTR failed to allocate space for ah");
    ACE_OS::exit (1);
  }
  if ((af = (char *)ACE_OS::malloc(PATH_SIZE)) == NULL)
  {
   ACE_OS::perror ("CTR failed to allocate space for af");
    ACE_OS::exit (1);
  }
  if (realTimeFlag) {
    if (SetRTpriority("CTR", 4)) realTimeFlag = 0;
  }

  // instantiate our command handler
  Command_Handler command_handler (cmdSocket);
  if (command_handler.init (argc,argv) == -1)
     ACE_ERROR_RETURN ((LM_ERROR,
                        "(%P|%t) command_handler: init returned -1"),
                       -1);

  // .. and register it with the reactor.
  if (TAO_ORB_Core_instance ()->reactor ()->register_handler (&command_handler,
                                                              ACE_Event_Handler::READ_MASK) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%P|%t) register_handler for command_handler failed\n"),
                        -1);

      int result = command_handler.run ();
      if (ABpid == 0)
        {
          ACE_DEBUG ((LM_DEBUG,"(%d) Restarting the ACE_Reactor::instance ()",ACE_OS::getpid ()));
          ACE_Reactor::instance ()->run_event_loop ();
        }

      if (VBpid == 0)
        {
          ACE_DEBUG ((LM_DEBUG,"(%d) Restarting the ACE_Reactor::instance ()",ACE_OS::getpid ()));
          ACE_Reactor::instance ()->run_event_loop ();
        }
      
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Exited the client command handler event loop\n"
                  "%p\n",
                  "run_event_loop"));

  return 0;
}
