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
#include <unistd.h>
#include <stdlib.h>
/*
#include <sys/types.h>
#include <sys/socket.h>
*/
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>
#include "include/common.h"
#include "newproto.h"
#include "global.h"
#include "dither.h"
#include "video.h"
#include "proto.h"

ACE_RCSID(mpeg_client, vd, "$Id$")

#define MAX_VDBLOCK_NUM 8
#define CQUEUE_SIZE	MAX_VDBLOCK_NUM + 2
#define ESTACK		0
#define CQUEUE		1

static int win_width, win_height;
static int ecountid, ccountid;
static int sid;

extern VidStream * curVidStream;

static struct shared_mem_block
{
  int size;
  int block_num;
  int width, height;
  int sptr, qhead, qtail;
  FrameBlock *estack[MAX_VDBLOCK_NUM];
  FrameBlock *cqueue[CQUEUE_SIZE]; /* cqueue will never get full */
} * shm = NULL;

static void InitDitherEnv(void);
static void DitherFrame(PictImage * pict, FrameBlock * frame);
static void DoDitherImage(unsigned char *l, unsigned char *Cr, unsigned char *Cb,
			  unsigned char *disp, int h, int w);

void VDinitBuf(int size)
/* size in bytes */
{
  shm = (struct shared_mem_block *)creat_shared_mem_id(size, &shared->VDbufId);
  shared->VDbufAddr = (char *)shm;
  
  shm->width = shm->height = 0;
  shm->size = size - sizeof(*shm);
  sid = creat_semaphore();
  ecountid = creat_semaphore();
  ccountid = creat_semaphore();
  enter_cs(ccountid);
  enter_cs(ecountid);
}

static void put_block(FrameBlock * bptr, int tag)
{
  if (tag == ESTACK)
  {
    shm->estack[shm->sptr] = bptr;
    shm->sptr ++;
    if (shm->sptr > MAX_VDBLOCK_NUM)
    {
      fprintf(stderr, "VD weird error: ESTACK full.\n");
      exit(1);
    }
  }
  else
  {
    if (shm->qtail == shm->qhead)  /* empty queue */
    {
      shm->qtail = 0;
      shm->qhead = 1;
      shm->cqueue[0] = bptr;
    }
    else
    {
      int val = CQUEUE_SIZE;
      int pos = (shm->qhead - 1 + val) % val; /* pos = shm->qhead - 1 */
      if (shared->cmd == CmdPLAY)
      while ((pos + 1) % val != shm->qtail)
      {
	if (shm->cqueue[pos]->display > bptr->display)
	{
	  shm->cqueue[(pos + 1) % val] = shm->cqueue[pos];
	  pos = (pos - 1 + val) % val;  /* pos -= 1 */
	}
	else
	  break;
      }
      shm->cqueue[(pos + 1) % val] = bptr;
      shm->qhead = (shm->qhead + 1) % val;
    }
  }
}

static FrameBlock * get_block(int tag)
{
  FrameBlock * ptr;

  if (tag == ESTACK)
  {
    if (shm->sptr <= 0)
    {
      fprintf(stderr, "VD get_block error: ESTACK empty.\n");
      return NULL;
    }
    shm->sptr --;
    ptr = shm->estack[shm->sptr];
  }
  else
  {
    if (shm->qtail == shm->qhead)
    {
      fprintf(stderr, "VD get_block error: CQUEUE empty.\n");
      return NULL;
    }
    ptr = shm->cqueue[shm->qtail];
    shm->qtail = (shm->qtail + 1) % (CQUEUE_SIZE);
  }
  return ptr;
}

