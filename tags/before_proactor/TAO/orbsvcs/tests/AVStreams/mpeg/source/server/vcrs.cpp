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
#include <stdio.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>

#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/time.h>
#include "../include/common.h"
#include "../mpeg_shared/com.h"    
#include "../mpeg_shared/routine.h"
#include "../mpeg_server/proto.h"

ACE_RCSID(server, vcrs, "$Id$")

#define SESSION_NUM 4

static int parentpid = -1;

static int listenSocketIn = -1;
static int listenSocketUn = -1;

static struct linger linger = {1,1};

static int live_audio = 0, live_video = 0;  /* 0 - no,  1 - to open,  2 - opened */

int drift_ppm = 0;  /* clock drift in ppm */

int session_limit = SESSION_NUM;
int session_num = 0;

static void int_handler(int sig)
{
  /*
  fprintf(stderr, "process %d killed by sig %d\n", getpid(), sig);
  */
  exit(0);
}

static void on_exit_routine(void)
{
  if (parentpid != getpid()) {
    /*
    fprintf(stderr, "process %d exiting...\n", getpid());
    */
    return;
  }
  /*
  fprintf(stderr, "deamon exiting . . .\n");
  */
  if (live_audio > 1) ExitLiveAudio();
  if (live_video > 1) ExitLiveVideo();
  ComCloseServer();
}

static void clear_child(int sig)
{
  int pid;
  int status;
  
  while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
  {
    session_num --;
    
    if (status == 0) {
      continue;
    }
    fprintf(stderr, "VCRS: child %d (status %d) ", pid, status);
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

main(int argc, char *argv[])
{
  int i, rttag = 0;
  int addrlen;

  for (i = 1; i < argc; i++) {
    if (!strcmp(argv[i], "-rt")) {
      rttag = 1;
    }
    else if (!strcmp(argv[i], "-nrt")) {
      rttag = 0;
    }
    else if (!strncmp(argv[i], "-d", 2)) {
      sscanf(&argv[i][2], "%d", &drift_ppm);
    }
    else if (!strncmp(argv[i], "-s", 2)) {
      sscanf(&argv[i][2], "%d", &session_limit);
      if (session_limit < 1)
        session_limit = SESSION_NUM;
    }
    else if (!strcmp(argv[i], "-lv")) {
      live_video = 1;
    }
    else if (!strcmp(argv[i], "-la")) {
      live_audio = 1;
    }
    else if (!strcmp(argv[i], "-rm")) {
      unlink(VCR_UNIX_PORT);
    }
    else if (!strncmp(argv[i], "-he", 3)) {
      fprintf(stderr, "Usage: %s [-rt] [-nrt] [-rm]\n", argv[0]);
      fprintf(stderr, "          [-d#int(clock drift in ppm)]\n");
      fprintf(stderr, "          [-s#int(limit on number of sessions)]\n");
      exit(1);
    }
  }
  if (drift_ppm) {
    fprintf(stderr, "Clock set to drift at %d ppm (positive is faster and neg. slower)\n",
	    drift_ppm);
  }
  
  setsignal(SIGCHLD, clear_child);
  setsignal(SIGPIPE, SIG_IGN);
  setsignal(SIGBUS, int_handler);
  setsignal(SIGINT, int_handler);
  setsignal(SIGTERM, int_handler);
  setsignal(SIGALRM, SIG_IGN);
  
  parentpid = getpid();

  atexit(on_exit_routine);

  if (live_audio) {
    if (InitLiveAudio(argc, argv) == -1)
      live_audio = 0;
    else
      live_audio = 2;
  }

  if (live_video) {
    if (InitLiveVideo(argc, argv) == -1)
      live_video = 0;
    else
      live_video = 2;
  }

  ComInitServer(VCR_TCP_PORT, VCR_UNIX_PORT, VCR_ATM_PORT);
  
  /*
  setpgrp();
  */
  {
    char buf[100];
    sprintf(buf, "%s%s", LOG_DIR, "vcrsSession.log");
    if (freopen(buf, "a", stdout) == NULL) {
      freopen("/dev/null", "w", stdout);
    }
  }

  for (;;)
  {
    int val;
    unsigned char cmd;
    int serverpid = -1;
    int cfd, dfd;
    int max_pkt_size;

    if (ComGetConnPair(&cfd, &dfd, &max_pkt_size) == -1) continue;
    
    if ((serverpid = fork()) == -1)
    {
      perror("VCRS error on creating service process");
      exit(1);
    }
    session_num ++;
    if (serverpid > 0)  /* parent process for forking servers */
    {
      ComCloseFd(cfd);
      ComCloseFd(dfd);
      continue;
    }
    else
    {
      if (session_num > session_limit) {
	time_t t;
	char *buf;
	t = time(NULL);
	buf = ctime(&t);
	buf[strlen(buf) - 1] = 0;
        fprintf(stderr, "VCRS: %s, session_limit %d, session_number %d\n",
		buf, session_limit, session_num);
      }
      
      if ((val = read(cfd, &cmd, 1)) < 0)
      {
        perror("VCRS fails to read command from service socket");
        exit(1);
      }
      if (val == 0) {
	fprintf(stderr, "Remote client has closed connection.\n");
	ComCloseConn(cfd);
	ComCloseConn(dfd);
	/* continue; -- I don't know why I wrote this line? scen 5-12-96 */
	exit(0);
      }
      ComCloseListen();
      if (cmd == CmdINITvideo)
      {
	/*
	fprintf(stderr, "Server forked a VideoServer process.\n");
	*/
	if (live_audio) LeaveLiveAudio();
	VideoServer(cfd, dfd, rttag, max_pkt_size);
	fprintf(stderr, "Weird: video server returned.\n");
      }
      else
      {
	/*
	fprintf(stderr, "Server forked a AudioServer process.\n");
	*/
	if (live_video) LeaveLiveVideo();
        AudioServer(cfd, dfd, rttag, max_pkt_size);
	fprintf(stderr, "Weird: audio server returned.\n");
      }
      exit(1);
    }
  }
}
