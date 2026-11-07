//=============================================================================
/**
 *  @file    rw_locks.cpp
 *
 *  test_rw_locks shows how to use ACE_Local_RLock, ACE_Local_WLock,
 *  ACE_Remote_RLock, and ACE_Remote_WLock.
 *
 *  @author Tim Harrison
 */
//=============================================================================

#include "ace/Get_Opt.h"
#include "ace/Local_Tokens.h"
#include "ace/Remote_Tokens.h"
#include "ace/Thread_Manager.h"
#include "ace/Token_Invariants.h"

#include <memory>

#if defined (ACE_HAS_THREADS) && defined (ACE_HAS_THREADS_LIBRARY)

using ACE_TOKEN_INVARIANTS = ACE_Token_Invariant_Manager;

struct Options
{
  ACE_TCHAR const *server_host {ACE_DEFAULT_SERVER_HOST};
  int server_port {ACE_DEFAULT_SERVER_PORT};
  int threads {2};
  int iterations {50};
  int reads {4};
  bool ignore_deadlock {};
  bool debug {};
  bool remote {};
  bool write_sleep {};
  bool renew {};
};

struct Thread_Options
{
  ACE_Token_Proxy *read_lock {};
  ACE_Token_Proxy *write_lock {};
  int iterations {};
  int reads {};
  bool write_sleep {};
  bool renew {};
};

static ACE_THR_FUNC_RETURN
run_thread (void *argument)
{
  Thread_Options const * const options = static_cast<Thread_Options const *> (argument);
  for (int iteration = 0; iteration < options->iterations; ++iteration)
    {
      int y {};
      for (; y < options->reads; ++y)
        {
          if (options->read_lock->acquire () == -1)
            {
              if (ACE_Log_Msg::instance ()->errnum () == EDEADLK)
                {
                  ACE_DEBUG ((LM_DEBUG, "rlock deadlock detected\n"));
                  goto READ_DEADLOCK;
                }
              else
                return ACE_THR_FUNC_RETURN_NULL;
            }

          if (ACE_TOKEN_INVARIANTS::instance ()->acquired (options->read_lock) == 0)
            ACE_ERROR_RETURN ((LM_ERROR, "reader acquire violated invariant.\n"), 0);

          ACE_DEBUG ((LM_DEBUG, "(%t) rlock acquired.\n"));
        }

      if (options->renew)
        {
          ACE_TOKEN_INVARIANTS::instance ()->releasing (options->read_lock);

          if (options->read_lock->renew () == -1)
            {
              if (ACE_Log_Msg::instance ()->errnum () == EDEADLK)
                {
                  ACE_DEBUG ((LM_DEBUG, "rlock deadlock detected during renew\n"));
                  goto READ_DEADLOCK;
                }
              else
                return ACE_THR_FUNC_RETURN_NULL;
            }

          ACE_DEBUG ((LM_DEBUG, "(%t) rlock renewed.\n"));

          if (ACE_TOKEN_INVARIANTS::instance ()->acquired (options->read_lock) == 0)
            ACE_ERROR_RETURN ((LM_ERROR, "reader renew violated invariant.\n"), 0);
        }

      READ_DEADLOCK:

      for (; y > 0; y--)
        {
          ACE_TOKEN_INVARIANTS::instance ()->releasing (options->read_lock);
          if (options->read_lock->release () == 0)
            ACE_DEBUG ((LM_DEBUG, "(%t) r-released.\n"));
        }

      if (options->write_lock->acquire () == -1)
        ACE_DEBUG ((LM_DEBUG, "wlock deadlock detected\n"));
      else
        {
          if (options->write_sleep)
            ACE_OS::sleep (1);
          ACE_DEBUG ((LM_DEBUG, "\t\t(%t) wlock acquired.\n"));

          if (ACE_TOKEN_INVARIANTS::instance ()->acquired (options->write_lock) == 0)
            ACE_ERROR_RETURN ((LM_ERROR, "writer acquire violated invariant.\n"), 0);

          if (options->renew)
            {
              ACE_TOKEN_INVARIANTS::instance ()->releasing (options->write_lock);

              if (options->write_lock->renew () == -1)
                {
                  if (ACE_Log_Msg::instance ()->errnum () == EDEADLK)
                    ACE_DEBUG ((LM_DEBUG, "wlock deadlock detected during renew\n"));
                  else
                    return ACE_THR_FUNC_RETURN_NULL;
                }

              ACE_DEBUG ((LM_DEBUG, "(%t) rlock renewed.\n"));

              if (ACE_TOKEN_INVARIANTS::instance ()->acquired (options->write_lock) == 0)
                ACE_ERROR_RETURN ((LM_ERROR, "writer renew violated invariant.\n"), 0);
            }

          ACE_TOKEN_INVARIANTS::instance ()->releasing (options->write_lock);

          if (options->write_lock->release () == 0)
            ACE_DEBUG ((LM_DEBUG, "\t\t(%t) w-released.\n"));
        }
    }

  ACE_DEBUG ((LM_DEBUG, "(%t) thread exiting.\n"));
  return ACE_THR_FUNC_RETURN_NULL;
}

