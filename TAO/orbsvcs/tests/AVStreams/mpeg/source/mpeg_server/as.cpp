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
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include "include/common.h"
#include "mpeg_shared/fileio.h"
#include "mpeg_shared/routine.h"
#include "mpeg_shared/com.h"
#include "server_proto.h"

ACE_RCSID(mpeg_server, as, "$Id$")

//extern int Mpeg_Global::session_limit, Mpeg_Global::session_num;

static bytes_sent = 0;
static time_t start_time;

static int conn_tag;

static int serviceSocket;
static int audioSocket = -1;

static char audioFile[PATH_SIZE];
static int fd;
static AudioParameter audioPara;
static int totalSamples;
static int fileSize;
static unsigned char cmd;
static live_source = 0;

static int CmdRead(char *buf, int psize)
{
  int res = wait_read_bytes(serviceSocket, buf, psize);
  if (res == 0) return (1);
  if (res == -1) {
    fprintf(stderr, "AS error on read cmdSocket, size %d", psize);
   ACE_OS::perror ("");
    return (-1);
  }
  return 0;
}

static void CmdWrite(char *buf, int size)
{
  int res = wait_write_bytes(serviceSocket, buf, size);
  if (res == -1) {
    if (errno != EPIPE)ACE_OS::perror ("AS writes to serviceSocket");
    ACE_OS::exit (errno != EPIPE);
  }
}

static int INITaudio(void)
{
  int result;
  int failureType; /* 0 - can't open file, 1 - can't open live source */
  INITaudioPara para;

  result = CmdRead((char *)&para, sizeof(para));
  if (result != 0)
    return result;
#ifdef NeedByteOrderConversion
  para.sn = ntohl(para.sn);
  para.version = ntohl(para.version);
  para.nameLength = ntohl(para.nameLength);
  para.para.encodeType = ntohl(para.para.encodeType);
  para.para.channels = ntohl(para.para.channels);
  para.para.samplesPerSecond = ntohl(para.para.samplesPerSecond);
  para.para.bytesPerSample = ntohl(para.para.bytesPerSample);
#endif
  if (para.nameLength>0)
    result = CmdRead(audioFile, para.nameLength);
  if (result != 0)
    return result;
  if (Mpeg_Global::session_num > Mpeg_Global::session_limit || para.version != VERSION) {
    char errmsg[128];
    cmd = CmdFAIL;
    CmdWrite((char *)&cmd, 1);
    if (Mpeg_Global::session_num > Mpeg_Global::session_limit) {
      sprintf(errmsg,
	      "Too many sessions being serviced, please try again later.\n");
    }
    else {
      sprintf(errmsg, "Version # not match, AS %d.%02d, Client %d.%02d",
	      VERSION / 100, VERSION % 100,
	      para.version / 100, para.version % 100);
    }
    write_string(serviceSocket, errmsg);
    return(1);
  }
  ACE_OS::memcpy (&audioPara, &para.para, sizeof(audioPara));
  /*
  fprintf(stderr, "Client Audio para: encode %d, ch %d, sps %d, bps %d.\n",
	  para.para.encodeType, para.para.channels,
	  para.para.samplesPerSecond, para.para.bytesPerSample);
   */
  audioFile[para.nameLength] = 0;
  {
    int len =ACE_OS::strlen (audioFile);
    if (strncasecmp("LiveAudio", audioFile, 9) &&
	strcasecmp(".au", audioFile+len-3)) {
      char errmsg[128];
      cmd = CmdFAIL;
      CmdWrite((char *)&cmd, 1);
      sprintf(errmsg, "%s without suffix .au", audioFile);
      write_string(serviceSocket, errmsg);
      return(1);
    }
  }
  /*
  fprintf(stderr, "Audio file %s got.\n", audioFile);
  */

  if (!strncasecmp("LiveAudio", audioFile, 9)) {
    fd = OpenLiveAudio(&(para.para));
    if (fd == -1) {
      failureType = 1;
      goto failure;
    }
    fileSize =0x7fffffff;
    totalSamples = fileSize / audioPara.bytesPerSample;
    live_source = 1;
  }
  else {
    LeaveLiveAudio();
    fd = open(audioFile, O_RDONLY);
    if (fd == -1)
    {
      fprintf(stderr, "AS error on opening audio file %s", audioFile);
     ACE_OS::perror ("");
      failureType = 0;
      goto failure;
    }

    /* Try to get audioFile format audioPara here */

    /* figure out totalsamples */
    fileSize = lseek(fd, 0L, SEEK_END);
    lseek(fd, 0L, SEEK_SET);
    totalSamples = fileSize / audioPara.bytesPerSample;
    /*
    fprintf(stderr, "Total Samples=%d in audio file %ss.\n", totalSamples, audioFile);
    */
  }
  {
    INITaudioReply reply;
    
    reply.para.encodeType = htonl(audioPara.encodeType);
    reply.para.channels = htonl(audioPara.channels);
    reply.para.samplesPerSecond = htonl(audioPara.samplesPerSecond);
    reply.para.bytesPerSample = htonl(audioPara.bytesPerSample);
    reply.totalSamples = htonl(totalSamples);

    reply.live = htonl(live_source);
    reply.format = htonl(AUDIO_RAW);
    
    CmdWrite((char *)&cmd, 1);
    CmdWrite((char *)&reply, sizeof(reply));
  }
  return 0;
  
 failure:
  {
    /*
    fprintf(stderr, "AS error: failed initializing audio file.\n");
    */
    cmd = CmdFAIL;
    CmdWrite((char *)&cmd, 1);
    write_string(serviceSocket,
		 failureType == 0 ? "Failed to open audio file for read." :
		 "Failed to connect to live audio source.");
    return(1);
  }
}

