// $Id$

// This test program verifies the functionality of the ACE_OS
// implementation of readers/writer locks on Win32 and Posix pthreads.

#include "ace/OS_main.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Atomic_Op.h"

ACE_RCSID(Threads, reader_writer, "$Id$")

#if defined (ACE_HAS_THREADS)

#include "ace/Guard_T.h"
#include "ace/RW_Mutex.h"

// Default number of iterations.
static int n_iterations = 1000;

// Default number of loops.
static int n_loops = 100;

// Default number of readers.
static int n_readers = 6;

// Default number of writers.
static int n_writers = 2;

// Thread id of last writer.
static ACE_thread_t shared_thr_id;

// Lock for shared_thr_id.
static ACE_RW_Mutex rw_mutex;

// Count of the number of readers and writers.
ACE_Atomic_Op<ACE_Thread_Mutex, int> current_readers, current_writers;

// Thread manager
static ACE_Thread_Manager thr_mgr;

// Explain usage and exit.
static void
print_usage_and_die (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "usage: %n [-r n_readers] [-w n_writers] [-n iteration_count]\n"));
  ACE_OS::exit (1);
}

// Parse the command-line arguments and set options.
static void
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT("r:w:n:l:"));

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
    {
    case 'r':
      n_readers = ACE_OS::atoi (get_opt.opt_arg ());
      break;
    case 'w':
      n_writers = ACE_OS::atoi (get_opt.opt_arg ());
      break;
    case 'n':
      n_iterations = ACE_OS::atoi (get_opt.opt_arg ());
      break;
    case 'l':
      n_loops = ACE_OS::atoi (get_opt.opt_arg ());
      break;
    default:
      print_usage_and_die ();
      break;
  }
}

// Iterate <n_iterations> each time checking that nobody modifies the
// data while we have a read lock.

static void *
reader (void *)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) reader starting\n"));

  for (int iterations = 1;
       iterations <= n_iterations; iterations++)
    {
      ACE_READ_GUARD_RETURN (ACE_RW_Mutex, g, rw_mutex, 0);

      ++current_readers;

      if (current_writers > 0)
        ACE_DEBUG ((LM_DEBUG, "(%t) writers found!!!\n"));

      ACE_thread_t thr_id = shared_thr_id;

      for (int loop = 1; loop <= n_loops; loop++)
        {
          ACE_Thread::yield();

          if (ACE_OS::thr_equal (shared_thr_id, thr_id) == 0)
            ACE_DEBUG ((LM_DEBUG,
                        "(%t) somebody changed %d to %d\n",
                        thr_id, shared_thr_id));
        }

      --current_readers;

      ACE_Thread::yield ();
    }
  return 0;
}

// Iterate <n_iterations> each time modifying the global data
// and checking that nobody steps on it while we can write it.

static void *
writer (void *)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) writer starting\n"));

  for (int iterations = 1;
       iterations <= n_iterations;
       iterations++)
    {
      ACE_WRITE_GUARD_RETURN (ACE_RW_Mutex, g, rw_mutex, 0);

      ++current_writers;

      if (current_writers > 1)
        ACE_DEBUG ((LM_DEBUG, "(%t) other writers found!!!\n"));

      if (current_readers > 0)
        ACE_DEBUG ((LM_DEBUG, "(%t) readers found!!!\n"));

      ACE_thread_t self = ACE_Thread::self ();
      shared_thr_id = self;

      for (int loop = 1; loop <= n_loops; loop++)
        {
          ACE_Thread::yield();

          if (ACE_OS::thr_equal (shared_thr_id, self) == 0)
            ACE_DEBUG ((LM_DEBUG, "(%t) somebody wrote on my data %d\n",
                        shared_thr_id));
        }

      --current_writers;

      ACE_Thread::yield ();
    }
  return 0;
}

// Spawn off threads.

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_LOG_MSG->open (argv[0]);
  parse_args (argc, argv);

  current_readers = 0; // Possibly already done
  current_writers = 0; // Possibly already done

  ACE_DEBUG ((LM_DEBUG, "(%t) main thread starting\n"));

  if (thr_mgr.spawn_n (n_readers, (ACE_THR_FUNC) reader, 0, THR_NEW_LWP) == -1 ||
      thr_mgr.spawn_n (n_writers, (ACE_THR_FUNC) writer, 0, THR_NEW_LWP) == -1)
     ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn_n"), 1);

  thr_mgr.wait ();

  ACE_DEBUG ((LM_DEBUG, "(%t) exiting main thread\n"));
  return 0;
}

#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */
