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

#if 1

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>

#include "include/common.h"
#include "server_proto.h"

ACE_RCSID(mpeg_server, las, "$Id$")

#ifdef LIVE_AUDIO

#include <AF/AFlib.h>
#include <AF/AFUtils.h>

static AFAudioConn *aud;
static AC ac;
static AFSetACAttributes attributes;
static int bps;
static int nextSample;
static audioActive = 0;

/*
 * Find a suitable default device
 * Returns -1 if no suitable device can be found.
 */
static int FindDefaultDevice(AFAudioConn *aud)
{
  AFDeviceDescriptor *audioDevice;
  int i;
  for(i=0; i<ANumberOfAudioDevices(aud); i++)
  {
    audioDevice = AAudioDeviceDescriptor(aud, i);
    if(audioDevice->inputsFromPhone == 0 && audioDevice->outputsToPhone == 0)
    return i;
  }
  return -1;
}
#endif


int OpenLiveAudio(AudioParameter * ap)
{
#ifndef LIVE_AUDIO
  return -1;
#else
  int device;
  int gain;
  unsigned int channels;
  int endianflag = 0;

  if (!audioActive) return -1;
  
  device = -1;
  attributes.rec_gain =  0;
  attributes.type = UNKNOWN_ENCODETYPE;

  if ( (aud = AFOpenAudioConn(NULL)) == NULL)
  {
    fprintf(stderr, "SA error: fails to open default AF connection.\n");
    return -1;
  }
  
  /* set up audio context, find sample size and sample rate
     At this time, we only support default setings */
  device = FindDefaultDevice(aud);
  attributes.type = aud->devices[device].playBufType;
  ac = AFCreateAC(aud, device,
		  ACRecordGain | ACEncodingType | endianflag, 
		  &attributes);
  if (ac == NULL)
  {
   ACE_OS::perror ("AS error: failed to create ac");
    AFCloseAudioConn(aud);
    return -1;
  }
  if (attributes.type == ap->encodeType) {
    bps = ap->bytesPerSample;
    return 0;
  }
  else {
    fprintf(stderr, "AS error: encodeType %d, client encodeType %d\n",
	    attributes.type, ap->encodeType);
    return -1;
  }
#endif
}

void StartPlayLiveAudio(void)
{
#ifndef LIVE_AUDIO
  return;
#else
  nextSample = AFGetTime(ac);
  SFprintf(stderr, "LAS nextSample %d\n", nextSample);
  return;
#endif
}

void StopPlayLiveAudio(void)
{
#ifndef LIVE_AUDIO
  return;
#else
  return;
#endif
}

int ReadLiveAudioSamples(char * buf, int samples)
{
#ifndef LIVE_AUDIO
  return 0;
#else
  /*
  SFprintf(stderr, "LAS to read %d bytes\n", samples * bps);
  */
  AFRecordSamples(ac, nextSample, samples * bps, (unsigned char *)buf, ABlock);
  nextSample += samples;
  /*
  SFprintf(stderr, "LAS read live %d samples\n", samples);
  */
  return samples;
#endif
}

#endif

#if 0

/* Following is a version using /dev/audio directly, since /dev/audio can't be
   shared by different sessions, at most one session can be served. */

#include <stdio.h>
#include <errno.h>
/*
#include <sys/types.h>
#include <sys/socket.h>
*/
#include <fcntl.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/filio.h>
#include <sys/stropts.h>

#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>

ACE_RCSID(mpeg_server, las, "$Id$")

static int sunfd1;
static int bps;
static int nextSample;


int OpenLiveAudio(AudioParameter * ap)
{
  if (!audioActive) return -1;
  
  if ((sunfd1 = open("/dev/audio", O_RDONLY, 0)) < 0) {
    fprintf(stderr, "dda can't open /dev/audio, errno message \" %d \"\n", 
	   errno);
    return -1;
  }
  bps = ap->bytesPerSample;
  ioctl(sunfd1, I_FLUSH, FLUSHR);
  return 0;
}

void StartPlayLiveAudio(void)
{
  ioctl(sunfd1, I_FLUSH, FLUSHR);
}

void StopPlayLiveAudio(void)
{
}

int ReadLiveAudioSamples(char * buf, int samples)
{
  int bytes = samples * bps;
  int len = ACE_OS::read (sunfd1, buf, bytes);
  if (len != bytes) {
    fprintf(stderr, "ReadLive: need %d samples (%dB), read %dB\n",
	    samples, bytes, len);
    return len / bps;
  }
  return samples;
}

#endif

int InitLiveAudio(int argc, char ** argv)
{
#ifndef LIVE_AUDIO
  return -1;
#else
  
  audioActive = 1;
  
  fprintf(stderr, "LAS initialized.\n");
  return 0;
#endif
}

void ExitLiveAudio(void)
{
  return;
}

void LeaveLiveAudio(void)
{
  return;
}