#define DATABUF_SIZE 500

static int databuf_size;
static int cmdsn;
static int nextsample;
static int sps; /* audio play speed given by the client: samples per second */
static int spslimit;
static int spp; /* samples per packet */
static AudioPacket * pktbuf = NULL;

/* send a packet of audio samples to audioSocket
   returns: 0 - no more data from audio file: EOF reached;
            1 - More data is available from the audio file */
static int send_packet(int firstSample, int samples)
{
  long offset = firstSample * audioPara.bytesPerSample;
  int size = samples * audioPara.bytesPerSample;
  char * buf = (char *)pktbuf + sizeof(*pktbuf);
  int len;
  int resent = 0;
  int segsize, sentsize;

  if (live_source) {
    len = ReadLiveAudioSamples(buf, samples);
    len *= audioPara.bytesPerSample;
  }
  else {
    lseek(fd, offset, SEEK_SET);
    while ((len = ACE_OS::read (fd, buf, size)) == -1) {
      if (errno == EINTR)
	continue;   /* interrupted */
     ACE_OS::perror ("AS error on read audio file");
      return(-1);
    }
    if (len < audioPara.bytesPerSample) {
      return 0;
    }
  }
  
  samples = len / audioPara.bytesPerSample;
  len = samples * audioPara.bytesPerSample;
  bytes_sent += len;
  pktbuf->firstSample = htonl(firstSample);
  pktbuf->samples = htonl(samples);
  pktbuf->actualSamples = htonl(samples);
  pktbuf->dataBytes = htonl(len);
  if (spslimit < sps) { /* interpolation needed */
    SFprintf(stderr, "AS audio sample interpolation not available yet.\n");
  }
  segsize = sizeof(*pktbuf) + len;
  if (conn_tag != 0) {
    while ((sentsize = ACE_OS::write (audioSocket, (char *)pktbuf, segsize)) == -1) {
      if (errno == EINTR) /* interrupted */
	continue;
      if (errno == ENOBUFS) {
	if (resent) {
	 ACE_OS::perror ("AS Warning, pkt discarded because");
	  break;
	}
	else {
	  resent = 1;
	  usleep(5000);
	  continue;
	}
      }
      if (errno != EPIPE) {
	fprintf(stderr, "AS error on send audio packet %d(%d):",
		firstSample, samples);
	perror("");
      }
      ACE_OS::exit ((errno != EPIPE));
    }
  }
  else {
    sentsize = wait_write_bytes(audioSocket, (char *)pktbuf, segsize);
    if (sentsize == -1) {
      if (errno != EPIPE) {
	fprintf(stderr, "AS error on send audio packet %d(%d):",
		firstSample, samples);
	perror("");
      }
      ACE_OS::exit ((errno != EPIPE));
     }
  }
  if (sentsize < segsize) {
    SFprintf(stderr, "AS warning: message size %dB, sent only %dB\n",
	    segsize, sentsize);
  }
  /*
  SFprintf(stderr, "AS sent audio packet %d(%d).\n",
	  firstSample, samples);
  */
  return (len < size ? 0 : 1);
}

/* send a packet of audio samples to audioSocket
   returns: 0 - no more data from audio file: EOF reached;
            1 - More data is available from the audio file */