void VDresizeBuf(int height, int width)
{
  char * ptr = (char*)shm + sizeof(*shm);
  int imagesize;
  int i;

  enter_cs(sid);

  if (get_semval(ecountid) < 0) {
    fprintf(stderr, "Error<weird>: pid %d get (ecountid) = %d < 0\n",
	    getpid(), get_semval(ecountid));
    exit(1);
  }
  /*
  fprintf(stderr, "VD before resizeBuf(): get_semval(ecountid) = %d\n",
	  get_semval(ecountid));
  */
  while (get_semval(ecountid) > 0) enter_cs(ecountid);
  
  shm->width = width;
  shm->height = height;
  width = ((width+15)>>4) << 4;
  height = ((height+15)>>4) << 4;
  imagesize = (((height * width)+3)>>2)<<2;
  if ((ditherType == Twox2_DITHER) || (ditherType == FULL_COLOR_DITHER))
    imagesize <<= 2;
  
  shm->block_num = (shm->size)/(sizeof(FrameBlock) + imagesize);
  if (shm->block_num > MAX_VDBLOCK_NUM)
    shm->block_num = MAX_VDBLOCK_NUM;
  
  Fprintf(stderr, "VD resizeBuf: got %d(max %d) blocks of size %d.\n",
	  shm->block_num,
	  (shm->size)/(sizeof(FrameBlock) + imagesize),
	  (sizeof(FrameBlock) + imagesize));
  
  if (shm->block_num < 3)
  {
    fprintf(stderr,
	    "VD error: VDbuf fails to hold at least 3(three) block.\n");
    exit(1);
  }
  shm->sptr = 0;  /* empty stack */
  shm->qhead = shm->qtail = 0;  /* empty cqueue */
  for (i=0; i<shm->block_num; i++)
  {
    put_block((FrameBlock *)ptr, ESTACK);
    ((FrameBlock *)ptr)->data = (unsigned char *)(ptr + sizeof(FrameBlock));
    ptr += sizeof(FrameBlock) + imagesize;
    leave_cs(ecountid);
  }
  leave_cs(sid);
  
  shared->VDframeNumber = shm->block_num;
  /*
  fprintf(stderr, "VD after resizeBuf(): get_semval(ecountid) = %d\n",
	  get_semval(ecountid));
  */
}

FrameBlock * VDgetBuf(void)     /* block version, return with interrupt */
{
  FrameBlock * ptr;
  enter_cs(ecountid);
  enter_cs(sid);
  ptr = get_block(ESTACK);
  if (ptr != NULL)
    ptr->refcount = 1;
  leave_cs(sid);
  if (ptr == NULL)
    fprintf(stderr, "VD error: VDgetBuf should not return NULL.\n");
  /*
  fprintf(stderr, "pid %d VDgetBuf() %u, ref %d, ecount %d\n",
	  getpid(), (unsigned)ptr, ptr->refcount, get_semval(ecountid));
  */
  return ptr;
}

int   VDcheckBuf(void)    /* non-block check, return True/False */
{
  if (get_semval(ecountid) > 0)
    return 1;
  else
    return 0;
}

void VDputMsg(FrameBlock * msgPtr)
{
  enter_cs(sid);
  put_block(msgPtr, CQUEUE);
  leave_cs(ccountid);
  leave_cs(sid);
  /*
  fprintf(stderr, "pid %d VDputMsg() %u, ref %d, ccount %d\n",
	  getpid(), (unsigned)msgPtr, msgPtr->refcount, get_semval(ccountid));
  */
}

FrameBlock * VDgetMsg(void)     /* block version, return with interrupt */
{
  FrameBlock * ptr;
  enter_cs(ccountid);
  enter_cs(sid);
  ptr = get_block(CQUEUE);
  leave_cs(sid);
  if (ptr == NULL)
    fprintf(stderr, "VD error: VDgetMsg should not return NULL.\n");
  /*
  fprintf(stderr, "pid %d VDgetMsg() %u, ref %d, ccount %d\n",
	  getpid(), (unsigned)ptr, ptr->refcount, get_semval(ccountid));
  */
  return ptr;
}

int VDcheckMsg(void)      /* non-block check, return Number of Msg in buffer */
{
  return get_semval(ccountid);
}

