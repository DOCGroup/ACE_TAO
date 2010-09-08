// $Id$

// ============================================================================
//
// = LIBRARY
//    (none)
//
// = FILENAME
//    childbirth_time.cpp
//
// = DESCRIPTION
//   This program is used to measure various child-creation mechanisms
//   on various platforms.  By default, the program measure the time
//   to 'fork' a new process using ACE_Process.spawn ().  Other tests
//   are possible as described below.   James Hu provides the idea to
//   batch measuring threads creation.
//
//   Usage: childbirth_time [-n ###] [-l ###] [-p|-f|-t|-a|-m|-x] [-h] [-e]
//
//          -n ###: Specify number of iteration in tens.  If this
//                  option is not specified, the default is
//                  MULTIPLY_FACTOR * (100 iterations,) which is
//                  equivalent to -n 10.
//
//          -l ###: Specify MULTIPLY_FACTOR.  Default is 10.
//
//             *-p: Measure the performance of forking a child process
//                  and exec an "empty" program.  This test uses
//                  ACE_Process.spawn ().  (Default)
//
//              -f: Measure the performance of native "fork" function
//                  call.  Notice that there is no equivalent NT
//                  function calls and this option is only available
//                  on UN*X platform.
//
//              -t: Measure the performance of native thread creation
//                  mechanisms.  On Solaris, this is thr_create ().
//                  On NT, this is CreateThread ().  Currently, only
//                  these two platforms are implemented.
//
//              -m: Measure the performance of Thread_Manager::spawn_n
//                  method.
//
//              -x: Test the baseline performance of ACE_Thread_Mutex.
//                  This really doesn't belong here
//
//              -a: Measure the performance of thread creation using
//                  ACE_OS::thr_create ().
//
//              -h: Use High Resolution Timer if supported by platform.
//
//              -e: Exec a program after fork ().  This option has no
//                  effect on NT.
//
//   = CREATION DATE
//      June 29, 1997
//
//   = AUTHOR
//      Nanbor Wang
//
// ============================================================================

// Process Creation profiling

#include "ace/OS_NS_unistd.h"
#include "ace/OS_main.h"
#include "ace/Get_Opt.h"
#include "ace/Process.h"
#include "ace/Profile_Timer.h"
#include "ace/Thread_Manager.h"

#define  ACE_STOP_SIGN  ACE_OS::sleep (0)

#define MAX_NO_ITERATION  10000
#if defined (ACE_WIN32)
#define SUBPROGRAM ACE_TEXT ("date.exe")
#else
#define SUBPROGRAM ACE_TEXT ("date")
#endif

size_t MULTIPLY_FACTOR = 10;
typedef double (*Profiler)(size_t);
static int do_exec_after_fork = 0;

/// do nothing thread function
extern "C" void *ace_empty (void*)
{
  return 0;
}

static double
prof_ace_process (size_t iteration)
{
  if (iteration != 0)
    {
      ACE_Process_Options popt;
      ACE_Process aProcess;

      popt.command_line (SUBPROGRAM);

      iteration *= MULTIPLY_FACTOR;

      if (do_exec_after_fork == 0)
        popt.creation_flags (ACE_Process_Options::NO_EXEC);

      ACE_Profile_Timer ptimer;
      ACE_Profile_Timer::ACE_Elapsed_Time et;
      double time = 0;
      pid_t result;

      for (size_t c = 0; c < iteration; c++)
        {
          ACE_STOP_SIGN;
          ptimer.start ();
          result = aProcess.spawn (popt);
          ptimer.stop ();

          if (result == -1)
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "process.spawn"), -1);
          else if (do_exec_after_fork == 0 && result == 0)
            ACE_OS::exit (0) ;
          else
            {
              ptimer.elapsed_time (et);
              time += et.real_time;
            }
        }

      return time / iteration;
    }
  else
    return -1.0;
}

static double
prof_fork (size_t iteration)
{
#if !defined (ACE_LACKS_EXEC)
  if (iteration != 0)
    {
      ACE_Profile_Timer ptimer;
      ACE_Profile_Timer::ACE_Elapsed_Time et;
      double time = 0;

      iteration *= MULTIPLY_FACTOR;

      for (size_t i = 0; i < iteration; i++)
        {
          ACE_STOP_SIGN;
          ptimer.start ();
          switch (ACE_OS::fork ())
            {
            case -1:
              ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "process.spawn"), -1);
              /* NOTREACHED */
            case 0:
              ACE_OS::exit (0);
              /* NOTREACHED */
              break;
            default:
              ptimer.stop ();
              ptimer.elapsed_time (et);
              time += et.real_time;
            }
        }
      return time / iteration;
    }
  else
    return -1.0;
#else
  ACE_UNUSED_ARG (iteration);
  ACE_ERROR_RETURN ((LM_ERROR, "fork () is not supported on this platform."), -1);
#endif
}

static double
prof_native_thread (size_t iteration)
{
#if defined (ACE_HAS_THREADS)  && (defined (ACE_HAS_WTHREADS) || defined (ACE_HAS_STHREADS))
  if (iteration != 0)
    {
      ACE_Profile_Timer ptimer;
      ACE_Profile_Timer::ACE_Elapsed_Time et;
      double time = 0;

      for (size_t i = 0; i < iteration; i++)
        {
          ACE_STOP_SIGN;
          ptimer.start ();
          for (size_t j = 0; j < MULTIPLY_FACTOR; j++)
            {
#if defined (ACE_HAS_WTHREADS)
              if (::CreateThread (0,
                                  0,
                                  LPTHREAD_START_ROUTINE (ace_empty),
                                  0,
                                  CREATE_SUSPENDED,
                                  0) == 0)
#elif defined (ACE_HAS_STHREADS)
                //FUZZ: disable check_for_lack_ACE_OS
                if (::thr_create (0,
                                  0,
                                  &ace_empty,
                                  0,
                                  THR_SUSPENDED,
                                  0) != 0)
#endif
                  ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CreateThread"), -1);
                //FUZZ: enable check_for_lack_ACE_OS
            }
          ptimer.stop ();
          ptimer.elapsed_time (et);
          time += et.real_time;
        }
      iteration *= MULTIPLY_FACTOR;
      return time / iteration;
    }
  else
    return -1.0;
#else
  ACE_UNUSED_ARG (iteration);
  ACE_ERROR_RETURN ((LM_ERROR, "Testing of native threads is not supported on this platform.\n"), -1);
#endif
}

