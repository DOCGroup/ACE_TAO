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

#include "Globals.h"

int Mpeg_Global::parentpid = -1;
int Mpeg_Global::listenSocketIn = -1;
int Mpeg_Global::listenSocketUn = -1;
struct linger Mpeg_Global::linger = {1,1};
int Mpeg_Global::live_audio = 0;
int Mpeg_Global::live_video = 0;  /* 0 - no,  1 - to open,  2 - opened */
int Mpeg_Global::drift_ppm = 0;  /* clock drift in ppm */
int Mpeg_Global::session_limit = SESSION_NUM;
int Mpeg_Global::session_num = 0;
int Mpeg_Global::rttag = 0;

int Video_Timer_Global::timerHeader = 0;
int Video_Timer_Global::timerGroup = 0;
int Video_Timer_Global::timerFrame = 0;
int Video_Timer_Global::timerOn = 0;
int Video_Timer_Global::timerAdjust = 0;
int Video_Timer_Global::preTimerVal = 0;

// initialize the nasty int's, doubles and their friends
Video_Global::Video_Global ()
{
  live_source = 0;
  video_format = 0;
  
  pkts_sent = 0;
  start_time = 0;
  
  conn_tag = 0;

  normalExit = 1;

  serviceSocket = 0;
  videoSocket = -1;

  ACE_OS::memset (videoFile, 
                  0,
                  PATH_SIZE);

  fp = 0;

  needHeader = 0;

  lastRef [0] = lastRef [1] = 0;

  lastRefPtr = 0;
  currentUPF = 0;
  addedUPF = 0;
  addedSignals = 0;
  VStimeAdvance = 0;
  fps = 0;  /* current frames-per-second: playback speed */
  frameRateLimit = 0;

  packet = 0;
  packetBufSize = 0;
  msgsn = 0;
  packetsn = 0;
  msgsize = 0;

  precmd = 0;
  cmd = 0;
  cmdsn = 0;
  nextFrame = 0;
  nextGroup = 0;
  firstPatternSize = 0;
  firstSendPattern = 0;
  sendPatternGops = 0;
  ACE_OS::memset (sendPattern,
                  0,
                  PATTERN_SIZE);

#ifdef STAT
  framesSent = 0;
#endif

  fileSize = 0;
  maxS = 0;
  maxG = 0;  
  maxI = 0;
  maxP = 0;
  maxB = 0;
  minS = 0x7fffffff;
  minG = 0x7fffffff;
  minI = 0x7fffffff;
  minP = 0x7fffffff;
  minB = 0x7fffffff;

  numS = 0;
  numG = 0;
  numF = 0;
  numI = 0;
  numP = 0;
  numB = 0;

  averageFrameSize = 0;
  horizontalSize = 0;
  verticalSize = 0;
  pelAspectRatio = 0;
  pictureRate = 0;
  vbvBufferSize = 0;
  firstGopFrames = 0;
  patternSize = 0;

  ACE_OS::memset (pattern,
                  0,
                  PATTERN_SIZE);

  // struct pointers
  systemHeader = 0;

  gopTable = 0;

  frameTable = 0;

  // playvideo local vars

  preGroup = -1;
  preHeader = -1;
  preFrame = -1;

  fast_preGroup = -1;
  fast_preHeader= -1;
}

void
Video_Timer_Global::StartTimer (void)
{
  VIDEO_SINGLETON::instance ()->addedUPF = 0;
  VIDEO_SINGLETON::instance ()->addedSignals = 0;
  timerAdjust = (VIDEO_SINGLETON::instance ()->VStimeAdvance * SPEEDUP_INV_SCALE) / VIDEO_SINGLETON::instance ()->currentUPF;
  /*
  SFprintf(stderr, "VS StartTimer(): fast-start frames %d\n",
	  timerAdjust / SPEEDUP_INV_SCALE);
  */
  TimerSpeed();
  //  setsignal(SIGALRM, timerHandler);
  timerOn = 1;
  preTimerVal = get_usec();
  /*
  fprintf(stderr, "VS: timer started at %d upf.\n", VIDEO_SINGLETON::instance ()->currentUPF + VIDEO_SINGLETON::instance ()->addedUPF);
  */
}

void
Video_Timer_Global::StopTimer (void)
{
  struct itimerval val;
  // @@Naga I have to incorporate this logic into the changed code 
  //  setsignal(SIGALRM, SIG_IGN);
  val.it_interval.tv_sec =  val.it_value.tv_sec = 0;
  val.it_interval.tv_usec = val.it_value.tv_usec = 0;
  setitimer(ITIMER_REAL, &val, NULL);
  timerOn = 0;
  /*
  fprintf(stderr, "VS: timer stopped.\n");
  */
}