static int SendPacket(void)
{
  int moredata;
  pktbuf->cmdsn = htonl(cmdsn);
  pktbuf->resend = htonl(0);
  pktbuf->samplesPerSecond = htonl(sps);
  moredata = send_packet(nextsample, spp);
  if (moredata) {
    nextsample += spp;
  }
  return moredata;
}

static void ResendPacket(int firstsample, int samples)
{
  pktbuf->cmdsn = htonl(cmdsn);
  pktbuf->resend = htonl(1);
  pktbuf->samplesPerSecond = htonl(sps);
  while (samples > 0) {
    int size = samples < spp ? samples : spp;
    send_packet(firstsample, size);
    firstsample += size;
    samples -= size;
    if (samples > 0) {
      usleep(10000);
    }
  }
}

#define SPEEDUP_SCALE 5
#define MAX_RESEND_REQS 10
#define FBBUF_SIZE (sizeof(AudioFeedBackPara) + \
                       (MAX_RESEND_REQS - 1) * sizeof(APdescriptor))

static AudioFeedBackPara * fbpara = NULL;

static int PLAYaudio(void)
{
  int hasdata = 1;
  int addSamples;
  int packets = 0;
  unsigned nextTime;
  int upp;  /* micro-seconds per packet */
  int delta_sps = 0;  /* compensation for sps from feedback msgs */
  int nfds = (serviceSocket > audioSocket ? serviceSocket : audioSocket) + 1;
  int result;
  /*
  fprintf(stderr, "PLAY . . .\n");
  */
  {
    PLAYaudioPara para;
    result = CmdRead((char *)&para, sizeof(para));
    if (result != 0)
      return result;
#ifdef NeedByteOrderConversion
    para.sn = ntohl(para.sn);
    para.nextSample = ntohl(para.nextSample);
    para.samplesPerSecond = ntohl(para.samplesPerSecond);
    para.samplesPerPacket = ntohl(para.samplesPerPacket);
    para.ABsamples = ntohl(para.ABsamples);
    para.spslimit = ntohl(para.spslimit);
#endif
    nextsample = para.nextSample;
    cmdsn = para.sn;
    sps = para.samplesPerSecond;
    spslimit = para.spslimit;
    spp = para.samplesPerPacket;
    addSamples = para.ABsamples / 2;
    if (spp * audioPara.bytesPerSample > databuf_size) {
      spp = databuf_size / audioPara.bytesPerSample;
    }
    /*
    SFprintf(stderr, "AS got CmdPLAY: sps %d\n", sps);
    */
  }
  /*
  fprintf(stderr, "AS: nextSampe=%d for PLAY.\n", para.nextSample);
  */

  upp = (int)(1000000.0 / ((double)sps / (double)spp));
  nextTime = get_usec();
  
  CmdWrite((char *)&nextTime, sizeof(int));

  if (live_source) {
    StartPlayLiveAudio();
  }
  
  for (;;)
  {
    struct fd_set read_mask, write_mask;
    struct timeval tval;
    unsigned curTime = get_usec();
    
    if (hasdata) {
      if (addSamples < - spp) {  /* slow down by not sending packets */
	nextTime += upp;
	addSamples += spp;
      }
      else {
	int need_sleep = 0;
	while (nextTime <= curTime && hasdata) {
	  if (need_sleep) usleep(5000);
	  hasdata = SendPacket();
	  need_sleep = 1;
	  packets ++;
	  nextTime += upp;
	  if (addSamples > 0 && packets % SPEEDUP_SCALE == 0) {
	    addSamples -= spp;
	    usleep(5000);
	    hasdata = SendPacket();
	    packets ++;
	  }
	}
      }
    }
    curTime = nextTime - curTime;
    if (curTime > 5000000) curTime = 5000000; /* limit on 5 second weit time
						 in case error happens */
    tval.tv_sec = curTime / 1000000;
    tval.tv_usec = curTime % 1000000;
    FD_ZERO(&read_mask);
    FD_SET(serviceSocket, &read_mask);
    FD_SET(audioSocket, &read_mask);
#ifdef _HPUX_SOURCE
    if (select(nfds, (int *)&read_mask, NULL, NULL, hasdata ? &tval : NULL) == -1)
#else
    if (select(nfds, &read_mask, NULL, NULL, hasdata ? &tval : NULL) == -1)
#endif
    {
      if (errno == EINTR)
        continue;
     ACE_OS::perror ("AS error on select reading or writing");
      return(-1);
    }
    if (FD_ISSET(serviceSocket, &read_mask)){  /* STOP, SPEED, or CLOSE*/
      unsigned char tmp;
      result = CmdRead((char *)&tmp, 1);
      if (result != 0)
        return result;
      switch (tmp)
      {
      case CmdSPEED:
	{
	  SPEEDaudioPara para;
	  result = CmdRead((char *)&para, sizeof(para));
          if (result != 0)
            return result;
#ifdef NeedByteOrderConversion
	  para.sn = ntohl(para.sn);
	  para.samplesPerSecond = ntohl(para.samplesPerSecond);
	  para.samplesPerPacket = ntohl(para.samplesPerPacket);
	  para.spslimit = ntohl(para.spslimit);
#endif
	  sps  = para.samplesPerSecond;
	  spslimit = para.spslimit;
	  spp = para.samplesPerPacket;
	  if (spp * audioPara.bytesPerSample > databuf_size) {
	    spp = databuf_size / audioPara.bytesPerSample;
	  }
	  delta_sps = 0;  /* reset compensation value */
	  upp = (int)(1000000.0 / ((double)sps / (double)spp));
	  /*
	  SFprintf(stderr, "AS got CmdSPEED: sps %d\n", sps);
	  */
	}
	break;
      case CmdSTOP:
	{
	  int val;
	  cmd = tmp;
	  /*
	     fprintf(stderr, "AS: CmdSTOP. . .\n");
	     */
	  result = CmdRead((char *)&val, sizeof(int));
          if (result != 0)
            return result;
	  /*
	  CmdWrite(AUDIO_STOP_PATTERN,ACE_OS::strlen (AUDIO_STOP_PATTERN));
	  */
	  if (live_source) {
	    StopPlayLiveAudio();
	  }
	  return 0;  /* return from PLAYaudio() */
	}
      case CmdCLOSE:
	if (live_source) {
	  StopPlayLiveAudio();
	}
	return(1);  /* The whole AS session terminates */
      default:
	if (live_source) {
	  StopPlayLiveAudio();
	}
	fprintf(stderr, "AS error: cmd=%d while expects STOP/SPEED/CLOSE.\n", tmp);
	return(-1);
      }
    }
    
    if (FD_ISSET(audioSocket, &read_mask)){  /* Feedback packet */
      int bytes, len;
      for (;;) {
	if (conn_tag >= 0) {
	  len = wait_read_bytes(audioSocket, (char *)fbpara, sizeof(*fbpara));
	  if (len == 0) return(1); /* connection broken */
	  else if (len < 0) { /* unexpected error */
	   ACE_OS::perror ("AS read1 FB");
	    return(-1);
	  }
	}
	else { /* discard mode packet stream, read the whole packet */
	  len = ACE_OS::read (audioSocket, (char *)fbpara,  FBBUF_SIZE);
	}
	if (len == -1) {
	  if (errno == EINTR) continue; /* interrupt */
	  else {
	    if (errno != EPIPE && errno != ECONNRESET)ACE_OS::perror ("AS failed to ACE_OS::read () fbmsg header");
	    break;
	  }
	}
	break;
      }
      if (len < sizeof(*fbpara)) {
	if (len > 0) fprintf(stderr,
			 "AS warn ACE_OS::read () len %dB < sizeof(*fbpara) %dB\n",
			 len, sizeof(*fbpara));
	continue;
      }
#ifdef NeedByteOrderConversion
      fbpara->type = ntohl(fbpara->type);
#endif
      bytes = (fbpara->type > 0) ?
              sizeof(APdescriptor) * (fbpara->type - 1) :
              0;
      if (bytes > 0) {
	if (conn_tag >= 0) { /* not discard mode packet stream,
				read the rest of packet */
	  len = wait_read_bytes(audioSocket,
				((char *)fbpara) + sizeof(*fbpara),
				bytes);
	  if (len == 0) return(1); /* connection broken */
	  else if (len < 0) { /* unexpected error */
	   ACE_OS::perror ("AS read2 FB");
	    return(-1);
	  }
	  len += sizeof(*fbpara);
	}
      }
      bytes += sizeof(*fbpara);
      if (len < bytes) {
	if (len > 0) fprintf(stderr,
			     "AS only read partial FBpacket, %dB out of %dB.\n",
			     len, bytes);
	continue;
      }
      if (live_source) {  /* ignore all feedback messags for live source */
	continue;
      }
      
#ifdef NeedByteOrderConversion
      fbpara->cmdsn = ntohl(fbpara->cmdsn);
#endif
      if (len != sizeof(*fbpara) +
	  (fbpara->type ? (fbpara->type -1) * sizeof(APdescriptor) : 0)) {
	/* unknown message, discard */
	SFprintf(stderr, "AS Unkown fb msg: len = %d, type = %d\n",
		len, fbpara->type);
	continue;
      }
      if (fbpara->cmdsn != cmdsn) {  /* discard the outdated message */
	continue;
      }
#ifdef NeedByteOrderConversion
      {
	int i, * ptr = (int *)fbpara + 2;
	for (i = 0; i < (len >> 2) - 2; i++) *ptr = ntohl(*ptr);
      }
#endif
      if (fbpara->type == 0) { /* feedback message */
	/*
	SFprintf(stderr, "AS got fbmsg: addsamples %d, addsps %d\n",
		fbpara->data.fb.addSamples, fbpara->data.fb.addsps);
	*/
	addSamples += fbpara->data.fb.addSamples;
	if (fbpara->data.fb.addsps) {
	  delta_sps += fbpara->data.fb.addsps;
	  upp = (int)(1000000.0 / ((double)(sps + delta_sps) / (double)spp));
	}
      }
      else { /* resend requests */
	APdescriptor * req = &(fbpara->data.ap);
	int i;
	/*
	SFprintf(stderr, "AS got %d resend reqs\n", fbpara->type);
	*/
	for (i = 0; i < fbpara->type; i ++) {
	  ResendPacket(req->firstSample, req->samples);
	  req ++;
	}
      }
    }
  }
}

