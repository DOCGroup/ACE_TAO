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
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Profile_Timer.h"


#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

#if defined (ACE_HAS_THREADS)

// Default number of iterations.
#if defined (ACE_HAS_WINCE)
static size_t n_iterations = 50;
#else
static size_t n_iterations = 50;
#endif /* ACE_HAS_WINCE */

// Default number of loops.
static size_t n_loops = 100;

// Default number of readers.
static size_t n_readers = 6;

// Default number of writers.
static size_t n_writers = 4;

// Thread id of last writer.
static ACE_thread_t shared_data;

// Lock for shared_data.
static ACE_RW_Thread_Mutex rw_mutex;

// Number of entries in the hash map
static unsigned int n_entries = 10;

// try to upgrade to a write lock, by default don't try
static unsigned int use_try_upgrade = 0;

static unsigned int upgraded = 0;
static unsigned int not_upgraded = 0;


// Count of the number of readers and writers.
static ACE_Atomic_Op<ACE_Thread_Mutex, int> current_readers;
static ACE_Atomic_Op<ACE_Thread_Mutex, int> current_writers;

typedef ACE_Hash_Map_Entry<ACE_CString, ACE_CString> Hash_Map_Entry;
typedef ACE_Hash_Map_Manager<ACE_CString, ACE_CString, ACE_Null_Mutex> Hash_Map;
typedef ACE_Hash_Map_Iterator<ACE_CString, ACE_CString, ACE_Null_Mutex> Hash_Map_Iterator;

Hash_Map *hash_Map_ptr;

// forward declaration
static int find (const char * search_string);


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
  //ACE_DEBUG ((LM_DEBUG, ASYS_TEXT (" (%t) reader starting\n")));

  // We use a random pause, around 2msec with 1msec jittering.
  int usecs = 1000;
  ACE_Time_Value pause(0, usecs);

  for (size_t iterations = 1; iterations <= n_iterations; iterations++)
    {
  //    ACE_OS::sleep(pause);
      int result;
      {
        ACE_Read_Guard<ACE_RW_Thread_Mutex> g (rw_mutex);

        find ("100");


        result = rw_mutex.tryacquire_write_upgrade ();
     
        if (result == 0 && use_try_upgrade) // true, when we were able to upgrade
        {
          find ("100");
          /*ACE_DEBUG ((LM_DEBUG,
                      "(%t) upgraded to write lock!\n"));*/

          upgraded++;

          // we were a writer
          return 0;
        }
      }

      ACE_Thread::yield ();      

      if ((result == -1 && errno == EBUSY) || !use_try_upgrade)
        {
          ACE_Write_Guard<ACE_RW_Thread_Mutex> g (rw_mutex);

          not_upgraded++;

          find ("100");

          /*ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("(%t) could not upgrade to write lock!\n")));*/
        }
      else // result == -1
        {
          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT ("(%t) failure in upgrading to write lock!\n"),
                      1));
        }
      //ACE_DEBUG ((LM_DEBUG, ASYS_TEXT (" (%t) done with reading guarded data\n")));

      /*ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%t) reader finished %d iterations at %T\n"),
                  iterations)); */
    }
  return 0;
}

// Iterate <n_iterations> each time modifying the global data
// and checking that nobody steps on it while we can write it.

static void *
writer (void *)
{
  // ACE_DEBUG ((LM_DEBUG, ASYS_TEXT (" (%t) writer starting\n")));

  // We use a random pause, around 2msec with 1msec jittering.
  int usecs = 1000;
  ACE_Time_Value pause(0, usecs);

  for (size_t iterations = 1; iterations <= n_iterations; iterations++)
    {
      //ACE_OS::sleep(pause);

      ACE_Write_Guard<ACE_RW_Thread_Mutex> g (rw_mutex);

      find ("100");
      ACE_Thread::yield ();

      current_writers--;
       
      /*ACE_DEBUG((LM_DEBUG, ASYS_TEXT (" (%t) write %d done at %T\n"), iterations));*/
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


static int 
find (const char * search_string)
{

  ACE_CString cString1 (search_string);
  ACE_CString cString2;
  if (hash_Map_ptr->find (cString1, cString2) == 0)
    return 0;
  else 
    return 1;

  /*
  Hash_Map_Entry *entry;
  unsigned int i = 0;
  

  for (Hash_Map_Iterator hash_Map_Iterator (hash_Map);
       hash_Map_Iterator.next (entry) != 0;
       hash_Map_Iterator.advance (), i++)
  {
     ACE_DEBUG ((LM_DEBUG, 
                 ASYS_TEXT ("iterating (%d): [%s, %s]\n"),
                 i,
                 (ASYS_TCHAR *) entry->ext_id_.c_str(),
                 (ASYS_TCHAR *) entry->int_id_.c_str()));

     if (entry->ext_id_.compare (ACE_CString (search_string)) == 0)
       return 0;
  }*/
  return 1;
}


int 
init (unsigned int kind)
{
  ACE_NEW_RETURN (hash_Map_ptr, 
                 Hash_Map (n_entries), 
                 -1);

  switch (kind)
  {
  default:
  case 0:
    char entry[100]; // the space will be reused several times,
                    // we can do this because the map copies the strings
    ACE_CString cString1, cString2;
    unsigned int i;

    for (i = 0; i < n_entries; i++)
    {
      itoa(i,entry,10);
      cString1.set (/*entry*/"100");
      cString2.set (ACE_OS::strcat (entry,"X"));
      hash_Map_ptr->bind (cString1, cString2);
    }    
    break;                     
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
  ACE_START_TEST (ASYS_TEXT ("Reader_Writer_Test"));

#if defined (ACE_HAS_THREADS)
  parse_args (argc, argv);

  current_readers = 0; // Possibly already done
  current_writers = 0; // Possibly already done

  init (0);

  ACE_DEBUG ((LM_DEBUG, 
              ASYS_TEXT (" (%t) main thread starting\n")));

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

  // stop the timer.
  timer.stop ();
  timer.elapsed_time (elapsed_time);

  // compute average time.
  print_stats (elapsed_time, 1);

  if (not_upgraded != 0 || upgraded != 0)
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("upgraded to not upgraded ratio = %f \n"),
                (float)upgraded/(float)(not_upgraded+upgraded)));

  ACE_DEBUG ((LM_DEBUG, 
              ASYS_TEXT (" (%t) exiting main thread\n")));

  delete hash_Map_ptr;

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
