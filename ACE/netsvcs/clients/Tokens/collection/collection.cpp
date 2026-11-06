//=============================================================================
/**
 *  @file    collection.cpp
 *
 *   Shows how applications can use the ACE_Token_Collection
 *   utility.  This example creates three collections and spawns a
 *   thread to operate on each.  The threads use the collective
 *   acquire, renew, and release features of ACE_Token_Collection.
 *
 *  @author Tim Harrison
 */
//=============================================================================
#include "ace/Get_Opt.h"
#include "ace/Local_Tokens.h"
#include "ace/Token_Collection.h"
#include "ace/Remote_Tokens.h"
#include "ace/Thread_Manager.h"
#include "ace/Service_Config.h"

#include <memory>

#if defined (ACE_HAS_THREADS) && defined (ACE_HAS_THREADS_LIBRARY)

struct Options
{
  ACE_TCHAR const *server_host {ACE_DEFAULT_SERVER_HOST};
  int server_port {ACE_DEFAULT_SERVER_PORT};
  int iterations {50};
  bool debug {};
  bool remote {};
};

struct Thread_Options
{
  ACE_Token_Proxy *collection {};
  int iterations {};
};

static ACE_THR_FUNC_RETURN
run_thread (void *vp)
{
  Thread_Options const * const options = static_cast<Thread_Options const *> (vp);
  ACE_Token_Proxy * const collection = options->collection;

  for (int iteration = 0; iteration < options->iterations; ++iteration)
    {
      if (collection->acquire () == -1)
        {
          if (ACE_OS::last_error () == EDEADLK)
            {
              ACE_DEBUG ((LM_DEBUG, "deadlock detected in acquire"));
              continue;
            }
          ACE_ERROR ((LM_ERROR, "(%t) %p acquire failed\n", "run_thread"));
          return ACE_THR_FUNC_RETURN_NULL;
        }

      ACE_DEBUG ((LM_DEBUG, "(%t) %s acquired.\n", collection->name ()));

      bool renew_error {};
      if (collection->renew () == -1)
        {
          if (ACE_OS::last_error () == EDEADLK)
            ACE_DEBUG ((LM_DEBUG, "deadlock detected"));
          else
            {
              ACE_ERROR ((LM_ERROR, "(%t) %p renew failed\n", "run_thread"));
              renew_error = true;
            }
        }
      else
        ACE_DEBUG ((LM_DEBUG, "(%t) %s renewed.\n", collection->name ()));

      if (collection->release () == -1)
        {
          ACE_ERROR ((LM_ERROR, "(%t) %p release failed\n", "run_thread"));
          return ACE_THR_FUNC_RETURN_NULL;
        }

      ACE_DEBUG ((LM_DEBUG, "(%t) %s released.\n", collection->name ()));
      if (renew_error)
        return ACE_THR_FUNC_RETURN_NULL;
    }

  ACE_DEBUG ((LM_DEBUG, "(%t) thread exiting.\n"));
  return ACE_THR_FUNC_RETURN_NULL;
}

static int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_LOG_MSG->open (argv[0], ACE_Log_Msg::STDERR); // | ACE_Log_Msg::VERBOSE);

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("un:dp:h:"), 1);

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
        case 'd':
          options.debug = true;
          break;
        case 'n':
          options.iterations = ACE_OS::atoi (get_opt.opt_arg ());
          break;
        case 'u':
          // usage same as unknown.
          ACE_FALLTHROUGH;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%n:\n"
                             "[-h <remote host>]\n"
                             "[-p <remote port>]\n"
                             "[-n <iterations>]\n"
                             "[-d debug]\n", 1), -1);
          /* NOTREACHED */
        }
    }

  if (options.iterations < 0)
    ACE_ERROR_RETURN ((LM_ERROR, "iterations cannot be negative\n"), -1);
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Options options;
  if (parse_args (argc, argv, options) == -1)
    return -1;

  std::unique_ptr<ACE_Token_Proxy> A;  // Mutex *A*.
  std::unique_ptr<ACE_Token_Proxy> B;  // Mutex *B*.
  std::unique_ptr<ACE_Token_Proxy> R;  // *R*eader Lock.
  std::unique_ptr<ACE_Token_Proxy> W;  // *W*riter Lock.

  // Depending on the command line arguments, we will create local or
  // remote tokens.  The names of the tokens are not important as long
  // as they are unique.
  if (options.remote)
    {
      ACE_Remote_Mutex::set_server_address (
        ACE_INET_Addr (options.server_port, options.server_host));
      A = std::make_unique<ACE_Remote_Mutex> (ACE_TEXT ("R Mutex A"), 0, options.debug);
      B = std::make_unique<ACE_Remote_Mutex> (ACE_TEXT ("R Mutex B"), 0, options.debug);
      R = std::make_unique<ACE_Remote_RLock> (ACE_TEXT ("R Reader Lock"), 0, options.debug);
      W = std::make_unique<ACE_Remote_WLock> (ACE_TEXT ("R Writer Lock"), 0, options.debug);
    }
  else
    {
      A = std::make_unique<ACE_Local_Mutex> (ACE_TEXT ("L Mutex A"), 0, options.debug);
      B = std::make_unique<ACE_Local_Mutex> (ACE_TEXT ("L Mutex B"), 0, options.debug);
      R = std::make_unique<ACE_Local_RLock> (ACE_TEXT ("L Reader Lock"), 0, options.debug);
      W = std::make_unique<ACE_Local_WLock> (ACE_TEXT ("L Writer Lock"), 0, options.debug);
    }

  // These collections will be treated as Tokens by the threads.
  ACE_Token_Collection collectionAR (options.debug, ACE_TEXT ("A and Reader"));
  ACE_Token_Collection collectionAW (options.debug, ACE_TEXT ("A and Writer"));
  ACE_Token_Collection collectionBR (options.debug, ACE_TEXT ("B and Reader"));

  // AR and BR can run concurrently.  Neither AR or BR can run when AW
  // is running.
  collectionAR.insert (*A);
  collectionAR.insert (*R);

  collectionAW.insert (*A);
  collectionAW.insert (*W);

  collectionBR.insert (*B);
  collectionBR.insert (*R);

  Thread_Options thread_options_ar {&collectionAR, options.iterations};
  Thread_Options thread_options_aw {&collectionAW, options.iterations};
  Thread_Options thread_options_br {&collectionBR, options.iterations};

  // Spawn off three threads.
  ACE_Thread_Manager *mgr = ACE_Thread_Manager::instance ();

  if (mgr->spawn (run_thread, &thread_options_ar, THR_BOUND | THR_SUSPENDED) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "spawn 1 failed"), -1);

  if (mgr->spawn (run_thread, &thread_options_aw, THR_BOUND | THR_SUSPENDED) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "spawn 2 failed"), -1);

  if (mgr->spawn (run_thread, &thread_options_br, THR_BOUND | THR_SUSPENDED) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "spawn 3 failed"), -1);

#if ! defined (ACE_HAS_PTHREADS)
  if (mgr->resume_all () == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "resume failed"), -1);
#endif

  // Wait for all threads to exit.
  mgr->wait ();

  return 0;
}

#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR_RETURN ((LM_ERROR,
                     "threads not supported on this platform\n"), -1);
}
#endif /* ACE_HAS_THREADS && ACE_HAS_TOKENS_LIBRARY */
