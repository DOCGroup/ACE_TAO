#define	_REENTRANT
// @(#)signal_thread.c	1.1	10/18/96

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <thread.h>
#include <synch.h>
#include <unistd.h>

void *handle (v)
     void *v;
{
  sigset_t set;
  
  sigemptyset (&set);
  sigaddset (&set, SIGINT);

  for (;;)
    if (sigwait (&set) != SIGINT)
      perror ("sigwait"), exit (1);
    else
      fprintf (stderr, "got sigint!\n");
}

int 
main (void)
{
  int      retval;
  sigset_t set;
  thread_t t_id;

  sigemptyset (&set);
  sigaddset (&set, SIGINT);

  if (sigprocmask (SIG_BLOCK, &set, 0) == -1)
    perror ("sigprocmask"), exit (1);

  if (thr_sigsetmask (SIG_BLOCK, &set, 0) == -1)
    perror ("sigprocmask"), exit (1); 

  if (thr_create (0, 0, handle, 0, THR_DETACHED, &t_id) != 0)
    perror ("thr_create"), exit (1);

  for (;;)
    {
      fprintf (stderr, "blocking for read in thread\n");
      if (read (0, &retval, sizeof retval) != sizeof retval)
	perror ("read"); 
    }
  fprintf (stderr, "I'm exiting!\n");
  return 0;
}
