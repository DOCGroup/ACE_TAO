/*
 * Copyright (c) 1992 The Regents of the University of California.
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */
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
#include <unistd.h>
#include <stdlib.h>
#include <X11/Intrinsic.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#ifndef MIPS
#include <netinet/in.h>
#else
#include <bsd/netinet/in.h>
#endif
#if defined(SunOS) || defined(FreeBSD)
#include <stdlib.h>
#endif
#include <time.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include "video.h"
#include "proto.h"
#include "../include/common.h"
#include "newproto.h"
#include "global.h"

#include "util.h"
#include "dither.h"

static int mainPid;

static time_t start_time;
/*
 *--------------------------------------------------------------
 *
 * usage --
 *
 *	Print mpeg_play usage
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	exits with a return value -1
 *
 *--------------------------------------------------------------
 */

void
usage(char *s)	/* program name */
{
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "%s [-rt] [-shmem] [-rmsem]\n", s);
    fprintf(stderr, "    [-dither ordered|ordered2|fs4|fs2|fs2fast|hybrid|\n");
    fprintf(stderr, "             hybrid2|2x2|gray|color|mono|threshold]\n");
    fprintf(stderr, "    [X-window options]\n");
    fprintf(stderr, "    [-v [host_name:]video_file_name]\n");
    fprintf(stderr, "    [-a [host_name:]audio_file_name]\n");
    
    fprintf(stderr, "    [-p movie_file_name]\n");
    fprintf(stderr, "    [-l movie_list_file_name]\n");
    
    fprintf(stderr, "    [-help]\n");
    fprintf(stderr, "    [-quiet]\n");
    
    exit (-1);
}

/*
 *--------------------------------------------------------------
 *
 * int_handler --
 *
 *	Handles Cntl-C interupts..
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

static int on_exit_tag = 1;

void set_exit_routine_tag(int tag)
{
  on_exit_tag = tag;
}
static void on_exit_routine(void)
{
  if (!on_exit_tag)
    return;
  if (mainPid == getpid())
  {
    extern void delete_semaphore();
    ABdeleteSem();
    VBdeleteSem();
    VDdeleteSem();
    delete_semaphore();
    if (getuid() != DEVELOPER_UID)
    {
      FILE * fp;
      char *buf = (char *)malloc(100);
      char *tbuf;
      if (buf != NULL) {
	sprintf(buf, "%s%s", LOG_DIR, "vcrSession.log");
	if ((fp = fopen(buf, "a")) != NULL) {
	  time_t val = time(NULL);
	  get_hostname(buf, 100);
	  buf[99] = 0;
	  tbuf = ctime(&start_time);
	  tbuf[strlen(tbuf) - 1] = 0;
	  fprintf(fp, "User %d on %s at %s %dm%ds\n", getuid(), buf,
		  tbuf, (val - start_time) / 60, (val - start_time) % 60);
	}
	free(buf);
      }
    }
  }
  on_exit_tag = 0;
  kill(0, SIGINT);
}

static void int_handler(int sig)
{
  exit(0);
}

static void clear_child(int sig)
{
  int pid;
  int status;
  
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
  {
    if (status == 0) {
      continue;
    }
    fprintf(stderr, "VCR: child %d (status %d) ", pid, status);
    if (WIFEXITED(status)) {
      fprintf(stderr, "exited with status %d\n", WEXITSTATUS(status));
    }
    else if (WIFSIGNALED(status)) {
#if defined(_HPUX_SOURCE) || defined(__svr4__) || defined(IRIX)
      fprintf(stderr, "terminated at signal %d%s.\n", WTERMSIG(status),
	      WCOREDUMP(status) ? ", core dumped" : "");
#else
      fprintf(stderr, "terminated at signal %d.\n", WTERMSIG(status));
#endif
    }
    else if (WIFSTOPPED(status)) {
      fprintf(stderr, "stopped at signal %d\n", WSTOPSIG(status));
    }

  }
}

/*
 *--------------------------------------------------------------
 *
 * main --
 *
 *	Parses command line, starts decoding and displaying.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	None.
 *
 *--------------------------------------------------------------
 */

