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
//    Tim Harrison
// 
// ============================================================================

#include "ace/OS.h"
#include "ace/Process.h"

#include "ace/Get_Opt.h"
#include "ace/Local_Tokens.h"
#include "ace/Token_Collection.h"
#include "ace/Remote_Tokens.h"
#include "ace/Thread_Manager.h"
#include "ace/Service_Config.h"
#include "ace/Token_Invariants.h"
#include "test_config.h"

#if defined (ACE_HAS_THREADS)

typedef ACE_Token_Invariant_Manager TOKEN_INVARIANTS;

static const char *server_host = "localhost";
static const int server_port = 23456;

struct Test_Params
{
public:
  ACE_Token_Proxy *token1_;
  ACE_Token_Proxy *token2_;
  const char *collection_name_;
};

static void *
run_thread (void *vp)
{
  ACE_Thread_Control tc (ACE_Service_Config::thr_mgr ());
  ACE_NEW_THREAD;
  Test_Params *tp = (Test_Params *) vp;
  ACE_Token_Collection collection (1, tp->collection_name_);
  collection.insert (*(tp->token1_));
  collection.insert (*(tp->token2_));

  ACE_DEBUG ((LM_DEBUG, "(%t) new thread.\n"));

  int count = 50;
  while (count--)
    {
      if (collection.acquire () == -1)
	{
	  if (ACE_OS::last_error () == EDEADLK)
	    {
	      ACE_DEBUG ((LM_DEBUG, "deadlock detected in acquire"));
	      continue;
	    }
	  ACE_ERROR ((LM_ERROR, "(%t) %p acquire failed\n","run_thread"));
	  return (void *) -1;
	}
      
      ACE_ASSERT ((TOKEN_INVARIANTS::instance ()->acquired (tp->token1_) == 1) ||
		  (TOKEN_INVARIANTS::instance ()->acquired (tp->token2_) == 1));

      ACE_DEBUG ((LM_DEBUG, "(%t) %s acquired.\n", collection.name ()));

      TOKEN_INVARIANTS::instance ()->releasing (tp->token1_);
      TOKEN_INVARIANTS::instance ()->releasing (tp->token2_);

      if (collection.renew () == -1)
	{
	  if (ACE_OS::last_error () == EDEADLK)
	    {
	      ACE_DEBUG ((LM_DEBUG, "deadlock detected"));
	      goto deadlock;
	    }
	  ACE_ERROR ((LM_ERROR, "(%t) %p renew failed\n","run_thread"));
	  return (void *) -1;
	}

      ACE_ASSERT ((TOKEN_INVARIANTS::instance ()->acquired (tp->token1_) == 1) ||
		  (TOKEN_INVARIANTS::instance ()->acquired (tp->token2_) == 1));

      ACE_DEBUG ((LM_DEBUG, "(%t) %s renewed.\n", collection.name ()));

    deadlock:

      TOKEN_INVARIANTS::instance ()->releasing (tp->token1_);
      TOKEN_INVARIANTS::instance ()->releasing (tp->token2_);

      if (collection.release () == -1)
	{
	  ACE_ERROR ((LM_ERROR, "(%t) %p release failed\n","run_thread"));
	  return (void *) -1;
	}

      ACE_DEBUG ((LM_DEBUG, "(%t) %s released.\n", collection.name ()));
    }


  ACE_DEBUG ((LM_DEBUG, "(%t) thread exiting.\n"));
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
  tp1.collection_name_ = "A and Reader";
  tp1.token1_ = A;
  tp1.token2_ = R;

  tp2.collection_name_ = "A and Writer";
  tp2.token1_ = A;
  tp2.token2_ = W;

  tp3.collection_name_ = "B and Reader";
  tp3.token1_ = B;
  tp3.token2_ = R;

  // Spawn off three threads.
  ACE_Thread_Manager *mgr = ACE_Service_Config::thr_mgr ();

  if (mgr->spawn (ACE_THR_FUNC (run_thread),
		 (void *) &tp1, THR_BOUND | THR_SUSPENDED) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "spawn 1 failed"), -1);

  if (mgr->spawn (ACE_THR_FUNC (run_thread),
		 (void *) &tp2, THR_BOUND | THR_SUSPENDED) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "spawn 2 failed"), -1);

  if (mgr->spawn (ACE_THR_FUNC (run_thread),
		 (void *) &tp3, THR_BOUND | THR_SUSPENDED) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "spawn 3 failed"), -1);

#if ! defined (ACE_HAS_PTHREADS)
  if (mgr->resume_all () == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "resume failed"), -1);
#endif

  // Wait for all threads to exit.
  mgr->wait ();

  ACE_DEBUG ((LM_DEBUG, "Test finished.\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */

int
main (int, char *[])
{
  ACE_START_TEST ("Tokens_Test");
#if defined (ACE_HAS_THREADS)
  ACE_Token_Proxy *A, *B, *R, *W;

  ACE_NEW_RETURN (A, ACE_Local_Mutex ("L Mutex A", 0, 0), -1);
  ACE_NEW_RETURN (B, ACE_Local_Mutex ("L Mutex B", 0, 0), -1);
  ACE_NEW_RETURN (R, ACE_Local_RLock ("L Reader Lock", 0, 0), -1);
  ACE_NEW_RETURN (W, ACE_Local_WLock ("L Writer Lock", 0, 0), -1);

  run_test (A,B,R,W);

  // Fork token server.
  char *s_argv[4];

  s_argv[0] = 
    ".." ACE_DIRECTORY_SEPARATOR_STR_A
    "netsvcs" ACE_DIRECTORY_SEPARATOR_STR_A
    "servers" ACE_DIRECTORY_SEPARATOR_STR_A
    "main" ACE_PLATFORM_EXE_SUFFIX;

  s_argv[1] = "-f";
  s_argv[2] = ACE_PLATFORM "tokens.conf";
  s_argv[3] = 0;

  ACE_DEBUG ((LM_DEBUG, "Forking %s %s %s.\n",
	      s_argv[0], s_argv[1], s_argv[2]));

  // Start up the token server.
  ACE_Process new_process;
  if (new_process.start (s_argv) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p fork failed.\n", "Tokens_Tests.cpp"), 0);
  else
    ACE_DEBUG ((LM_DEBUG, "Server forked with pid = %d.\n", new_process.getpid ()));

  // Wait for the server to start.
  ACE_OS::sleep (3);

  ACE_DEBUG ((LM_DEBUG, 
	      "Using Token Server on %s at port %d.\n", 
	      server_host, server_port));
  ACE_Remote_Mutex::set_server_address (ACE_INET_Addr (server_port, server_host));

  ACE_NEW_RETURN (A, ACE_Remote_Mutex ("R Mutex A", 0, 1), -1);
  ACE_NEW_RETURN (B, ACE_Remote_Mutex ("R Mutex B", 0, 1), -1);
  ACE_NEW_RETURN (R, ACE_Remote_RLock ("R Reader Lock", 0, 1), -1);
  ACE_NEW_RETURN (W, ACE_Remote_WLock ("R Writer Lock", 0, 1), -1);

  run_test (A, B, R, W);

  // Wait for the server to finish.
  ACE_OS::sleep (3);

  // Kill the token server.
  if (new_process.kill () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "Kill failed.\n"), -1);

  ACE_DEBUG ((LM_DEBUG, "(%t) main thread exiting.\n"));
#else
  ACE_ERROR ((LM_ERROR, 
	      "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
