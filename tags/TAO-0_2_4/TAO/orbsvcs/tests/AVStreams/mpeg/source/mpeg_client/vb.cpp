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
#if defined(__svr4__) || defined(IRIX)
#include <stropts.h>
#include <sys/conf.h>
#endif
#include "include/common.h"
#include "newproto.h"
#include "global.h"
#include "mpeg_shared/filters.h"
#include "mpeg_shared/fileio.h"
#include "mpeg_shared/com.h"

ACE_RCSID(mpeg_client, vb, "$Id$")

/* magic number -- deviation is considered
   caused by clock drift only if rate <= 1/MAX_CLOCK_DRIFT.
   */
#define MAX_CLOCK_DRIFT 50

#define SHCODE 0x000001b3

#define max(a,b) ((a)>(b) ? (a) : (b))

static struct block
{
  unsigned shcode;
  struct block * next;
  int full;
} ** head, ** tail;

struct header
{
  struct block *h, *t;
};

static char * buf;
static int bufsize;
static int sid, countid;
static exit_tag = 0;
static int conn_tag;
static int savedSocket;

void VBinitBuf(int size)
/* size in byte */
{
  bufsize = size - sizeof(struct header);
  buf = creat_shared_mem(size);
  head = &((struct header *)buf)->h;
  tail = &((struct header *)buf)->t;
  buf += sizeof(struct header);
  sid = creat_semaphore();
  countid = creat_semaphore();
  enter_cs(countid);
  *head = *tail = (struct block *)buf;
  (*tail)->full = 0;
  (*tail)->next = NULL;
  (*tail)->shcode = SHCODE;
}

char * VBgetBuf(int size)  /* block version */
{
  return 0;
}

int VBcheckBuf(int size) /* non-block check, return True/False*/
{
  return 0;
}

void VBputMsg(char * msgPtr)
{
}

char * VBgetMsg()          /* block version */
{
  char *ptr;

#ifdef STAT
  if (shared->collectStat && *head == *tail)
    shared->stat.VBemptyTimes ++;
#endif
  enter_cs(countid);
  enter_cs(sid);
  while (*tail != *head && (*tail)->full == 0)
    *tail = (*tail)->next;
  leave_cs(sid);
  if (*head == *tail)
  {
    fprintf(stderr, "VB: getMsg run out of msg unexpectedly.\n");
    exit(1);
  }
  ptr = ((char*)*tail)+sizeof(**tail)+sizeof(VideoMessage);

  //  fprintf(stderr,"VBgetMsg: buf:%x, msg:%x\n", (int)buf, (int)ptr);

  return ptr;
}

int  VBcheckMsg()        /* non-block check, return Number of Msgs in buffer */
{
  return get_semval(countid);
}

int VBbufEmpty(void)
{
  /*
  Fprintf(stderr, "VB countid %d\n", get_semval(countid));
  */
  return get_semval(countid) <= 0;
}

void VBreclaimMsg(char * msgPtr)
{
  enter_cs(sid);
  *tail = (*tail)->next;
  leave_cs(sid);
}

void VBdeleteBuf(void)
{
  remove_shared_mem(buf - sizeof(struct header));
}
void VBdeleteSem(void)
{
  remove_semaphore(sid);
  remove_semaphore(countid);
}
  
/* SIGUSR1 from CTR is for killing this process, without affecting any other ones. */

static void exit_on_kill(void)
{
  ACE_DEBUG ((LM_DEBUG,"(%P|%t) VBProcess killed\n"));
  extern void set_exit_routine_tag(int tag);
  set_exit_routine_tag(0);
  ComCloseConn(savedSocket);
  VBdeleteBuf();
  exit(0);
}

static void usr1_handler(int sig)
{
  cerr << "VBProcess got sigusr1\n";
  exit_on_kill ();
  exit_tag = 1;
}


static void usr2_handler(int sig)
{
  /*
  fprintf(stderr, "VB void usr2_handler (supposed for stat).\n");
  */
}

static int send_feedback(int sock, int addupf, int addf, int advance)
{
  int res;
  VideoFeedBackPara para;
  para.cmdsn = htonl(shared->cmdsn);
  para.addUsecPerFrame = htonl(addupf);
  para.addFrames = htonl(addf);
  para.needHeader = htonl(shared->needHeader);
  shared->needHeader = 0;
  para.frameRateLimit1000 =
      htonl((long)(shared->frameRateLimit * 1000.0));
  para.sendPatternGops = htonl(shared->sendPatternGops);
  memcpy(para.sendPattern, shared->sendPattern, PATTERN_SIZE);
  /*
  fprintf(stderr, "VB to send a fb packet...");
  */
  if (conn_tag != 0) { /* packet stream */
    while ((res = write(sock, (char *)&para, sizeof(para))) == -1) {
      if (errno == EINTR)
	continue;
      if (errno == ENOBUFS) {
	perror("VB Warning, fb packet discarded for");
	return -1;
      }
      perror("VB error, fb packet sending failed");
      exit(1);
    }
  }
  else {
    res = write(sock, (char *)&para, sizeof(para));
    if (res == -1) {
      perror("VB error, fb packet sending failed");
      exit(1);
    }
  }
  if (res < sizeof(para)) {
    fprintf(stderr, "VB send_feedback() warn: res %dB < sizeof(para) %dB\n",
	    res, sizeof(para));
  }
#ifdef STAT
  {
    int i;
    if ((i = shared->stat.fbPacketNumber) < MAX_FB_PACKETS) {
      shared->stat.fbPackets[i].frameId = shared->nextFrame;
      shared->stat.fbPackets[i].addUsecPerFrame = addupf;
      shared->stat.fbPackets[i].addFrames = addf;
      shared->stat.fbPackets[i].frames = shared->sendPatternGops *
                                         shared->patternSize;
      shared->stat.fbPackets[i].framesDropped = shared->framesDropped;
      shared->stat.fbPackets[i].frameRateLimit = shared->frameRateLimit;
      shared->stat.fbPackets[i].advance = advance;
    }
    shared->stat.fbPacketNumber ++;
  }
#endif
  Fprintf(stderr, "VB sent fb pkt frame%d gop%d addf%d addupf%d frate%5.2f.\n",
	  shared->nextFrame, shared->nextGroup,
	  addf, addupf, shared->frameRateLimit);
  return 0;
}

static void skip_message(int fd, VideoMessage * msg)
{
  char buf[1024];
  if (conn_tag >= 0) {
    int size = msg->msgSize;
    while (size > 0) {
      int res;
      int bytes = size > 1024 ? 1024 : size;
      read_bytes(fd, buf, bytes);
      /*
      if ((res = read(fd, buf, bytes)) < bytes) {
	fprintf(stderr, "VB Error skip_message: try read %dB, got %dB\n",
		bytes, res);
      }
      */
      size -= bytes;
    }
  }
  else { /* discard mode packet stream packet has been read */
  }
  if (exit_tag) exit_on_kill();
}

