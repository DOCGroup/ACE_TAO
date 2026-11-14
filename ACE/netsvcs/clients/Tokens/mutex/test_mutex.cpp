
//=============================================================================
/**
 *  @file    test_mutex.cpp
 *
 *  @author Tim Harrison
 */
//=============================================================================

#include "ace/Get_Opt.h"
#include "ace/Local_Tokens.h"
#include "ace/Remote_Tokens.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include <memory>

#if defined (ACE_HAS_THREADS) && defined (ACE_HAS_THREADS_LIBRARY)

struct Test_Options
{
  std::unique_ptr<ACE_Token_Proxy> mutex {};
  bool remote_mutexes {};
  ACE_TCHAR const *server_host {ACE_DEFAULT_SERVER_HOST};
  int server_port {ACE_DEFAULT_SERVER_PORT};
  int iterations {100};
  int spawn_count {2};
};

static void *
run_test (void *arg)
{
  Test_Options const *options = static_cast<Test_Options const *> (arg);
  int count = options->iterations;

  while (count--)
    {
      if (options->mutex->acquire () == -1)
        {
          ACE_ERROR ((LM_ERROR, "(%t) %p acquire failed\n", "test_mutex"));
          return reinterpret_cast<void *> (-1);
        }

      if (options->mutex->renew () == -1)
        {
          ACE_ERROR ((LM_ERROR, "(%t) %p renew failed\n", "test_mutex"));
          return reinterpret_cast<void *> (-1);
        }

      if (options->mutex->release () == -1)
        {
          ACE_ERROR ((LM_ERROR, "(%t) %p release failed\n", "test_mutex"));
          return reinterpret_cast<void *> (-1);
        }
    }

  return ACE_THR_FUNC_RETURN_NULL;
}

static int
parse_args (int argc, ACE_TCHAR *argv[], Test_Options &options)
{
  ACE_LOG_MSG->open (argv[0]);

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("t:uh:p:n:"), 1);

  for (int c {}; (c = get_opt ()) != -1; )
    {
      switch (c)
        {
        case 't':
          options.spawn_count = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'h':  // specify the host machine on which the server is running
          options.server_host = get_opt.opt_arg ();
          options.remote_mutexes = true;
          break;
        case 'p':  // specify the port on which the server is running
          options.server_port = ACE_OS::atoi (get_opt.opt_arg ());
          options.remote_mutexes = true;
          break;
        case 'n':
          options.iterations = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'u':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%n:\n"
                             "[-h <remote host>]\n"
                             "[-p <remote port>]\n"
                             "[-n <iterations>]\n"
                             "[-t <threads>]\n"),
                            -1);
        /* NOTREACHED */
        }
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Thread_Manager thread_mgr;
  Test_Options options {};

  if (parse_args (argc, argv, options) == -1)
    return -1;

  if (options.remote_mutexes)
    {
      ACE_Remote_Mutex::set_server_address (
        ACE_INET_Addr (options.server_port, options.server_host));
      options.mutex = std::make_unique<ACE_Remote_Mutex> ("Remote TOKEN", 0, 1);
    }
  else
    {
      options.mutex = std::make_unique<ACE_Local_Mutex> ("Local TOKEN", 0, 1);
    }

  if (thread_mgr.spawn_n (options.spawn_count,
                          ACE_THR_FUNC (run_test),
                          &options,
                          THR_BOUND) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "spawn"), -1);

  thread_mgr.wait ();

  return 0;
}
#else
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR, "you must have threads to run this test program\n"), -1);
}
#endif /* ACE_HAS_THREADS */
