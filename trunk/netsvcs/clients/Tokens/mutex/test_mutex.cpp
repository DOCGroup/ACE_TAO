// ============================================================================
// $Id$

//
// = LIBRARY
//    examples
// 
// = FILENAME
//    test_mutex.cpp
//
// = DESCRIPTION
//
// = AUTHOR
//    Tim Harrison
// 
// ============================================================================

#include "ace/OS.h"

#include "ace/Get_Opt.h"
#include "ace/Local_Tokens.h"
#include "ace/Remote_Tokens.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"

#if defined (ACE_HAS_THREADS)

static ACE_Token_Proxy *mutex;
static int remote_mutexes = 0;
static char *server_host = ACE_DEFAULT_SERVER_HOST;
static int server_port = ACE_DEFAULT_SERVER_PORT;
static int iterations = 100;
static int spawn_count = 2;

static void *
run_test (void *vp)
{
  ACE_Thread_Manager *thr_mgr = (ACE_Thread_Manager*) vp;
  ACE_Thread_Control tc (thr_mgr);

  int count = iterations;
  // test recursive acquisition of a global proxy
  while (count--)
    {
      if (mutex->acquire () == -1)
	{
	  ACE_ERROR ((LM_ERROR, "(%t) %p acquire failed\n","test_mutex"));
	  return (void *) -1;
	}
      
//      mutex->acquire ();
      if (mutex->renew () == -1)
	{
	  ACE_ERROR ((LM_ERROR, "(%t) %p renew failed\n","test_mutex"));
	  return (void *) -1;
	}

      if (mutex->release () == -1)
	{
	  ACE_ERROR ((LM_ERROR, "(%t) %p release failed\n","test_mutex"));
	  return (void *) -1;
	}

//      mutex->release ();
    }

  return 0;
}

static int
parse_args (int argc, char *argv[])
{
  ACE_LOG_MSG->open (argv[0]);

  ACE_Get_Opt get_opt (argc, argv, "t:uh:p:n:", 1);

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
	{
	case 't':
	  spawn_count = ACE_OS::atoi (get_opt.optarg);
	  break;
	case 'h':  // specify the host machine on which the server is running
	  server_host = get_opt.optarg;
	  remote_mutexes = 1;
	  break;
	case 'p':  // specify the port on which the server is running
	  server_port = ACE_OS::atoi (get_opt.optarg);
	  remote_mutexes = 1;
	  break;
	case 'n':  // specify the port on which the server is running
	  iterations = ACE_OS::atoi (get_opt.optarg);
	  break;
	case 'u':
	default:
	  ACE_ERROR_RETURN ((LM_ERROR, 
			    "%n:\n"
			    "[-h <remote host>]\n"
			    "[-p <remote port>]\n"
			    "[-n <iterations>]\n"
			    "[-t <threads>]\n"
			    "[-h <remote host>]\n"
			    "[-p <remote port>]\n", 1), -1);
	  break;
	}
    }

  return 0;
}

int
main (int argc, char* argv[])
{
  ACE_Thread_Manager thread_mgr;

  if (parse_args (argc, argv) == -1)
    return -1;

  if (remote_mutexes)
    {
      ACE_Remote_Mutex::set_server_address (ACE_INET_Addr (server_port, server_host));
      mutex = new ACE_Remote_Mutex ("Remote TOKEN", 0, 1);
    }
  else
    {
      mutex = new ACE_Local_Mutex ("Local TOKEN", 0, 1);
    }

  if (thread_mgr.spawn_n (spawn_count,
			  ACE_THR_FUNC (run_test),
			  (void *) &thread_mgr, THR_BOUND) == -1)
    ACE_ERROR_RETURN ((LM_DEBUG, "%p\n", "spawn"), -1);

  thread_mgr.wait ();

  return 42;
}
#else
int main (void)
{
  ACE_ERROR_RETURN ((LM_ERROR, "you must have threads to run this test program\n"), -1);
}
#endif /* ACE_HAS_THREADS */