void
Video_Timer_Global::TimerSpeed (void)
{
  struct itimerval val;
  int usec = VIDEO_SINGLETON::instance ()->currentUPF + VIDEO_SINGLETON::instance ()->addedUPF;
  if (Mpeg_Global::drift_ppm) {
    /*
    int drift = (double)usec * (double)Mpeg_Global::drift_ppm / 1000000.0;
    SFprintf(stderr, "Mpeg_Global::drift_ppm %d, usec %d, drift %d, new usec %d\n",
	    Mpeg_Global::drift_ppm, usec, drift, usec - drift);
     */
    usec -= (int)((double)usec * (double)Mpeg_Global::drift_ppm / 1000000.0);
  }
  if (timerAdjust > 1)
    usec = (int)(((double)usec * (double)(SPEEDUP_INV_SCALE - 1)) /
		 (double)SPEEDUP_INV_SCALE);
  val.it_interval.tv_sec =  val.it_value.tv_sec = usec / 1000000;
  val.it_interval.tv_usec = val.it_value.tv_usec = usec % 1000000;
  setitimer(ITIMER_REAL, &val, NULL);
  /*
  SFprintf(stderr,
	  "VS TimerSpeed() at %s speed, timerAdjust %d VIDEO_SINGLETON::instance ()->addedSignals %d.\n",
	  (timerAdjust > 1) ? "higher" : "normal", timerAdjust, VIDEO_SINGLETON::instance ()->addedSignals);
  */

}

void
Video_Timer_Global::TimerProcessing (void)
{
/*  
  fprintf(stderr, "VS: timerHandler...\n");
*/
  if (!timerOn) {
    return;
  }
  if (timerAdjust < 0)
  {
    timerAdjust += SPEEDUP_INV_SCALE;
    return;
  }
  if (timerAdjust >0)
  {
    if ((--timerAdjust) == 0)
      TimerSpeed();
  }
  if (VIDEO_SINGLETON::instance ()->cmd == CmdPLAY)
  {
    if (timerGroup == VIDEO_SINGLETON::instance ()->numG - 1 && timerFrame >= VIDEO_SINGLETON::instance ()->gopTable[timerGroup].totalFrames - 1)
    {
      timerFrame ++;  /* force sending of END_SEQ when PLAY VIDEO_SINGLETON::instance ()->cmd */
      StopTimer();
      return;
    }
    else
    {
      timerFrame ++;
      if (timerFrame >= VIDEO_SINGLETON::instance ()->gopTable[timerGroup].totalFrames)
      {
        timerGroup ++;
        timerFrame = 0;
        timerHeader = VIDEO_SINGLETON::instance ()->gopTable[timerGroup].systemHeader;
      }
    }
  }
  else {
    if (VIDEO_SINGLETON::instance ()->cmd == CmdFF) {
      if (timerGroup == VIDEO_SINGLETON::instance ()->numG - 1) {
	StopTimer();
	return;
      }
      timerGroup ++;
      timerHeader = VIDEO_SINGLETON::instance ()->gopTable[timerGroup].systemHeader;
    }
    else {
      if (timerGroup == 0) {
	StopTimer();
	return;
      }
      timerGroup --;
      timerHeader = VIDEO_SINGLETON::instance ()->gopTable[timerGroup].systemHeader;
    }
  }

}

void
Video_Timer_Global::timerHandler (int sig)
{
  //  ACE_DEBUG ((LM_DEBUG,
  //              "Video_Timer_Global::timerHandler\n"));

  int val2, val3;
  int usec = VIDEO_SINGLETON::instance ()->currentUPF + VIDEO_SINGLETON::instance ()->addedUPF;
  
  if (Mpeg_Global::drift_ppm) {
    usec -= (int)((double)usec * (double)Mpeg_Global::drift_ppm / 1000000.0);
  }
  
  if (timerAdjust > 1)
    usec = (int)(((double)usec * (double)(SPEEDUP_INV_SCALE - 1)) /
		 (double)SPEEDUP_INV_SCALE);
  val3 = get_duration(preTimerVal, (val2 = get_usec()));
  /*
  if (val3 >= usec<< 1))
    fprintf(stderr, "Slower: %d out of VIDEO_SINGLETON::instance ()->currentUPF %d.\n",
            val3, usec);
  else
    fprintf(stderr, "+\n");
  */
  preTimerVal = val2;
  if (val3 < 0 || val3 > 100000000)
    val3 = usec;
  val2 = (val3 + (usec>>1)) / usec;
  if (val2 < 0) val2 = 0;
  if (val2) {
    TimerProcessing();
    val2 --;
  }
  VIDEO_SINGLETON::instance ()->addedSignals += val2;
  
  if (VIDEO_SINGLETON::instance ()->addedSignals) {
    val2 = timerAdjust;
    if (timerAdjust < MAX_TIMER_ADJUST) {
      timerAdjust += VIDEO_SINGLETON::instance ()->addedSignals * SPEEDUP_INV_SCALE;
      if (val2 < SPEEDUP_INV_SCALE) {
	TimerSpeed();
      }
    }
    else {
      /*
      fprintf(stderr, "VS timerAdjust %d, VIDEO_SINGLETON::instance ()->addedSignals %d, timerFrame %d\n",
	      timerAdjust, VIDEO_SINGLETON::instance ()->addedSignals, timerFrame);
      */
      for (val3 = 0; val3 < VIDEO_SINGLETON::instance ()->addedSignals; val3 ++)
        TimerProcessing();
    }
    VIDEO_SINGLETON::instance ()->addedSignals = 0;
  }
}
