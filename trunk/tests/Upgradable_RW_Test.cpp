// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Upgradable_RW_Test.cpp
//
// = DESCRIPTION
//      This test program verifies the functionality of the ACE_OS
//      implementation of readers/writer locks on Win32 and Posix
//      pthreads.  Use the RW_Mutex define switch to use
//      readers/writer mutexes or regular mutexes.
//
// = AUTHOR
//    Michael Kircher <mk1@cs.wustl.edu>
//
// ============================================================================

#include "Upgradable_RW_Test.h"

ACE_RCSID(tests, Upgradable_RW_Test, "$Id$")

#if defined (__BORLANDC__) && __BORLANDC__ >= 0x0530
  USELIB ("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined (__BORLANDC__) && __BORLANDC__ >= 0x0530 */

#if defined (ACE_HAS_THREADS)

// Default number of iterations.
static size_t n_iterations = 50;

// Maximum string length used
static const size_t MAX_STRING_SIZE = 200;

// switch on RW mutexes, else use ordinary mutexes
// #define RW_MUTEX 1

// Default number of readers.
static size_t n_readers = 10;

// Default number of writers.
static size_t n_writers = 0;

// Number of entries in the hash map
static u_int n_entries = 10;

// Try to upgrade to a write lock, by default don't try.
static u_long use_try_upgrade = 0;

// number of readers, which were able to upgrade
static u_long upgraded = 0;

// count the number of find calls
static u_long find_called = 0;

// number of readers, failing or not allowed to upgrade
static u_int not_upgraded = 0;

// Lock for shared_data (upgraded, not_upgraded, hash_Map)
#if defined (RW_MUTEX)
static ACE_RW_Thread_Mutex rw_mutex;
#else
static ACE_Thread_Mutex mutex;
#endif /* RW_MUTEX */

// Count of the number of readers and writers.
static ACE_Atomic_Op<ACE_Thread_Mutex, int> current_readers;
static ACE_Atomic_Op<ACE_Thread_Mutex, int> current_writers;

static Linked_List *linked_list_ptr;

// Returns 1 if found,
//         0 if not found,
//        -1 on an error
static int
find_last (void)
{
  find_called++;

  char search_string[MAX_STRING_SIZE];
  ACE_OS::sprintf (search_string,
                   "%d",
                   n_entries - 1);
  ACE_CString cString (search_string);
  Element* element_ptr;

  for (ACE_Double_Linked_List_Iterator<Element> iterator (*linked_list_ptr);
       !iterator.done ();
       iterator.advance ())
    {
      element_ptr = iterator.next ();
      if (element_ptr)
        if (*element_ptr->value () == cString)
          return 1;
    }

  return 0;

}

// Explain usage and exit.
static void
print_usage_and_die (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("usage: %n [-r n_readers] [-w n_writers]\n"
                         "   [-e max_entries] [-u try update] [-n iteration_count]\n")));
  ACE_OS::exit (1);
}

static void
parse_args (int argc, ASYS_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ASYS_TEXT ("e:r:w:n:u"));

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 'e':
        n_entries = ACE_OS::atoi (get_opt.optarg);
        break;
      case 'r':
        n_readers = ACE_OS::atoi (get_opt.optarg);
        break;
      case 'w':
        n_writers = ACE_OS::atoi (get_opt.optarg);
        break;
      case 'n':
        n_iterations = ACE_OS::atoi (get_opt.optarg);
        break;
      case 'u':
        use_try_upgrade = 1;
        break;
      default:
        print_usage_and_die ();
        break;
      }
}

// Iterate <n_iterations> each time checking that nobody modifies the data
// while we have a read lock.

