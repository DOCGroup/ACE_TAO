#include "ace/Get_Opt.h"
#include "ace/Local_Tokens.h"
#include "ace/Remote_Tokens.h"
#include "ace/Thread_Manager.h"

#include <memory>

#if defined (ACE_HAS_THREADS) && defined (ACE_HAS_THREADS_LIBRARY)

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
};

struct Thread_Options
{
  ACE_Token_Proxy *read_lock {};
  ACE_Token_Proxy *write_lock {};
  int iterations {};
  int reads {};
  bool write_sleep {};
};

static ACE_THR_FUNC_RETURN
run_thread (void *argument)
{
  Thread_Options const * const options = static_cast<Thread_Options const *> (argument);
  for (int iteration = 0; iteration < options->iterations; ++iteration)
    {
      int acquired_reads {};
      bool read_error {};
      for (; acquired_reads < options->reads; ++acquired_reads)
        {
          if (options->read_lock->acquire () == -1)
            {
              int const error = ACE_Log_Msg::instance ()->errnum ();
              if (error == EDEADLK)
                ACE_DEBUG ((LM_DEBUG, "rlock deadlock detected\n"));
              else
                {
                  ACE_ERROR ((LM_ERROR, "(%t) rlock acquire failed\n"));
                  read_error = true;
                }
              break;
            }

          ACE_DEBUG ((LM_DEBUG, "(%t) rlock acquired.\n"));
        }

      while (acquired_reads > 0)
        {
          if (options->read_lock->release () == -1)
            {
              ACE_ERROR ((LM_ERROR, "(%t) rlock release failed\n"));
              return ACE_THR_FUNC_RETURN_NULL;
            }
          --acquired_reads;
          ACE_DEBUG ((LM_DEBUG, "(%t) r-released.\n"));
        }
      if (read_error)
        return ACE_THR_FUNC_RETURN_NULL;

      if (options->write_lock->acquire () == -1)
        {
          if (ACE_Log_Msg::instance ()->errnum () == EDEADLK)
            ACE_DEBUG ((LM_DEBUG, "wlock deadlock detected\n"));
          else
            ACE_ERROR ((LM_ERROR, "(%t) wlock acquire failed\n"));
          continue;
        }
      if (options->write_sleep)
        ACE_OS::sleep (1);
      ACE_DEBUG ((LM_DEBUG, "\t\t(%t) wlock acquired.\n"));
      if (options->write_lock->release () == -1)
        {
          ACE_ERROR ((LM_ERROR, "(%t) wlock release failed\n"));
          return ACE_THR_FUNC_RETURN_NULL;
        }
      ACE_DEBUG ((LM_DEBUG, "\t\t(%t) w-released.\n"));

    }

  ACE_DEBUG ((LM_DEBUG, "(%t) thread exiting.\n"));
  return ACE_THR_FUNC_RETURN_NULL;
}

static int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_LOG_MSG->open (argv[0], ACE_Log_Msg::STDERR); // | ACE_Log_Msg::VERBOSE);

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("t:iun:drR:sp:h:"), 1);

  for (int option {}; (option = get_opt ()) != -1; )
    {
      switch (option)
        {
        case 'h':  // specify the host machine on which the server is running
          options.server_host = get_opt.opt_arg ();
          break;
        case 'p':  // specify the port on which the server is running
          options.server_port = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 't':
          options.threads = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'R':
          options.reads = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'd':
          options.debug = true;
          break;
        case 'r':
          options.remote = true;
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
                             "[-i ignore deadlock]\n"
                             "[-n <iterations>]\n"
                             "[-R <reads>]\n"
                             "[-r use remote locks]\n"
                             "[-d debug]\n"
                             "[-s sleep during writes]\n"
                             "[-t <threads>\n", 1), -1);
          break;
        }
    }

  if (options.threads <= 0 || options.iterations < 0 || options.reads < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "threads must be positive; iterations and reads cannot be negative\n"),
                      -1);
  return 0;
}

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
    options.write_sleep
  };

  ACE_Thread_Manager mgr;

  if (mgr.spawn_n (options.threads, run_thread,
                   &thread_options,
                   THR_BOUND | THR_SUSPENDED) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "spawn failed"), -1);

  if (mgr.resume_all () == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "resume failed"), -1);

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