static void on_exit_routine(void)
{
  struct sockaddr_in peeraddr_in;
  int size = sizeof(peeraddr_in);
  
  /*
  fprintf(stderr, "An AS session terminated\n");
  */
  if (getpeername(serviceSocket,
		  (struct sockaddr *)&peeraddr_in, &size) == 0 &&
      peeraddr_in.sin_family == AF_INET) {
    if (strncmp(inet_ntoa(peeraddr_in.sin_addr), "129.95.50", 9)) {
      struct hostent *hp;
      time_t val =ACE_OS::time (NULL);
      char * buf = ACE_OS::ctime (&start_time);

      hp = gethostbyaddr((char *)&(peeraddr_in.sin_addr), 4, AF_INET);
      buf[strlen(buf)-1] = 0;
      printf("%s: %s %3dm%02ds %dB %s\n",
	     buf,
	     hp == NULL ? inet_ntoa(peeraddr_in.sin_addr) : hp->h_name,
	     (val - start_time) / 60, (val - start_time) % 60,
	     bytes_sent, audioFile);
    }
  }
  ComCloseConn(serviceSocket);
  ComCloseConn(audioSocket);
}

int AudioServer(int ctr_fd, int data_fd, int rttag, int max_pkt_size)
{
  int result;
  
  serviceSocket = ctr_fd;
  audioSocket = data_fd;
  conn_tag = max_pkt_size;

  if (max_pkt_size < 0) max_pkt_size = -max_pkt_size;
  else if (max_pkt_size == 0) max_pkt_size = 1024 * 1024;
  
  start_time =ACE_OS::time (NULL);
  
  atexit(on_exit_routine);

  result = INITaudio();

  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%P|%t) AudioServer: "),
                      result);

  fbpara = (AudioFeedBackPara *)ACE_OS::malloc(FBBUF_SIZE);
  if (fbpara == NULL) {
   ACE_OS::perror ("AS failed to allocate mem for fbpara");
    return (-1);
  }

  databuf_size = max_pkt_size - sizeof(*pktbuf);
  if (databuf_size > DATABUF_SIZE) databuf_size = DATABUF_SIZE;
  
  pktbuf = (AudioPacket *)ACE_OS::malloc(sizeof(*pktbuf) + databuf_size);
  if (pktbuf == NULL) {
   ACE_OS::perror ("AS failed to allocate mem for pktbuf");
    return(-1);
  }

  for (;;)
  {
    /*
    fprintf(stderr, "AS: waiting for a new command...\n");
    */
    result = CmdRead((char *)&cmd, 1);
    if (result != 0)
      return result;
    switch (cmd)
    {
    case CmdPLAY:
      result = PLAYaudio();
      if (result != 0)
        return result;
      break;
    case CmdCLOSE:
      /*
      fprintf(stderr, "a session closed.\n");
      */
      return(0);
      break;
    default:
      fprintf(stderr, "audio channel command %d not recoganizeable\n", cmd);
      break;
    }
  }
}