static int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_LOG_MSG->open (argv[0], ACE_Log_Msg::STDERR); // | ACE_Log_Msg::VERBOSE);

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("t:iun:dr:sp:h:R"), 1);

  for (int option {}; (option = get_opt ()) != -1; )
    {
      switch (option)
        {
        case 'h':  // specify the host machine on which the server is running
          options.server_host = get_opt.opt_arg ();
          options.remote = true;
          break;
        case 'p':  // specify the port on which the server is running
          options.server_port = ACE_OS::atoi (get_opt.opt_arg ());
          options.remote = true;
          break;
        case 't':
          options.threads = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'R':
          options.renew = true;
          break;
        case 'r':
          options.reads = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'd':
          options.debug = true;
          break;
        case 's':
          options.write_sleep = true;
          break;
        case 'n':
          options.iterations = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'i':
          options.ignore_deadlock = true;
          break;
        case 'u':
          // usage same as unknown.
          ACE_FALLTHROUGH;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%n:\n"
                             "[-h <remote host>]\n"
                             "[-p <remote port>]\n"
                             "[-i ignore deadlock]\n"
                             "[-n <iterations>]\n"
                             "[-R perform renews]\n"
                             "[-r <reads>]\n"
                             "[-d debug]\n"
                             "[-s sleep during writes]\n"
                             "[-t <threads>\n", 1), -1);
        }
    }

  if (options.threads <= 0 || options.iterations < 0 || options.reads < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "threads must be positive; iterations and reads cannot be negative\n"),
                      -1);
  return 0;
}

#if defined (ACE_HAS_PTHREADS)
#define SUSPEND 0
#else
#define SUSPEND THR_SUSPENDED
#endif

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Options options;
  if (parse_args (argc, argv, options) == -1)
    return -1;

  std::unique_ptr<ACE_Token_Proxy> read_lock;
  std::unique_ptr<ACE_Token_Proxy> write_lock;
  if (options.remote)
    {
      ACE_Remote_Mutex::set_server_address (
        ACE_INET_Addr (options.server_port, options.server_host));
      read_lock = std::make_unique<ACE_Remote_RLock> (ACE_TEXT ("THE_TOKEN"),
                                                     options.ignore_deadlock,
                                                     options.debug);
      write_lock = std::make_unique<ACE_Remote_WLock> (ACE_TEXT ("THE_TOKEN"),
                                                      options.ignore_deadlock,
                                                      options.debug);
    }
  else
    {
      read_lock = std::make_unique<ACE_Local_RLock> (ACE_TEXT ("THE_TOKEN"),
                                                    options.ignore_deadlock,
                                                    options.debug);
      write_lock = std::make_unique<ACE_Local_WLock> (ACE_TEXT ("THE_TOKEN"),
                                                     options.ignore_deadlock,
                                                     options.debug);
    }

  Thread_Options thread_options {
    read_lock.get (),
    write_lock.get (),
    options.iterations,
    options.reads,
    options.write_sleep,
    options.renew
  };

  ACE_Thread_Manager mgr;

  if (mgr.spawn_n (options.threads, run_thread,
                   &thread_options,
                   THR_BOUND | SUSPEND) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "spawn failed"), -1);

#if ! defined (ACE_HAS_PTHREADS)
  if (mgr.resume_all () == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "resume failed"), -1);
#endif

  mgr.wait ();

  return 0;
}

#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "threads not supported on this platform\n"), -1);
}
#endif /* ACE_HAS_THREADS */
