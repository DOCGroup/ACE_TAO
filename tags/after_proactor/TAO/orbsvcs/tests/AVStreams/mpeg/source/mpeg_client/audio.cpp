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

/* ##################################################################

   When you port to a new architecture, you much add native audio
   device support in this file by adding stuff to all relevant
   functions, otherwise, it is disabled by default.
   
   ################################################################# */

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#ifdef __svr4__
#include <sys/audioio.h>
#elif defined(FreeBSD)
#include <machine/pcaudioio.h>
#elif defined(sun)
#include <sys/audioio.h>
#elif defined(_HPUX_SOURCE)
#include <sys/audio.h>
#elif defined(IRIX)
#include <audio.h>
#endif

#ifdef FreeBSD
#define AUDIO_DEVICE "/dev/pcaudio"
#define AUDIO_CONTROL "/dev/pcaudioctl"
#elif defined(__svr4__) || defined(sun)
#define AUDIO_DEVICE "/dev/audio"
#define AUDIO_CONTROL "/dev/audioctl"
#elif defined(_HPUX_SOURCE)
#define AUDIO_DEVICE "/dev/audio"
#define AUDIO_CONTROL "/dev/audioCtl"
#elif defined(IRIX)
#define AUDIO_DEVICE "proprietary device"
#define AUDIO_CONTROL "proprietary device"
#elif defined(LINUX)
#define AUDIO_DEVICE "/dev/audio"
#define AUDIO_CONTROL "NO CONTROL DEV"
#endif

#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>

#include "include/common.h"
#include "newproto.h"
#include "global.h"

ACE_RCSID(mpeg_client, audio, "$Id$")

static int cfd = -1, afd = -1;  /* afd == -1 mean no native device available */
static long start_time;
static long current_time;

#ifdef IRIX
#define audioDevice AL_DEFAULT_DEVICE
static ALport playPort = NULL;
#endif

#ifdef AUDIOFILE

#include <AF/AFlib.h>
#include <AF/AFUtils.h>

static int useAF = 0;

static AFAudioConn *aud;
static AC ac;
static AFSetACAttributes attributes;

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

