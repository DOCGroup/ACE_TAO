// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Map_Manager_Test.cpp
//
// = DESCRIPTION
//     This is a simple test of the <ACE_Map_Manager> and
//     <ACE_Hash_Map_Manager> that illustrates how to use the forward
//     and reverse iterators.
//
// = AUTHOR
//    Irfan Pyarali and Douglas C. Schmidt
//
// ============================================================================

#include "test_config.h"

#if defined (ACE_HAS_TEMPLATE_SPECIALIZATION)

#include "ace/Map_Manager.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Profile_Timer.h"
#include "ace/Synch.h"

ACE_RCSID(tests, Map_Manager_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

typedef ACE_Null_Mutex MUTEX;
typedef size_t TYPE;

#if defined (ACE_HAS_TEMPLATE_SPECIALIZATION)

// We need this template specialization since TYPE is defined as a
// size_t, which doesn't have a hash() method defined on it.  Template
// specialization is a powerful C++ feature that makes it possible to
// customize the implementation of designated template methods to
// either (1) improve performance or (2) support built-in types that
// lack "methods" (as in this case, where size_t lacks hash()).

u_long
ACE_Hash_Map_Manager<TYPE, TYPE, MUTEX>::hash (const TYPE& ext_id)
{
  return ext_id;
}

#endif /* ACE_HAS_TEMPLATE_SPECIALIZATION */

typedef ACE_Map_Manager <TYPE, TYPE, MUTEX> MAP_MANAGER;
typedef ACE_Map_Iterator <TYPE, TYPE, MUTEX> ITERATOR;
typedef ACE_Map_Reverse_Iterator <TYPE, TYPE, MUTEX> REVERSE_ITERATOR;
typedef ACE_Map_Entry <TYPE, TYPE> ENTRY;
typedef ACE_Hash_Map_Manager <TYPE, TYPE, MUTEX> HASH_MAP_MANAGER;
typedef ACE_Hash_Map_Iterator <TYPE, TYPE, MUTEX> HASH_ITERATOR;
typedef ACE_Hash_Map_Reverse_Iterator <TYPE, TYPE, MUTEX> HASH_REVERSE_ITERATOR;
typedef ACE_Hash_Map_Entry <TYPE, TYPE> HASH_ENTRY;

static void
test_hash_map_manager (size_t table_size, size_t iterations)
{
  HASH_MAP_MANAGER map (table_size);
  TYPE i;
  TYPE j;
  ssize_t k;

  for (i = 0; i < iterations; i++)
    ACE_ASSERT (map.bind (i, i) != -1);

  {
    i = 0;

    for (HASH_ITERATOR iter = map.begin (), end = map.end ();
         iter != end;
         iter++, i++)
      {
        HASH_ENTRY &entry = *iter;
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("(%d|%d|%d)"),
                    i,
                    entry.ext_id_,
                    entry.int_id_));
      }

    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
    ACE_ASSERT (i == iterations);
  }

  {
    HASH_ITERATOR iterator (map);

    HASH_ENTRY *entry = 0;

    for (entry = 0, i = 0;
         iterator.next (entry) != 0;
         iterator.advance (), i++)
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%d|%d|%d)"),
                  i,
                  entry->ext_id_,
                  entry->int_id_));

    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
    ACE_ASSERT (i == iterations);
  }

  {
    k = iterations - 1;

    for (HASH_REVERSE_ITERATOR iter = map.rbegin (), rend = map.rend ();
         iter != rend;
         iter++, k--)
      {
        HASH_ENTRY &entry = *iter;
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("(%d|%d|%d)"),
                    k,
                    entry.ext_id_,
                    entry.int_id_));
      }

    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
    ACE_ASSERT (k == -1);
  }

  {
    HASH_REVERSE_ITERATOR iterator (map);

    HASH_ENTRY *entry = 0;
    for (entry = 0, k = iterations - 1;
         iterator.next (entry) != 0;
         iterator.advance (), k--)
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%d|%d|%d)"),
                  k,
                  entry->ext_id_,
                  entry->int_id_));
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
    ACE_ASSERT (k == -1);
  }

  {
    i = 0;

    for (HASH_MAP_MANAGER::iterator iter = map.begin (), end = map.end ();
         iter != end;
         iter++, i++)
      {
        HASH_MAP_MANAGER::ENTRY &entry = *iter;
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("(%d|%d|%d)"),
                    i,
                    entry.ext_id_,
                    entry.int_id_));
      }
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
    ACE_ASSERT (i == iterations);
  }
  {
    HASH_MAP_MANAGER::ITERATOR iterator (map);

    HASH_MAP_MANAGER::ENTRY *entry = 0;
    for (entry = 0, i = 0;
         iterator.next (entry) != 0;
         iterator.advance (), i++)
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%d|%d|%d)"),
                  i,
                  entry->ext_id_,
                  entry->int_id_));
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
    ACE_ASSERT (i == iterations);
  }

  {
    k = iterations - 1;

    for (HASH_MAP_MANAGER::reverse_iterator iter = map.rbegin (), rend = map.rend ();
         iter != rend;
         iter++, k--)
      {
        HASH_MAP_MANAGER::ENTRY &entry = *iter;
        ACE_UNUSED_ARG (entry);
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("(%d|%d|%d)"),
                    k,
                    entry.ext_id_,
                    entry.int_id_));
      }
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
    ACE_ASSERT (k == -1);
  }
  {
    HASH_MAP_MANAGER::REVERSE_ITERATOR iterator (map);

    HASH_MAP_MANAGER::ENTRY *entry = 0;
    for (entry = 0, k = iterations - 1;
         iterator.next (entry) != 0;
         iterator.advance (), k--)
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%d|%d|%d)"),
                  k,
                  entry->ext_id_,
                  entry->int_id_));
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
    ACE_ASSERT (k == -1);
  }

  for (i = 0; i < iterations; i++)
    {
      ACE_ASSERT (map.find (i, j) != -1);
      ACE_ASSERT (i == j);
    }

  for (i = 0; i < iterations; i++)
    ACE_ASSERT (map.unbind (i) != -1);

  ACE_ASSERT (map.current_size () == 0);
}

