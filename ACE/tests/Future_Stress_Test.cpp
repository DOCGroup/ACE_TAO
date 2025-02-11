
//=============================================================================
/**
 *  @file    Future_Stress_Test.cpp
 *
 *  This example tests the ACE Future set() and get() operations in
 *  multithreaded environment and concurrent access.
 *
 *  Usage: Future_Stress_Test [-t <duration in seconds>]
 *            [-n <number of threads>]
 *
 *  @see https://github.com/DOCGroup/ACE_TAO/issues/2163
 *
 *  @author Andres Kruse <Frank.Hilliger@cs-sol.de>
 */
//=============================================================================

#include "test_config.h"
#include <ace/Time_Value.h>
#include <ace/Countdown_Time.h>
#include <ace/Future.h>
#include <ace/Get_Opt.h>

#include <random>


#if defined (ACE_HAS_THREADS)

struct Worker_Config
{
  int a;
  int b;
  int c;
  ACE_Future<int> result;
};

void* worker (void* args)
{
  Worker_Config* config = static_cast<Worker_Config*>(args);
  int r = config->a + config->b * config->c;
  config->result.set(r);

  return 0;
}

void* runner (void* args)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) runner start\n")));

  std::random_device              rd;
  std::mt19937                    gen(rd());
  std::uniform_int_distribution<> dis(1,1000000);
  ACE_Time_Value* duration = static_cast<ACE_Time_Value*>(args);
  ACE_Countdown_Time timer(duration);
  timer.start();
  uint64_t runNum = 0;
  do
  {
    if( ++runNum % 5000 == 0 )
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) runner iteration %u\n"), runNum));
    }
    ACE_Future<int> result;
    Worker_Config config;
    config.a = dis(gen);
    config.b = dis(gen);
    config.c = dis(gen);
    config.result = result;
    ACE_hthread_t thread_id;
    int expected_res = config.a+config.b*config.c;
    int actual_res = -1;
    if (ACE_Thread::spawn((ACE_THR_FUNC)worker,
                        static_cast<void*>(&config), THR_NEW_LWP | THR_JOINABLE, 0,
                        &thread_id) == -1)
    {
        ACE_ERROR ((LM_INFO,
              ACE_TEXT ("worker thread spawn failed\n")));
    }
    result.get(actual_res);
    if( actual_res != expected_res )
    {
      // hit the bug...
      ACE_ERROR ((LM_INFO,
          ACE_TEXT ("unexpected ACE_Future result\n")));
      abort();
    }
    ACE_THR_FUNC_RETURN status;
    ACE_Thread::join(thread_id, &status);
    timer.update();
  } while( *duration != ACE_Time_Value::zero );
   ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) runner done\n"), runNum));

   return 0;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Future_Stress_Test"));

  ACE_Time_Value duration(5);
  long n_threads = 5;

  ACE_Get_Opt getopt (argc, argv, ACE_TEXT ("t:n:"));
  bool valid = true;
  int c;
  while ((c = getopt ()) != -1 && valid)
  {
  //FUZZ: enable check_for_lack_ACE_OS
    switch (c)
    {
      case 't':
        duration.set(ACE_OS::atoi (getopt.opt_arg ()));
        break;
      case 'n':
        n_threads = ACE_OS::atoi (getopt.opt_arg ());
        break;
      default:
        ACE_ERROR ((LM_ERROR,
                    "Usage: Future_Stress_Test [-t <duration in seconds>]"
                    "\t[-n <number of threads>]\n"));
        valid = false;
        break;
    }
  }

  if (valid)
  {
    ACE_Thread_Manager::instance ()->spawn_n (n_threads,
      ACE_THR_FUNC (runner),
      static_cast<void*>(&duration),
      THR_NEW_LWP | THR_DETACHED);

    ACE_Thread_Manager::instance ()->wait ();
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) All threads finished, cleanup and exit\n")));
  }
  ACE_END_TEST;
  return 0;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
}
#endif /* ACE_HAS_THREADS */