int VDbufEmpty(void)
{
  /*
  Fprintf(stderr, "VD ecountid %d, shm-block_num %d\n",
	  get_semval(ecountid), shm->block_num);
  */
  return (get_semval(ecountid) == shm->block_num);
}

FrameBlock * VDpeekMsg(void)
{
  FrameBlock * ptr;

  enter_cs(sid);
  
  if (shm->qhead == shm->qtail)
    ptr = NULL;
  else
    ptr = shm->cqueue[shm->qtail];
  leave_cs(sid);
  /*
  if (ptr != NULL) {
    fprintf(stderr, "pid %d VDpeekMsg() = %u, ref=%d\n",
	    getpid(), (unsigned)ptr, ptr->refcount);
  }
  */
  return ptr;
}

void VDreferMsg(FrameBlock * msgPtr)
{
  enter_cs(sid);
  msgPtr->refcount ++;
  leave_cs(sid);
  /*
  fprintf(stderr, "pid %d VDreferMsg() %u, ccount %d\n",
	  getpid(), (unsigned)msgPtr, get_semval(ccountid));
  */
}

void VDreclaimMsg(FrameBlock * msgPtr)
{
  int ref;
  enter_cs(sid);
  ref = msgPtr->refcount;
  if (msgPtr->refcount <= 1)
  {
    put_block(msgPtr, ESTACK);
    leave_cs(ecountid);
  }
  else
    msgPtr->refcount --;
  leave_cs(sid);
  /*
  fprintf(stderr, "pid %d VDreclaimMsg() %u, ref %d, ecount %d\n",
	  getpid(), (unsigned)msgPtr, ref, get_semval(ecountid));
  */
  if (ref <= 0) {
    Fprintf(stderr, "pid %d VDreclaimMsg() %u WEIRD, :ref %d:, ecount %d\n",
	    getpid(), (unsigned)msgPtr, ref, get_semval(ecountid));
  }
}

void VDdeleteBuf(void)
{
  remove_shared_mem((char*)shm);
}

void VDdeleteSem(void)
{
  remove_semaphore(ecountid);
  remove_semaphore(ccountid);
  remove_semaphore(sid);
}

int 
get_more_data(unsigned int *buf_start, int max_length, 
	      int *length_ptr, unsigned int **buf_ptr)
{
  fprintf(stderr,
	  "Fatel error: vd.c get_more_data() should not be called, bufLength = %d.!\n",
	  *length_ptr);
  fprintf(stderr, "  **** Please report this bug. ****\n");
  //  exit(1);
  return 0;
}

static void printPacket(VideoPacket *p)
{
  fprintf(stderr, "VD: Packet(%x)=cmd-%d, cmdsn-%d sh-%d, gop-%d\n",
          (int)p, p->cmd, p->cmdsn, p->sh, p->gop);
  fprintf(stderr, "           frame-%d, display-%d, future-%d, past-%d, bytes-%d\n",
	  p->frame, p->display, p->future,
          p->past, p->dataBytes);
}

static void usr1_handler(int sig)
{
  fprintf(stderr, "VD void usr1_handler.\n");
}

static void usr2_handler(int sig)
{
  fprintf(stderr, "VD void usr2_handler\n");
}