/*
   returns: 0 - succ
           -1 - failed
*/
int InitAudioDevice(void)
{
#ifdef AUDIOFILE
  int device;
  int gain;
  unsigned int channels;
  int endianflag = 0;
#endif

#ifdef AUDIOFILE
  device = -1;
  attributes.preempt = Mix;
  attributes.start_timeout = 0;
  attributes.end_silence = 0;
  attributes.play_gain = 0;
  attributes.rec_gain =  0;
  attributes.type = UNKNOWN_ENCODETYPE;

  if ( (aud = AFOpenAudioConn(NULL)) == NULL)
  {
    fprintf(stderr, "Failed locating default AudioFile.\n");
    goto native_audio;
  }
  
  /* set up audio context, find sample size and sample rate
     At this time, we only support default setings */
  device = FindDefaultDevice(aud);
  attributes.type = aud->devices[device].playBufType;
  ac = AFCreateAC(aud, device, (ACPlayGain | ACEncodingType | endianflag), 
		  &attributes);
  if (ac == NULL)
  {
    fprintf(stderr, "Failed Initializing default AudioFile.\n");
    AFCloseAudioConn(aud);
    goto native_audio;
  }
  AFSync(aud, 0);	/* Make sure we confirm encoding type support. */
  shared->AFPara.encodeType = attributes.type;
  shared->AFPara.channels = ac->device->playNchannels;
  shared->AFPara.bytesPerSample =
               BytesPerUnit(attributes.type) / SampsPerUnit(attributes.type);
  shared->AFPara.samplesPerSecond = ac->device->playSampleFreq;
  /*
  memcpy(&(shared->AFPara), &(shared->config.audioPara), sizeof(AudioParameter));
  */
  useAF = 1;
  fprintf(stderr, "Using default AudioFile.\n");
  return 0;
#elif defined(__svr4__) || defined(sun) || defined(FreeBSD) \
  || defined(_HPUX_SOURCE) || defined(IRIX) || defined(LINUX)
  fprintf(stderr, "This vcr compiled without AudioFile, try native audio.\n");
#else
  fprintf(stderr, "This vcr compiled without AudioFile, audio to be ignored\n");
  afd = -1;
  return -1;
#endif
  
 native_audio:
  
#if defined(__svr4__) || defined(sun) || defined(FreeBSD) || defined(_HPUX_SOURCE)

  afd = open(AUDIO_DEVICE, O_WRONLY | O_NDELAY);
  if (afd == -1) {
    fprintf(stderr, "%s", AUDIO_DEVICE);
    perror(" can't be opened for write");
    return -1;
  }
  cfd = open(AUDIO_CONTROL, O_WRONLY | O_NDELAY);
  if (cfd == -1) {
    fprintf(stderr, "%s", AUDIO_DEVICE);
    perror(" can't be opened for write");
    close(afd);
    afd = -1;
    return -1;
  }
#if defined(_HPUX_SOURCE)
  
  if (ioctl(cfd, AUDIO_SET_DATA_FORMAT, AUDIO_FORMAT_ULAW) < 0) {
    fprintf(stderr, "%s" AUDIO_DEVICE);
    perror("can't be set to u-law");
    close(afd);
    close(cfd);
    afd = -1;
    return -1;
  }
  if (ioctl(cfd, AUDIO_SET_CHANNELS, 1) < 0) {
    fprintf(stderr, "%s" AUDIO_DEVICE);
    perror("can't be set to have single channel");
    close(afd);
    close(cfd);
    afd = -1;
    return -1;
  }
  if (ioctl(cfd, AUDIO_SET_SAMPLE_RATE, 8000) < 0) {
    fprintf(stderr, "%s" AUDIO_DEVICE);
    perror("can't be set to 8000sps");
    close(afd);
    close(cfd);
    afd = -1;
    return -1;
  }
  if (ioctl(cfd, AUDIO_SET_OUTPUT, AUDIO_OUT_SPEAKER | AUDIO_OUT_HEADPHONE))
  {
    fprintf(stderr, "%s" AUDIO_DEVICE);
    perror("can't be set output to both internal speaker and headphone");
    close(afd);
    close(cfd);
    afd = -1;
    return -1;
  }
  if(ioctl(cfd, AUDIO_SET_TXBUFSIZE, 1024*64) < 0)
  {
    fprintf(stderr, "%s" AUDIO_DEVICE);
    perror("can't be set output buffer size to 64K");
    close(afd);
    close(cfd);
    afd = -1;
    return -1;
  }
#endif  /* _HPUX_SOURCE */

#elif defined(IRIX)
  {
#define NCH 1
#define MSQUEUE 100
    ALconfig c;
    long params[2];

    params[0] = AL_OUTPUT_RATE;
    params[1] = 8000;
    ALsetparams(audioDevice, params, 2);

    if ((c = ALnewconfig()) == NULL) {
      afd = -1;
      return -1;
    }
    ALsetwidth(c, AL_SAMPLE_16);
    ALsetchannels(c, AL_MONO);
    ALsetqueuesize(c, 10240); /* Minimal queue size */
    playPort = ALopenport(AUDIO_DEVICE, "w", c);
    ALfreeconfig(c);
    if (playPort == NULL) {
      afd = -1;
      return -1;
    }
    afd = 0;
  }
#elif defined(LINUX)
  
  afd = open(AUDIO_DEVICE, O_WRONLY);
  if (afd == -1) {
    fprintf(stderr, "%s", AUDIO_DEVICE);
    perror(" can't be opened for write");
    return -1;
  }
  
#else  /* no audio device supported on other platforms */

  fprintf(stderr, "Audio device access not supported, audio to be ignored\n");
  afd = -1;
  return -1;

#endif  /* sun, FreeBSD, _HPUX_SOURCE, ... */
  shared->AFPara.encodeType = AudioTypeMuLaw;
  shared->AFPara.channels = 1;
  shared->AFPara.bytesPerSample = 1;
  shared->AFPara.samplesPerSecond = 8000;
  start_time = get_msec();
  current_time = 0;
  fprintf(stderr, "Using native %s\n", AUDIO_DEVICE);
  return 0;
}

