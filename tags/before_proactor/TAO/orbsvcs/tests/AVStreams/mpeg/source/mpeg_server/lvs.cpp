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
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#include <time.h>
#include <signal.h>
#include <sys/time.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Intrinsic.h>

#include "include/common.h" 
#include "mpeg_shared/routine.h"
#include "server_proto.h"

ACE_RCSID(mpeg_server, lvs, "$Id$")

#ifdef LIVE_VIDEO
#include "lvs_shared.h"

LvsSharedData * shared = NULL;
static int lvspid = -1;  /* -1: no lvs, 0 - lvs child,  >0 - vs proc */
static int prefid = -1, first_fid, first_frame;
static int curbuf;

#define PID_FILE "~/.vcr/lvsPid"
#define QUALITY_FILE "~/.vcr/lvsQuality"

static int count = 0;
static unsigned session_time;

static int curpid = 0;

static void on_exit_routine(void)
{
  char buf[256];
  if (getpid() != curpid) return;
  get_full_path(PID_FILE, buf, 256);
  unlink(buf);
}

static void register_pid(void)
{
  char buf[256];
  FILE *fp;
  get_full_path(PID_FILE, buf, 256);
  fp = fopen(buf, "w");
  fprintf(fp, "%d -- pid of current VS process\n", getpid());
  fclose(fp);
}

static void set_quality_level(void)
{
  char buf[256];
  FILE *fp;
  int qos = -1;
  get_full_path(QUALITY_FILE, buf, 256);
  fp = fopen(buf, "r");
  if (fp == NULL) return;
  
  fscanf(fp, "%d", &qos);
  fclose(fp);
  if (qos >= 0 && qos <= 100) {
    shared->qos_val = qos;
  }
}

static void hup_handler(int status)
{
  setsignal(SIGHUP, SIG_IGN);
  if (getpid() != curpid) return;
  set_quality_level();
  setsignal(SIGHUP, hup_handler);
}

#endif

int InitLiveVideo(int argc, char ** argv)  /* -1 -- failed, 0 - succ */
{
#ifndef LIVE_VIDEO
  return -1;
#else
/*
#define BUFSIZE 768 * 576 * 3 / 8
#define BUFSIZE 640 * 480 * 3 / 2
*/
#define BUFSIZE 1024 * 1024
  
  char * buf = creat_shared_mem(BUFSIZE);
  int sid = creat_semaphore();
  int susid = creat_semaphore();

  enter_cs(susid);
  
  shared = (LvsSharedData *)buf;
  shared->sid = sid;
  shared->susid = susid;
  shared->initState = 0;
  shared->bufDataBytes = BUFSIZE - sizeof(*shared);
  shared->buf = (LvsBuffer *)((char *)shared + sizeof(*shared));

  register_pid();
  set_quality_level();
  
  /* all other fields to be initialized by LVS process */

  while ((lvspid = fork()) == -1);

  if (lvspid == 0) { /* child process, reading from camera */
    LiveVideoProcess(argc, argv);
    exit(1);
  }

  while (shared->initState == 0) usleep(100000);  /* wait for SunVideo to init */

  if (shared->initState == -1) {
    fprintf(stderr, "LVS error: init of child process failed\n");
    kill(lvspid, SIGINT);
    remove_shared_mem(buf);
    remove_semaphore(sid);
    remove_semaphore(susid);
    lvspid = -1;
    return -1;
  }
  else {
    
    fprintf(stderr, "LVS initialized.\n");

    curpid = getpid();
    
    atexit(on_exit_routine);

    setsignal(SIGHUP, hup_handler);
  
    return 0;
  }
#endif
}

void LeaveLiveVideo(void)
{
#ifndef LIVE_VIDEO
  return;
#else
  if (lvspid == -1) return;
  remove_shared_mem((char *)shared);
  shared = NULL;
#endif
}

void ExitLiveVideo(void)
{
#ifndef LIVE_VIDEO
  return;
#else
  if (lvspid == -1) return;
  kill(lvspid, SIGINT);
  remove_semaphore(shared->sid);
  remove_semaphore(shared->susid);
  remove_shared_mem((char *)shared);
  lvspid = -1;
  shared = NULL;
#endif
}

int OpenLiveVideo(int * format,
		  int * width, int * height,
		  int * averageFrameSize,
		  double * fps,
		  int * pelAspectRatio  /* MPEG1 encoding used here */
		  )                     /* -1 -- failed, 0 - succ */
{
#ifndef LIVE_VIDEO
  return -1;
#else
  if (lvspid == -1) return -1;
  *format = shared->format;
  *width = shared->width;
  *height = shared->height;
  *averageFrameSize = shared->averageFrameSize;
  *fps = shared->fps;
  *pelAspectRatio = shared->pelAspectRatio;
  
  fprintf(stderr, "LVS session %d opened.\n", getpid());
  
  return 0;
#endif
}