int
Reader_Task::svc (void)
{
  ACE_Profile_Timer timer;
  ACE_Profile_Timer::ACE_Elapsed_Time elapsed_time;

  barrier_.wait ();
  // Wait at the barrier.

  // We start an ACE_Profile_Timer here...
  timer.start ();

  for (size_t iterations = 1;
       iterations <= n_iterations;
       iterations++)
    {
      ACE_Thread::yield ();

      int result = 0;

      {
#if defined (RW_MUTEX)
        ACE_Read_Guard<ACE_RW_Thread_Mutex> g (rw_mutex);
#else
        ACE_Guard<ACE_Thread_Mutex> g (mutex);
#endif /* RW_MUTEX */
        find_last ();
#if defined (RW_MUTEX)
        if (use_try_upgrade)
          result =
            rw_mutex.tryacquire_write_upgrade ();
#endif /* RW_MUTEX */

        // True, when we were able to upgrade.
        if (result == 0 && use_try_upgrade)
          {
            //find_last (); try to find something which is not in
            //there
            upgraded++;
            continue;
          }
      }

      if (result == -1 && errno == EBUSY // we tried and failed
          || !use_try_upgrade)           // we did not try at all
        {
#if defined (RW_MUTEX)
          ACE_Write_Guard<ACE_RW_Thread_Mutex> g (rw_mutex);
#else
          ACE_Guard<ACE_Thread_Mutex> g (mutex);
#endif /* RW_MUTEX */

          not_upgraded++;
          find_last ();
        }
      else if (result == -1 && errno != EBUSY)
        ACE_ERROR ((LM_ERROR,
                    ASYS_TEXT (" (%t) failure in upgrading to write lock!\n"),
                    1));
    }

  // Stop the timer.
  timer.stop ();
  timer.elapsed_time (elapsed_time);

  this->time_Calculation_.report_time (elapsed_time);

  return 0;
}

// Iterate <n_iterations> each time modifying the global data and
// checking that nobody steps on it while we can write it.

int
Writer_Task::svc (void)
{
  ACE_Profile_Timer timer;
  ACE_Profile_Timer::ACE_Elapsed_Time elapsed_time;

  barrier_.wait ();
  // Wait at the barrier

  // We start an ACE_Profile_Timer here...
  timer.start ();

  for (size_t iterations = 1;
       iterations <= n_iterations;
       iterations++)
    {
      ACE_Thread::yield ();

#if defined (RW_MUTEX)
      ACE_Write_Guard<ACE_RW_Thread_Mutex> g (rw_mutex);
#else
      ACE_Guard<ACE_Thread_Mutex> g (mutex);
#endif /* RW_MUTEX */

      find_last ();

      current_writers--;
    }

  // Stop the timer.
  timer.stop ();
  timer.elapsed_time (elapsed_time);

  this->time_Calculation_.report_time (elapsed_time);

  return 0;
}

void
Time_Calculation::report_time (ACE_Profile_Timer::ACE_Elapsed_Time &elapsed_time)
{
  ACE_Guard<ACE_Thread_Mutex> g (mutex_);

  this->times_.real_time += elapsed_time.real_time;
  this->times_.user_time += elapsed_time.user_time;
  this->times_.system_time += elapsed_time.system_time;

  this->reported_times_++;
}

void
Time_Calculation ::print_stats (void)
{
  ACE_Profile_Timer::ACE_Elapsed_Time elapsed_time = this->times_;
  u_int iterations = 1;

  if (iterations > 0)
    {
      elapsed_time.real_time *= ACE_ONE_SECOND_IN_MSECS;
      elapsed_time.user_time *= ACE_ONE_SECOND_IN_MSECS;
      elapsed_time.system_time *= ACE_ONE_SECOND_IN_MSECS;

      elapsed_time.real_time /= iterations;
      elapsed_time.user_time /= iterations;
      elapsed_time.system_time /= iterations;

      double tmp = 1000 / elapsed_time.real_time;

      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("\n"
                             "\treal_time\t = %0.06f ms, \n"
                             "\tuser_time\t = %0.06f ms, \n"
                             "\tsystem_time\t = %0.06f ms, \n"
                             "\t%0.00f calls/second\n"),
                  elapsed_time.real_time   < 0.0 ? 0.0 : elapsed_time.real_time,
                  elapsed_time.user_time   < 0.0 ? 0.0 : elapsed_time.user_time,
                  elapsed_time.system_time < 0.0 ? 0.0 : elapsed_time.system_time,
                  tmp < 0.0 ? 0.0 : tmp));

      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("Number of reported times: %d\n"),
                  this->reported_times_));
    }
  else
    ACE_ERROR ((LM_ERROR,
                "\tNo time stats printed.  Zero iterations or error ocurred.\n"));
}