void VBprocess(int initSocket, int normalSocket)
{
  VideoMessage * msg, msghd;
  VideoPacket * packet;
  int len, bsize, msgsn = -1;
  int psize, poffset;
  char * ptr, *ptr1;
  char *tmp_buf = NULL;
  int dataSocket = initSocket;

  /* INIT frame is received from CTR through initSocket */

#ifdef STAT
  int to_count = 1;
  int gap_msgsn = -1;
#endif
  
  /* following variables are for feedback */
  int pcmdsn, pcmd;  /* to store the cmdsn and cmd of previous frame,
			to detect new commands */
  int pfid, pgop;    /* frame-id of gopid of previous frame */
  int cmdsn = -1;    /* cmdsn of previous command */
  int state = 0;     /* state of the control law/regulator */
  int startpos;      /* gopid or frameid when the regulator is in 'start' state 1 */
  int delay;	     /* #frames to delay after 'start' */
  int qosRecomputes = 0; /* indicate if qos has been recomputed recently */
  int action_delay;	/* microseconds to delay after feedback action */
  int action_time;     /* the time when feedback action is taken, used to enforce
			  action_delay */
  int upf, cupf;	/* upf -- currentUPF as carried in all frames,
			   cupf -- real current UPF, being adjusted by FB */
  double fv;		/* current buffer fill level in term of #frames */	
  double init_fv;	/* init_fv and init_pos record the fv value and position
			   (frameid or gopid) when fv is very close to med */
  double init_pos;
  int high, low, med;	/* median, high and low water marks, in term of #frames */
  int period;		/* interms of #frames, (high-med, period) defined the max
			   buffer fill drift which can be assumed as caused by
			   clock drift */
  Filter * f = NULL;	/* buffer fill level filter */

  int advance, min_advance; /* in microseconds, advance holds the
			       current target buffer-fill-level as
			       determined by jitter level
			       estimate. min_advance sets a absolute
			       minimun level, no matter how low jitter
			       level is.
			       'advance' does not change when
			       play-speed changes, but med/high/low
			       will be updated upon speed change */
  double fav;		/* #frames, jitter level output by the filter */
  Filter * fa = NULL;	/* jitter filter */
  int reach_limit = 0;	/* indicate that the target 'advance' has reached client VB
			   buffer limit, and can not further increase, even if
			   jitter increases */
  int not_action = 1;	/* indicates entering state=3 is not caused by
			   feedback action. If not_action = 1 entering state=3
			   is caused either by 'start' or speed change, and
			   the jitter filter needs to be reset */

  exit_tag = 0;
  conn_tag = shared->videoMaxPktSize;

  savedSocket = normalSocket;
#if 0  
#ifdef __svr4__
  
  /* On solaris 2.x for i86pc, a datagram socket may not discard the
     remaining of a message if recv() fails to read the whole
     thing. Following systems are tried but seems doesn't work. So for
     this platform, This causes problem with skip_message() defined
     above, which tries to discard whole packets by reading only a few
     bytes. An alternative here is to allocate a big enough buffer,
     read and discard the whole packet, this is less efficient because
     of more data copy.

     But if we call read(2) instead of recv(), then it works.
     */
  if (!conn_tag) {
    len = ioctl(dataSocket, I_SRDOPT, RMSGD);  /* discard unread part of the message */
    if (len) perror("error message");
  }
#endif
#endif
  
  setsignal(SIGUSR1, usr1_handler);
  setsignal(SIGUSR2, usr2_handler);

  if (conn_tag < 0) {  /* buffer big enough for discard mode packet stream */
    tmp_buf = (char *)malloc(-conn_tag);
    if (tmp_buf == NULL) {
      fprintf(stderr, "AB failed to allocate %d bytes");
      perror("of tmp_buf");
      exit(1);
    }
  }

  for (;;)
  {
    //    fprintf(stderr, "VB wait for a new packet.\n");
    if (conn_tag >= 0) {
      len = wait_read_bytes(dataSocket, (char *)&msghd, sizeof(msghd));
    }
    else {  /* discard mode packet stream, read all bytes */
      len = read(dataSocket, tmp_buf, -conn_tag);
      memcpy((char *)&msghd, tmp_buf, sizeof(msghd));
    }
    //    ACE_DEBUG ((LM_DEBUG, "(%P|%t) vb: Got a packet of length = %d\n", len));
    if (exit_tag) exit_on_kill();
    if (len == -1) {
      if (errno == EWOULDBLOCK || errno == EAGAIN) {
	perror("VB sleep for 10ms");
	usleep(10000);
	if (exit_tag) exit_on_kill();
	continue;
      }
      perror("VB read() data");
      exit(1);
    }
    if (len == 0) {  /* EOF, connection closed by peer */
      fprintf(stderr, "Error: VB found dataSocket broken\n");
      for (;;) {
	usleep(1000000);
	if (exit_tag) exit_on_kill();
      }
    }
    if (len < sizeof(msghd)) {
      fprintf(stderr, "VD warn: PEEK1ed %dB < expected %dB\n",
	      len, sizeof(msghd));
      continue;
    }
#ifdef NeedByteOrderConversion
    msghd.packetsn = ntohl(msghd.packetsn);
    msghd.packetSize = ntohl(msghd.packetSize);
    msghd.msgsn = ntohl(msghd.msgsn);
    msghd.msgOffset = ntohl(msghd.msgOffset);
    msghd.msgSize = ntohl(msghd.msgSize);
#endif
    
    /*
    fprintf(stderr, "VB PEEK1 a msg sn-%d, size-%d, pkt-%d, pktsize-%d\n",
	    msghd.msgsn, msghd.msgSize, msghd.packetsn, msghd.packetSize);
    */
  start_new_packet:

    
#ifdef STAT
    if (to_count) {
      int gap = msghd.msgsn - gap_msgsn;
      gap = (gap >MSGGAP_MAX) ? MSGGAP_MAX : gap < MSGGAP_MIN ? MSGGAP_MIN : gap;
      shared->stat.VBmsgGaps[gap - MSGGAP_MIN] ++;
      if (gap >0) gap_msgsn = msghd.msgsn;
    }
    to_count = 1;
#endif
    if (msghd.msgsn <= msgsn)  /* outdated msg */
    {
    
      fprintf(stderr, "VB discard outdated msgsn %d, pktsn %d when expecting first\n",
	      msghd.msgsn, msghd.packetsn);
    
      skip_message(dataSocket, &msghd);
      continue;
    }
    else if (msghd.msgOffset != 0)  /* not first msg of a packet */
    {
    
      /*
      Fprintf(stderr, "VB discard non-first msg msgsn %d, pktsn %d\n",
	      msghd.msgsn, msghd.packetsn);
      */
      skip_message(dataSocket, &msghd);
      continue;
    }
    else
      msgsn = msghd.msgsn;
    
    /* allocate packet for the incoming msg */
    bsize =  msghd.packetSize + sizeof(**head)*2 + sizeof(msghd);
    bsize = ((bsize+3)>>2)<<2;
    enter_cs(sid);
    if (*head >= *tail)
    {
      if (bufsize - (int)((char*)*head - buf) >= bsize )
        msg =(VideoMessage *)((char*)*head + sizeof(**head));
      else if ((int)((char*)*tail - buf) >= bsize)
      {
	(*head)->next = (struct block *)buf;
	(*head)->full = 0;
	*head = (struct block *)buf;
	msg = (VideoMessage *)(buf + sizeof(**head));
	*head = (struct block *)buf;
	(*head)->shcode = SHCODE;
      }
      else /* not enough buffer, discard current message */
      {
	leave_cs(sid);
#ifdef STAT
	if (shared->collectStat)
  	  shared->stat.VBdroppedFrames ++;
#endif
	/*
	Fprintf(stderr, "VB not enough space 1, drop msg.sn %d pktsn %d\n",
		msghd.msgsn, msghd.packetsn);
        */
                
        skip_message(dataSocket, &msghd);
	continue;
      }
    }
    else /* *head < *tail */
    if ((char*)*tail - (char*)*head >= bsize)
      msg = (VideoMessage *)((char*)*head + sizeof(**head));
    else   /* not enough buffer, abandon current message */
    {
      leave_cs(sid);
#ifdef STAT
      if (shared->collectStat)
	shared->stat.VBdroppedFrames ++;
#endif
      /*
      Fprintf(stderr, "VB not enough space 1, drop msg.sn %d pktsn %d\n",
	      msghd.msgsn, msghd.packetsn);

      */
      skip_message(dataSocket, &msghd);
      continue;
    }
    leave_cs(sid);

    //    fprintf(stderr, "VB allocated a buffer for comming packet.\n");

    psize = msghd.packetSize;
    poffset = 0;
    packet = (VideoPacket *)((char*)msg + sizeof(msghd));
    *(((int*)packet)+(msghd.packetSize>>2)) = 0;
                             /* clear the last no more than three bytes, for
				proper detecting the end of packet by VD */
    ptr = (char*)msg;
    for (;;)
    {
      int bytes;
      bytes = sizeof(msghd) + msghd.msgSize;
      if (conn_tag >= 0) {
	len = bytes;
	read_bytes(dataSocket, ptr + sizeof(msghd), bytes - sizeof(msghd));
      }
      else {
	len = bytes;
	memcpy(ptr + sizeof(msghd), tmp_buf + sizeof(msghd), bytes - sizeof(msghd));
      }
      if (exit_tag) exit_on_kill();
      if (len == -1) {
	perror("VB fails to recv data");
	exit(1);
      }
      if (len < bytes) {
	fprintf(stderr, "VB warn: recv() len %dB < bytes %dB\n", len, bytes);
      }
      /* because message header is already PEEKed to msghd before recv(), so we can
	 use info in msghd instead of *(VideoMessage*)ptr   */
      if (len != sizeof(msghd)+msghd.msgSize)
        /* some of msg contents not successfully received, abandon current packet */
      {
        /*
	fprintf(stderr, "VB got corrupted msg, len=%d, supposed len=%d\n",
		len, sizeof(msghd)+msghd.msgSize);
        */
        break;
      }
      poffset += msghd.msgSize;
      psize -= msghd.msgSize;
      ptr += msghd.msgSize;

      //      fprintf(stderr, "VB packet remain size %d\n", psize);

      if (psize == 0)
      {
	/* finished receiving the current packet */
#ifdef NeedByteOrderConversion
	packet->cmd = ntohl(packet->cmd);
	packet->cmdsn = ntohl(packet->cmdsn);
	packet->sh = ntohl(packet->sh);
	packet->gop = ntohl(packet->gop);
	packet->frame = ntohl(packet->frame);
	packet->display = ntohl(packet->display);
	packet->future = ntohl(packet->future);
	packet->past = ntohl(packet->past);
	packet->currentUPF = ntohl(packet->currentUPF);
	packet->dataBytes = ntohl(packet->dataBytes);
#endif
	pcmdsn = packet->cmdsn;
	pcmd = packet->cmd;
	pfid = packet->frame;
	pgop = packet->gop;
	shared->VBheadFrame = (pcmd == CmdPLAY) ? pfid : pgop;
	
	shared->currentUPF = packet->currentUPF;
	enter_cs(sid);
	(*head)->full = 1;
	psize = sizeof(**head) + sizeof(*msg) + msghd.packetSize;
	psize = ((psize+3)>>2)<<2;
	ptr = (char*)*head + psize;
	(*head)->next = (struct block *) ptr;
	(*head) = (struct block *)ptr;
	(*head)->shcode = SHCODE;
	leave_cs(countid);
	leave_cs(sid);

	/* VB receives all frame except for the INIT one through normalSocket */
	if (dataSocket != normalSocket) {
	  /*
	  Fprintf(stderr, "VB got INIT frame.\n");
	  */
	  write(initSocket, (char *)&initSocket, 1);  /* write a garbage byte */
	  close(initSocket);
	  dataSocket = normalSocket;
	}
	
	/* following is synchronization feedback algorithm */
	
	if (shared->config.syncEffective) {
	  if (state > 1 && state != 4 && (len = shared->qosRecomputes) != qosRecomputes) {
	    /* QoS feedback packet is sent if at any time send pattern is
	       recomputed, and sync feedback is not in active state*/
	    send_feedback(dataSocket, 0, 0, advance);
	    qosRecomputes = len;
	  }
	  switch (state) {
	  case 4:  /* active */
	    if (pcmdsn != cmdsn) {
	      cmdsn = pcmdsn;
	      if (!(pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB)) {
		state = 0;
	      }
	      else {
		state = 1;
	      }
	      break;
	    }
	    if (pcmd == CmdPLAY && shared->usecPerFrame != upf) {
	      /* jump to state 5 if speed changes */
	      state = 5;
	      break;
	    }
	    {
	      int interval = shared->usecPerFrame;
	      double val = (double)(pcmd == CmdPLAY ?
			           pfid - shared->nextFrame :
			           (pcmd == CmdFF ?
			            pgop - shared->nextGroup :
			            shared->nextGroup - pgop));
	      fv = DoFilter(f, val); /* get average #frames in the whole client
					pipeline, including all stages */
	      val = val - fv;
	      fav = DoFilter(fa, val >= 0.0 ? val : -val);
	      /* get average #frames jitter in the whole client pipeline */
							      
	      val = fav * interval * 6;
	      /* convert deviation in frame into microseconds, 6 is a magic number */

	      /* tries to recompute advance (in microseconds), and med/high/low
		 in adaptation to current jitter level */
	      if ((val > advance && !reach_limit) ||
		  (advance > min_advance && val < advance >> 3)) {
		advance = (int) max(2 * val, min_advance);
		med = advance / interval;
		/*
		if (pcmd == CmdPLAY) {
		  if (med < shared->VDframeNumber) {
		    med = shared->VDframeNumber;
		  }
		}
		else
		*/
		if (med < 2 ) { /* but keep minimum buffer fill level */
		  med = 2;
		}
		if (med > (VB_BUF_SIZE / shared->averageFrameSize) / 2) {
		  reach_limit = 1;
		  med = (VB_BUF_SIZE / shared->averageFrameSize) / 2;
		  Fprintf(stderr,
			  "VB VSadvance control: VBbuf limit reached, med %d.\n", med);
		}
		else reach_limit = 0;
		high = med + med / 2;
		low = med - med / 2;
		period = med * MAX_CLOCK_DRIFT;
		Fprintf(stderr,
		  "VB: VS advance control: fav %5.2f, med %d, advance %d at nextFrame %d\n",
			fav, med, advance, shared->nextFrame);
	      }
	    }
	    /* record the current time (fid or gopid) if buffer fill level drift
	       only very little */
	    if ((int)fv == med) {
	      init_fv = fv;
	      init_pos = pcmd == CmdPLAY ? pfid : pgop;
	      break;
	    }
	    /*
	    fprintf(stderr, "VB fb: fv %lf\n", fv);
	    */
	    /* try send action if low/high water mark is passed, or qos recomputed */
	    /* There is problem here, the deltas of upf are sent, instead of upf
	       and frame themself. This scheme is not robust in case case feedback
	       packets are lost, and get resent */
	    len = shared->qosRecomputes;
	    if (fv >= high || fv <= low || len != qosRecomputes) {
	      int addupf, addf;
	      int pos = pcmd == CmdPLAY ? pfid : pgop;
	      int dist = (int)(pcmd == CmdFB ? init_pos - pos : pos - init_pos);
	      if (fv >= high || fv <= low) {
		if (dist < period) { /* try skip or stall */
		  addf =  (int)(med - fv);
		  addupf = 0;
		}
		else { /* try adjust VS clock rate */
		  int added = (int)((double)cupf * (fv - (double)med) / (double) dist);
		  addf = (int)(med - fv);
		  addupf = added;
		  cupf += added;
		}
		state = 6;
	      }
	      else {  /* fb only recomputed sendpattern, no state change */
		addupf = 0;
		addf = 0;
	      }

	      /* tries to send a feedback packet. */
	      if (shared->live) {  /* no sync feedback with live video */
		qosRecomputes = len;
	      }
	      else if (send_feedback(dataSocket, addupf, addf, advance) == -1) {
		/* Keep trying action repeatedly if failed to send the action
		   packet (by keeping in state 4).
		   An action packet can also be lost in the network */
		state = 4;
	      }
	      else {
		qosRecomputes = len;
	      }
	      if (state == 6) { /* record the time if an action packet is
				   successfully send, and indicate that an
				   feedback action leads to state 6, which after
				   delay sometime leads to state 3.
				   The action_delay should have been related
				   to round-trip time. */
		action_time = get_usec();
		action_delay = shared->usecPerFrame * 100;
		not_action = 0;
	      }
	    }
	    break;
	  case 6:  /* reset after action */
	    if (pcmdsn != cmdsn) {
	      cmdsn = pcmdsn;
	      if (!(pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB)) {
		state = 0;
	      }
	      else {
		state = 1;
	      }
	      break;
	    }
	    if (pcmd == CmdPLAY && shared->usecPerFrame != upf) {
	      state = 5;
	      break;
	    }
	    /* Jump to transition state 3 only after delay for some
	       time, when feedback action has been taken, and the
	       effect has been propogated back to the client */
	    if (get_duration(action_time, get_usec()) >= action_delay) {
	      state = 3;
	    }
	    break;
	  case 5:  /* reset after speed change, feedback stays in this
		      state as long as play speed is changing */
	    if (pcmdsn != cmdsn) {
	      cmdsn = pcmdsn;
	      if (!(pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB)) {
		state = 0;
	      }
	      else {
		state = 1;
	      }
	      break;
	    }
	    /* Jump to transition state 3, indicating that the transition
	       is not caused by feedback action */
	    if (shared->currentUPF == shared->usecPerFrame) {
	      not_action = 1;
	      state = 3;
	    }
	    break;
	  case 2:  /* delay after start, this delay is for avoiding feedback
		      action during server fast start-up. */
	    if (pcmdsn != cmdsn) {
	      cmdsn = pcmdsn;
	      if (!(pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB)) {
		state = 0;
	      }
	      else {
		state = 1;
	      }
	      break;
	    }
	    if (pcmd == CmdPLAY) {
	      if (pfid - startpos >= delay) {
		advance = shared->VStimeAdvance;
		state = 3;
	      }
	    }
	    else if (pcmd == CmdFF) {
	      if (pgop - startpos >= delay) {
		advance = shared->VStimeAdvance;
		state = 3;
	      }
	    }
	    else {  /* CmdFB */
	      if (startpos - pgop >= delay) {
		advance = shared->VStimeAdvance;
		state = 3;
	      }
	    }
	    break;
	  case 3:  /* transient state, entered after start-up delay,
		      action-delay, or play-speed change */
	    if (pcmdsn != cmdsn) {
	      cmdsn = pcmdsn;
	      if (!(pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB)) {
		state = 0;
	      }
	      else {
		state = 1;
	      }
	      break;
	    }
	    /* Initialize both buffer-fill-level and jitter filters */
	    if (f == NULL) {
	      f = NewFilter(FILTER_LOWPASS, shared->config.filterPara >= 1 ?
			    shared->config.filterPara : 100);
	    }
	    else {
	      f = ResetFilter(f, shared->config.filterPara >= 1 ?
			      shared->config.filterPara : 100);
	    }
	    if (fa == NULL) {
	      fa = NewFilter(FILTER_LOWPASS, shared->config.filterPara >= 1 ?
			    shared->config.filterPara : 100);
	      DoFilter(fa, 0.0);
	    }
	    else if (not_action) { /* reset jitter level filter only
				      if entering this state is not
				      cause by feedback action */
	      fa = ResetFilter(fa, shared->config.filterPara >= 1 ?
			      shared->config.filterPara : 100);
	    }
	    if (f == NULL || fa == NULL) {
	      perror("VB failed to allocate space for filters");
	      state = 0;
	    }
	    else {
	      init_fv =
	        DoFilter(f, (double)(pcmd == CmdPLAY ?
			    pfid - shared->nextFrame :
			    (pcmd == CmdFF ?
			     pgop - shared->nextGroup :
			     shared->nextGroup - pgop)));
	      init_pos = pcmd == CmdPLAY ? pfid : pgop;
	      upf = shared->currentUPF;
	      cupf = upf;
	      {
		int interval = shared->usecPerFrame;

		/* upon speed change, 'advance', in microseconds, will
                   not change, but med/high/low will be updated. This
                   may suggest that in the new toolkit version of the
                   same feedback systems, the buffer-fill level and
                   jitter is measured directly in microseconds, not in
                   #frames then converting to microseconds. */
		med = advance / interval;
		/*
		if (pcmd == CmdPLAY) {
		  if (med < shared->VDframeNumber) {
		    med = shared->VDframeNumber;
		    min_advance = advance = med * interval;
		  }
		}
		else
		*/
		if (med < 2 ) {
		  med = 2;
		  advance = med * interval;
		  min_advance = max(advance, shared->VStimeAdvance);
		}
		else min_advance = shared->VStimeAdvance;
		if (med > (VB_BUF_SIZE / shared->averageFrameSize) / 2) {
		  reach_limit = 1;
		  med = (VB_BUF_SIZE / shared->averageFrameSize) / 2;
		  if (not_action) {
		    Fprintf(stderr,
		       "VB start/speed-change: VBbuf limit reached, med %d.\n", med);
		  }
		}
		else reach_limit = 0;
	      }
	      high = med + med / 2;
	      low = med - med / 2;
	      delay = med * (SPEEDUP_INV_SCALE - 1); /* this delay is to avoid
							feedback when VS is in
							fast-start period */
	      period = med * MAX_CLOCK_DRIFT;
	      if (not_action) {
		Fprintf(stderr,
			"VB start/speed-change: med %d, advance %d at nextFrame %d\n",
			med, advance, shared->nextFrame);
	      }
	      state = 4;
	    }
	    break;
	  case 0:  /* idle */
	    if (pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB) {
	      cmdsn = pcmdsn;
	      state = 1;
	    }
	    break;
	  case 1:  /* start */
	    if (pcmdsn != cmdsn) {
	      cmdsn = pcmdsn;
	      if (!(pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB)) {
		state = 0;
	      }
	      break;
	    }
	    startpos = pcmd == CmdPLAY ? pfid : pgop;
	    advance = shared->VStimeAdvance;

	    /*  following from vs.c:
		timerAdjust = (VStimeAdvance * SPEEDUP_INV_SCALE) / currentUPF;
	        */
	    med = advance / shared->usecPerFrame;
	    /*
	    if (pcmd != CmdPLAY) med /= shared->patternSize;
	    */
	    delay = med * (SPEEDUP_INV_SCALE - 1); /* this delay is to avoid
						      feedback when VS is in
						      fast-start period */
	    not_action = 1;
	    state = 2;
	    break;
	  default:
	    fprintf(stderr, "VB: unknown Feedback state %d reached.\n", state);
	    state = 0;
	    break;
	  }
	}   /* end if (shared->config.syncEffective) */
	else {
	  state = 0;
	}
#ifdef STAT
	if (shared->collectStat && packet->cmd != CmdREF)
	{
	  char * val, * val1;
	  int size;
	  size = packet->frame;
	  shared->stat.VBframesReceived[size >>3] |= 1 << (size % 8);
	  val = (char *)*head;
	  val1 = (char *)*tail;
	  if (val >= val1)  /* *head >= *tail */
	    size = val - val1;
	  else
	    size = bufsize - (val1 - val);
	  shared->stat.VBmaxBytes = max(shared->stat.VBmaxBytes, size);
	}
#endif
	/*
        fprintf(stderr, "VB: Packet(%x, buf:%x)=cmd-%d, cmdsn-%d sh-%d, gop-%d\n",
	        (int)packet, (int)buf, packet->cmd, packet->cmdsn, packet->sh, packet->gop);
        fprintf(stderr, "           frame-%d, display-%d, future-%d, past-%d, bytes-%d\n",
	        packet->frame, packet->display, packet->future,
		packet->past, packet->dataBytes);
	*/
	break;  /* got the whole packet, break to the out-most loop for next packet */
      }  /* end if (psize == 0) */
      else if (psize < 0)
      {
	fprintf(stderr, "VB error: received too many msgs for a packet.\n");
	exit(1);
      }

      /* try get another message for current packet */
      for (;;)
      {
	if (conn_tag >= 0) {
	  len = wait_read_bytes(dataSocket, (char *)&msghd, sizeof(msghd));
	}
	else {
	  len = read(dataSocket, tmp_buf, -conn_tag);
	  memcpy((char *)&msghd, tmp_buf, sizeof(msghd));
	}
	if (exit_tag) exit_on_kill();
        //        ACE_DEBUG ((LM_DEBUG, "(%P|%t) vb: Got a packet of length = %d\n", len));
	if (len <= 0)
	{
	  if (errno == EWOULDBLOCK || errno == EAGAIN) {
	    perror("VB sleep2 for 10ms");
	    usleep(10000);
	    if (exit_tag) exit_on_kill();
	    continue;
	  }
          perror("VB recv data(MSG_PEEK)");
          exit(1);
	}
	if (len < sizeof(msghd)) {
	  fprintf(stderr, "VD warn: PEEK1ed %dB < expected %dB\n",
		  len, sizeof(msghd));
	  continue;
	}
#ifdef NeedByteOrderConversion
	msghd.packetsn = ntohl(msghd.packetsn);
	msghd.packetSize = ntohl(msghd.packetSize);
	msghd.msgsn = ntohl(msghd.msgsn);
	msghd.msgOffset = ntohl(msghd.msgOffset);
	msghd.msgSize = ntohl(msghd.msgSize);
#endif
	/*
        fprintf(stderr, "VB PEEK2 a msg: sn-%d, size-%d, pkt-%d, pktsize-%d\n",
	        msghd.msgsn, msghd.msgSize, msghd.packetsn, msghd.packetSize);
	*/
#ifdef STAT
	{
	  int gap = msghd.msgsn - gap_msgsn;
	  gap = (gap >MSGGAP_MAX) ? MSGGAP_MAX : gap < MSGGAP_MIN ? MSGGAP_MIN : gap;
	  shared->stat.VBmsgGaps[gap - MSGGAP_MIN] ++;
	  if (gap >0) gap_msgsn = msghd.msgsn;
	}
#endif
	if (msghd.msgsn <= msgsn) { /* outdated message, wait for next one */
	  /*
	  fprintf(stderr, "VB discard outdated or dup msgsn %d, pktsn %d\n",
		  msghd.msgsn, msghd.packetsn);
	  */
	  skip_message(dataSocket, &msghd);
	  continue;
	}
	break;  /* got a message for current packet */
      }  /* end for (;;) */
      
      if (msghd.msgsn > msgsn + 1 || msghd.msgOffset == 0)
      {
        /* message out of order, abandon current packet */
	/*
	fprintf(stderr, "VB msg out of order for current packet, discard it.\n");
	*/
#ifdef STAT
	to_count = 0;
#endif
        goto start_new_packet;
      }
      else {
        msgsn = msghd.msgsn;
      }
    }
  }
}


