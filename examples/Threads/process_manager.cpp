// $Id$

// Test out the mechanisms provided by the ACE_Process_Manager.

#include "ace/Service_Config.h"
#include "ace/Process_Manager.h"

#if !defined (ACE_WIN32) && defined (ACE_HAS_THREADS)

// Global process manager.
static ACE_Process_Manager proc_mgr;

void *
sig_handler (void *)
{
  ACE_Thread_Control tc (ACE_Service_Config::thr_mgr ());

  ACE_Sig_Set sigset;

  // Register a dummy signal handler so that our disposition isn't
  // SIG_IGN (which is the default).
  ACE_Sig_Action sa ((ACE_SignalHandler) sig_handler, SIGCHLD);
  ACE_UNUSED_ARG (sa);

  // Register signal handlers.
  sigset.sig_add (SIGINT);
  sigset.sig_add (SIGCHLD);

  for (;;)
    {
      int signum = ACE_OS::sigwait (sigset);

      ACE_DEBUG ((LM_DEBUG, 
		  "(%P|%t) received signal %S\n",
		  signum));

      switch (signum)
	{
	case SIGINT:
	  // Shutdown using <ACE_OS::exit>.
	  ACE_DEBUG ((LM_DEBUG, 
		      "(%P|%t) shutting down %n%a\n", 1));
	  /* NOTREACHED */
	case SIGCHLD:
	  
	  for (;;)
	    {
	      pid_t pid = proc_mgr.reap ();
	      
	      switch (pid)
		{
		case -1:
		  ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "reap"), 0);
		  /* NOTREACHED */
		default:
		  ACE_DEBUG ((LM_DEBUG, "(%P|%t) reaped pid %d\n", pid));
		}
	    }
	  
	  break;
	case -1:
	  ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "sigwait"), 0);
	  /* NOTREACHED */
	default:
	  ACE_ERROR_RETURN ((LM_ERROR, 
			     "(%P|%t) signal %S unexpected\n", signum), 0);
	  /* NOTREACHED */
	}
    }
}

static void
worker (size_t iterations)
{
  for (size_t i = 0; i < iterations; i++)
    if ((i % 100) == 0)
      {
	ACE_DEBUG ((LM_DEBUG, "(%P|%t) sleeping\n"));
	ACE_OS::sleep (5);
      }
}

extern "C" void
exithook (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) later...\n"));
}

static const int DEFAULT_THREADS = ACE_DEFAULT_THREADS;
static const int DEFAULT_ITERATIONS = 100000;

int
main (int argc, char *argv[])
{
  ACE_Service_Config daemon;

  daemon.open (argv[0]);

  ::atexit (exithook);

  ACE_Sig_Set sigset (1); // Block all signals.

  if (ACE_OS::thr_sigsetmask (SIG_BLOCK, sigset, 0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "thr_sigsetmask"), 1);
  else if (ACE_Service_Config::thr_mgr ()->spawn (sig_handler, 0, THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn"), 1);

  int n_processes = argc > 1 ? ACE_OS::atoi (argv[1]) : DEFAULT_THREADS;
  ACE_UNUSED_ARG (n_processes);
  int n_iterations = argc > 2 ? ACE_OS::atoi (argv[2]) : DEFAULT_ITERATIONS;

  pid_t pid = proc_mgr.start (0);

  switch (pid)
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "start_n"), 1);
      /* NOTREACHED */
    case 0:
      worker (n_iterations);
      break;
      /* NOTREACHED */
    default:
      // Give the child a chance to start running.
      ACE_OS::sleep (1);

      // Shutdown the child.
      if (proc_mgr.kill (pid, SIGINT) == -1)
	ACE_ERROR_RETURN ((LM_DEBUG, "(%P|%t) %p\n", "kill"), 1);

      // Perform a barrier wait until all the processes and threads
      // have shut down.
      proc_mgr.wait ();
      ACE_Service_Config::thr_mgr ()->wait ();
    }

  return 0;
}
#else
int 
main (int, char *[])
{
  ACE_ERROR_RETURN ((LM_ERROR, "process manager not supported on this platform\n"), -1);
}
#endif /* !ACE_WIN32 && ACE_HAS_THREADS */