static int
init (void)
{
  char entry[MAX_STRING_SIZE];
  ACE_CString *cString_ptr;
  Element *element_ptr;

  ACE_NEW_RETURN (linked_list_ptr,
                  Linked_List,
                  -1);

  for (u_long i = 0; i < n_entries; i++)
    {
      ACE_OS::sprintf (entry, "%d", i);
      ACE_NEW_RETURN (cString_ptr,
                      ACE_CString (entry),
                      -1);
      ACE_NEW_RETURN (element_ptr,
                      Element (cString_ptr),
                      -1);
      linked_list_ptr->insert_tail (element_ptr);
    }
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Atomic_Op<ACE_Thread_Mutex, int>;
template class ACE_Read_Guard<ACE_RW_Mutex>;
template class ACE_Write_Guard<ACE_RW_Mutex>;
template class ACE_Guard<ACE_RW_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Atomic_Op<ACE_Thread_Mutex, int>
#pragma instantiate ACE_Read_Guard<ACE_RW_Mutex>
#pragma instantiate ACE_Write_Guard<ACE_RW_Mutex>
#pragma instantiate ACE_Guard<ACE_RW_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* ACE_HAS_THREADS */

// Spawn off threads.

int
main (int argc, ASYS_TCHAR *argv[])
{
  ACE_START_TEST (ASYS_TEXT ("Upgradable_RW_Test"));

#if defined (ACE_HAS_THREADS)
  parse_args (argc, argv);
#if !defined (RW_MUTEX)
  use_try_upgrade = 0;
  // make sure that we have to acquire the write lock
#endif /* RW_MUTEX */

  current_readers = 0; // Possibly already done
  current_writers = 0; // Possibly already done

  init ();

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT (" (%t) main thread starting\n")));

  Time_Calculation time_Calculation;
  // for the time calculation

  ACE_Barrier barrier (n_readers + n_writers);
  // for a nice start of all threads (for much contention)

  // Initialize the readers.
  Reader_Task** reader_tasks;

  ACE_NEW_RETURN (reader_tasks,
                  Reader_Task*[n_readers],
                  -1);
  u_int i;

  for (i = 0;
       i < n_readers;
       i++)
    {
      ACE_NEW_RETURN (reader_tasks[i],
                      Reader_Task (time_Calculation,
                                  barrier),
                      -1);
      reader_tasks[i]->activate (THR_BOUND | THR_SCHED_FIFO,
                                 1,
                                 0,
                                 ACE_DEFAULT_THREAD_PRIORITY);
    }

  // Create all the writers
  Writer_Task** writer_tasks;

  ACE_NEW_RETURN (writer_tasks,
                  Writer_Task*[n_writers],
                  -1);

  for (i = 0;
       i < n_writers;
       i++)
    {
      ACE_NEW_RETURN (writer_tasks[i],
                      Writer_Task (time_Calculation,
                                  barrier),
                      -1);
      writer_tasks[i]->activate (THR_BOUND | ACE_SCHED_FIFO,
                                 1,
                                 0,
                                 ACE_DEFAULT_THREAD_PRIORITY);
    }

  ACE_Thread_Manager::instance ()->wait ();

  // compute average time.
  time_Calculation.print_stats ();

  if (not_upgraded != 0 || upgraded != 0)
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("upgraded to not upgraded ratio = %f \n"),
                (float) upgraded / (float) (not_upgraded + upgraded)));

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("Number of times, that find was called: %d\n"),
              find_called));


  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT (" (%t) exiting main thread\n")));

  // Delete the memory of the Double_Linked_List
  ACE_CString *cString_ptr;
  Element *element_ptr;

  for (i = 0;
       i < n_entries;
       i++)
    {
      if ((element_ptr = linked_list_ptr->delete_head ()))
        {
          cString_ptr = element_ptr->value ();
          delete cString_ptr;
          delete element_ptr;
        }
    }

  delete linked_list_ptr;

  for (i = 0;
       i < n_writers;
       i++)
    delete writer_tasks[i];

  delete [] writer_tasks;

  for (i = 0;
       i < n_readers;
       i++)
    delete reader_tasks [i];

  delete [] reader_tasks;
#else
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR ((LM_INFO,
              ASYS_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Double_Linked_List<Element>;
template class ACE_Double_Linked_List_Iterator<Element>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Double_Linked_List<Element>
#pragma instantiate ACE_Double_Linked_List_Iterator<Element>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