static void
test_map_manager (size_t table_size, size_t iterations)
{
  MAP_MANAGER map (table_size);
  TYPE i;
  TYPE j;
  ssize_t k;

  for (i = 0; i < iterations; i++)
    ACE_ASSERT (map.bind (i, i) != -1);

  {
    i = 0;
    for (ITERATOR iter = map.begin (), end = map.end ();
         iter != end;
         i++, iter ++)
      {
        ENTRY &entry = *iter;
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("(%d|%d|%d)"),
                    i,
                    entry.ext_id_,
                    entry.int_id_));
      }
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
    ACE_ASSERT (i == iterations);
  }
  {
    ITERATOR iterator (map);
    ENTRY *entry = 0;
    for (entry = 0, i = 0;
         iterator.next (entry) != 0;
         iterator.advance (), i++)
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%d|%d|%d)"),
                  i,
                  entry->ext_id_,
                  entry->int_id_));
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
    ACE_ASSERT (i == iterations);
  }

  {
    ENTRY entry;
    k = iterations - 1;
    for (REVERSE_ITERATOR iter = map.rbegin (), rend = map.rend ();
         iter != rend;
         k--, iter ++)
      {
        entry = *iter;
        ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("%u "), entry.int_id_));
      }
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
    ACE_ASSERT (k == -1);
  }
  {
    REVERSE_ITERATOR iterator (map);

    ENTRY *entry = 0;
    for (entry = 0, k = iterations - 1;
         iterator.next (entry) != 0;
         iterator.advance (), k--)
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%d|%d|%d)"),
                  k,
                  entry->ext_id_,
                  entry->int_id_));
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
    ACE_ASSERT (k == -1);
  }
  {
    MAP_MANAGER::ENTRY entry;
    i = 0;
    for (MAP_MANAGER::iterator iter = map.begin (), end = map.end ();
         iter != end;
         i++, iter ++)
      {
        entry = *iter;
        ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("%u "), entry.int_id_));
      }
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
    ACE_ASSERT (i == iterations);
  }
  {
    MAP_MANAGER::ITERATOR iterator (map);

    MAP_MANAGER::ENTRY *entry = 0;
    for (entry = 0, i = 0;
         iterator.next (entry) != 0;
         iterator.advance (), i++)
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%d|%d|%d)"),
                  i,
                  entry->ext_id_,
                  entry->int_id_));
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
    ACE_ASSERT (i == iterations);
  }
  {
    ENTRY entry;
    k = iterations - 1;

    for (MAP_MANAGER::reverse_iterator iter = map.rbegin (), rend = map.rend ();
         iter != rend;
         k--, iter ++)
      {
        entry = *iter;
        ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("%u "), entry.int_id_));
      }
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
    ACE_ASSERT (k == -1);
  }

  {
    MAP_MANAGER::REVERSE_ITERATOR iterator (map);

    MAP_MANAGER::ENTRY *entry = 0;
    for (entry = 0, k = iterations - 1;
         iterator.next (entry) != 0;
         iterator.advance (), k--)
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%d|%d|%d)"),
                  k,
                  entry->ext_id_,
                  entry->int_id_));
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\n")));
    ACE_ASSERT (k == -1);
  }

  for (i = 0; i < iterations; i++)
    {
      ACE_ASSERT (map.find (i, j) != -1);
      ACE_ASSERT (i == j);
    }

  for (i = 0; i < iterations; i++)
    ACE_ASSERT (map.unbind (i) != -1);

  ACE_ASSERT (map.current_size () == 0);
}