void MyVBprocess(int initSocket, int normalSocket)
{
  VideoMessage * msg, msghd;
  VideoPacket * packet;
  int len, bsize, msgsn = -1;
  int psize, poffset;
  char * ptr, *ptr1;
  char *tmp_buf = NULL;
  int dataSocket = initSocket;

  /* INIT frame is received from CTR through initSocket */

#ifdef STAT
  int to_count = 1;
  int gap_msgsn = -1;
#endif
  
  /* following variables are for feedback */
  int pcmdsn, pcmd;  /* to store the cmdsn and cmd of previous frame,
			to detect new commands */
  int pfid, pgop;    /* frame-id of gopid of previous frame */
  int cmdsn = -1;    /* cmdsn of previous command */
  int state = 0;     /* state of the control law/regulator */
  int startpos;      /* gopid or frameid when the regulator is in 'start' state 1 */
  int delay;	     /* #frames to delay after 'start' */
  int qosRecomputes = 0; /* indicate if qos has been recomputed recently */
  int action_delay;	/* microseconds to delay after feedback action */
  int action_time;     /* the time when feedback action is taken, used to enforce
			  action_delay */
  int upf, cupf;	/* upf -- currentUPF as carried in all frames,
			   cupf -- real current UPF, being adjusted by FB */
  double fv;		/* current buffer fill level in term of #frames */	
  double init_fv;	/* init_fv and init_pos record the fv value and position
			   (frameid or gopid) when fv is very close to med */
  double init_pos;
  int high, low, med;	/* median, high and low water marks, in term of #frames */
  int period;		/* interms of #frames, (high-med, period) defined the max
			   buffer fill drift which can be assumed as caused by
			   clock drift */
  Filter * f = NULL;	/* buffer fill level filter */

  int advance, min_advance; /* in microseconds, advance holds the
			       current target buffer-fill-level as
			       determined by jitter level
			       estimate. min_advance sets a absolute
			       minimun level, no matter how low jitter
			       level is.
			       'advance' does not change when
			       play-speed changes, but med/high/low
			       will be updated upon speed change */
  double fav;		/* #frames, jitter level output by the filter */
  Filter * fa = NULL;	/* jitter filter */
  int reach_limit = 0;	/* indicate that the target 'advance' has reached client VB
			   buffer limit, and can not further increase, even if
			   jitter increases */
  int not_action = 1;	/* indicates entering state=3 is not caused by
			   feedback action. If not_action = 1 entering state=3
			   is caused either by 'start' or speed change, and
			   the jitter filter needs to be reset */

  exit_tag = 0;
  conn_tag = shared->videoMaxPktSize;

  savedSocket = normalSocket;
#if 0  
#ifdef __svr4__
  
  /* On solaris 2.x for i86pc, a datagram socket may not discard the
     remaining of a message if recv() fails to read the whole
     thing. Following systems are tried but seems doesn't work. So for
     this platform, This causes problem with skip_message() defined
     above, which tries to discard whole packets by reading only a few
     bytes. An alternative here is to allocate a big enough buffer,
     read and discard the whole packet, this is less efficient because
     of more data copy.

     But if we call read(2) instead of recv(), then it works.
     */
  if (!conn_tag) {
    len = ioctl(dataSocket, I_SRDOPT, RMSGD);  /* discard unread part of the message */
    if (len) perror("error message");
  }
#endif
#endif
  
  setsignal(SIGUSR1, usr1_handler);
  setsignal(SIGUSR2, usr2_handler);

  if (conn_tag < 0) {  /* buffer big enough for discard mode packet stream */
    tmp_buf = (char *)malloc(-conn_tag);
    if (tmp_buf == NULL) {
      fprintf(stderr, "AB failed to allocate %d bytes");
      perror("of tmp_buf");
      exit(1);
    }
  }

  for (;;)
  {
    //    fprintf(stderr, "VB wait for a new packet.\n");
    if (conn_tag >= 0) {
      len = wait_read_bytes(dataSocket, (char *)&msghd, sizeof(msghd));
    }
    else {  /* discard mode packet stream, read all bytes */
      len = read(dataSocket, tmp_buf, -conn_tag);
      memcpy((char *)&msghd, tmp_buf, sizeof(msghd));
    }
    if (exit_tag) exit_on_kill();
    if (len == -1) {
      if (errno == EWOULDBLOCK || errno == EAGAIN) {
	perror("VB sleep for 10ms");
	usleep(10000);
	if (exit_tag) exit_on_kill();
	continue;
      }
      perror("VB read() data");
      exit(1);
    }
    if (len == 0) {  /* EOF, connection closed by peer */
      fprintf(stderr, "Error: VB found dataSocket broken\n");
      for (;;) {
	usleep(1000000);
	if (exit_tag) exit_on_kill();
      }
    }
    if (len < sizeof(msghd)) {
      fprintf(stderr, "VD warn: PEEK1ed %dB < expected %dB\n",
	      len, sizeof(msghd));
      continue;
    }
#ifdef NeedByteOrderConversion
    msghd.packetsn = ntohl(msghd.packetsn);
    msghd.packetSize = ntohl(msghd.packetSize);
    msghd.msgsn = ntohl(msghd.msgsn);
    msghd.msgOffset = ntohl(msghd.msgOffset);
    msghd.msgSize = ntohl(msghd.msgSize);
#endif
    /*
    fprintf(stderr, "VB PEEK1 a msg sn-%d, size-%d, pkt-%d, pktsize-%d\n",
	    msghd.msgsn, msghd.msgSize, msghd.packetsn, msghd.packetSize);
    */

    
  start_new_packet:

    
#ifdef STAT
    if (to_count) {
      int gap = msghd.msgsn - gap_msgsn;
      gap = (gap >MSGGAP_MAX) ? MSGGAP_MAX : gap < MSGGAP_MIN ? MSGGAP_MIN : gap;
      shared->stat.VBmsgGaps[gap - MSGGAP_MIN] ++;
      if (gap >0) gap_msgsn = msghd.msgsn;
    }
    to_count = 1;
#endif
    if (msghd.msgsn <= msgsn)  /* outdated msg */
    {
      /*
      fprintf(stderr, "VB discard outdated msgsn %d, pktsn %d when expecting first\n",
	      msghd.msgsn, msghd.packetsn);
      */
      skip_message(dataSocket, &msghd);
      continue;
    }
    else if (msghd.msgOffset != 0)  /* not first msg of a packet */
    {
      /*
      Fprintf(stderr, "VB discard non-first msg msgsn %d, pktsn %d\n",
	      msghd.msgsn, msghd.packetsn);
      */
      skip_message(dataSocket, &msghd);
      continue;
    }
    else
      msgsn = msghd.msgsn;
    
    /* allocate packet for the incoming msg */
    bsize =  msghd.packetSize + sizeof(**head)*2 + sizeof(msghd);
    bsize = ((bsize+3)>>2)<<2;
    enter_cs(sid);
    if (*head >= *tail)
    {
      if (bufsize - (int)((char*)*head - buf) >= bsize )
        msg =(VideoMessage *)((char*)*head + sizeof(**head));
      else if ((int)((char*)*tail - buf) >= bsize)
      {
	(*head)->next = (struct block *)buf;
	(*head)->full = 0;
	*head = (struct block *)buf;
	msg = (VideoMessage *)(buf + sizeof(**head));
	*head = (struct block *)buf;
	(*head)->shcode = SHCODE;
      }
      else /* not enough buffer, discard current message */
      {
	leave_cs(sid);
#ifdef STAT
	if (shared->collectStat)
  	  shared->stat.VBdroppedFrames ++;
#endif
	/*
	Fprintf(stderr, "VB not enough space 1, drop msg.sn %d pktsn %d\n",
		msghd.msgsn, msghd.packetsn);
	*/
        skip_message(dataSocket, &msghd);
	continue;
      }
    }
    else /* *head < *tail */
    if ((char*)*tail - (char*)*head >= bsize)
      msg = (VideoMessage *)((char*)*head + sizeof(**head));
    else   /* not enough buffer, abandon current message */
    {
      leave_cs(sid);
#ifdef STAT
      if (shared->collectStat)
	shared->stat.VBdroppedFrames ++;
#endif
      /*
      Fprintf(stderr, "VB not enough space 1, drop msg.sn %d pktsn %d\n",
	      msghd.msgsn, msghd.packetsn);
      */
      skip_message(dataSocket, &msghd);
      continue;
    }
    leave_cs(sid);
    /*
    fprintf(stderr, "VB allocated a buffer for comming packet.\n");
    */
    psize = msghd.packetSize;
    poffset = 0;
    packet = (VideoPacket *)((char*)msg + sizeof(msghd));
    *(((int*)packet)+(msghd.packetSize>>2)) = 0;
                             /* clear the last no more than three bytes, for
				proper detecting the end of packet by VD */
    ptr = (char*)msg;
    for (;;)
    {
      int bytes;
      bytes = sizeof(msghd) + msghd.msgSize;
      if (conn_tag >= 0) {
	len = bytes;
	read_bytes(dataSocket, ptr + sizeof(msghd), bytes - sizeof(msghd));
      }
      else {
	len = bytes;
	memcpy(ptr + sizeof(msghd), tmp_buf + sizeof(msghd), bytes - sizeof(msghd));
      }
      if (exit_tag) exit_on_kill();
      if (len == -1) {
	perror("VB fails to recv data");
	exit(1);
      }
      if (len < bytes) {
	fprintf(stderr, "VB warn: recv() len %dB < bytes %dB\n", len, bytes);
      }
      /* because message header is already PEEKed to msghd before recv(), so we can
	 use info in msghd instead of *(VideoMessage*)ptr   */
      if (len != sizeof(msghd)+msghd.msgSize)
        /* some of msg contents not successfully received, abandon current packet */
      {
	/*
	fprintf(stderr, "VB got corrupted msg, len=%d, supposed len=%d\n",
		len, sizeof(msghd)+msghd.msgSize);
	 */
        break;
      }
      poffset += msghd.msgSize;
      psize -= msghd.msgSize;
      ptr += msghd.msgSize;
      /*
      fprintf(stderr, "VB packet remain size %d\n", psize);
      */
      if (psize == 0)
      {
	/* finished receiving the current packet */
#ifdef NeedByteOrderConversion
	packet->cmd = ntohl(packet->cmd);
	packet->cmdsn = ntohl(packet->cmdsn);
	packet->sh = ntohl(packet->sh);
	packet->gop = ntohl(packet->gop);
	packet->frame = ntohl(packet->frame);
	packet->display = ntohl(packet->display);
	packet->future = ntohl(packet->future);
	packet->past = ntohl(packet->past);
	packet->currentUPF = ntohl(packet->currentUPF);
	packet->dataBytes = ntohl(packet->dataBytes);
#endif
	pcmdsn = packet->cmdsn;
	pcmd = packet->cmd;
	pfid = packet->frame;
	pgop = packet->gop;
	shared->VBheadFrame = (pcmd == CmdPLAY) ? pfid : pgop;
	
	shared->currentUPF = packet->currentUPF;
	enter_cs(sid);
	(*head)->full = 1;
	psize = sizeof(**head) + sizeof(*msg) + msghd.packetSize;
	psize = ((psize+3)>>2)<<2;
	ptr = (char*)*head + psize;
	(*head)->next = (struct block *) ptr;
	(*head) = (struct block *)ptr;
	(*head)->shcode = SHCODE;
	leave_cs(countid);
	leave_cs(sid);

	/* VB receives all frame except for the INIT one through normalSocket */
	if (dataSocket != normalSocket) {
	  /*
	  Fprintf(stderr, "VB got INIT frame.\n");
	  */
	  write(initSocket, (char *)&initSocket, 1);  /* write a garbage byte */
	  close(initSocket);
	  dataSocket = normalSocket;
	}
	
	/* following is synchronization feedback algorithm */
	
	if (shared->config.syncEffective) {
	  if (state > 1 && state != 4 && (len = shared->qosRecomputes) != qosRecomputes) {
	    /* QoS feedback packet is sent if at any time send pattern is
	       recomputed, and sync feedback is not in active state*/
	    send_feedback(dataSocket, 0, 0, advance);
	    qosRecomputes = len;
	  }
	  switch (state) {
	  case 4:  /* active */
	    if (pcmdsn != cmdsn) {
	      cmdsn = pcmdsn;
	      if (!(pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB)) {
		state = 0;
	      }
	      else {
		state = 1;
	      }
	      break;
	    }
	    if (pcmd == CmdPLAY && shared->usecPerFrame != upf) {
	      /* jump to state 5 if speed changes */
	      state = 5;
	      break;
	    }
	    {
	      int interval = shared->usecPerFrame;
	      double val = (double)(pcmd == CmdPLAY ?
			           pfid - shared->nextFrame :
			           (pcmd == CmdFF ?
			            pgop - shared->nextGroup :
			            shared->nextGroup - pgop));
	      fv = DoFilter(f, val); /* get average #frames in the whole client
					pipeline, including all stages */
	      val = val - fv;
	      fav = DoFilter(fa, val >= 0.0 ? val : -val);
	      /* get average #frames jitter in the whole client pipeline */
							      
	      val = fav * interval * 6;
	      /* convert deviation in frame into microseconds, 6 is a magic number */

	      /* tries to recompute advance (in microseconds), and med/high/low
		 in adaptation to current jitter level */
	      if ((val > advance && !reach_limit) ||
		  (advance > min_advance && val < advance >> 3)) {
		advance = (int) max(2 * val, min_advance);
		med = advance / interval;
		/*
		if (pcmd == CmdPLAY) {
		  if (med < shared->VDframeNumber) {
		    med = shared->VDframeNumber;
		  }
		}
		else
		*/
		if (med < 2 ) { /* but keep minimum buffer fill level */
		  med = 2;
		}
		if (med > (VB_BUF_SIZE / shared->averageFrameSize) / 2) {
		  reach_limit = 1;
		  med = (VB_BUF_SIZE / shared->averageFrameSize) / 2;
		  Fprintf(stderr,
			  "VB VSadvance control: VBbuf limit reached, med %d.\n", med);
		}
		else reach_limit = 0;
		high = med + med / 2;
		low = med - med / 2;
		period = med * MAX_CLOCK_DRIFT;
		Fprintf(stderr,
		  "VB: VS advance control: fav %5.2f, med %d, advance %d at nextFrame %d\n",
			fav, med, advance, shared->nextFrame);
	      }
	    }
	    /* record the current time (fid or gopid) if buffer fill level drift
	       only very little */
	    if ((int)fv == med) {
	      init_fv = fv;
	      init_pos = pcmd == CmdPLAY ? pfid : pgop;
	      break;
	    }
	    /*
	    fprintf(stderr, "VB fb: fv %lf\n", fv);
	    */
	    /* try send action if low/high water mark is passed, or qos recomputed */
	    /* There is problem here, the deltas of upf are sent, instead of upf
	       and frame themself. This scheme is not robust in case case feedback
	       packets are lost, and get resent */
	    len = shared->qosRecomputes;
	    if (fv >= high || fv <= low || len != qosRecomputes) {
	      int addupf, addf;
	      int pos = pcmd == CmdPLAY ? pfid : pgop;
	      int dist = (int)(pcmd == CmdFB ? init_pos - pos : pos - init_pos);
	      if (fv >= high || fv <= low) {
		if (dist < period) { /* try skip or stall */
		  addf =  (int)(med - fv);
		  addupf = 0;
		}
		else { /* try adjust VS clock rate */
		  int added = (int)((double)cupf * (fv - (double)med) / (double) dist);
		  addf = (int)(med - fv);
		  addupf = added;
		  cupf += added;
		}
		state = 6;
	      }
	      else {  /* fb only recomputed sendpattern, no state change */
		addupf = 0;
		addf = 0;
	      }

	      /* tries to send a feedback packet. */
	      if (shared->live) {  /* no sync feedback with live video */
		qosRecomputes = len;
	      }
	      else if (send_feedback(dataSocket, addupf, addf, advance) == -1) {
		/* Keep trying action repeatedly if failed to send the action
		   packet (by keeping in state 4).
		   An action packet can also be lost in the network */
		state = 4;
	      }
	      else {
		qosRecomputes = len;
	      }
	      if (state == 6) { /* record the time if an action packet is
				   successfully send, and indicate that an
				   feedback action leads to state 6, which after
				   delay sometime leads to state 3.
				   The action_delay should have been related
				   to round-trip time. */
		action_time = get_usec();
		action_delay = shared->usecPerFrame * 100;
		not_action = 0;
	      }
	    }
	    break;
	  case 6:  /* reset after action */
	    if (pcmdsn != cmdsn) {
	      cmdsn = pcmdsn;
	      if (!(pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB)) {
		state = 0;
	      }
	      else {
		state = 1;
	      }
	      break;
	    }
	    if (pcmd == CmdPLAY && shared->usecPerFrame != upf) {
	      state = 5;
	      break;
	    }
	    /* Jump to transition state 3 only after delay for some
	       time, when feedback action has been taken, and the
	       effect has been propogated back to the client */
	    if (get_duration(action_time, get_usec()) >= action_delay) {
	      state = 3;
	    }
	    break;
	  case 5:  /* reset after speed change, feedback stays in this
		      state as long as play speed is changing */
	    if (pcmdsn != cmdsn) {
	      cmdsn = pcmdsn;
	      if (!(pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB)) {
		state = 0;
	      }
	      else {
		state = 1;
	      }
	      break;
	    }
	    /* Jump to transition state 3, indicating that the transition
	       is not caused by feedback action */
	    if (shared->currentUPF == shared->usecPerFrame) {
	      not_action = 1;
	      state = 3;
	    }
	    break;
	  case 2:  /* delay after start, this delay is for avoiding feedback
		      action during server fast start-up. */
	    if (pcmdsn != cmdsn) {
	      cmdsn = pcmdsn;
	      if (!(pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB)) {
		state = 0;
	      }
	      else {
		state = 1;
	      }
	      break;
	    }
	    if (pcmd == CmdPLAY) {
	      if (pfid - startpos >= delay) {
		advance = shared->VStimeAdvance;
		state = 3;
	      }
	    }
	    else if (pcmd == CmdFF) {
	      if (pgop - startpos >= delay) {
		advance = shared->VStimeAdvance;
		state = 3;
	      }
	    }
	    else {  /* CmdFB */
	      if (startpos - pgop >= delay) {
		advance = shared->VStimeAdvance;
		state = 3;
	      }
	    }
	    break;
	  case 3:  /* transient state, entered after start-up delay,
		      action-delay, or play-speed change */
	    if (pcmdsn != cmdsn) {
	      cmdsn = pcmdsn;
	      if (!(pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB)) {
		state = 0;
	      }
	      else {
		state = 1;
	      }
	      break;
	    }
	    /* Initialize both buffer-fill-level and jitter filters */
	    if (f == NULL) {
	      f = NewFilter(FILTER_LOWPASS, shared->config.filterPara >= 1 ?
			    shared->config.filterPara : 100);
	    }
	    else {
	      f = ResetFilter(f, shared->config.filterPara >= 1 ?
			      shared->config.filterPara : 100);
	    }
	    if (fa == NULL) {
	      fa = NewFilter(FILTER_LOWPASS, shared->config.filterPara >= 1 ?
			    shared->config.filterPara : 100);
	      DoFilter(fa, 0.0);
	    }
	    else if (not_action) { /* reset jitter level filter only
				      if entering this state is not
				      cause by feedback action */
	      fa = ResetFilter(fa, shared->config.filterPara >= 1 ?
			      shared->config.filterPara : 100);
	    }
	    if (f == NULL || fa == NULL) {
	      perror("VB failed to allocate space for filters");
	      state = 0;
	    }
	    else {
	      init_fv =
	        DoFilter(f, (double)(pcmd == CmdPLAY ?
			    pfid - shared->nextFrame :
			    (pcmd == CmdFF ?
			     pgop - shared->nextGroup :
			     shared->nextGroup - pgop)));
	      init_pos = pcmd == CmdPLAY ? pfid : pgop;
	      upf = shared->currentUPF;
	      cupf = upf;
	      {
		int interval = shared->usecPerFrame;

		/* upon speed change, 'advance', in microseconds, will
                   not change, but med/high/low will be updated. This
                   may suggest that in the new toolkit version of the
                   same feedback systems, the buffer-fill level and
                   jitter is measured directly in microseconds, not in
                   #frames then converting to microseconds. */
		med = advance / interval;
		/*
		if (pcmd == CmdPLAY) {
		  if (med < shared->VDframeNumber) {
		    med = shared->VDframeNumber;
		    min_advance = advance = med * interval;
		  }
		}
		else
		*/
		if (med < 2 ) {
		  med = 2;
		  advance = med * interval;
		  min_advance = max(advance, shared->VStimeAdvance);
		}
		else min_advance = shared->VStimeAdvance;
		if (med > (VB_BUF_SIZE / shared->averageFrameSize) / 2) {
		  reach_limit = 1;
		  med = (VB_BUF_SIZE / shared->averageFrameSize) / 2;
		  if (not_action) {
		    Fprintf(stderr,
		       "VB start/speed-change: VBbuf limit reached, med %d.\n", med);
		  }
		}
		else reach_limit = 0;
	      }
	      high = med + med / 2;
	      low = med - med / 2;
	      delay = med * (SPEEDUP_INV_SCALE - 1); /* this delay is to avoid
							feedback when VS is in
							fast-start period */
	      period = med * MAX_CLOCK_DRIFT;
	      if (not_action) {
		Fprintf(stderr,
			"VB start/speed-change: med %d, advance %d at nextFrame %d\n",
			med, advance, shared->nextFrame);
	      }
	      state = 4;
	    }
	    break;
	  case 0:  /* idle */
	    if (pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB) {
	      cmdsn = pcmdsn;
	      state = 1;
	    }
	    break;
	  case 1:  /* start */
	    if (pcmdsn != cmdsn) {
	      cmdsn = pcmdsn;
	      if (!(pcmd == CmdPLAY || pcmd == CmdFF || pcmd == CmdFB)) {
		state = 0;
	      }
	      break;
	    }
	    startpos = pcmd == CmdPLAY ? pfid : pgop;
	    advance = shared->VStimeAdvance;

	    /*  following from vs.c:
		timerAdjust = (VStimeAdvance * SPEEDUP_INV_SCALE) / currentUPF;
	        */
	    med = advance / shared->usecPerFrame;
	    /*
	    if (pcmd != CmdPLAY) med /= shared->patternSize;
	    */
	    delay = med * (SPEEDUP_INV_SCALE - 1); /* this delay is to avoid
						      feedback when VS is in
						      fast-start period */
	    not_action = 1;
	    state = 2;
	    break;
	  default:
	    fprintf(stderr, "VB: unknown Feedback state %d reached.\n", state);
	    state = 0;
	    break;
	  }
	}   /* end if (shared->config.syncEffective) */
	else {
	  state = 0;
	}
#ifdef STAT
	if (shared->collectStat && packet->cmd != CmdREF)
	{
	  char * val, * val1;
	  int size;
	  size = packet->frame;
	  shared->stat.VBframesReceived[size >>3] |= 1 << (size % 8);
	  val = (char *)*head;
	  val1 = (char *)*tail;
	  if (val >= val1)  /* *head >= *tail */
	    size = val - val1;
	  else
	    size = bufsize - (val1 - val);
	  shared->stat.VBmaxBytes = max(shared->stat.VBmaxBytes, size);
	}
#endif
	/*
        fprintf(stderr, "VB: Packet(%x, buf:%x)=cmd-%d, cmdsn-%d sh-%d, gop-%d\n",
	        (int)packet, (int)buf, packet->cmd, packet->cmdsn, packet->sh, packet->gop);
        fprintf(stderr, "           frame-%d, display-%d, future-%d, past-%d, bytes-%d\n",
	        packet->frame, packet->display, packet->future,
		packet->past, packet->dataBytes);
	*/
	break;  /* got the whole packet, break to the out-most loop for next packet */
      }  /* end if (psize == 0) */
      else if (psize < 0)
      {
	fprintf(stderr, "VB error: received too many msgs for a packet.\n");
	exit(1);
      }

      /* try get another message for current packet */
      for (;;)
      {
	if (conn_tag >= 0) {
	  len = wait_read_bytes(dataSocket, (char *)&msghd, sizeof(msghd));
	}
	else {
	  len = read(dataSocket, tmp_buf, -conn_tag);
	  memcpy((char *)&msghd, tmp_buf, sizeof(msghd));
	}
	if (exit_tag) exit_on_kill();
	if (len <= 0)
	{
	  if (errno == EWOULDBLOCK || errno == EAGAIN) {
	    perror("VB sleep2 for 10ms");
	    usleep(10000);
	    if (exit_tag) exit_on_kill();
	    continue;
	  }
          perror("VB recv data(MSG_PEEK)");
          exit(1);
	}
	if (len < sizeof(msghd)) {
	  fprintf(stderr, "VD warn: PEEK1ed %dB < expected %dB\n",
		  len, sizeof(msghd));
	  continue;
	}
#ifdef NeedByteOrderConversion
	msghd.packetsn = ntohl(msghd.packetsn);
	msghd.packetSize = ntohl(msghd.packetSize);
	msghd.msgsn = ntohl(msghd.msgsn);
	msghd.msgOffset = ntohl(msghd.msgOffset);
	msghd.msgSize = ntohl(msghd.msgSize);
#endif
	/*
        fprintf(stderr, "VB PEEK2 a msg: sn-%d, size-%d, pkt-%d, pktsize-%d\n",
	        msghd.msgsn, msghd.msgSize, msghd.packetsn, msghd.packetSize);
	*/
#ifdef STAT
	{
	  int gap = msghd.msgsn - gap_msgsn;
	  gap = (gap >MSGGAP_MAX) ? MSGGAP_MAX : gap < MSGGAP_MIN ? MSGGAP_MIN : gap;
	  shared->stat.VBmsgGaps[gap - MSGGAP_MIN] ++;
	  if (gap >0) gap_msgsn = msghd.msgsn;
	}
#endif
	if (msghd.msgsn <= msgsn) { /* outdated message, wait for next one */
	  /*
	  fprintf(stderr, "VB discard outdated or dup msgsn %d, pktsn %d\n",
		  msghd.msgsn, msghd.packetsn);
	  */
	  skip_message(dataSocket, &msghd);
	  continue;
	}
	break;  /* got a message for current packet */
      }  /* end for (;;) */
      
      if (msghd.msgsn > msgsn + 1 || msghd.msgOffset == 0)
      {
        /* message out of order, abandon current packet */
	/*
	fprintf(stderr, "VB msg out of order for current packet, discard it.\n");
	*/
#ifdef STAT
	to_count = 0;
#endif
        goto start_new_packet;
      }
      else {
        msgsn = msghd.msgsn;
      }
    }
  }
}