void SetAudioParameter(AudioParameter * para)
{
}

unsigned int GetAudioTime(void)
{
#ifdef AUDIOFILE
  if (useAF) return AFGetTime(ac);
#endif
#if defined(__svr4__) || defined(sun) || defined(FreeBSD) \
  || defined(_HPUX_SOURCE) || defined(IRIX) || defined(LINUX)
  if (afd == -1) return 0;
  return get_duration(start_time, get_msec()) * 8; /* 8K sampling speed */
#else
  return 0;
#endif
}

/* Play size number of samples to AF */
unsigned int PlayAudioSamples(unsigned int time, char * buf, int size)
{
#ifdef AUDIOFILE
  if (useAF) return AFPlaySamples(ac, time, size * shared->audioPara.bytesPerSample,
				  (unsigned char *)buf);
#endif

#if defined(__svr4__) || defined(sun) || defined(FreeBSD) \
  || defined(_HPUX_SOURCE) || defined(IRIX) || defined(LINUX)
  if (afd == -1) return 0;

#ifdef IRIX
  {
#define BUFSIZE 1024
    static short   AF_cvt_u2s[]={
        -32124,-31100,-30076,-29052,-28028,-27004,-25980,-24956,
        -23932,-22908,-21884,-20860,-19836,-18812,-17788,-16764,
        -15996,-15484,-14972,-14460,-13948,-13436,-12924,-12412,
        -11900,-11388,-10876,-10364, -9852, -9340, -8828, -8316,
         -7932, -7676, -7420, -7164, -6908, -6652, -6396, -6140,
         -5884, -5628, -5372, -5116, -4860, -4604, -4348, -4092,
         -3900, -3772, -3644, -3516, -3388, -3260, -3132, -3004,
         -2876, -2748, -2620, -2492, -2364, -2236, -2108, -1980,
         -1884, -1820, -1756, -1692, -1628, -1564, -1500, -1436,
         -1372, -1308, -1244, -1180, -1116, -1052,  -988,  -924,
          -876,  -844,  -812,  -780,  -748,  -716,  -684,  -652,
          -620,  -588,  -556,  -524,  -492,  -460,  -428,  -396,
          -372,  -356,  -340,  -324,  -308,  -292,  -276,  -260,
          -244,  -228,  -212,  -196,  -180,  -164,  -148,  -132,
          -120,  -112,  -104,   -96,   -88,   -80,   -72,   -64,
           -56,   -48,   -40,   -32,   -24,   -16,    -8,     0,
         32124, 31100, 30076, 29052, 28028, 27004, 25980, 24956,
         23932, 22908, 21884, 20860, 19836, 18812, 17788, 16764,
         15996, 15484, 14972, 14460, 13948, 13436, 12924, 12412,
         11900, 11388, 10876, 10364,  9852,  9340,  8828,  8316,
          7932,  7676,  7420,  7164,  6908,  6652,  6396,  6140,
          5884,  5628,  5372,  5116,  4860,  4604,  4348,  4092,
          3900,  3772,  3644,  3516,  3388,  3260,  3132,  3004,
          2876,  2748,  2620,  2492,  2364,  2236,  2108,  1980,
          1884,  1820,  1756,  1692,  1628,  1564,  1500,  1436,
          1372,  1308,  1244,  1180,  1116,  1052,   988,   924,
           876,   844,   812,   780,   748,   716,   684,   652,
           620,   588,   556,   524,   492,   460,   428,   396,
           372,   356,   340,   324,   308,   292,   276,   260,
           244,   228,   212,   196,   180,   164,   148,   132,
           120,   112,   104,    96,    88,    80,    72,    64,
            56,    48,    40,    32,    24,    16,     8,     0
      };
    int left = size;
    unsigned char * src = buf;
    int i;
    while (left > 0) {
      short buffer[BUFSIZE];
      short * dst = buffer;
      int len = left;
      if (len > BUFSIZE) len = BUFSIZE;
      left -= len;
      for (i = 0; i < len; i ++) {
	*(dst ++) = AF_cvt_u2s[*(src ++)];
      }
      ALwritesamps(playPort, buffer, len);
    }
  }
#else
  write(afd, buf, size);
#endif

  current_time += size;
  return get_duration(start_time, get_msec()) * 8; /* 8K sampling speed */
#else
  return 0;
#endif
}