void VDprocess(int CTRpid)
{
  FrameBlock * curBlk = NULL;
  PictImage * curPict = NULL;

  InitDitherEnv();
  
  curVidStream = NewVidStream();
  if (curVidStream == NULL)
  {
    fprintf(stderr, "VD: unable to allocat curVidStream.\n");
    exit(1);
  }
  setsignal(SIGUSR1, usr1_handler);
  setsignal(SIGUSR2, usr2_handler);
  
  for(;;)
  {
    int curcmd, curcmdsn;
    int i;
    int single_tag;
    VideoPacket *p;

    if (curBlk != NULL) {
      VDreclaimMsg(curBlk);
      curBlk = NULL;
    }
    p = (VideoPacket*)VBgetMsg();   /* guarranteed to get a Packet */

    //printPacket(p);


    //    fprintf(stderr, "VD: got frame %d\n", p->frame);

    curcmd = shared->cmd;
    curcmdsn = shared->cmdsn;
    if (p->cmdsn != shared->cmdsn || p->cmdsn != curcmdsn)
    {
      
      /*
      fprintf(stderr, "VD: frame not for current Cmd, discard it\n");
      
      printPacket(p);
      */
      VBreclaimMsg((char*)p);
      continue;
    }
    if (curcmd == CmdINIT)
    {
      /* free/allocate all PictImages, resize VDbuffer */
      curVidStream->future = curVidStream->past = curVidStream->current = NULL;
      win_width = ((shared->horizontalSize + 15)>>4)<<4;
      win_height = ((shared->verticalSize + 15)>>4)<<4;
      for (i = 0; i < RING_BUF_SIZE; i++)
      {
	if (curVidStream->ring[i] != NULL)
	  DestroyPictImage(curVidStream->ring[i]);
        curVidStream->ring[i] =
	  NewPictImage(win_width, win_height);
      }
      for (i = 0; i < 500; i ++) {
	if (!VDbufEmpty()) usleep(1000);
	else break;
      }
      if (!VDbufEmpty()) {
	fprintf(stderr, "VD error<weired>: VDbuf can't become empty.\n");
	exit(1);
      }
      VDresizeBuf(shared->verticalSize, shared->horizontalSize);
    }
    else if (curcmd == CmdPLAY || curcmd == CmdSTEP)
    {
      /* to check if the packet can be decoded or not */
      if (p->future == -1)
      { /* swap because the existing VD decoding algorithm refer to 'future' frame
	   to decode a P frame */
	int tmp = p->past;
	p->past = p->future;
	/* following is the bug found on 2-14-96, this may be the cause of decoding
	   P frames incorrectly, and showing corrupted messages, when previous I/P
	   frames are dropped. This happens frequently when the server is a remote one.
	   
	p->future = p->past;

	*/
	p->future = tmp;
	
      }
      if ((p->future >= 0 &&
	   (curVidStream->future == NULL || curVidStream->future->frame != p->future)) ||
          (p->past >= 0 &&
	   (curVidStream->past == NULL || curVidStream->past->frame != p->past)))
      {
	/*
	fprintf(stderr,
	        "VD: unable to decode packet -- future and/or past frame no available.\n");
	printPacket(p);
	*/
	if (curcmd == CmdSTEP && curcmdsn == shared->cmdsn) {
	  /*
	  Fprintf(stderr, "VD failed to decode f%d, USR1 to CTR for STEP\n", p->frame);
	  */
	  kill(getppid(), SIGUSR1);  /* notify CTR anyway if STEP */
	}
#ifdef STAT
	else if (shared->collectStat && curcmd == CmdPLAY)
	  shared->stat.VDnoRef ++;
#endif
	VBreclaimMsg((char*)p);
	continue;
      }
    }
    curBlk = VDgetBuf();

    single_tag = 0;
    /* to check if it's in time for PLAY, FF and FB */
    if (curcmd == CmdPLAY)
    {

#if 0
      /* checking against sendPattern seems irreasonable, because send-pattern of a less
	 frame-rate is not necessarily a  subset of the one of a bigger frame-rate.
	 */
      /* check against sendPattern */
      if (p->frame - shared->firstGopFrames > 0)
      {
	int i = (p->frame - shared->firstGopFrames) %
	        (shared->patternSize * shared->sendPatternGops);
	if (shared->sendPattern[i] == 0)
	{
	  VBreclaimMsg((char*)p);
#ifdef STAT
	  if (shared->collectStat)
	    shared->stat.VDagainstSendPattern ++;
#endif
	  continue;
	}
      }
#endif

      if (VBcheckMsg() > 0) { /* a frame is to be dropped only if
				 there are more frames in VB */
	if (p->past >= 0) { /* 'B' */
	  if (p->display <= shared->nextFrame) {
#ifdef STAT
	    if (shared->collectStat)
	    shared->stat.VDtooLateB ++;
#endif
	    goto frameTooLate;
	  }
	}
	else if (p->future >= 0) { /* 'P' */
	  if (shared->lastIframeDecoded + shared->IframeGap <= shared->nextFrame) {
#ifdef STAT
	    if (shared->collectStat)
	    shared->stat.VDtooLateP ++;
#endif
	    goto frameTooLate;
	  }
	}
	else { /* 'I' */
	  if (!shared->live)
	  {
	    if (p->display + shared->IframeGap <= shared->nextFrame) {
#ifdef STAT
	      if (shared->collectStat)
	      shared->stat.VDtooLateI ++;
#endif
	    frameTooLate:
	      if (shared->rtplay)  /* too late, drop the frame */
	      {
		VBreclaimMsg((char*)p);
		/*
		Fprintf(stderr, "VD: frame %d too late, shared->nextFrame %d.\n",
			p->frame, shared->nextFrame);
		*/
		continue;
	      }
	    }
	  }
	  else {  /* live video */
	    if (p->display < shared->nextFrame) {
	      VBreclaimMsg((char*)p);
	      /*
	      Fprintf(stderr, "VD live video frame %d too late, nextFrame=%d\n",
		      p->frame, shared->nextFrame);
	      */
	      continue;
	    }
	  }
	}
      }
    }
    else if (curcmd == CmdFF)
    {
      if (p->gop < shared->nextGroup && VDcheckMsg() > 0)
      {
	/*
	fprintf(stderr, "VD: a frame too late and dropped when FF.\n");
	printPacket(p);
	*/
	VBreclaimMsg((char*)p);
	continue;
      }
    }
    else if (curcmd == CmdFB)
    {
      if (p->gop > shared->nextGroup && VDcheckMsg() > 0)
      {
	/*
	fprintf(stderr, "VD: a frame too late and dropped when FB.\n");
	printPacket(p);
	*/
	VBreclaimMsg((char*)p);
	continue;
      }
    }
    else
      single_tag = 1;
    
    if (shared->videoFormat == VIDEO_JPEG || shared->videoFormat == VIDEO_SIF) {
      int picsize = win_width * win_height;
      unsigned char * l, *cr, *cb;

      if (shared->videoFormat == VIDEO_SIF) {
	l = (unsigned char*)p + sizeof(*p);
	cr = l + picsize;
	cb = l + picsize + (picsize >> 2);
      }
      else {   /* decode the JPEG frame */
	l = cr = cb = (unsigned char *)curBlk->data;
	memcpy(l, (unsigned char*)p + sizeof(*p), p->dataBytes);
      }
      DoDitherImage(l, cr, cb,
		(unsigned char *)curBlk->data, win_height, win_width);
      curBlk->sh = p->sh;
      curBlk->gop = p->gop;
      curBlk->frame = p->frame;
      curBlk->display = p->display;
      curBlk->future = p->future;
      curBlk->past = p->past;
      VDputMsg(curBlk);
      curBlk = NULL;
      /*
      Fprintf(stderr, "VD: SIF frame %d decoded.\n", p->frame);
      */
      goto end_decode_loop;
    }
#ifdef NeedByteOrderConversion
    else {
      unsigned int * ptr = (unsigned int *)((char*)p + sizeof(*p));
      /* the added two extra words to max_buf_length: one is for SHCODE, already
	 in right byte order, the second is for prevent calling of get_more_data().
	 (which seem required for the UCB decoder used here ), this second word
	 contains value used by VB algorithm, and can't be changed elsewhere.
	 So they should not be swapped byte order. */
      for (i=0; i<((p->dataBytes + 11)>>2) - 2; i++)
      {
	*ptr = ntohl(*ptr);
	ptr++;
      }
    }
#endif

    if (!mpegVidRsrc((char*)p))  /* successfully decoded */
    {
      curPict = curVidStream->current;

      //      fprintf(stderr, "VD successfully decodes a frame.\n");
      //      printPacket(p);

      if (curcmd == CmdPLAY && shared->rtplay)
      {
	if ((curVidStream->picture.code_type == I_TYPE) ||
	    (curVidStream->picture.code_type == P_TYPE))
	{
	  if (curVidStream->future == NULL)
	  {
	    curVidStream->future = curVidStream->current;
	    curVidStream->future->locked |= FUTURE_LOCK;
	  }
	  else
	  {
	    if (curVidStream->past != NULL)
	    {
	      curVidStream->past->locked &= ~PAST_LOCK;
	    }
	    else if (curcmd == CmdPLAY) {
	      /* this case should happen only at the begining of PLAY */
	      DitherFrame(curVidStream->future, VDgetBuf());
	    }
	    curVidStream->past = curVidStream->future;
	    curVidStream->past->locked &= ~FUTURE_LOCK;
	    curVidStream->past->locked |= PAST_LOCK;
	    curVidStream->future = curVidStream->current;
	    curVidStream->future->locked |= FUTURE_LOCK;
	  }
	}
      }
      else if (curcmd == CmdSTEP || curcmd == CmdPLAY)  /* !shared->rtplay */
      {
	if ((curVidStream->picture.code_type == I_TYPE) ||
	    (curVidStream->picture.code_type == P_TYPE))
	{
	  if (curVidStream->future == NULL)
	  {
	    curVidStream->future = curVidStream->current;
	    curVidStream->future->locked |= FUTURE_LOCK;
	  }
	  else
	  {
	    if (curVidStream->past != NULL)
	    {
	      curVidStream->past->locked &= ~PAST_LOCK;
	    }
	    curVidStream->past = curVidStream->future;
	    curVidStream->past->locked &= ~FUTURE_LOCK;
	    curVidStream->past->locked |= PAST_LOCK;
	    curVidStream->future = curVidStream->current;
	    curVidStream->future->locked |= FUTURE_LOCK;
	    curPict = curVidStream->past;
	  }
	}
      }
      else /* only I-frame for all other Cmds */
      {
	if (curVidStream->future != NULL) {
	  curVidStream->future->locked &= ~FUTURE_LOCK;
	  curVidStream->future = NULL;
	}
	if (curVidStream->past != NULL)
	{
	  curVidStream->past->locked &= ~PAST_LOCK;
	  curVidStream->past = NULL;
	}
	/*
	curVidStream->future = curVidStream->current;
	curVidStream->future->locked |= FUTURE_LOCK;
	*/
      }
      
      if (p->past == -1 && p->future == -1)
        shared->lastIframeDecoded = p->frame;

#ifdef STAT
      if (shared->collectStat && p->cmd != CmdREF)
      {
	int val = curPict->frame;
	shared->stat.VDframesDecoded[val>>3] |= 1 << (val % 8);
	shared->stat.VDlastFrameDecoded = val;
      }
#endif
      /*
      if (single_tag && p->cmd != CmdREF)
      {
        fprintf(stderr, "VD: display %d for single.\n", curPict->display);
      }
      */
      /*
      Fprintf(stderr, "p->cmd %d, p->cmdsn %d, shared->cmdsn %d\n",
	      p->cmd, p->cmdsn, shared->cmdsn);
      */
      if (p->cmd != CmdREF && curcmdsn == shared->cmdsn) {
	DitherFrame(curPict, curBlk);
	curBlk = NULL;
      }
    }
    else  /* decoding failed */
    {
      /*
      fprintf(stderr, "VD error -- tried but failed decoding the packet.\n");
      printPacket(p);
      */
    }

  end_decode_loop:
    
    /* signal CTR for singlular operation: STEP, POSITION, INIT */
    if (single_tag && p->cmd != CmdREF && curcmdsn == shared->cmdsn)
    {
      /*
      Fprintf(stderr, "VD decoded f%d, USR1 to CTR\n", p->frame);
      */
      kill(getppid(), SIGUSR1);
    }
    VBreclaimMsg((char*)p);
  }
}


