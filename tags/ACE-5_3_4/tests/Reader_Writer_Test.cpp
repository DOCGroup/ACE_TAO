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
//    Prashant Jain <pjain@cs.wustl.edu> and Doug C. Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Synch.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Atomic_Op.h"

ACE_RCSID(tests, Reader_Writer_Test, "$Id$")

#if defined (ACE_HAS_THREADS)

// Default number of iterations.
#if defined (VXWORKS) || defined (__Lynx__)
  // So the test doesn't run for too long . . .
  static size_t n_iterations = 25;
#else
  static size_t n_iterations = 50;
#endif /* ACE_HAS_WINCE */

// Default number of loops.
#if defined (VXWORKS)
  // thr_yield () and/or thr_equal () are expensive on VxWorks, apparently.
  static size_t n_loops = 10;
#else
  static size_t n_loops = 100;
#endif /* ACE_HAS_WINCE */

// Default number of readers.
static size_t n_readers = 6;

// Default number of writers.
static size_t n_writers = 4;

// Thread id of last writer.
static ACE_thread_t shared_data;

// Lock for shared_data.
static ACE_RW_Thread_Mutex rw_mutex;

// Count of the number of readers and writers.
static ACE_Atomic_Op<ACE_Thread_Mutex, int> current_readers;
static ACE_Atomic_Op<ACE_Thread_Mutex, int> current_writers;

// Explain usage and exit.
static void
print_usage_and_die (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("usage: %n [-r n_readers] [-w n_writers] [-n iteration_count]\n")));
  ACE_OS::exit (1);
}

static void
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("r:w:n:"));

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
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%t) reader starting\n")));

  // We use a random pause, around 2msec with 1msec jittering.
  int usecs = 1000 + ACE_OS::rand() % 2000;
  ACE_Time_Value pause(0, usecs);

  for (size_t iterations = 1; iterations <= n_iterations; iterations++)
    {
      ACE_OS::sleep (pause);
      ACE_Read_Guard<ACE_RW_Thread_Mutex> g (rw_mutex);
      // int n = ++current_readers;
      // ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%t) I'm reader number %d\n"), n));

      if (current_writers > 0)
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%t) writers found!!!\n")));

      ACE_thread_t data = shared_data;

      for (size_t loop = 1; loop <= n_loops; loop++)
        {
          ACE_Thread::yield ();

          if (!ACE_OS::thr_equal (shared_data, data))
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT (" (%t) somebody changed %d to %d\n"),
                        data, shared_data));
        }

      int result = rw_mutex.tryacquire_write_upgrade ();

      if (result == 0)
        {
          current_readers--;
          current_writers++;

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%t) upgraded to write lock!\n")));

          ACE_thread_t self = ACE_Thread::self ();

          shared_data = self;
          data = self;

          for (size_t loop = 1; loop <= n_loops; loop++)
            {
              if (ACE_OS::thr_equal (shared_data, data) == 0)
                ACE_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("(%t) upgraded writer error: somebody changed %d to %d\n"),
                            data,
                            shared_data));
            }

          current_writers--;
          // we were a writer
        }
      else if (result == -1 && errno == EBUSY)
        {
          current_readers--;
          // we were still a reader

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%t) could not upgrade to write lock!\n")));

        }
      else // result == -1
        {
# if defined (ACE_LACKS_RWLOCK_T)
          // In this case we do not have native RW locks, but therefore the
          // emulation, which supports upgradable write locks.
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%t) failure in upgrading to write lock!\n"),
                      1));
# else
          // In this case we have native RW locks support, but native RW
          // locks do not support upgrading!
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%t) no support for upgradable write locks!\n"),
                      1));
# endif /* ACE_LACKS_RWLOCK_T */
        }
      //ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%t) done with reading guarded data\n")));

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) reader finished %d iterations at %T\n"),
                  iterations));
    }
  return 0;
}

// Iterate <n_iterations> each time modifying the global data
// and checking that nobody steps on it while we can write it.

static void *
writer (void *)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT (" (%t) writer starting\n")));

  // We use a random pause, around 2msec with 1msec jittering.
  int usecs = 1000 + ACE_OS::rand() % 2000;
  ACE_Time_Value pause(0, usecs);

  for (size_t iterations = 1; iterations <= n_iterations; iterations++)
    {
      ACE_OS::sleep (pause);

      ACE_Write_Guard<ACE_RW_Thread_Mutex> g (rw_mutex);

      current_writers++;

      if (current_writers > 1)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT (" (%t) other writers found!!!\n")));

      if (current_readers > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT (" (%t) readers found!!!\n")));

      ACE_thread_t self = ACE_Thread::self ();

      shared_data = self;

      for (size_t loop = 1; loop <= n_loops; loop++)
        {
          ACE_Thread::yield ();

          if (!ACE_OS::thr_equal (shared_data, self))
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT (" (%t) somebody wrote on my data %d\n"),
                        shared_data));
        }

      current_writers--;

      ACE_DEBUG((LM_DEBUG, ACE_TEXT (" (%t) write %d done at %T\n"), iterations));
    }
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Atomic_Op<ACE_Thread_Mutex, int>;
template class ACE_Atomic_Op_Ex<ACE_Thread_Mutex, int>;
template class ACE_Read_Guard<ACE_RW_Mutex>;
template class ACE_Write_Guard<ACE_RW_Mutex>;
template class ACE_Guard<ACE_RW_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Atomic_Op<ACE_Thread_Mutex, int>
#pragma instantiate ACE_Atomic_Op_Ex<ACE_Thread_Mutex, int>
#pragma instantiate ACE_Read_Guard<ACE_RW_Mutex>
#pragma instantiate ACE_Write_Guard<ACE_RW_Mutex>
#pragma instantiate ACE_Guard<ACE_RW_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_HAS_THREADS */

// Spawn off threads.

int run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Reader_Writer_Test"));

#if defined (ACE_HAS_THREADS)
  parse_args (argc, argv);

  current_readers = 0; // Possibly already done
  current_writers = 0; // Possibly already done

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%t) main thread starting\n")));

  if (ACE_Thread_Manager::instance ()->spawn_n (n_readers,
                                               ACE_THR_FUNC (reader),
                                               0,
                                               THR_NEW_LWP) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("spawn_n")), 1);
  else if (ACE_Thread_Manager::instance ()->spawn_n (n_writers,
                                                    ACE_THR_FUNC (writer),
                                                    0,
                                                    THR_NEW_LWP) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("spawn_n")), 1);

  ACE_Thread_Manager::instance ()->wait ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" (%t) exiting main thread\n")));
#else
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