static double
prof_ace_os_thread (size_t iteration)
{
#if defined (ACE_HAS_THREADS)
  if (iteration != 0)
    {
      ACE_Profile_Timer ptimer;
      ACE_Profile_Timer::ACE_Elapsed_Time et;
      double time = 0;

      for (size_t i = 0; i < iteration; i++)
        {
          ACE_STOP_SIGN;
          ptimer.start ();

          for (size_t j = 0; j < MULTIPLY_FACTOR; j++)
            if (ACE_OS::thr_create ((ACE_THR_FUNC) ace_empty,
                                    0,
                                    THR_SUSPENDED,
                                    0) == -1)
              ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CreateThread"), -1);

          ptimer.stop ();
          ptimer.elapsed_time (et);
          time += et.real_time;
        }
      iteration *= MULTIPLY_FACTOR;
      return time / iteration;
    }
  else
    return -1.0;
#else
  ACE_UNUSED_ARG (iteration);
  ACE_ERROR_RETURN ((LM_ERROR, "Threads are not supported on this platform.\n"), -1);
#endif
}

static double
prof_tm_thread (size_t iteration)
{
#if defined (ACE_HAS_THREADS)
  if (iteration != 0)
    {
      ACE_Profile_Timer ptimer;
      ACE_Profile_Timer::ACE_Elapsed_Time et;
      double time = 0;

      for (size_t i = 0; i < iteration; i++)
        {
          ACE_STOP_SIGN;
          ptimer.start ();

          if (ACE_Thread_Manager::instance ()->spawn_n (MULTIPLY_FACTOR,
                                                        (ACE_THR_FUNC) ace_empty,
                                                        0,
                                                        THR_SUSPENDED) == -1)
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CreateThread"), -1);

          ptimer.stop ();
          ptimer.elapsed_time (et);
          time += et.real_time;
        }
      iteration *= MULTIPLY_FACTOR;
      return time / iteration;
    }
  else
    return -1.0;
#else
  ACE_UNUSED_ARG (iteration);
  ACE_ERROR_RETURN ((LM_ERROR, "Threads are not supported on this platform."), -1);
#endif
}

static double
prof_mutex_base (size_t iteration)
{
#if defined (ACE_HAS_THREADS)
  ACE_Thread_Mutex plain;
  if (iteration != 0)
    {
      ACE_Profile_Timer ptimer;
      ACE_Profile_Timer::ACE_Elapsed_Time et;
      double time = 0;

      for (size_t i = 0; i < iteration; i++)
        {
          ACE_STOP_SIGN;
          ptimer.start ();

          for (size_t j=0; j < MULTIPLY_FACTOR; j++)
            {
              plain.acquire ();
              plain.release ();
            }

          ptimer.stop ();
          ptimer.elapsed_time (et);
          time += et.real_time;
        }
      iteration *= MULTIPLY_FACTOR;
      return time / iteration;
    }
  else
    return -1.0;
#else
  ACE_UNUSED_ARG (iteration);
  ACE_ERROR_RETURN ((LM_ERROR, "Threads are not supported on this platform."), -1);
#endif
}

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("n:l:pftahmxe"));
  int c;
  size_t iteration = 10;
  Profiler profiler = 0;
  const char *profile_name = 0 ;

  while ((c=get_opt ()) != -1)
    {
      switch (c)
        {
        case 'n':
          iteration = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'l':
          MULTIPLY_FACTOR = static_cast<size_t> (ACE_OS::atoi (get_opt.opt_arg ()));
          break;
        case 'p':                       // test ACE_Process.spawn ()
          profiler = prof_ace_process;
          profile_name = "ACE_Process.spawn ()";
          break;
        case 'f':                       // test fork ()
          profiler = prof_fork;
          profile_name = "fork ()";
          break;
        case 't':                       // test native thread creation
          profiler = prof_native_thread;
          profile_name = "native threads";
          break;
        case 'a':                       // test ACE_OS::thr_create
          profiler = prof_ace_os_thread;
          profile_name = "ACE_OS::thr_create ()";
          break;
        case 'm':
          profiler = prof_tm_thread;
          profile_name = "ACE_Thread_Manager::spawn_n ()";
          break;
        case 'x':
          profiler = prof_mutex_base;
          profile_name = "ACE_Thread_Mutex Baseline";
          break;
        case 'h':                       // use high resolution timer
          ACE_High_Res_Timer::get_env_global_scale_factor ();
          break;
        case 'e':
          do_exec_after_fork = 1;
          break;
        default:
          break;
        }
    }

  if (profiler == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Usage: childbirth_time {-p|-f|-t|-a|-m|-x} [-n ###] [-L ###] [-h] [-e]\n"), 1);
  else
    {
      double time = profiler (iteration);
      if (time > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Average performance of %d iterations of %s: %.0f usec\n",
                    iteration * MULTIPLY_FACTOR, profile_name, time * 1e6));
    }
  return 0;
}
