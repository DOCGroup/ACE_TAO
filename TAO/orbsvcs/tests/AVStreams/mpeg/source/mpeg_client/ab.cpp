
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
#include <signal.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>
#ifdef __svr4__
#include <stropts.h>
#include <sys/conf.h>
#endif

#include "include/common.h"
#include "newproto.h"
#include "global.h"
#include "mpeg_shared/filters.h"
#include "mpeg_shared/fileio.h"
#include "mpeg_shared/com.h"

ACE_RCSID(mpeg_client, ab, "$Id$")

/* magic number -- deviation is considered
   caused by clock drift only if rate <= 1/MAX_CLOCK_DRIFT.
   */
#define MAX_CLOCK_DRIFT 50

#define max(a,b) ((a)>(b) ? (a) : (b))
#define min(a,b) ((a)<(b) ? (a) : (b))

typedef struct {
  int bufsize; /* number of bytes for the buffer pointed by 'buf' */
  char * buf;  /* pointer to the data buffer area */
  int bps; /* current byte-per-sample */
  int size;  /* number of samples the buffer can hold */
  int samples; /* number of samples in the buffer; */
  int stuff; /* number of stuff samples to be read by ABgetSamples() */
  int ts;  /* tail-sample: the next sample to be comsumed by CTR */
  int hs;  /* head-sample: the next sample to be expected from the network */
  int tind; /* index of the ts-sample in the buf */
} ABBuffer;

static ABBuffer * abuf;
static int sid;
static int exit_tag = 0;
static int savedSocket;

static void set_silence(char *buf, int samples)
{
  memset(buf, 0xff, samples * shared->audioPara.bytesPerSample);
}

void ABinitBuf(int size) /* size in bytes */
{
  abuf = (ABBuffer *)creat_shared_mem(size);
  abuf->bufsize = size - sizeof(*abuf);
  abuf->buf = (char *)abuf + sizeof(*abuf);
  sid = creat_semaphore();
}

void ABflushBuf(int nextSample)  /* flush the whole buffer */
{
  enter_cs(sid);
  abuf->bps = shared->audioPara.bytesPerSample;
  abuf->size = abuf->bufsize / abuf->bps;
  abuf->samples = 0;
  abuf->stuff = 0;
  abuf->ts = abuf->hs = nextSample;
  abuf->tind = 0;
  set_silence(abuf->buf, abuf->size);
  leave_cs(sid);
}

int ABcheckSamples(void) /* returns # of samples in ABbuf */
{
  return abuf->samples;
}

int ABgetSamples(char * buf, int samples)
    /* read at most given number of samples from AB to buf, returns
       number of sample actually read */
{
  int as;
  int tsamples = samples;
  enter_cs(sid);
  if (abuf->stuff > 0) {
    as = min(abuf->stuff, samples);
    set_silence(buf, as);
    abuf->stuff -= as;
    samples -= as;
    buf += as * abuf->bps;
  }
  if (samples > 0) {
    as = min(samples, max(0, abuf->samples));
	 /* there may be fewer samples in abuf */
    if (as > 0) {
      int part1 = min(as, abuf->size - abuf->tind);
      memcpy(buf, abuf->buf + (abuf->bps * abuf->tind), part1 * abuf->bps);
      set_silence(abuf->buf + (abuf->bps * abuf->tind), part1);
      if (part1 < as) {  /* This read cross the boundary of abuf */
	memcpy(buf + (part1 * abuf->bps),
	       abuf->buf,
	       (as - part1) * abuf->bps);
	set_silence(abuf->buf, as - part1);
      }
    }
    abuf->samples -= samples;
    abuf->ts += samples;
    abuf->tind = (abuf->tind + samples) % abuf->size;
    if (as < samples) set_silence(buf + (as * abuf->bps), samples - as);
  }
  leave_cs(sid);
  return tsamples;
}

/* if samples < 0; then stuff |samples| silient samples to ABgetSamples(),
   otherwise wipe out this number of samples from AB */
int ABskipSamples(int samples)
{
  enter_cs(sid);
  if (samples <= 0) {
    abuf->stuff += (- samples);
  }
  else {
    abuf->samples -= samples;
    abuf->ts += samples;
    abuf->tind = (abuf->tind + samples) % abuf->size;
  }
  leave_cs(sid);
  return samples;
}

void ABdeleteBuf(void)
{
  remove_shared_mem((char *)abuf);
}

void ABdeleteSem(void)
{
  remove_semaphore(sid);
}

/* SIGUSR1 from CTR is for killing this process, without affecting any other ones. */

static void exit_on_kill(void)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) ABprocess killed \n"));
  extern void set_exit_routine_tag(int tag);
  set_exit_routine_tag(0);
  //  ComCloseConn(savedSocket);
  VBdeleteBuf();
  exit(0);
}

static void usr1_handler(int sig)
{
  cerr << "ABprocess got sigusr1\n";
  exit_on_kill ();
}


static void usr2_handler(int sig)
{
  Fprintf(stderr, "VB void usr2_handler (supposed for stat).\n");
}