void StartPlayLiveVideo(void)
{
#ifndef LIVE_VIDEO
  return;
#else
  if (lvspid == -1) return;

  count = 0;
  session_time = get_msec();
  
  enter_cs(shared->sid);
  
  prefid = -1;
  first_fid = -1;
  first_frame = -1;
  
  if (shared->activeSessions <= 0) shared->activeSessions = 1;
  else shared->activeSessions ++;
  
  leave_cs(shared->sid);

  while (shared->curbuf <= shared->wait_curbuf) {
    /*
    fprintf(stderr, "curbuf %d, wait_curbuf %d\n",
	    shared->curbuf, shared->wait_curbuf);
    */
    usleep(10000);
  }
  
  fprintf(stderr, "LVS session %d started\n", getpid());
  
  return;
#endif
}

void StopPlayLiveVideo(void)
{
#ifndef LIVE_VIDEO
  return;
#else
  if (lvspid == -1) return;
  enter_cs(shared->sid);
  shared->activeSessions --;
  leave_cs(shared->sid);
  
  fprintf(stderr, "LVS session %d stopped, at %5.2f fps.\n", getpid(),
	  ((double)count * 1000.0) / get_duration(session_time, get_msec()));
  
  return;
#endif
}

#if 1
int ReadLiveVideoPicture(int * frame, char * buf, int size) /* ret # bytes */
{
#ifndef LIVE_VIDEO
  return 0;
#else
  int pframe = * frame;
  int ssize;
  int bufptr;
  
  if (lvspid == -1) return 0;

  if (first_frame == -1) {  /* first frame in current session */
    curbuf = shared->curbuf - 1;
    bufptr = curbuf % shared->bufnum;
    enter_cs(shared->sid);
  }
  else {
    int bufnum = shared->bufnum < 3 ? shared->bufnum : 3;
    do {
      curbuf ++;
      if (curbuf <= shared->curbuf - bufnum) {
	/*
	fprintf(stderr, "LVS session %d skipped %d buffers\n",
		getpid(), shared->curbuf - bufnum + 1 - curbuf);
	*/
	curbuf = shared->curbuf - bufnum + 1;
      }
      bufptr = curbuf % shared->bufnum;
      while (shared->buf[bufptr].fid <= prefid) {
	/*
	fprintf(stderr,
		"LVS session %d wait: curbuf %d, shared->curbuf %d, prefid %d, shared->fid %d\n",
		getpid(), curbuf, shared->curbuf, prefid, shared->buf[bufptr].fid);
	*/
	enter_cs(shared->sid);
	shared->suscount ++;
	leave_cs(shared->sid);
	enter_cs(shared->susid);
      }
    } while (pframe > first_frame + (shared->buf[bufptr].fid - first_fid));
    
    enter_cs(shared->sid);
    /*
    fprintf(stderr, "pframe %d, aframe %d\n",
	    pframe, first_frame + (shared->buf[bufptr].fid - first_fid));
    */
  }
  shared->buf[bufptr].refcnt ++;
  leave_cs(shared->sid);
  
  ssize = shared->buf[bufptr].size;

  ssize = (ssize < size ? ssize : size);
  memcpy(buf, shared->buf[bufptr].data, ssize);

  enter_cs(shared->sid);

  shared->buf[bufptr].refcnt --;
  prefid = shared->buf[bufptr].fid;
  if (first_frame == -1) {
    first_fid = prefid;
    first_frame = * frame;
  }
  else {
    * frame = first_frame + (prefid - first_fid);
  }
  leave_cs(shared->sid);
  /*
  fprintf(stderr,
	  "LVS session %d read curbuf %d fid %d, frame %d size %d (ffid %d, fframe %d)\n",
	  getpid(), curbuf, prefid, *frame, ssize, first_fid, first_frame);
  */
  /*
  fprintf(stderr, "LVS read fid %d, frame %d\n", prefid, *frame);
  */
  count ++;
  return ssize;
#endif
}
#endif

#if 0
/* following version is for testing the speed of the vcr path, excluding the
   xil stuff */

static long start_time;

int ReadLiveVideoPicture(int * frame, char * buf, int size) /* ret # bytes */
{
#ifndef LIVE_VIDEO
  return 0;
#else
  int pframe = * frame;
  int ssize;
  char * sbuf;
  if (lvspid == -1) return 0;

 loop_begin:
  /*
  enter_cs(shared->sid);
  */
  if (first_frame == -1) {
    first_frame = *frame;
    first_fid = shared->fid;
    start_time = get_msec();
  }
  else {
    int f = first_frame + ((double)get_duration(start_time, get_msec()) /
			    ((double)1000.0 / (double)shared->fps));
    if (f < *frame) {
      
      leave_cs(shared->sid);
      /*
      fprintf(stderr, "f=%d < *frame=%d\n", f, *frame);
      */
      usleep(10000);
      goto loop_begin;
    }
    *frame = f;
  }
  
  prefid = shared->fid;
  sbuf = shared->buf[shared->curbuf].data;
  ssize = shared->buf[shared->curbuf].size;

  ssize = (ssize < size ? ssize : size);
  memcpy(buf, sbuf, ssize);
  /*
  leave_cs(shared->sid);
  */
  /*
  fprintf(stderr, "LVS read fid %d, frame %d size %d (ffid %d, fframe %d)\n",
	  shared->fid, *frame, ssize, first_fid, first_frame);
  */
  count ++;
  return ssize;
#endif
}
#endif