void main(int argc, char ** argv)
{

  int mark;
  int rmsemFlag = 0;

  argc_share	= argc;
  argv_share	= argv;
  mark = 1;
  argc--;

  displayName[0] = 0;
  ditherType = ORDERED2_DITHER;
  LUM_RANGE = 8;
  CR_RANGE = CB_RANGE = 4;
  noDisplayFlag = 0;

#ifdef SH_MEM
  shmemFlag = 0;
#endif

  if (getenv("DISPLAY")!= NULL)
  {
    char * disp = getenv("DISPLAY");
    if (strncmp(disp, "anquetil:0", 10) == 0 ||
	strncmp(disp, "helix:0", 7) == 0 ||
	strncmp(disp, "hinault:0", 9) == 0 ||
	strncmp(disp, "lemond:0", 8) == 0 ||
	strncmp(disp, "indurain:0", 10) == 0)
      ditherType = ORDERED_DITHER;
    else
      ditherType = ORDERED_DITHER;
  }

  while (argc) {
    if (strcmp(argv[mark], "-rt") == 0) {
      realTimeFlag = 1;
      argc--; mark++;
    }
    else if (strcmp(argv[mark], "-rt1") == 0) {
      realTimeFlag = 2;
      argc--; mark++;
    }
    else if (strcmp(argv[mark], "-rt2") == 0) {
      realTimeFlag = 3;
      argc--; mark++;
    }
    else if (strcmp(argv[mark], "-shmem") == 0) {
      argc--; mark++;
      shmemFlag = 1;
    }
    else if (strcmp(argv[mark], "-rmsem") == 0) {
      argc--; mark++;
      rmsemFlag = 1;
    }
#if 0
    else if (strcmp(argv[mark], "-nop") == 0) {
      TogglePFlag();
      argc--; mark++;
    } else if (strcmp(argv[mark], "-nob") == 0) {
      ToggleBFlag();
      argc--; mark++;
    }
#endif
    else if (strcmp(argv[mark], "-l") == 0) {   /* program list file name */
      strcpy(proglistName, argv[++mark]);
      argc -= 2; mark++;
      
    } else if (strcmp(argv[mark], "-display") == 0) {
      strcpy(displayName,argv[++mark]);
      argc -= 2; mark++;
    } else if (strcmp(argv[mark], "-dither") == 0) {
      argc--; mark++;
      if (argc < 1) {
	perror("Must specify dither option after -dither flag");
	usage(argv[0]);
      }
      if (strcmp(argv[mark], "hybrid") == 0) {
	argc--; mark++;
	ditherType = HYBRID_DITHER;
      } else if (strcmp(argv[mark], "hybrid2") == 0) {
	argc--; mark++;
	ditherType = HYBRID2_DITHER;
      } else if (strcmp(argv[mark], "fs4") == 0) {
	argc--; mark++;
	ditherType = FS4_DITHER;
      } else if (strcmp(argv[mark], "fs2") == 0) {
	argc--; mark++;
	ditherType = FS2_DITHER;
      } else if (strcmp(argv[mark], "fs2fast") == 0) {
	argc--; mark++;
	ditherType = FS2FAST_DITHER;
      } else if (strcmp(argv[mark], "hybrid2") == 0) {
	argc--; mark++;
	ditherType = HYBRID2_DITHER;
      } else if (strcmp(argv[mark], "2x2") == 0) {
	argc--; mark++;
	ditherType = Twox2_DITHER;
      } else if (strcmp(argv[mark], "gray") == 0) {
	argc--; mark++;
	ditherType = GRAY_DITHER;
      } else if (strcmp(argv[mark], "color") == 0) {
	argc--; mark++;
	ditherType = FULL_COLOR_DITHER;
      }
      /*
      else if (strcmp(argv[mark], "none") == 0) {
	argc--; mark++;
	ditherType = NO_DITHER;
      }
      */
      else if (strcmp(argv[mark], "ordered") == 0) {
	argc--; mark++;
	ditherType = ORDERED_DITHER;
      } else if (strcmp(argv[mark], "ordered2") == 0) {
	argc--; mark++;
	ditherType = ORDERED2_DITHER;
      }
      /*
      else if (strcmp(argv[mark], "mbordered") == 0) {
	argc--; mark++;
	ditherType = MBORDERED_DITHER;
      }
      */
      else if (strcmp(argv[mark], "mono") == 0) {
	argc--; mark++;
	ditherType = MONO_DITHER;
      } else if (strcmp(argv[mark], "threshold") == 0) {
	argc--; mark++;
	ditherType = MONO_THRESHOLD;
      } else {
	perror("Illegal dither option.");
	usage(argv[0]);
      }
    }
    else if (strcmp(argv[mark], "-quiet") == 0) {
      argc--; mark++;
      quietFlag = 1;
    }
    else if (strcmp(argv[mark], "-l_range") == 0) {
      argc--; mark++;
      LUM_RANGE = atoi(argv[mark]);
      if (LUM_RANGE < 1) {
	fprintf(stderr, "Illegal luminance range value: %d\n", LUM_RANGE);
	exit(1);
      }
      argc--; mark++;
    }
    else if (strcmp(argv[mark], "-cr_range") == 0) {
      argc--; mark++;
      CR_RANGE = atoi(argv[mark]);
      if (CR_RANGE < 1) {
	fprintf(stderr, "Illegal cr range value: %d\n", CR_RANGE);
	exit(1);
      }
      argc--; mark++;
    }
    else if (strcmp(argv[mark], "-cb_range") == 0) {
      argc--; mark++;
      CB_RANGE = atoi(argv[mark]);
      if (CB_RANGE < 1) {
	fprintf(stderr, "Illegal cb range value: %d\n", CB_RANGE);
	exit(1);
      }
      argc--; mark++;
    }
    else if (strcmp(argv[mark], "-help") == 0) {
      usage(argv[0]);
    }
    else {
      argc--; mark++;
    }
    /*
    else if (argv[mark][0] == '-') {
      fprintf(stderr, "Un-recognized flag %s\n",argv[mark]);
      usage(argv[0]);
    }
    */
    /* the input video and audio files with options '-v', '-a' and '-p' are processed
       at the end of 'ui.c' */
  }
  if (ditherType == MBORDERED_DITHER)
  {
    fprintf(stderr, "Sorry, mbordered dithertype no longer supported.\n");
    exit(1);
  }
  setsignal(SIGCHLD, clear_child);
  setsignal(SIGINT, int_handler);
  setsignal(SIGHUP, int_handler);
  /*
  setsignal(SIGKILL, int_handler);
  setsignal(SIGSEGV, int_handler);
  */
  setsignal(SIGQUIT, int_handler);
  setsignal(SIGTERM, int_handler);
  setsignal(SIGALRM, SIG_IGN);
  setsignal(SIGPIPE, SIG_IGN);


  atexit(on_exit_routine);
  
  if (rmsemFlag) {
    if (geteuid() == 0) { /* root, refuse to remove shm and sem ids */
      fprintf(stderr, "You are ROOT, -rmsem is ignored 'cause it's too dangerous.\n");
    }
    else {
      char *tmpf = tempnam("/tmp", "");
      if (tmpf != NULL) {
	char buf[128];
	FILE *fp;
	sprintf(buf, "ipcs >%s", tmpf);
	system(buf);
	if ((fp = fopen(tmpf, "r")) != NULL) {
	  while (fgets(buf, 128, fp) != NULL && (int)(strlen(buf)) > 10) {
	    int id;
	    sscanf(&buf[1], "%d", &id);
	    if (buf[0] == 'm') {
	      shmctl(id, IPC_RMID, NULL);
	    }
	    else if (buf[0] == 's') {
	      semctl(id, 0, IPC_RMID, 0);
	    }
	  }
	  fclose(fp);
	  unlink(tmpf);
	}
	free(tmpf);
      }
    }
  }

  if (quietFlag) {
    if ((freopen("/dev/null", "w", stdout) == NULL) ||
	(freopen("/dev/null", "w", stderr) == NULL)) {
      exit(10);
    }
  }
  
  start_time = time(NULL);

  setsid();  /* break controlling terminal and creat a new session */

  mainPid = getpid();

  CTRmain();
}
