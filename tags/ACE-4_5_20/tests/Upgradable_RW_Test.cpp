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
//    Michael Kircher <mk1@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Synch.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "ace/Profile_Timer.h"

#if defined (__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB ("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined (__BORLANDC__) && __BORLANDC__ >= 0x0530 */

#if defined (ACE_HAS_THREADS)

// Default number of iterations.
static size_t n_iterations = 50;

// Maximum string length used
#define MAX_STRING_SIZE 200

// switch on RW mutexes, else use ordinary mutexes
#define RW_MUTEX 1

// Default number of loops.
static size_t n_loops = 1000;

// Default number of readers.
static size_t n_readers = 10;

// Default number of writers.
static size_t n_writers = 0;

// Number of entries in the hash map
static u_int n_entries = 10;

// Try to upgrade to a write lock, by default don't try.
static unsigned long use_try_upgrade = 0;

// number of readers, which were able to upgrade
static unsigned long upgraded = 0;

// count the number of find calls
static unsigned long find_called = 0;

// number of readers, failing or not allowed to upgrade
static u_int not_upgraded = 0;

// Lock for shared_data (upgraded, not_upgraded, hash_Map)
#if defined RW_MUTEX
  static ACE_RW_Thread_Mutex rw_mutex;
#else
  static ACE_Thread_Mutex mutex;
#endif

// Count of the number of readers and writers.
static ACE_Atomic_Op<ACE_Thread_Mutex, int> current_readers;
static ACE_Atomic_Op<ACE_Thread_Mutex, int> current_writers;

class Element;

class Element
{
  friend class ACE_Double_Linked_List<Element>;
  friend class ACE_Double_Linked_List_Iterator<Element>;

public:
  Element (ACE_CString* item = 0, Element* p = 0, Element* n = 0)
    : item_(item), prev_(p), next_(n)
  { 
  }

  ACE_CString* value (void)
  {
    return this->item_;
  }

private:
  Element* next_;
  Element* prev_;
  ACE_CString* item_;
};

typedef ACE_Double_Linked_List<Element>  Linked_List;

Linked_List *linked_List_ptr;

// Returns 1 if found,
//         0 if not found,
//        -1 on an error
static int 
find_last ()
{
  find_called++;

  char search_string[MAX_STRING_SIZE];
  ACE_OS::sprintf(search_string,"%d",n_entries-1);
  ACE_CString cString (search_string);
  Element* element_ptr;

  for (ACE_Double_Linked_List_Iterator<Element> iterator(*linked_List_ptr); 
       !iterator.done();
       iterator.advance())
       {
         if (element_ptr = iterator.next())
          if (*element_ptr->value() == cString)
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

static void *
reader (void *)
{
  // ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("(%t) reader starting\n")));


  for (size_t iterations = 1;
       iterations <= n_iterations;
       iterations++)
    {
      ACE_Thread::yield ();      

      int result = 0;

      {
#if defined RW_MUTEX
          ACE_Read_Guard<ACE_RW_Thread_Mutex> g (rw_mutex);
#else
          ACE_Guard<ACE_Thread_Mutex> g (mutex);
#endif

        find_last ();

#if defined RW_MUTEX
        if (use_try_upgrade)
          result = rw_mutex.tryacquire_write_upgrade ();
#endif
     
        // True, when we were able to upgrade.
        if (result == 0 && use_try_upgrade) 
          {
            //find_last ();
            // try to find something which is not in there
            upgraded++;
            
            continue;
          }
      }


      if ((result == -1 && errno == EBUSY)  // we tried and failed
          || !use_try_upgrade)              // we did not try at all
        {
#if defined RW_MUTEX
            ACE_Write_Guard<ACE_RW_Thread_Mutex> g (rw_mutex);
#else
            ACE_Guard<ACE_Thread_Mutex> g (mutex);
#endif

          not_upgraded++;

          find_last ();

        }
      else if (result == -1 && errno != EBUSY) 
        {
          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT ("(%t) failure in upgrading to write lock!\n"),
                      1));
        }
    }

  return 0;
}

// Iterate <n_iterations> each time modifying the global data and
// checking that nobody steps on it while we can write it.

static void *
writer (void *)
{

  for (size_t iterations = 1;
       iterations <= n_iterations;
       iterations++)
    {
      ACE_Thread::yield ();

#if defined RW_MUTEX
      ACE_Write_Guard<ACE_RW_Thread_Mutex> g (rw_mutex);
#else
      ACE_Guard<ACE_Thread_Mutex> g (mutex);
#endif

      find_last ();

      current_writers--;
    }
  return 0;
}