#define PACKET_SIZE 8192
#define STARTUP_WAIT 10000000
#define ACTION_WAIT 5000000

void ABprocess(int dataSocket)
{
  
  AudioPacket * packet;
  char * pkt_data;
  int conn_tag = shared->audioMaxPktSize;

  /* following are for feedback */
  int fbstate = 0;
  unsigned waketime;
  int pcmdsn = -1; /* previous cmdsn */
  
  exit_tag = 0;
  
  savedSocket = dataSocket;
  
  setsignal(SIGUSR1, usr1_handler);
  setsignal(SIGUSR2, usr2_handler);

  packet = (AudioPacket *)malloc(PACKET_SIZE);
  if (packet == NULL) {
    perror("AB failed to allocate mem for packet buffer");
    exit(1);
  }
  pkt_data = (char *)packet + sizeof(*packet);

  for (;;) {
    int len;
    int bytes;
    if (conn_tag >= 0) {
      bytes = sizeof(*packet);


      len = wait_read_bytes(dataSocket, (char *)packet, bytes);
      if (exit_tag) exit_on_kill();
    }
    else {  /* discard mode packet stream, read all bytes */
      bytes = PACKET_SIZE;
      len = read(dataSocket, (char *)packet, bytes);
      //      ACE_DEBUG ((LM_DEBUG,"(%P|%t) ABprocess: got a %d sized packet\n",len));
      if (exit_tag) exit_on_kill();
      if (len == -1) {
	if (errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN) {
	  usleep(10000);
	  if (exit_tag) exit_on_kill();
	  continue;
	}
	perror("AB read() audio packet from discard-mode packet stream");
	exit(1);
      }
    }
    if (len == 0) {
      fprintf(stderr, "Error: AB found dataSocket broken\n");
      exit(1);
    }
    if (len < sizeof(*packet)) { /* unknown packet */
      fprintf(stderr, "Warn: AB discard len = %d bytes of supposed header.\n", len);
      continue;
    }
#ifdef NeedByteOrderConversion
    packet->dataBytes = ntohl(packet->dataBytes);
#endif

    //    ACE_DEBUG ((LM_DEBUG,"(%P|%t) ABprocess: Received  %d sized packet\n",len));
    if (packet->dataBytes <= 0) {
      fprintf(stderr, "AB Error: pkt->dataBytes %d, len %d\n",
	      packet->dataBytes,
	      len);
      exit(1);
    }
    bytes = packet->dataBytes + sizeof(*packet);
    
    if (bytes > PACKET_SIZE) {
      Fprintf(stderr, "Fatal error: AB packet buf (%dB) too small (%d)\n",
	      PACKET_SIZE, bytes);
      exit(1);
    }
    if (conn_tag >= 0) {
      len = wait_read_bytes(dataSocket, (char *)packet + sizeof(*packet),
			    bytes - sizeof(*packet));
      if (len <= 0) {
	if (len == -1) perror("AB encounter error on wait_read_bytes()");
	else fprintf(stderr, "AB encounter EOF on wait_read_bytes()\n");
      }
    }
#ifdef NeedByteOrderConversion
    packet->cmdsn = ntohl(packet->cmdsn);
    packet->samplesPerSecond = ntohl(packet->samplesPerSecond);
    packet->resend = ntohl(packet->resend);
    packet->firstSample = ntohl(packet->firstSample);
    packet->samples = ntohl(packet->samples);
    packet->actualSamples = ntohl(packet->actualSamples);
    /* dataBytes already byte-reordered */
#endif
    /*
    Fprintf(stderr, "AB got a packet: %d(%d)\n",
	packet->firstSample, packet->samples);
    */
    /*
    if (packet->firstSample % 10240 && !packet->resend) continue;
    */
    if (packet->samples * abuf->bps > PACKET_SIZE - sizeof(*packet)) {
      fprintf(stderr, "Fatal error: AB has too small packet buffer, %d out of %d\n",
	      PACKET_SIZE, packet->samples * abuf->bps + sizeof(*packet));
      exit(1);
    }

    if (packet->cmdsn != shared->cmdsn) { /* outdated packet */
      /*
      Fprintf(stderr, "AB discarded an outdated packet\n");
      */
      continue;
    }
    enter_cs(sid);
    if (packet->firstSample + packet->samples <= abuf->ts) {
      /* all samples too late, discard it */
      abuf->hs = max(abuf->hs, packet->firstSample + packet->samples);
      abuf->samples = abuf->hs - abuf->ts;
      leave_cs(sid);
      /*
      Fprintf(stderr, "AB all sample in packet %d(%d) too late\n",
	      packet->firstSample, packet->samples);
      */
      goto feedback_code;
    }
    if (packet->firstSample >= abuf->ts + abuf->size) {
      /* all samples too early, discard the packet */
      abuf->hs = max(abuf->hs, packet->firstSample + packet->samples);
      abuf->samples = abuf->hs - abuf->ts;
      leave_cs(sid);
      /*
      Fprintf(stderr, "AB all sample in packet %d(%d) too early\n",
	      packet->firstSample, packet->samples);
      */
      goto feedback_code;
    }
    
    if (packet->samples > packet->actualSamples) {
      leave_cs(sid);
      fprintf(stderr, "Error: AB interpolation not available yet.\n");
      exit(1);
    }
    
    {
      int oldhs = abuf->hs;
      int firstSample = max(packet->firstSample, abuf->ts);
      int samples = min(packet->samples -
			  (firstSample - packet->firstSample),
			(abuf->ts + abuf->size) - packet->firstSample);
      char * data = pkt_data +
                    (firstSample - packet->firstSample) * abuf->bps;
      int dstart = (abuf->tind + (firstSample - abuf->ts)) % abuf->size;
      int part1 = min(samples, abuf->size - dstart);
      memcpy(abuf->buf + (dstart * abuf->bps), data, part1 * abuf->bps);
      if (part1 < samples) {
	memcpy(abuf->buf, data + part1 * abuf->bps,
	       (samples - part1) * abuf->bps);
      }
      abuf->hs = max(abuf->hs, packet->firstSample + packet->samples);
      abuf->samples = abuf->hs - abuf->ts;
      dstart =max(oldhs, abuf->ts);
      
      leave_cs(sid);
      
      part1 = firstSample - dstart;
      if (packet->resend) {
	Fprintf(stderr, "AB got resent %d(%d)\n",
		packet->firstSample, packet->samples);
      }
      else if (part1 > 0) {
	int res;
	AudioFeedBackPara para;
	Fprintf(stderr, "AB found gap %d(%d)\n", dstart, part1);
	para.cmdsn = htonl(shared->cmdsn);
	para.type = htonl(1);
	para.data.ap.firstSample = htonl(dstart);
	para.data.ap.samples = htonl(part1);
	if (conn_tag != 0) { /* packet stream */
	  while ((res = write(dataSocket, (char *)&para, sizeof(para))) == -1)
	  {
	    if (errno == EINTR)
	      continue;
	    if (errno == ENOBUFS) {
	      perror("AB Warning, resend-req packet discarded for");
	      break;
	    }
	    perror("AB error, resend-req  packet sending failed");
	    exit(1);
	  }
	}
	else {
	  res = wait_write_bytes(dataSocket, (char *)&para, sizeof(para));
	  if (res == -1) {
	    perror("AB error, resend-req  packet sending failed");
	    exit(1);
	  }
	}
	if (res < sizeof(para)) {
	  fprintf(stderr, "AB warn: send() for gap res %dB < sizeof(para) %dB\n",
		  res, sizeof(para));
	}
      }
    }
	
    /* following is feedback algorithm */
    
  feedback_code:
    
    if (shared->live || (!shared->config.syncEffective)) continue;
    /*
    fprintf(stderr, "AB fbstate = %d\n", fbstate);
    */
    switch (fbstate) {
    case 0: /* startup init */
      if (pcmdsn != packet->cmdsn) {
	fbstate = 0;
	pcmdsn = packet->cmdsn;
      }
      else {
	fbstate = 1;
	waketime = get_usec() + STARTUP_WAIT;
      }
      break;
    case 1: /* startup wait */
      if (pcmdsn != packet->cmdsn) {
	fbstate = 0;
	break;
      }
      if (get_usec() >= waketime) {
	fbstate = 2;
      }
      break;
    case 2: /* monitoring */
      if (pcmdsn != packet->cmdsn) {
	fbstate = 0;
	break;
      }
      if (abuf->samples < abuf->size >>2 ||
	  abuf->samples > (abuf->size * 3) >> 2) {
	/* feedback action needed */
	AudioFeedBackPara para;
	int res;
	para.data.fb.addsps = 0;
	para.data.fb.addSamples = (abuf->size >> 2) - abuf->samples;
	
	Fprintf(stderr, "AB sends fb: %dsps, %dsamples\n",
		para.data.fb.addsps,
		para.data.fb.addSamples);
	para.cmdsn = htonl(shared->cmdsn);
	para.type = htonl(0);
	para.data.fb.addsps = htonl(para.data.fb.addsps);
	para.data.fb.addSamples = htonl(para.data.fb.addSamples);
	if (conn_tag != 0) {
	  while ((res = write(dataSocket, (char *)&para, sizeof(para))) == -1)
	  {
	    if (errno == EINTR)
	    continue;
	    if (errno == ENOBUFS) {
	      perror("AB Warning, fb packet discarded for");
	      break;
	    }
	    perror("AB error, fb  packet sending failed");
	    exit(1);
	  }
	}
	else {
	  res = wait_write_bytes(dataSocket, (char *)&para, sizeof(para));
	  if (res == -1) {
	    perror("AB error, fb  packet sending failed");
	    exit(1);
	  }
	}
	if (res < sizeof(para)) {
	  fprintf(stderr, "AB warn: send() for sync res %dB < sizeof(para) %dB\n",
		  res, sizeof(para));
	}
	
	waketime = get_usec() + STARTUP_WAIT;
	fbstate = 1;
      }
      break;
    default:
      break;
    }
  }
}  
