// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Reader_Writer_Test.cpp
//
// = DESCRIPTION
//      This test program verifies the functionality of the ACE_OS
//      implementation of readers/writer locks on Win32 and Posix
//      pthreads.
//
// = AUTHOR
//    Prashant Jain and Doug C. Schmidt
//
// ============================================================================

#include "test_config.h"
#include "ace/Synch.h"
#include "ace/Thread.h"
#include "ace/Service_Config.h"
#include "ace/Get_Opt.h"

#if defined (ACE_HAS_THREADS)

// Default number of iterations.
static size_t n_iterations = 100;

// Default number of loops.
static size_t n_loops = 100;

// Default number of readers.
static size_t n_readers = 6;

// Default number of writers.
static size_t n_writers = 4;

// Thread id of last writer.
static ACE_thread_t shared_data;

// Lock for shared_data.
static ACE_RW_Mutex rw_mutex;

// Count of the number of readers and writers.
static ACE_Atomic_Op<ACE_Thread_Mutex, int> current_readers;
static ACE_Atomic_Op<ACE_Thread_Mutex, int> current_writers;

// Explain usage and exit.
static void
print_usage_and_die (void)
{
  ACE_DEBUG ((LM_DEBUG,
	      "usage: %n [-r n_readers] [-w n_writers] [-n iteration_count]\n"));
  ACE_OS::exit (1);
}

static void
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "r:w:n:");

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
    {
    case 'r':
      n_readers = ACE_OS::atoi (get_opt.optarg);
      break;
    case 'w':
      n_writers = ACE_OS::atoi (get_opt.optarg);
      break;
    case 'n':
      n_iterations = atoi (get_opt.optarg);
      break;
    default:
      print_usage_and_die ();
      break;
  }
}

// Iterate <n_iterations> each time checking that nobody modifies the data
// while we have a read lock.

static void *
reader (void *)
{
  ACE_NEW_THREAD;

  ACE_DEBUG ((LM_DEBUG, " (%t) reader starting\n"));

  // We use a random pause, around 2msec with 1msec jittering.
  int usecs = 1000 + ACE_OS::rand() % 2000;
  ACE_Time_Value pause(0, usecs);

  for (size_t iterations = 1; iterations <= n_iterations; iterations++)
    {
      ACE_OS::sleep(pause);
      ACE_Read_Guard<ACE_RW_Mutex> g (rw_mutex);
      // int n = ++current_readers;
      // ACE_DEBUG ((LM_DEBUG, " (%t) I'm reader number %d\n", n));

      if (current_writers > 0)
        ACE_DEBUG ((LM_DEBUG, " (%t) writers found!!!\n"));

      ACE_thread_t data = shared_data;

      for (size_t loop = 1; loop <= n_loops; loop++)
        {
	  ACE_Thread::yield ();

	  if (!ACE_OS::thr_equal (shared_data, data))
            ACE_DEBUG ((LM_DEBUG,
			" (%t) somebody changed %d to %d\n",
			data, shared_data));
        }

      --current_readers;
      //ACE_DEBUG ((LM_DEBUG, " (%t) done with reading guarded data\n"));

      ACE_DEBUG((LM_DEBUG, " (%t) read %d done at %T\n", iterations));
      // ACE_Thread::yield ();
    }
  return 0;
}

// Iterate <n_iterations> each time modifying the global data
// and checking that nobody steps on it while we can write it.

static void *
writer (void *)
{
  ACE_NEW_THREAD;

  ACE_DEBUG ((LM_DEBUG, " (%t) writer starting\n"));

  // We use a random pause, around 2msec with 1msec jittering.
  int usecs = 1000 + ACE_OS::rand() % 2000;
  ACE_Time_Value pause(0, usecs);

  for (size_t iterations = 1; iterations <= n_iterations; iterations++)
    {
      ACE_OS::sleep(pause);

      ACE_Write_Guard<ACE_RW_Mutex> g (rw_mutex);

      ++current_writers;
      //ACE_DEBUG ((LM_DEBUG, " (%t) writing to guarded data\n"));

      if (current_writers > 1)
        ACE_DEBUG ((LM_DEBUG, " (%t) other writers found!!!\n"));

      if (current_readers > 0)
        ACE_DEBUG ((LM_DEBUG, " (%t) readers found!!!\n"));

      ACE_thread_t self = ACE_Thread::self ();

      shared_data = self;

      for (size_t loop = 1; loop <= n_loops; loop++)
        {
	  ACE_Thread::yield ();

	  if (!ACE_OS::thr_equal (shared_data, self))
            ACE_DEBUG ((LM_DEBUG,
			" (%t) somebody wrote on my data %d\n",
			shared_data));
        }

      --current_writers;

      //ACE_DEBUG ((LM_DEBUG, " (%t) done with guarded data\n"));

      ACE_DEBUG((LM_DEBUG, " (%t) write %d done at %T\n", iterations));
      // ACE_Thread::yield ();
    }
  return 0;
}

#endif /* ACE_HAS_THREADS */

// Spawn off threads.

int main (int argc, char *argv[])
{
  ACE_START_TEST ("Reader_Writer_Test");

#if defined (ACE_HAS_THREADS)
  parse_args (argc, argv);

  current_readers = 0; // Possibly already done
  current_writers = 0; // Possibly already done

  ACE_DEBUG ((LM_DEBUG, " (%t) main thread starting\n"));

  if (ACE_Thread_Manager::instance ()->spawn_n (n_readers,
					       ACE_THR_FUNC (reader),
					       0,
					       THR_NEW_LWP) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn_n"), 1);
  else if (ACE_Thread_Manager::instance ()->spawn_n (n_writers,
						    ACE_THR_FUNC (writer),
						    0,
						    THR_NEW_LWP) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn_n"), 1);

  ACE_Thread_Manager::instance ()->wait ();

  ACE_DEBUG ((LM_DEBUG, " (%t) exiting main thread\n"));
#else
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Read_Guard<ACE_RW_Mutex>;
template class ACE_Write_Guard<ACE_RW_Mutex>;
template class ACE_Guard<ACE_RW_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Read_Guard<ACE_RW_Mutex>
#pragma instantiate ACE_Write_Guard<ACE_RW_Mutex>
#pragma instantiate ACE_Guard<ACE_RW_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