static void
print_stats (ACE_Profile_Timer::ACE_Elapsed_Time &elapsed_time,
             int iterations)
{
  if (iterations > 0)
    {
      elapsed_time.real_time *= ACE_ONE_SECOND_IN_MSECS;
      elapsed_time.user_time *= ACE_ONE_SECOND_IN_MSECS;
      elapsed_time.system_time *= ACE_ONE_SECOND_IN_MSECS;

      elapsed_time.real_time /= iterations;
      elapsed_time.user_time /= iterations;
      elapsed_time.system_time /= iterations;

      double tmp = 1000 / elapsed_time.real_time;

      ACE_DEBUG ((LM_DEBUG,"\n"
		  "\treal_time\t = %0.06f ms, \n"
		  "\tuser_time\t = %0.06f ms, \n"
		  "\tsystem_time\t = %0.06f ms, \n"
		  "\t%0.00f calls/second\n",
		  elapsed_time.real_time   < 0.0 ? 0.0 : elapsed_time.real_time,
		  elapsed_time.user_time   < 0.0 ? 0.0 : elapsed_time.user_time,
		  elapsed_time.system_time < 0.0 ? 0.0 : elapsed_time.system_time,
		  tmp < 0.0 ? 0.0 : tmp));
    }
  else
    ACE_ERROR ((LM_ERROR,
		"\tNo time stats printed.  Zero iterations or error ocurred.\n"));
}

int 
init ()
{
  char entry[MAX_STRING_SIZE]; 
  ACE_CString* cString_ptr;
  Element* element_ptr;

  ACE_NEW_RETURN (linked_List_ptr, 
                  Linked_List, 
                  -1);

  for (unsigned long i = 0; i < n_entries; i++)
  {
    ACE_OS::sprintf(entry,"%d",i);
    ACE_NEW_RETURN (cString_ptr, ACE_CString(entry), -1);
    ACE_NEW_RETURN (element_ptr, Element(cString_ptr), -1);

    linked_List_ptr->insert_tail(element_ptr);
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

int main (int argc, ASYS_TCHAR *argv[])
{
  ACE_START_TEST (ASYS_TEXT ("Upgradable_RW_Test"));

#if defined (ACE_HAS_THREADS)
  parse_args (argc, argv);

#if !defined RW_MUTEX
        use_try_upgrade = 0;
        // make sure that we have to acquire the write lock
#endif


  current_readers = 0; // Possibly already done
  current_writers = 0; // Possibly already done

  init ();

  ACE_DEBUG ((LM_DEBUG, 
              ASYS_TEXT ("(%t) main thread starting\n")));

  ACE_Profile_Timer timer;
  ACE_Profile_Timer::ACE_Elapsed_Time elapsed_time;

  // We start an ACE_Profile_Timer here...
  timer.start ();

  if (ACE_Thread_Manager::instance ()->spawn_n (n_readers,
                                                ACE_THR_FUNC (reader),
                                                0,
                                                THR_NEW_LWP) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       ASYS_TEXT ("%p\n"), 
                       ASYS_TEXT ("spawn_n")), 1);
  else if (ACE_Thread_Manager::instance ()->spawn_n (n_writers,
                                                     ACE_THR_FUNC (writer),
                                                     0,
                                                     THR_NEW_LWP) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
                       ASYS_TEXT ("%p\n"), 
                       ASYS_TEXT ("spawn_n")), 1);

  ACE_Thread_Manager::instance ()->wait ();
  
  // Stop the timer.
  timer.stop ();
  timer.elapsed_time (elapsed_time);

  // compute average time.
  print_stats (elapsed_time, 1);

  if (not_upgraded != 0 || upgraded != 0)
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("upgraded to not upgraded ratio = %f \n"),
                (float) upgraded/ (float) (not_upgraded + upgraded)));

  ACE_DEBUG ((LM_DEBUG, 
              ASYS_TEXT ("Number of times, that find was called: %d\n"), 
              find_called));


  ACE_DEBUG ((LM_DEBUG, 
              ASYS_TEXT ("(%t) exiting main thread\n")));

  // delete the memory of the Double_Linked_List
  ACE_CString* cString_ptr;
  Element* element_ptr;
  for (unsigned int i = 0; 
       i < n_entries;
       i++)
       {
         if (element_ptr = linked_List_ptr->delete_head())
         {
           cString_ptr = element_ptr->value();
           delete cString_ptr;
           delete element_ptr;
         }
       }
  delete linked_List_ptr;

#else
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR ((LM_ERROR, 
              ASYS_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Entry<MAP_STRING, MAP_STRING>;
template class ACE_Hash_Map_Manager<MAP_STRING, MAP_STRING, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<MAP_STRING, MAP_STRING, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base<MAP_STRING, MAP_STRING, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Entry<MAP_STRING, MAP_STRING>
#pragma instantiate ACE_Hash_Map_Manager<MAP_STRING, MAP_STRING, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<MAP_STRING, MAP_STRING, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base<MAP_STRING, MAP_STRING, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
