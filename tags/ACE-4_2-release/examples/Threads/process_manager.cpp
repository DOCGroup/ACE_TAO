// $Id$

// Test out the mechanisms provided by the ACE_Process_Manager.

#include "ace/Service_Config.h"
#include "ace/Process_Manager.h"
#include "ace/Get_Opt.h"

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

static int n_processes = ACE_DEFAULT_THREADS;
static int n_iterations = 100000;
static int child = 0;

// Parse the command-line arguments and set options.
static void
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "p:i:cu");

  int c; 

  while ((c = get_opt ()) != -1)
    switch (c)
    {
    case 'p':
      n_processes = ACE_OS::atoi (get_opt.optarg);
      break;
    case 'i':
      n_iterations = ACE_OS::atoi (get_opt.optarg);
      break;
    case 'c':
      child = 1;
      break;
    case 'u':
    default:
      ACE_DEBUG ((LM_DEBUG, "usage:\n"
		  "-p <processes>\n"
		  "-i <iterations>\n"));
      break;
  }
}

int
main (int argc, char *argv[])
{
  ACE_Service_Config daemon;

  daemon.open (argv[0]);

  ACE_UNUSED_ARG (n_processes);

  atexit (exithook);

  parse_args (argc, argv);

  ACE_Sig_Set sigset (1); // Block all signals.

  if (ACE_OS::thr_sigsetmask (SIG_BLOCK, sigset, 0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "thr_sigsetmask"), 1);
  else if (ACE_Service_Config::thr_mgr ()->spawn (sig_handler, 0, THR_DETACHED) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn"), 1);

  if (child)
    {
      worker (n_iterations);
      return 0;
    }

  ACE_Process_Options options;
  options.command_line ("%s -c", argv[0]);
  pid_t pid = proc_mgr.spawn (options);

  if (pid == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%P|%t) %p\n", "start_n"), 1);
  else
    {
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
