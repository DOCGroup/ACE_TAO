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
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <limits.h>
#ifdef __svr4__
#include <sys/systeminfo.h>
#endif
#ifdef _HPUX_SOURCE
#include <sys/rtprio.h>
#elif  defined(__svr4__)
#include <sys/priocntl.h>
#include <sys/rtpriocntl.h>
#include <sys/tspriocntl.h>
#endif

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/sem.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <string.h>
#include "routine.h"

ACE_RCSID(mpeg_shared, routine, "$Id$")

#define TEST_TYPE 1
#define SIGSETMASK 0
#define stacksize 10000
#define TIME_ROUND 4000
#define BLOCK_NUM 5
#define DELTA_MSEC 20

union semun sem_union;

int get_hostname(char *name, int len)
{
#ifdef __svr4__
  if (sysinfo(SI_HOSTNAME, name, len) == -1) return -1;
  else return 0;
#else
  return ACE_OS::hostname(name, len);
#endif
}

long get_usec(void)
{
  struct timeval tp;
  struct timezone tzp;
  int err;
  if (err=gettimeofday(&tp, &tzp))
  {
    return 0;
  }
  else {
    /*
    return ((long)1000000)*(tp.tv_sec%TIME_ROUND) + (tp.tv_usec);
    */
    return ((long)1000000)*(tp.tv_sec) + (tp.tv_usec);
  }
}

long get_msec(void)
{
  struct timeval tp;
  struct timezone tzp;
  int err;
  if (err=gettimeofday(&tp, &tzp))
  {
    return 0;
  }
  else
  {
    return ((long)1000)*(tp.tv_sec) + (tp.tv_usec / 1000);
  }
}

long get_sec(void)
{
  struct timeval tp;
  struct timezone tzp;
  int err;
  if (err=gettimeofday(&tp, &tzp))
  {
    return 0;
  }
  else
  {
    return tp.tv_sec;
  }
}

/* returns the difference of the two usec time values */

long get_duration(long val1, long val2)
{
  /*
  return val2>=val1 ? val2-val1 : (val2+TIME_ROUND-val1);
  */
  return val2>=val1 ? val2-val1 : ((LONG_MAX - val1) + (val2 - LONG_MIN) + 1);
}

#ifdef LINUX
#else
int usleep(unsigned int usec)
{
  struct timeval val;

  if (usec <= 0) return -1;
  val.tv_sec = usec / 1000000;
  val.tv_usec = usec % 1000000;
  if (select(0, NULL, NULL, NULL, &val) == -1 && errno != 4)
  {
   ACE_OS::perror ("sleep with select");
    ACE_OS::exit (1);
  }
  return 0;
}
#endif

void beep(void)
{
  fprintf(stderr, "\007");
}

#ifndef FreeBSD
#include <malloc.h>
#endif
#include <sys/shm.h>

void remove_shmem_id(int id)
{
 ACE_OS::shmctl (id, IPC_RMID, NULL);
#if 0
  /* weird also on some sunOs4 machines */
  /* weird, HPUX always report error on this operation */
  if (errno)
  {
    fprintf(stderr, "shmctl shm_id:%d to IPC_RMID error:", id);
   ACE_OS::perror ("");
  }
#endif
}

char * creat_shared_mem_id(int size, int * id)
{
  char * shm;
  int shm_id;
  
  shm_id = ACE_OS::shmget (IPC_PRIVATE, size, IPC_CREAT | 0666);
  if (shm_id == -1)
  {
    shm_id = ACE_OS::shmget (IPC_PRIVATE, 0, IPC_CREAT | 0666);
    if (shm_id == -1)
    {
     ACE_OS::perror ("Shared MEM shmget1 error");
      ACE_OS::exit (1);
    }
    else
    {
      if (shmctl(shm_id, IPC_RMID, NULL) == -1)
      {
       ACE_OS::perror ("Shared MEM shmctl error");
        ACE_OS::exit (1);
      }
      shm_id = ACE_OS::shmget (IPC_PRIVATE, 0, IPC_CREAT | 0666);
      if (shm_id == -1)
      {
       ACE_OS::perror ("Shared MEM shmget2 error");
        ACE_OS::exit (1);
      }
    }
  }
  shm = (char *)shmat(shm_id, (char *)0, 0);
  if ((int)shm == -1)
  {
   ACE_OS::perror ("Shared MEM shmat error");
    ACE_OS::exit (1);
  }
  *id = shm_id;
  return shm;
}

char * creat_shared_mem(int size)
{
  int shm_id;
  char * shm = creat_shared_mem_id(size, &shm_id);
  remove_shmem_id(shm_id);
  return shm;
}

void remove_shared_mem(char *ptr)
{
  ACE_OS::shmdt (ptr);
}

#define SEM_NUM 6
static int semId = -1;
static int masterPid = -1;
static int nextSem = -1;

void enter_cs(int semaphore_id)
{
  struct sembuf sop;
  sop.sem_num = semaphore_id;
  sop.sem_op = -1;
  sop.sem_flg = 0;
  while (ACE_OS::semop(semId, &sop, 1) == -1)
  {
    if (errno == EINTR)
      continue;
    fprintf(stderr, "semop(enter_cs) error: pid=%d",ACE_OS::getpid ());
   ACE_OS::perror ("");
    ACE_OS::exit (1);
  }
}

