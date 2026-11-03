
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

static std::unique_ptr<ACE_Token_Proxy> global_rlock;
static std::unique_ptr<ACE_Token_Proxy> global_wlock;

static ACE_TCHAR const *server_host = ACE_DEFAULT_SERVER_HOST;
static int server_port = ACE_DEFAULT_SERVER_PORT;
static bool ignore_deadlock = false;
static int threads = 2;
static int iterations = 50;
static bool debug = false;
static bool remote = false;
static int reads = 4;
static bool write_sleep = false;
static bool renew = false;

static ACE_THR_FUNC_RETURN
run_thread (void *)
{
  for (int x = 0; x < iterations; x++)
    {
      int y = 0;
      for (; y < reads; y++)
        {
          if (global_rlock->acquire () == -1)
            {
              if (ACE_Log_Msg::instance ()->errnum () == EDEADLK)
                {
                  ACE_DEBUG ((LM_DEBUG, "rlock deadlock detected\n"));
                  goto READ_DEADLOCK;
                }
              else
                return ACE_THR_FUNC_RETURN_NULL;
            }

          if (ACE_TOKEN_INVARIANTS::instance ()->acquired (global_rlock.get ()) == 0)
            ACE_ERROR_RETURN ((LM_ERROR, "reader acquire violated invariant.\n"), 0);

          ACE_DEBUG ((LM_DEBUG, "(%t) rlock acquired.\n"));
        }

      if (renew)
        {
          ACE_TOKEN_INVARIANTS::instance ()->releasing (global_rlock.get ());

          if (global_rlock->renew () == -1)
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

          if (ACE_TOKEN_INVARIANTS::instance ()->acquired (global_rlock.get ()) == 0)
            ACE_ERROR_RETURN ((LM_ERROR, "reader renew violated invariant.\n"), 0);
        }

      READ_DEADLOCK:

      for (; y > 0; y--)
        {
          ACE_TOKEN_INVARIANTS::instance ()->releasing (global_rlock.get ());
          if (global_rlock->release () == 0)
            ACE_DEBUG ((LM_DEBUG, "(%t) r-released.\n"));
        }

      if (global_wlock->acquire () == -1)
        ACE_DEBUG ((LM_DEBUG, "wlock deadlock detected\n"));
      else
        {
          if (write_sleep)
            ACE_OS::sleep (1);
          ACE_DEBUG ((LM_DEBUG, "\t\t(%t) wlock acquired.\n"));

          if (ACE_TOKEN_INVARIANTS::instance ()->acquired (global_wlock.get ()) == 0)
            ACE_ERROR_RETURN ((LM_ERROR, "writer acquire violated invariant.\n"), 0);

          if (renew)
            {
              ACE_TOKEN_INVARIANTS::instance ()->releasing (global_wlock.get ());

              if (global_wlock->renew () == -1)
                {
                  if (ACE_Log_Msg::instance ()->errnum () == EDEADLK)
                  {
                    ACE_DEBUG ((LM_DEBUG, "wlock deadlock detected during renew\n"));
                  }
                  else
                    return ACE_THR_FUNC_RETURN_NULL;
                }

              ACE_DEBUG ((LM_DEBUG, "(%t) rlock renewed.\n"));

              if (ACE_TOKEN_INVARIANTS::instance ()->acquired (global_wlock.get ()) == 0)
                ACE_ERROR_RETURN ((LM_ERROR, "writer renew violated invariant.\n"), 0);
            }

          ACE_TOKEN_INVARIANTS::instance ()->releasing (global_wlock.get ());

          if (global_wlock->release () == 0)
            ACE_DEBUG ((LM_DEBUG, "\t\t(%t) w-released.\n"));
        }
    }

  ACE_DEBUG ((LM_DEBUG, "(%t) thread exiting.\n"));
  return ACE_THR_FUNC_RETURN_NULL;
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_LOG_MSG->open (argv[0], ACE_Log_Msg::STDERR); // | ACE_Log_Msg::VERBOSE);

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("t:iun:dr:sp:h:R"), 1);

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
        {
        case 'h':  // specify the host machine on which the server is running
          server_host = get_opt.opt_arg ();
          remote = true;
          break;
        case 'p':  // specify the port on which the server is running
          server_port = ACE_OS::atoi (get_opt.opt_arg ());
          remote = true;
          break;
        case 't':
          threads = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'R':
          renew = true;
          break;
        case 'r':
          reads = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'd':
          debug = true;
          break;
        case 's':
          write_sleep = true;
          break;
        case 'n':
          iterations = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'i':
          ignore_deadlock = true;
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

  return 0;
}

#if defined (ACE_HAS_PTHREADS)
#define SUSPEND 0
#else
#define SUSPEND THR_SUSPENDED
#endif

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  if (parse_args (argc, argv) == -1)
    return -1;

  if (remote)
    {
      ACE_Remote_Mutex::set_server_address (ACE_INET_Addr (server_port, server_host));
      global_rlock = std::make_unique<ACE_Remote_RLock> (ACE_TEXT ("THE_TOKEN"),
                                                        ignore_deadlock, debug);
      global_wlock = std::make_unique<ACE_Remote_WLock> (ACE_TEXT ("THE_TOKEN"),
                                                        ignore_deadlock, debug);
    }
  else
    {
      global_rlock = std::make_unique<ACE_Local_RLock> (ACE_TEXT ("THE_TOKEN"),
                                                       ignore_deadlock, debug);
      global_wlock = std::make_unique<ACE_Local_WLock> (ACE_TEXT ("THE_TOKEN"),
                                                       ignore_deadlock, debug);
    }

  ACE_Thread_Manager mgr;

  if (mgr.spawn_n (threads, run_thread,
                   nullptr,
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
ACE_TMAIN(int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "threads not supported on this platform\n"), -1);
}
#endif /* ACE_HAS_THREADS */