static void
run_test (void (*ptf) (size_t, size_t),
          size_t table_size,
          size_t iterations,
          const ASYS_TCHAR *test_name)
{
  ACE_Profile_Timer timer;
  timer.start ();

  (*ptf) (table_size, iterations);

  timer.stop ();

  ACE_Profile_Timer::ACE_Elapsed_Time et;

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("time to test a map of size %d for %d iterations using %s\n"),
              table_size,
              iterations,
              test_name));

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("real time = %f secs, user time = %f secs, system time = %f secs\n"),
              et.real_time,
              et.user_time,
              et.system_time));

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("time per call = %f usecs\n"),
              (et.real_time / double (iterations)) * 1000000));
}

int
main (int argc, ASYS_TCHAR *argv[])
{
  ACE_START_TEST (ASYS_TEXT ("Map_Manager_Test"));
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::VERBOSE_LITE);

  size_t table_size = ACE_MAX_ITERATIONS / 2;
  size_t iterations = ACE_MAX_ITERATIONS;

  if (argc > 1)
    table_size = ACE_OS::atoi (argv[1]);

  if (argc > 2)
    iterations = ACE_OS::atoi (argv[2]);

  // Test the <ACE_Hash_Map_Manager>.
  run_test (&test_hash_map_manager,
            table_size,
            iterations,
            ASYS_TEXT ("Hash_Map_Manager"));

  // Test the <ACE_Map_Manager>.
  run_test (&test_map_manager,
            table_size,
            iterations,
            ASYS_TEXT ("Map_Manager"));

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::VERBOSE_LITE);
  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Manager<TYPE, TYPE, MUTEX>;
template class ACE_Hash_Map_Iterator_Base<TYPE, TYPE, MUTEX>;
template class ACE_Hash_Map_Iterator<TYPE, TYPE, MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<TYPE, TYPE, MUTEX>;
template class ACE_Hash_Map_Entry<TYPE, TYPE>;
template class ACE_Map_Manager<TYPE, TYPE, MUTEX>;
template class ACE_Map_Iterator_Base<TYPE, TYPE, MUTEX>;
template class ACE_Map_Iterator<TYPE, TYPE, MUTEX>;
template class ACE_Map_Reverse_Iterator<TYPE, TYPE, MUTEX>;
template class ACE_Map_Entry<TYPE, TYPE>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Manager<TYPE, TYPE, MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base<TYPE, TYPE, MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<TYPE, TYPE, MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TYPE, TYPE, MUTEX>
#pragma instantiate ACE_Hash_Map_Entry<TYPE, TYPE>
#pragma instantiate ACE_Map_Manager<TYPE, TYPE, MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<TYPE, TYPE, MUTEX>
#pragma instantiate ACE_Map_Iterator<TYPE, TYPE, MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<TYPE, TYPE, MUTEX>
#pragma instantiate ACE_Map_Entry<TYPE, TYPE>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else  /* ACE_HAS_TEMPLATE_SPECIALIZATION */

int
main (int argc, ASYS_TCHAR *argv[])
{
  ACE_START_TEST (ASYS_TEXT ("Map_Manager_Test"));

  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR ((LM_ERROR, ASYS_TEXT ("Template specializations not supported on this platform\n")));

  ACE_END_TEST;
  return 0;
}

#endif /* ACE_HAS_TEMPLATE_SPECIALIZATION */
