// $Id$

// This program tests out all the various ACE_Malloc combinations and
// the ACE_Allocator_Adapter.

#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/Malloc.h"
#include "ace/Signal.h"
#include "ace/Truncate.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/OS_NS_unistd.h"
#include "Malloc.h"
#include "Options.h"



static int
gen_size (void)
{
#if defined (ACE_HAS_THREADS)
  ACE_RANDR_TYPE seed =
    static_cast<ACE_RANDR_TYPE> (reinterpret_cast<uintptr_t> (&seed));

  return (
    ACE_Utils::truncate_cast<int> (
      ACE_OS::rand_r (seed) % Options::instance ()->max_msg_size ()) + 1);
#else
  return (
    ACE_Utils::truncate_cast<int> (
      ACE_OS::rand () % Options::instance ()->max_msg_size ()) + 1);
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
        {
          // Note that you'll need to #define ACE_HAS_MALLOC_STATS in
          // the main ACE config.h file and remake ACE to enable this.
          ACE_MALLOC_STATS (Malloc::instance ()->print_stats ());
        }
    }
  else
    {
      int alloc_size = gen_size ();
      void *ptr = Malloc::instance ()->malloc (alloc_size);

      if (ptr == 0)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) *** malloc of size %d failed, %p\n%a",
                   "malloc",
                    alloc_size));
      else
        {
          ACE_OS::memset (ptr, default_char++, alloc_size);

          if (Options::instance ()->debug ())
            ACE_DEBUG ((LM_INFO,
                        "(%P|%t) %u (alloc), size = %d\n",
                        ptr,
                        alloc_size));

          // Call ourselves recursively
          malloc_recurse (count - 1);

          if (Options::instance ()->debug ())
            ACE_DEBUG ((LM_INFO,
                        "(%P|%t) %u (free), size = %d\n",
                        ptr,
                        alloc_size));
          Malloc::instance ()->free (ptr);
        }
    }

  return 0;
}

#if defined (ACE_HAS_THREADS)
static void *
worker (void *arg)
{
  // Cast the arg to a long, first, because a pointer is the same
  // size as a long on all current ACE platforms.
  malloc_recurse (static_cast<int> (reinterpret_cast<intptr_t> (arg)));

  return 0;
}
#endif /* ACE_HAS_THREADS */

// Create the appropriate type of process/thread.

static void
spawn (void)
{
  if (Options::instance ()->spawn_threads ())
    {
#if defined (ACE_HAS_THREADS)
      if (ACE_Thread_Manager::instance ()->spawn (ACE_THR_FUNC (worker),
                                                  (void *) Options::instance ()->iteration_count (),
                                                  THR_BOUND) == -1)
        ACE_ERROR ((LM_ERROR, "%p\n%a", "thread create failed"));
#else
      if (Options::instance ()->spawn_count () > 1)
        ACE_ERROR ((LM_ERROR,
                    "only one thread may be run in a process on this platform\n%a",
                    1));
#endif /* ACE_HAS_THREADS */
    }
#if !defined (ACE_WIN32)
  else if (ACE_OS::fork (ACE_TEXT_CHAR_TO_TCHAR (Options::instance ()->program_name ())) == 0)
    {
      if (Options::instance ()->exec_slave ())
        {
          char iterations[20];
          char msg_size[20];

          ACE_OS::sprintf (iterations, "%lu",
                           (unsigned long)
                             Options::instance ()->iteration_count ());
          ACE_OS::sprintf (msg_size, "%lu",
                           (unsigned long)
                             Options::instance ()->max_msg_size ());
          const char *cp = 0;

          if (Options::instance ()->debug ())
            cp = "-d";
          else
            cp = "";

          const char *argv[] =
          {
            Options::instance ()->slave_name (),
            "-p",
            "-n",
            iterations,
            "-L",
            msg_size,
            cp,
            0
          };

          if (ACE_OS::execv (Options::instance ()->program_name (),
                             (char *const *) argv) == -1)
            ACE_ERROR ((LM_ERROR, "%p\n", "exec failed"));
          ACE_OS::_exit (1);
        }
      else
        {
          ACE_DEBUG ((LM_INFO,
                      "(%P|%t) about to recurse with iteration count = %d\n",
                      Options::instance ()->iteration_count ()));

          malloc_recurse (Options::instance ()->iteration_count ());
          Malloc::instance ()->remove ();
          ACE_OS::exit (0);
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
      ACE_Thread_Manager::instance ()->wait ();
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
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
      Malloc::instance ()->remove ();
    }
  else
#endif /* ACE_WIN32 */
    {
      for (size_t i = 0;
           i < Options::instance ()->spawn_count ();
           i++)
        spawn ();

      wait_for_children ();
      Malloc::instance ()->remove ();
    }
  return 0;
}
