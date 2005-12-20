/* $Id$ */
#include <stdio.h>
// @(#)context.c	1.1	10/18/96

#include <stdlib.h>
#include <thread.h>

#define NSLEEP 100
#define TMAX 2
int count[TMAX];

void *
work (void *n)
{
  int ni = (int) n;

  while (1)
    {
      thr_yield ();
      count[ni]++;
    }
  return 0;
}

main (int argc, char *argv[])
{
  int ncorr, t1arg, t0arg, orig_ncorr;
  thread_t tid1, tid0;
  float rate;

  if (argc != 6)
    {
      printf ("usage: %s t0_bound t0_new_lwp t1_bound t1_new_lwp ncorr\n", argv[0]);
      exit (1);
    }
  t0arg = THR_DETACHED;
  if (atoi (argv[1]))
    t0arg |= THR_BOUND;
  if (atoi (argv[2]))
    t0arg |= THR_NEW_LWP;

  t1arg = THR_DETACHED;
  if (atoi (argv[3]))
    t1arg |= THR_BOUND;
  if (atoi (argv[4]))
    t1arg |= THR_NEW_LWP;

  ncorr = atoi (argv[5]);

  if (thr_create (NULL, 0, work, 0, t0arg, &tid0) != 0)
    perror ("couldn't create thread 0");
  if (thr_create (NULL, 0, work, (void *) 1, t1arg, &tid1) != 0)
    perror ("couldn't create thread 1");

  orig_ncorr = thr_getconcurrency ();
  if (ncorr)
    thr_setconcurrency (ncorr);
  sleep (NSLEEP);
  rate = (count[0] + count[1]) / ((float) NSLEEP);
    printf ("\n------------------------------------------------------------------------\n");
  printf ("t0arg 0x%x (%s, %s, %s)\nt1arg 0x%x (%s, %s, %s)\ncount[0] %d count[1] %d\n\
ncorr_orig %d ncorr_set %d ncorr_end %d rate %.3f per_cxt %.2f usec\n",
	  t0arg,
	  (t0arg & THR_DETACHED) ? "THR_DETACHED" : "Not Detached",
	  (t0arg & THR_BOUND) ? "THR_BOUND" : "Not Bound",
	  (t0arg & THR_NEW_LWP) ? "THR_NEW_LWP" : "No New_LWP",
	  t1arg,
	  (t1arg & THR_DETACHED) ? "THR_DETACHED" : "Not Detached",
	  (t1arg & THR_BOUND) ? "THR_BOUND" : "Not Bound",
	  (t1arg & THR_NEW_LWP) ? "THR_NEW_LWP" : "No New_LWP",
	  count[0], count[1],
	  orig_ncorr, ncorr, thr_getconcurrency (), rate, 1.0e6 / rate);
}