void StartAudioPlaySession(void)
{
  SetAudioGain();
#ifdef AUDIOFILE
  if (useAF) return;
#endif
#if defined(__svr4__) || defined(sun) || defined(FreeBSD) \
  || defined(_HPUX_SOURCE) || defined(IRIX)
  if (afd == -1) return;
#else
  return;
#endif
}
void SetAudioGain(void)
{
#if defined(__svr4__) || defined(sun) || defined(FreeBSD)
  audio_info_t sun_gapo;
#elif defined(_HPUX_SOURCE)
  struct audio_gain ag;
#elif defined(IRIX)
  long params[4];
#endif
  int volume = shared->volumePosition;

#ifdef AUDIOFILE
  if (useAF) {
    AFSetOutputGain(ac, volume * 2);
    /*
      AFSetOutputGain(ac, (volume - 10)/3); 
    fprintf(stderr, "CTR audio.c: volume changed to %d.\n", volume);
    */
    return;
  }
#endif
#if defined(__svr4__) || defined(sun) || defined(FreeBSD)
   if (afd == -1) return;
  ioctl(cfd, AUDIO_GETINFO, &sun_gapo);
  sun_gapo.play.gain = volume * 2;
  ioctl(cfd, AUDIO_SETINFO, &sun_gapo);
  return;
#elif defined(_HPUX_SOURCE)
  ioctl(cfd, AUDIO_GET_GAINS, &ag);
  ag.cgain[0].transmit_gain = AUDIO_OFF_GAIN+(AUDIO_MAX_GAIN-AUDIO_OFF_GAIN)*volume/100;
  ag.cgain[1].transmit_gain = ag.cgain[0].transmit_gain;
  ioctl(cfd, AUDIO_SET_GAINS, &ag);
  return;
#elif defined(IRIX)
  params[0] = AL_LEFT_SPEAKER_GAIN;
  params[1] = volume;
  params[2] = AL_RIGHT_SPEAKER_GAIN;
  params[3] = volume;
  ALsetparams(audioDevice, params, 4);
  return;
#else
  return;
#endif
}

void AudioInterpolate(char *rBuf, int len, char * cBuf, int convLen)
{
  unsigned char *rawBuf = (unsigned char *)rBuf;
  unsigned char *convBuf = (unsigned char *)cBuf;
  int i;
  float factor = (float)len / (float)convLen;

#ifdef AUDIOFILE
  if ((!useAF) && afd == -1) return;
#else
  if (afd == -1) return;
#endif
  
  /*
  Fprintf(stderr, "factor %5.2lf, len %d, convLen %d\n", factor, len, convLen);
  */
  if (shared->audioPara.bytesPerSample == 1)
  {
    for (i=0; i<convLen; i++)
    {
      float a = i * factor;
      int k = (int) (a + 0.5);
      /*
      int k = a;
      a = a - k;
      */
      if (k < len - 1) { /* interpolation algorithm can be improved */
	/*
        convBuf[i] = AF_comp_u[(int)(a * AF_exp_u[(int)rawBuf[k+1] & 0xff] + (1.0 - a) *
				     AF_exp_u[(int)rawBuf[k] & 0xff]) + 8192];
	*/
	
        convBuf[i] = rawBuf[k];
	
      }
      else
        convBuf[i] = rawBuf[len-1];
    }
  }
  else
  {
    fprintf(stderr, "CTR audio.c error: multi_byte samples not supported.\n");
  }
}
