// This program tests out all the various ACE_Malloc combinations and
// the ACE_Allocator_Adapter.

#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/Malloc.h"
#include "ace/Signal.h"
#include "ace/Truncate.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/OS_NS_unistd.h"
#include "Malloc.h"
#include "Options.h"
#include <atomic>
#include <cstdio>
#include <cstring>


static int
gen_size ()
{
#if defined (ACE_HAS_THREADS)
  unsigned int seed =
    static_cast<unsigned int> (reinterpret_cast<uintptr_t> (&seed));

  return (
    ACE_Utils::truncate_cast<int> (
      ACE_OS::rand_r (&seed) % Options::instance ()->max_msg_size ()) + 1);
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
  static std::atomic<unsigned char> default_char {};

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
      int const alloc_size = gen_size ();
      // The allocator owns this block until the matching free below.
      void *const ptr = Malloc::instance ()->malloc (alloc_size);

      if (ptr == nullptr)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) *** malloc of size %d failed, %p\n%a",
                   "malloc",
                    alloc_size));
      else
        {
          unsigned char const fill_value = default_char.fetch_add (1);
          std::memset (ptr, fill_value, alloc_size);

          if (Options::instance ()->debug ())
            ACE_DEBUG ((LM_INFO,
                        "(%P|%t) %p (alloc), size = %d\n",
                        ptr,
                        alloc_size));

          // Call ourselves recursively
          malloc_recurse (count - 1);

          if (Options::instance ()->debug ())
            ACE_DEBUG ((LM_INFO,
                        "(%P|%t) %p (free), size = %d\n",
                        ptr,
                        alloc_size));
          Malloc::instance ()->free (ptr);
        }
    }

  return 0;
}

#if defined (ACE_HAS_THREADS)
static ACE_THR_FUNC_RETURN
worker (void *arg)
{
  // The thread argument carries the iteration count through intptr_t.
  int const iteration_count = static_cast<int> (reinterpret_cast<intptr_t> (arg));
  malloc_recurse (iteration_count);

  return ACE_THR_FUNC_RETURN_NULL;
}
#endif /* ACE_HAS_THREADS */

// Create the appropriate type of process/thread.

static void
spawn ()
{
  Options const *const options = Options::instance ();
  if (options->spawn_threads ())
    {
#if defined (ACE_HAS_THREADS)
      int const iteration_count =
        ACE_Utils::truncate_cast<int> (options->iteration_count ());
      if (ACE_Thread_Manager::instance ()->spawn (
            &worker,
            reinterpret_cast<void *> (static_cast<intptr_t> (iteration_count)),
            THR_BOUND) == -1)
        ACE_ERROR ((LM_ERROR, "%p\n%a", "thread create failed"));
#else
      if (options->spawn_count () > 1)
        ACE_ERROR ((LM_ERROR,
                    "only one thread may be run in a process on this platform\n%a",
                    1));
#endif /* ACE_HAS_THREADS */
    }
#if !defined (ACE_WIN32)
  else if (ACE_OS::fork (ACE_TEXT_CHAR_TO_TCHAR (options->program_name ().c_str ())) == 0)
    {
      if (options->exec_slave ())
        {
          char iterations[32] {};
          char msg_size[32] {};

          std::snprintf (iterations, sizeof (iterations), "%lu",
                         static_cast<unsigned long> (options->iteration_count ()));
          std::snprintf (msg_size, sizeof (msg_size), "%lu",
                         static_cast<unsigned long> (options->max_msg_size ()));
          char const *const debug_option = options->debug () ? "-d" : "";

          char const *const argv[] =
          {
            options->slave_name ().c_str (),
            "-p",
            "-n",
            iterations,
            "-L",
            msg_size,
            debug_option,
            nullptr
          };

          if (ACE_OS::execv (options->program_name ().c_str (),
                             const_cast<char *const *> (argv)) == -1)
            ACE_ERROR ((LM_ERROR, "%p\n", "exec failed"));
          ACE_OS::_exit (1);
        }
      else
        {
          ACE_DEBUG ((LM_INFO,
                      "(%P|%t) about to recurse with iteration count = %d\n",
                      options->iteration_count ()));

          malloc_recurse (ACE_Utils::truncate_cast<int> (options->iteration_count ()));
          Malloc::instance ()->remove ();
          ACE_OS::exit (0);
        }
    }
#endif /* ACE_WIN32 */
}

// Wait for all the child processes/threads to exit.

static void
wait_for_children ()
{
  Options const *const options = Options::instance ();
  if (options->spawn_threads ())
    {
#if defined (ACE_HAS_THREADS)
      // Wait for the threads to terminate.
      ACE_Thread_Manager::instance ()->wait ();
#else
      malloc_recurse (ACE_Utils::truncate_cast<int> (options->iteration_count ()));
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
  ACE_Sig_Action sa (&handler, SIGINT);
  ACE_UNUSED_ARG (sa);

  Options *const options = Options::instance ();
  options->parse_args (argc, argv);

#if !defined (ACE_WIN32)
  if (options->child ())
    {
      ACE_DEBUG ((LM_INFO,
                  "(%P|%t) about to recurse with iteration count = %d, debug = %d\n",
                  options->iteration_count ()));

      // We've been forked...
      malloc_recurse (ACE_Utils::truncate_cast<int> (options->iteration_count ()));
      Malloc::instance ()->remove ();
    }
  else
#endif /* ACE_WIN32 */
    {
      for (size_t i = 0;
           i < options->spawn_count ();
           i++)
        spawn ();

      wait_for_children ();
      Malloc::instance ()->remove ();
    }
  return 0;
}
