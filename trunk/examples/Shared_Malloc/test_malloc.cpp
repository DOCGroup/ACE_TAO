// $Id$

// This program tests out all the various ACE_Malloc combinations and
// the ACE_Allocator_Adapter.

#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "Malloc.h"
#include "Options.h"

// Global thread manager.
static ACE_Thread_Manager thread_manager;

static int 
gen_size (void)
{
#if defined (ACE_HAS_THREADS)
  ACE_RANDR_TYPE seed = ACE_RANDR_TYPE (&seed);
  return (ACE_OS::rand_r (ACE_RANDR_TYPE (seed)) % Options::instance ()->max_msg_size ()) + 1;
#else
  return (ACE_OS::rand () % Options::instance ()->max_msg_size ()) + 1;
#endif /* ACE_HAS_THREADS */
}

// Recursively allocate and deallocate dynamic memory.

static int
malloc_recurse (int count)
{
  static char default_char = 0;

  if (count <= 0)
    {
      if (Options::instance ()->debug ())
	AMS (Malloc::instance ()->print_stats ());
    }
  else
    {
      int alloc_size = gen_size ();
      void *ptr = Malloc::instance ()->malloc (alloc_size);

      if (ptr == 0)
	ACE_ERROR ((LM_ERROR, "(%P|%t) *** malloc of size %d failed, %p\n%a", 
		   "malloc", alloc_size));
      else
	{
	  ACE_OS::memset (ptr, default_char++, alloc_size);

	  if (Options::instance ()->debug ()) 
	    ACE_DEBUG ((LM_INFO, "(%P|%t) %u (alloc), size = %d\n", ptr, alloc_size));

	  // Call ourselves recursively
	  malloc_recurse (count - 1);

	  if (Options::instance ()->debug ()) 
	    ACE_DEBUG ((LM_INFO, "(%P|%t) %u (free), size = %d\n", ptr, alloc_size));

	  Malloc::instance ()->free (ptr);
	}
    }
  return 0;
}

static void *
worker (void *arg)
{
  // Allocate a thread control object, which automatically removes the
  // thread from the thread manager on exit.
  ACE_Thread_Control tc (&thread_manager);

  malloc_recurse (int (arg));
  return 0;
}

// Create the appropriate type of process/thread.

static void
spawn (void)
{
  if (Options::instance ()->spawn_threads ())
    {
#if defined (ACE_HAS_THREADS)
      if (thread_manager.spawn (ACE_THR_FUNC (worker),
			 (void *) Options::instance ()->iteration_count (),
			 THR_BOUND) == -1)
	ACE_ERROR ((LM_ERROR, "%p\n%a", "thread create failed"));
#else
      if (Options::instance ()->spawn_count () > 1)
	ACE_ERROR ((LM_ERROR, "only one thread may be run in a process on this platform\n%a", 1));
#endif /* ACE_HAS_THREADS */	
    }
#if !defined (ACE_WIN32)
  else
    {
      if (ACE_OS::fork () == 0)
	{
	  if (Options::instance ()->exec_slave ())
	    {
	      char iterations[20];
	      char msg_size[20];
	      
	      ACE_OS::sprintf (iterations, "%d", Options::instance ()->iteration_count ());
	      ACE_OS::sprintf (msg_size, "%d", Options::instance ()->max_msg_size ());

	      char *argv[8];
	      argv[0] = (char *) Options::instance ()->slave_name ();
	      argv[1] = "-p";
	      argv[2] = "-n";
	      argv[3] = iterations;
	      argv[4] = "-L";
	      argv[5] = msg_size;
	      argv[6] = Options::instance ()->debug () ? "-d" : "";
	      argv[7] = (char *) 0;

	      if (ACE_OS::execv (Options::instance ()->program_name (), argv) == -1)
		ACE_ERROR ((LM_ERROR, "%p\n", "exec failed"));
	      ACE_OS::_exit (1);
	    }
	  else
	    {
	      ACE_LOG_MSG->sync (Options::instance ()->program_name ());

	      ACE_DEBUG ((LM_INFO,
			  "(%P|%t) about to recurse with iteration count = %d\n",
			  Options::instance ()->iteration_count ()));

	      malloc_recurse (Options::instance ()->iteration_count ());
	      ACE_OS::exit (0);
	    }
	}
    }
#endif /* ACE_WIN32 */
}

// Wait for all the child processes/threads to exit.

static void 
wait_for_children (void)
{
  if (Options::instance ()->spawn_threads ())
    {
#if defined (ACE_HAS_THREADS)      
      // Wait for the threads to terminate.
      thread_manager.wait ();
#else
      malloc_recurse (Options::instance ()->iteration_count ());
#endif /* ACE_HAS_THREADS */	
    }
#if !defined (ACE_WIN32)
  else
    {
      pid_t pid;

      while ((pid = ACE_OS::wait (0)) != -1)
	ACE_DEBUG ((LM_DEBUG, "(%P|%t) reaped pid = %d\n", pid));
    }
#endif /* ACE_WIN32 */
}

extern "C" void 
handler (int)
{
  Malloc::instance ()->remove ();
  ACE_ERROR ((LM_ERROR, "(%P|%t) removed handler\n%a", 0));
}

int
main (int argc, char *argv[])
{
  // Register a signal handler.
  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGINT);
  ACE_UNUSED_ARG (sa);

  Options::instance ()->parse_args (argc, argv);

#if !defined (ACE_WIN32)
  if (Options::instance ()->child ())
    {
      ACE_DEBUG ((LM_INFO,
		  "(%P|%t) about to recurse with iteration count = %d, debug = %d\n",
		  Options::instance ()->iteration_count ()));

      // We've been forked...
      malloc_recurse (Options::instance ()->iteration_count ());
    }
  else
#endif /* ACE_WIN32 */
    {
      for (size_t i = 0; i < Options::instance ()->spawn_count (); i++)
	spawn ();

      wait_for_children ();
      Malloc::instance ()->remove ();
    }
  return 0;
}

