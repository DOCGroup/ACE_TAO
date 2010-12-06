// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Tokens_Test.cpp
//
// = DESCRIPTION
//     This application tests the ACE Token library including local
//     and remote readers/writer and mutex locks, and token
//     collections.  This is accomplished with the ACE Token Invariant
//     utilities that allow and application to check that
//     readers/writer and mutex lock invariants are always satisfied.
//     Throughout this test, ACE_ASSERTs are used in conjunction with
//     Token Invariant operations, so that errors are reported using
//     the ACE tests convention.  This application performs a local
//     test and then fork_execs a token server and performs the same
//     test remotely.
//
// = AUTHOR
//    Tim Harrison <harrison@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Process.h"
#include "ace/Get_Opt.h"
#include "ace/Local_Tokens.h"
#include "ace/Token_Collection.h"
#include "ace/Remote_Tokens.h"
#include "ace/Thread_Manager.h"
#include "ace/Token_Invariants.h"
#include "ace/Barrier.h"
#include "ace/OS_NS_unistd.h"



#if defined (ACE_HAS_THREADS) && defined (ACE_HAS_TOKENS_LIBRARY)

typedef ACE_Token_Invariant_Manager TOKEN_INVARIANTS;

static const ACE_TCHAR *server_host = ACE_DEFAULT_SERVER_HOST;
static const int server_port = 23456;

// Synchronize starts of threads, so that they all start before one
// has a chance to finish and clean up the TSS objects.  To avoid
// creating a static object, it is dynamically allocated, before
// spawning any threads.
static ACE_Barrier *thread_start;

struct Test_Params
{
public:
  ACE_Token_Proxy *token1_;
  ACE_Token_Proxy *token2_;
  const ACE_TCHAR *collection_name_;
};

static void *
run_thread (void *vp)
{
  Test_Params *tp = (Test_Params *) vp;
  ACE_Token_Collection collection (1, tp->collection_name_);
  collection.insert (*(tp->token1_));
  collection.insert (*(tp->token2_));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) new thread.\n")));
  thread_start->wait ();

  int count = 50;
  while (count--)
    {
      if (collection.acquire () == -1)
        {
          if (ACE_OS::last_error () == EDEADLK)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("deadlock detected in acquire")));
              continue;
            }
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%t) %p acquire failed\n"),
                      ACE_TEXT ("run_thread")));
          return (void *) -1;
        }

      ACE_ASSERT ((TOKEN_INVARIANTS::instance ()->acquired (tp->token1_) == 1) ||
                  (TOKEN_INVARIANTS::instance ()->acquired (tp->token2_) == 1));

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) %s acquired.\n"),
                  collection.name ()));

      TOKEN_INVARIANTS::instance ()->releasing (tp->token1_);
      TOKEN_INVARIANTS::instance ()->releasing (tp->token2_);

      if (collection.renew () == -1)
        {
          if (ACE_OS::last_error () == EDEADLK)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("deadlock detected")));
              goto deadlock;
            }
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p renew failed\n"),
                      ACE_TEXT ("run_thread")));
          return (void *) -1;
        }

      ACE_ASSERT (TOKEN_INVARIANTS::instance ()->acquired (tp->token1_) == 1 ||
                  TOKEN_INVARIANTS::instance ()->acquired (tp->token2_) == 1);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) %s renewed.\n"),
                  collection.name ()));

    deadlock:

      TOKEN_INVARIANTS::instance ()->releasing (tp->token1_);
      TOKEN_INVARIANTS::instance ()->releasing (tp->token2_);

      if (collection.release () == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%t) %p release failed\n"),
                      ACE_TEXT ("run_thread")));
          return (void *) -1;
        }

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) %s released.\n"),
                  collection.name ()));
    }


  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) thread finished.\n")));

  return 0;
}

