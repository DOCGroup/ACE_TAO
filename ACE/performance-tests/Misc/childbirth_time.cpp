//=============================================================================
/**
 *  @file    childbirth_time.cpp
 *
 * This program is used to measure various child-creation mechanisms
 * on various platforms.  By default, the program measure the time
 * to 'fork' a new process using ACE_Process.spawn ().  Other tests
 * are possible as described below.   James Hu provides the idea to
 * batch measuring threads creation.
 *
 * Usage: childbirth_time [-n ###] [-l ###] [-p|-f|-t|-a|-m|-x] [-h] [-e]
 *
 *        -n ###: Specify number of iteration in tens.  If this
 *                option is not specified, the default is
 *                multiply_factor * (100 iterations,) which is
 *                equivalent to -n 10.
 *
 *        -l ###: Specify the multiply factor.  Default is 10.
 *
 *           *-p: Measure the performance of forking a child process
 *                and exec an "empty" program.  This test uses
 *                ACE_Process.spawn ().  (Default)
 *
 *            -f: Measure the performance of native "fork" function
 *                call.  Notice that there is no equivalent NT
 *                function calls and this option is only available
 *                on UN*X platform.
 *
 *            -t: Measure the performance of native thread creation
 *                mechanisms.
 *                On NT, this is CreateThread ().  Currently, only
 *                these two platforms are implemented.
 *
 *            -m: Measure the performance of Thread_Manager::spawn_n
 *                method.
 *
 *            -x: Test the baseline performance of ACE_Thread_Mutex.
 *                This really doesn't belong here
 *
 *            -a: Measure the performance of thread creation using
 *                ACE_OS::thr_create ().
 *
 *            -h: Use High Resolution Timer if supported by platform.
 *
 *            -e: Exec a program after fork ().  This option has no
 *                effect on NT.
 *
 *  @author   Nanbor Wang
 */
//=============================================================================

// Process Creation profiling
#include "ace/OS_NS_unistd.h"
#include "ace/OS_main.h"
#include "ace/Get_Opt.h"
#include "ace/Process.h"
#include "ace/Profile_Timer.h"
#include "ace/Thread_Manager.h"

#define  ACE_STOP_SIGN  ACE_OS::sleep (0)

static constexpr size_t MAX_NO_ITERATION = 10000;
#if defined (ACE_WIN32)
#define SUBPROGRAM ACE_TEXT ("date.exe")
#else
#define SUBPROGRAM ACE_TEXT ("date")
#endif

static size_t multiply_factor {10};
using Profiler = double (*) (size_t);
static bool do_exec_after_fork {};

/// do nothing thread function
#if defined (ACE_HAS_WTHREADS)
DWORD WINAPI ace_empty (LPVOID)
{
  return 0;
}
#else
extern "C" void *ace_empty (void*)
{
  return 0;
}
#endif

static double
prof_ace_process (size_t iteration)
{
  if (iteration != 0)
    {
      ACE_Process_Options popt;
      ACE_Process aProcess;

      popt.command_line (SUBPROGRAM);

      iteration *= multiply_factor;

      if (!do_exec_after_fork)
        popt.creation_flags (ACE_Process_Options::NO_EXEC);

      ACE_Profile_Timer ptimer;
      ACE_Profile_Timer::ACE_Elapsed_Time et;
      double time {};
      pid_t result;

      for (size_t c = 0; c < iteration; ++c)
        {
          ACE_STOP_SIGN;
          ptimer.start ();
          result = aProcess.spawn (popt);
          ptimer.stop ();

          if (result == -1)
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "process.spawn"), -1);
          else if (!do_exec_after_fork && result == 0)
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
      double time {};

      iteration *= multiply_factor;

      for (size_t i = 0; i < iteration; ++i)
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
#if defined (ACE_HAS_THREADS) && defined (ACE_HAS_WTHREADS)
  if (iteration != 0)
    {
      ACE_Profile_Timer ptimer;
      ACE_Profile_Timer::ACE_Elapsed_Time et;
      double time {};

      for (size_t i = 0; i < iteration; ++i)
        {
          ACE_STOP_SIGN;
          ptimer.start ();
          for (size_t j = 0; j < multiply_factor; ++j)
            {
#if defined (ACE_HAS_WTHREADS)
              if (::CreateThread (nullptr,
                                  0,
                                  ace_empty,
                                  nullptr,
                                  CREATE_SUSPENDED,
                                  nullptr) == nullptr)
#endif
                  ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CreateThread"), -1);
                //FUZZ: enable check_for_lack_ACE_OS
            }
          ptimer.stop ();
          ptimer.elapsed_time (et);
          time += et.real_time;
        }
      iteration *= multiply_factor;
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
      double time {};

      for (size_t i = 0; i < iteration; ++i)
        {
          ACE_STOP_SIGN;
          ptimer.start ();

          for (size_t j = 0; j < multiply_factor; ++j)
            if (ACE_OS::thr_create (ace_empty,
                                    nullptr,
                                    THR_SUSPENDED,
                                    nullptr) == -1)
              ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CreateThread"), -1);

          ptimer.stop ();
          ptimer.elapsed_time (et);
          time += et.real_time;
        }
      iteration *= multiply_factor;
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
      double time {};

      for (size_t i = 0; i < iteration; ++i)
        {
          ACE_STOP_SIGN;
          ptimer.start ();

          if (ACE_Thread_Manager::instance ()->spawn_n (multiply_factor,
                                                        ace_empty,
                                                        nullptr,
                                                        THR_SUSPENDED) == -1)
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "CreateThread"), -1);

          ptimer.stop ();
          ptimer.elapsed_time (et);
          time += et.real_time;
        }
      iteration *= multiply_factor;
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
      double time {};

      for (size_t i = 0; i < iteration; ++i)
        {
          ACE_STOP_SIGN;
          ptimer.start ();

          for (size_t j = 0; j < multiply_factor; ++j)
            {
              plain.acquire ();
              plain.release ();
            }

          ptimer.stop ();
          ptimer.elapsed_time (et);
          time += et.real_time;
        }
      iteration *= multiply_factor;
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
  int c {};
  size_t iteration {10};
  Profiler profiler {};
  char const *profile_name {};

  while ((c = get_opt ()) != -1)
    {
      switch (c)
        {
        case 'n':
          {
            int const value = ACE_OS::atoi (get_opt.opt_arg ());
            if (value <= 0)
              ACE_ERROR_RETURN ((LM_ERROR, "The iteration count must be positive.\n"), 1);
            iteration = static_cast<size_t> (value);
          }
          break;
        case 'l':
          {
            int const value = ACE_OS::atoi (get_opt.opt_arg ());
            if (value <= 0)
              ACE_ERROR_RETURN ((LM_ERROR, "The multiply factor must be positive.\n"), 1);
            multiply_factor = static_cast<size_t> (value);
          }
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
          do_exec_after_fork = true;
          break;
        default:
          break;
        }
    }

  if (profiler == nullptr || profile_name == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR, "Usage: childbirth_time {-p|-f|-t|-a|-m|-x} [-n ###] [-l ###] [-h] [-e]\n"), 1);
  else if (iteration > MAX_NO_ITERATION / multiply_factor)
    ACE_ERROR_RETURN ((LM_ERROR, "Requested iteration count exceeds the supported maximum.\n"), 1);
  else
    {
      double time = profiler (iteration);
      if (time > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Average performance of %B iterations of %s: %.0f usec\n",
                    iteration * multiply_factor, profile_name, time * 1e6));
    }
  return 0;
}