void leave_cs(int semaphore_id)
{
  struct sembuf sop;
  sop.sem_num = semaphore_id;
  sop.sem_op = 1;
  sop.sem_flg = 0;
  while (ACE_OS::semop(semId, &sop, 1) == -1)
  {
    if (errno == EINTR)
      continue;
    fprintf(stderr, "semop(leave_cs) error, pid=%d",ACE_OS::getpid ());
   ACE_OS::perror ("");
    ACE_OS::exit (1);
  }
}

int creat_semaphore(void)
{
  int i;
  int sem_val;
  int semaphore_id;
  if (semId == -1) {
    semId = ACE_OS::semget (IPC_PRIVATE, SEM_NUM, IPC_CREAT | 0666);
    if (semId == -1) {
     ACE_OS::perror ("Semaphore semget error");
      ACE_OS::exit (1);
    }
    masterPid =ACE_OS::getpid ();
    nextSem = 0;
  }
  else if (masterPid !=ACE_OS::getpid ()) {
    fprintf(stderr, "Error: this creat_semaphore() assumes semaphores are allocated\n");
    fprintf(stderr, "  only in single process %d, while current pid=%d\n",
	    masterPid,ACE_OS::getpid ());
    ACE_OS::exit (1);
  }
  semaphore_id = nextSem ++;
  if (semaphore_id >= SEM_NUM) {
    fprintf(stderr, "Error: all of %d semaphores used up.\n", SEM_NUM);
    ACE_OS::exit (1);
  }
  sem_val = ACE_OS::semctl (semId, semaphore_id, GETVAL, sem_union);
/*
   fprintf(stderr, "Initial semaphore value: %d\n", sem_val);
*/
  if (sem_val == 0)
    leave_cs(semaphore_id);
  return semaphore_id;
}

void delete_semaphore()
{
  if (masterPid ==ACE_OS::getpid () && semId >= 0) {
	sem_union.val = 0;
    ACE_OS::semctl (semId, 0, IPC_RMID, sem_union);
  } else {
    if (masterPid !=ACE_OS::getpid ())
    fprintf(stderr, "Pid %d not supposed to remove semId created by pid %d\n",
	   ACE_OS::getpid (), masterPid);
    else
    fprintf(stderr, "The semaphore has been deleted.\n");
  }
  return;
}

void remove_semaphore(int sid)
{
  return;
  //  ACE_OS::semctl (semId, semaphore_id, IPC_RMID, 0);
}

void remove_all_semaphores (void)
{
	sem_union.val = 0;
  ACE_OS::semctl (semId,0, IPC_RMID, sem_union);
  return;
}

int get_semval(int sid)
{
  int val, val1 = 0;
  errno = 0;
	sem_union.val = 0;
  val = ACE_OS::semctl (semId, sid, GETVAL, sem_union);
  if (val == -1) {
   ACE_OS::perror ("getting value of a semaphore");
    ACE_OS::exit (1);
  }
  /*
  if (val == 0) {
    fprintf(stderr, "pid %d to call ACE_OS::semctl (%d, 0, GETZCNT)\n",ACE_OS::getpid (), sid);
	sem_union.val = 0;
    val = ACE_OS::semctl (semId, 0, GETZCNT, 0);
    if (val == -1) {
     ACE_OS::perror ("getting semzcnt of a semaphore");
      ACE_OS::exit (1);
    }
    
    fprintf(stderr, "pid %d to call ACE_OS::semctl (%d, 0, GETNCNT)\n",ACE_OS::getpid (), sid);
    usleep(10000000);
	sem_union.val = 0;
    val1 = ACE_OS::semctl (semId, sid, GETNCNT, sem_union);
    if (val1 == -1) {
     ACE_OS::perror ("getting semncnt of a semaphore");
    }
    fprintf(stderr, "pid %d to called ACE_OS::semctl (GETNCNT)\n",ACE_OS::getpid ());
    
    fprintf(stderr, "Semval val %d, val1 %d\n", val, val1);
    return (0-(val + val1));
  }
  else
  */
  return val;
}

void get_full_path(char *filename, char * buffer, int bufsize)
{
  char path[MAXPATHLEN];

  path[0] = 0;
  
  if (*filename == '/')
    strncpy(path, filename, MAXPATHLEN);
  else if (*filename == '~')
  {
    char * env = getenv("HOME");
    if (env != NULL)
    {
      strncpy(path, env, MAXPATHLEN);
      strncat(path, filename+1, MAXPATHLEN -ACE_OS::strlen (path));
    }
    else
      strncpy(path, filename, MAXPATHLEN);
  }
  else  /* from current dir */
  {
    getcwd(path, MAXPATHLEN);
    if (errno)
      strncpy(path, filename, MAXPATHLEN);
    else
    {
      path[MAXPATHLEN-4] = 0;
      strcat(path, "/");
      strncat(path, filename, MAXPATHLEN -ACE_OS::strlen (path));
    }
  }
  path[MAXPATHLEN-1] = 0;
  strncpy(buffer, path, bufsize);
  buffer[bufsize-1] = 0;
  /*
  fprintf(stderr, "%s extended to %s\n", filename, buffer);
  */
}