static void InitDitherEnv(void)
{
  lum_values = (int *) malloc(LUM_RANGE*sizeof(int));
  cr_values = (int *) malloc(CR_RANGE*sizeof(int));
  cb_values = (int *) malloc(CB_RANGE*sizeof(int));
  init_tables();

  while (!shared->pixelValid) {
    usleep(10000);
  }
  memcpy(pixel, shared->pixel, 256);

  switch (ditherType) {
    
  case HYBRID_DITHER:
    
    InitColor();
    InitHybridDither();
    break;
    
    case HYBRID2_DITHER:
    InitColor();
    InitHybridErrorDither();
    break;
    
  case FS4_DITHER:
    InitColor();
    InitFS4Dither();
    break;
    
  case FS2_DITHER:
    InitColor();
    InitFS2Dither();
    break;
    
  case FS2FAST_DITHER:
    InitColor();
    InitFS2FastDither();
    break;
    
  case Twox2_DITHER:
    InitColor();
    Init2x2Dither();
    PostInit2x2Dither();
    break;

  case GRAY_DITHER:
    break;

  case FULL_COLOR_DITHER:
    InitColorDither();
    break;

  case ORDERED_DITHER:
    InitColor();
    InitOrderedDither();
    break;

  case MONO_DITHER:
  case MONO_THRESHOLD:
    break;

  case ORDERED2_DITHER:
    InitColor();
    InitOrdered2Dither();
    break;
  }
}

