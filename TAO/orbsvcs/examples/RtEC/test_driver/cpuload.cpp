// $Id$

#include "cpuload.h"

#include <stdio.h>
#include <sys/time.h>
#include <signal.h>

namespace
{
  int calibration_time = 1000; //msec
  unsigned long loop_count=0;
  int calibration_done = 0;
  volatile int done = 0;
  unsigned long per_iter_loop_count;
  unsigned long calibrated_loop_count;
}

static void
handle_signal (int signum)
{
  switch (signum)
    {
    case SIGVTALRM:
      //printf ("VTALRM recvd\n");
      per_iter_loop_count = loop_count;
      done = 1;
      break;

    default:
      printf ("signal %d unexpected\n", signum);
    }
}

int is_prime (const unsigned long n,
              const unsigned long min_factor,
              const unsigned long max_factor)
{
  if (n > 3)
    for (unsigned long factor = min_factor;
         factor <= max_factor;
         ++factor)
      if (n / factor * factor == n)
        return factor;

  return 0;
}

void do_computation (unsigned long& loop_count)
{
  static unsigned long prime_number = 961989999;
  ++loop_count;

  is_prime (prime_number, 2, prime_number/2);
}

unsigned long do_calibrate ()
{
  per_iter_loop_count = 0;
  loop_count = 0;

  signal (SIGVTALRM, handle_signal);
  itimerval timerval;

  timerval.it_value.tv_sec = 0;
  timerval.it_value.tv_usec = calibration_time*1000;

  if (setitimer(ITIMER_VIRTUAL, &timerval, 0) != 0)
    {
      printf("setitimer failed\n");
    }

  for (;!done;)
    {
      do_computation (loop_count);
    }
  //printf("iter done\n");
  done = 0;
  return per_iter_loop_count;
}

void CPULoad::run (struct timeval& tv)
{
  unsigned long loops=0;
  unsigned long iters = (unsigned long)
    (((float)calibrated_loop_count/(float)(calibration_time*1000)) *
     (tv.tv_usec + tv.tv_sec*1000000));
  iters = iters + 1;

  // printf("tv = %lu sec %lu usec, iters = %lu\n",
  //     tv.tv_sec, tv.tv_usec, iters);

  for (;iters>0;--iters)
    {
      //loops is really not used here. It is used only during calibration
      do_computation (loops);
    }
}

void CPULoad::calibrate (int num_iters)
{
  if (calibration_done)
    return;

  printf("calibrating ...\n");
  for (int i=0; i<num_iters; ++i)
    {
      unsigned long tmp = do_calibrate ();
      //printf("calibration ended calibrated (%dmsec) loop_count = %lu\n",
      //     calibration_time, tmp);

      if (calibrated_loop_count == 0)
        {
          calibrated_loop_count = tmp;
        }
      else
        {
          //calibrated_loop_count = (calibrated_loop_count + tmp)/2;
          if (calibrated_loop_count < tmp)
            calibrated_loop_count = tmp;
        }
      loop_count = 0;
    }

  calibration_done = 1;
  signal (SIGVTALRM, SIG_DFL);
  //printf("calibration done\n");
}