void setsignal(int sig, void (func)(int))
{
#ifdef _HPUX_SOURCE
  struct sigvec sv;

  sv.sv_handler = func;
  sv.sv_mask    = 0;
  sv.sv_flags   = 0;
  if (sigvector (sig, &sv, (struct sigvec *) NULL) == -1) {
    fprintf(stderr, "sigvector(%d) error", sig);
   ACE_OS::perror ("");
    ACE_OS::exit (1);
  }
#elif defined(__svr4__) || defined(IRIX)
  if (sigset(sig, func) == SIG_ERR)  {
    fprintf(stderr, "sigset(%d,func) error", sig);
   ACE_OS::perror ("");
    ACE_OS::exit (1);
  }
#elif defined(sun) || defined(FreeBSD) || defined(ULTRIX) || defined(LINUX)
  {
    struct sigaction act;
    act.sa_handler = func;
#ifdef FreeBSD
    act.sa_flags = SV_INTERRUPT;
#else
    act.sa_flags =/* SA_INTERRUPT;*/ SA_RESTART;
#endif
    // @@ Naga commented this line.
    //    act.sa_mask = 0;
    if (!sigaction(sig, &act, NULL)) return;
    fprintf(stderr, "sigaction(%d,...) error", sig);
   ACE_OS::perror ("");
    ACE_OS::exit (1);
  }
  /*
  if (signal(sig, func) == SIG_ERR) {
    fprintf(stderr, "signal(%d,func) error", sig);
   ACE_OS::perror ("");
    ACE_OS::exit (1);
  }
  */
#else
  fprintf(stderr,
	  "Error: code for setsignal(%d,func) is missing in source/mpeg_shared/routine.cpp\n",
	  sig);
  ACE_OS::exit (1);
#endif
}

/* set to 'pri': 0 -lowest RT, higher value higher RT priority. -1 - Time Sharing.
   For HPUX, 0 -> 127, 1 -> 127, ..., pri -> 127 - pri;
   For Solaris, -> 0, 1 -> 1, pri -> pri;

   The function returns 0 on success, -1 on failure
   */
int SetRTpriority(char *msg, int pri)
{
  if (pri >= 0) {
#ifdef _HPUX_SOURCE
    if (rtprio(0, 127 - pri) == -1)
    {
      fprintf(stderr, "%s fails to be set to RT priority %d", msg, 127 - pri);
     ACE_OS::perror ("");
      return -1;
    }
    return 0;
#elif defined(__svr4__)
    pcinfo_t pci;
    pcparms_t pcp;
    rtparms_t * rtp = (rtparms_t *)pcp.pc_clparms;
    strcpy(pci.pc_clname, "RT");
    if (priocntl(P_PID, P_MYID, PC_GETCID, (char *)&pci) == -1) {
      fprintf(stderr, "%s priocntl(PC_GETCID) failed for RT pri %d", msg, pri);
     ACE_OS::perror ("");
      return -1;
    }
    pcp.pc_cid = pci.pc_cid;
    rtp->rt_pri = pri;
    rtp->rt_tqsecs = 0;
    rtp->rt_tqnsecs = 10000000;  /* 10 (ten) milliseconds */
    if (priocntl(P_PID, P_MYID, PC_SETPARMS, (char *)&pcp) == -1) {
      fprintf(stderr, "%s priocntl(PC_SETPARMS) failed for RT pri %d", msg, pri);
     ACE_OS::perror ("");
      return -1;
    }
    return 0;
#else
    fprintf(stderr, "%s warning: RT priority not available on this arch.\n", msg);
    return -1;
#endif
  }
  else {  /* set to TS priority */
#ifdef _HPUX_SOURCE
    if (rtprio(0, RTPRIO_RTOFF) == -1) {
      fprintf(stderr, "%s fails to be set to RTPRIO_RTOFF", msg);
     ACE_OS::perror ("");
      return -1;
    }
    return 0;
#elif defined(__svr4__)
    pcinfo_t pci;
    pcparms_t pcp;
    tsparms_t * tsp = (tsparms_t *)pcp.pc_clparms;
    strcpy(pci.pc_clname, "TS");
    if (priocntl(P_PID, P_MYID, PC_GETCID, (char *)&pci) == -1) {
      fprintf(stderr, "%s priocntl(PC_GETCID) failed for TS priority", msg);
     ACE_OS::perror ("");
      return -1;
    }
    pcp.pc_cid = pci.pc_cid;
    tsp->ts_uprilim = 0;
    tsp->ts_upri = 0;
    if (priocntl(P_PID, P_MYID, PC_SETPARMS, (char *)&pcp) == -1) {
      fprintf(stderr, "%s priocntl(PC_SETPARMS) failed for TS priority", msg);
     ACE_OS::perror ("");
      return -1;
    }
    return 0;
#endif
  }
}