/*
 *--------------------------------------------------------------
 *
 * DoDitherImage --
 *
 *	Called when image needs to be dithered. Selects correct
 *      dither routine based on info in ditherType.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

static void DoDitherImage(unsigned char *l, unsigned char *Cr, unsigned char *Cb,
			  unsigned char *disp, int h, int w) 
{

  switch(ditherType) {
  case HYBRID_DITHER:
    HybridDitherImage(l, Cr, Cb, disp, h, w);
    break;

  case HYBRID2_DITHER:
    HybridErrorDitherImage(l, Cr, Cb, disp, h, w);
    break;

  case FS2FAST_DITHER:
    FS2FastDitherImage(l, Cr, Cb, disp, h, w);
    break;

  case FS2_DITHER:
    FS2DitherImage(l, Cr, Cb, disp, h, w);
    break;

  case FS4_DITHER:
    FS4DitherImage(l, Cr, Cb, disp, h, w);
    break;

  case Twox2_DITHER:
    Twox2DitherImage(l, Cr, Cb, disp, h, w);
    break;

  case FULL_COLOR_DITHER:
    ColorDitherImage(l, Cr, Cb, disp, h, w);
    break;

  case GRAY_DITHER:
    GrayDitherImage(l, Cr, Cb, disp, h, w);
    break;

  case NO_DITHER:
    break;

  case ORDERED_DITHER:
    OrderedDitherImage(l, Cr, Cb, disp, h, w);
    break;

  case MONO_DITHER:
    MonoDitherImage(l, Cr, Cb, disp, h, w);
    break;

  case MONO_THRESHOLD:
    MonoThresholdImage(l, Cr, Cb, disp, h, w);
    break;

  case ORDERED2_DITHER:
    Ordered2DitherImage(l, Cr, Cb, disp, h, w);
    break;

  case MBORDERED_DITHER:
    MBOrderedDitherImage(l, Cr, Cb, disp, h, w);
    break;
  }
}

static void DitherFrame(PictImage * pict, FrameBlock *frame)
{
  DoDitherImage((unsigned char *)pict->luminance,
		(unsigned char *)pict->Cr, (unsigned char *)pict->Cb,
		(unsigned char *)frame->data, win_height, win_width);
  frame->sh = pict->sh;
  frame->gop = pict->gop;
  frame->frame = pict->frame;
  frame->display = pict->display;
  frame->future = pict->future;
  frame->past = pict->past;
  VDputMsg(frame);
}