static int
run_test (ACE_Token_Proxy *A,
          ACE_Token_Proxy *B,
          ACE_Token_Proxy *R,
          ACE_Token_Proxy *W)
{
  // Parameters to be passed to the threads.
  Test_Params tp1, tp2, tp3;

  // tp1 and tp2 can run concurrently.  Neither tp1 or tp3 can run
  // when tp2 is running.
  tp1.collection_name_ = ACE_TEXT ("A and Reader");
  tp1.token1_ = A;
  tp1.token2_ = R;

  tp2.collection_name_ = ACE_TEXT ("A and Writer");
  tp2.token1_ = A;
  tp2.token2_ = W;

  tp3.collection_name_ = ACE_TEXT ("B and Reader");
  tp3.token1_ = B;
  tp3.token2_ = R;

  // Spawn off three threads.
  ACE_Thread_Manager *mgr = ACE_Thread_Manager::instance ();

  if (mgr->spawn (ACE_THR_FUNC (run_thread),
                 (void *) &tp1,
                  THR_BOUND) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("spawn 1 failed")),
                      -1);

  if (mgr->spawn (ACE_THR_FUNC (run_thread),
                 (void *) &tp2,
                  THR_BOUND) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("spawn 2 failed")),
                      -1);

  if (mgr->spawn (ACE_THR_FUNC (run_thread),
                 (void *) &tp3,
                  THR_BOUND) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("spawn 3 failed")), -1);

  // Wait for all threads to exit.
  mgr->wait ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Test finished.\n")));
  return 0;
}
#endif /* ACE_HAS_THREADS && ACE_HAS_TOKENS_LIBRARY */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Tokens_Test"));
#if defined (ACE_HAS_THREADS)
#if defined (ACE_HAS_TOKENS_LIBRARY)
  ACE_Token_Proxy *A = 0, *B = 0, *R = 0, *W = 0;

  ACE_NEW_RETURN (A,
                  ACE_Local_Mutex (ACE_TEXT ("L Mutex A"), 0, 0),
                  -1);
  ACE_NEW_RETURN (B,
                  ACE_Local_Mutex (ACE_TEXT ("L Mutex B"), 0, 0),
                  -1);
  ACE_NEW_RETURN (R,
                  ACE_Local_RLock (ACE_TEXT ("L Reader Lock"), 0, 0),
                  -1);
  ACE_NEW_RETURN (W,
                  ACE_Local_WLock (ACE_TEXT ("L Writer Lock"), 0, 0),
                  -1);
  ACE_NEW_RETURN (thread_start,
                  ACE_Barrier (3),
                  -1);

  run_test (A, B, R, W);

  const ACE_TCHAR *token_exe =
    ACE_TEXT ("..") ACE_DIRECTORY_SEPARATOR_STR
    ACE_TEXT ("netsvcs") ACE_DIRECTORY_SEPARATOR_STR
    ACE_TEXT ("servers") ACE_DIRECTORY_SEPARATOR_STR
    ACE_TEXT ("main") ACE_PLATFORM_EXE_SUFFIX;

  int status = ACE_OS::access (token_exe, F_OK);
  if (status == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                token_exe));
  else
    {
      const ACE_TCHAR *cl =
        ACE_TEXT ("..") ACE_DIRECTORY_SEPARATOR_STR
        ACE_TEXT ("netsvcs") ACE_DIRECTORY_SEPARATOR_STR
        ACE_TEXT ("servers") ACE_DIRECTORY_SEPARATOR_STR
        ACE_TEXT ("main") ACE_PLATFORM_EXE_SUFFIX
        ACE_TEXT (" -f ") ACE_PLATFORM
        ACE_TEXT ("tokens.conf");

      ACE_Process_Options options;
      options.command_line (cl);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Forking Token Service.\n")));

      // Start up the token server for the remote test.
      ACE_Process new_process;
      if (new_process.spawn (options) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%n; %p (%s), will not run remote test.\n"),
                    ACE_TEXT ("Server fork failed"),
                    cl));
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Server forked with pid = %d.\n"),
                      new_process.getpid ()));

          // Wait for the server to start.
          ACE_OS::sleep (3);

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Using Token Server on %s at port %d.\n"),
                      server_host,
                      server_port));
          ACE_Remote_Mutex::set_server_address (ACE_INET_Addr (server_port,
                                                               server_host));

          delete A;
          delete B;
          delete R;
          delete W;

          ACE_NEW_RETURN (A,
                          ACE_Remote_Mutex (ACE_TEXT ("R Mutex A"), 0, 1),
                          -1);
          ACE_NEW_RETURN (B,
                          ACE_Remote_Mutex (ACE_TEXT ("R Mutex B"), 0, 1),
                          -1);
          ACE_NEW_RETURN (R,
                          ACE_Remote_RLock (ACE_TEXT ("R Reader Lock"), 0, 1),
                          -1);
          ACE_NEW_RETURN (W,
                          ACE_Remote_WLock (ACE_TEXT ("R Writer Lock"), 0, 1),
                          -1);

          run_test (A, B, R, W);

          // Wait for the server to finish.
          ACE_OS::sleep (3);

          // Kill the token server.
          if (new_process.terminate () == -1)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Kill failed.\n")),
                              -1);
        }

      delete thread_start;
      thread_start = 0;
      delete A;
      delete B;
      delete R;
      delete W;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) main thread exiting.\n")));
#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("ACE must be compiled with -DACE_HAS_TOKENS_LIBRARY to run this test\n")));
#endif /* ACE_HAS_TOKENS_LIBRARY */
#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
