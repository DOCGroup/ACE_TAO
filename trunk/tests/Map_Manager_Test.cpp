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
//      This is a simple test of the ACE_Map_Manager and
//      ACE_Hash_Map_Manager that illustrates how to use the forward
//      and reverse iterators.
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

typedef ACE_Null_Mutex MUTEX;
typedef size_t TYPE;


#if defined (ACE_HAS_TEMPLATE_SPECIALIZATION)

// We need this template specialization since TYPE is defined as a
// size_t, which doesn't have a hash() method defined on it.

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

  for (i = 0; i < iterations; i++)
    ACE_ASSERT (map.bind (i, i) != -1);

#if 1
  {
    i = 0;

    for (HASH_ITERATOR iter = map.begin ();
	 iter != map.end ();
	 iter++, i++)
      {
	HASH_ENTRY &item = *iter;
        ACE_UNUSED_ARG (item);
	ACE_DEBUG ((LM_DEBUG, "%d ", i));
      }
    ACE_DEBUG ((LM_DEBUG, "\n"));
  }
#else
  {
    HASH_ITERATOR iterator (map);

    HASH_ENTRY *entry = 0;
    for (entry = 0, i = 0;
	 iterator.next (entry) != 0;
	 iterator.advance (), i++)
      ACE_DEBUG ((LM_DEBUG, "%d ", i));
    ACE_DEBUG ((LM_DEBUG, "\n"));
  }
#endif /* 1 */

#if 1
  {
    i = iterations - 1;

    for (HASH_REVERSE_ITERATOR iter = map.rbegin ();
	 iter != map.rend ();
	 iter++, i--)
      {
	HASH_ENTRY &item = *iter;
        ACE_UNUSED_ARG (item);
	ACE_DEBUG ((LM_DEBUG, "%d ", i));
      }
    ACE_DEBUG ((LM_DEBUG, "\n"));
  }
#else
  {
    HASH_REVERSE_ITERATOR iterator (map);

    HASH_ENTRY *entry = 0;
    for (entry = 0, i = iterations - 1;
	 iterator.next (entry) != 0;
	 iterator.advance (), i--)
      ACE_DEBUG ((LM_DEBUG, "%d ", i));
    ACE_DEBUG ((LM_DEBUG, "\n"));
  }
#endif /* 1 */

#if 1
  {
    i = 0;

    for (HASH_MAP_MANAGER::iterator iter = map.begin ();
	 iter != map.end ();
	 iter++, i++)
      {
	HASH_MAP_MANAGER::ENTRY &item = *iter;
        ACE_UNUSED_ARG (item);
	ACE_DEBUG ((LM_DEBUG, "%d ", i));
      }
    ACE_DEBUG ((LM_DEBUG, "\n"));
  }
#else
  {
    HASH_MAP_MANAGER::ITERATOR iterator (map);

    HASH_MAP_MANAGER::ENTRY *entry = 0;
    for (entry = 0, i = 0;
	 iterator.next (entry) != 0;
	 iterator.advance (), i++)
      ACE_DEBUG ((LM_DEBUG, "%d ", i));
    ACE_DEBUG ((LM_DEBUG, "\n"));
  }
#endif /* 1 */

#if 1
  {
    i = iterations - 1;

    for (HASH_MAP_MANAGER::reverse_iterator iter = map.rbegin ();
	 iter != map.rend ();
	 iter++, i--)
      {
	HASH_MAP_MANAGER::ENTRY &item = *iter;
        ACE_UNUSED_ARG (item);
	ACE_DEBUG ((LM_DEBUG, "%d ", i));
      }
    ACE_DEBUG ((LM_DEBUG, "\n"));
  }
#else
  {
    HASH_MAP_MANAGER::REVERSE_ITERATOR iterator (map);

    HASH_MAP_MANAGER::ENTRY *entry = 0;
    for (entry = 0, i = iterations - 1;
	 iterator.next (entry) != 0;
	 iterator.advance (), i--)
      ACE_DEBUG ((LM_DEBUG, "%d ", i));
    ACE_DEBUG ((LM_DEBUG, "\n"));
  }
#endif /* 1 */

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

  for (i = 0; i < iterations; i++)
    ACE_ASSERT (map.bind (i, i) != -1);

#if 1
  {
    ENTRY item;
	i = 0;
    for (ITERATOR iter = map.begin ();
         iter != map.end (); i++, iter ++)
      {
        item = *iter;
        ACE_DEBUG ((LM_DEBUG, "%d ", i));
      }
    ACE_DEBUG ((LM_DEBUG, "\n"));
  }
#else
  {
    ITERATOR iterator (map);
    ENTRY *entry = 0;
    for (entry = 0, i = 0;
 	 iterator.next (entry) != 0;
 	 iterator.advance (), i++)
      ACE_DEBUG ((LM_DEBUG, "%d ", i));
    ACE_DEBUG ((LM_DEBUG, "\n"));
  }
#endif /* 1 */

#if 1
  {
    ENTRY item;
	i = iterations - 1;
    for (REVERSE_ITERATOR iter = map.rbegin ();
         iter != map.rend (); i--, iter ++)
      {
        item = *iter;
        ACE_DEBUG ((LM_DEBUG, "%d ", i));
      }
    ACE_DEBUG ((LM_DEBUG, "\n"));
  }
#else
  {
    REVERSE_ITERATOR iterator (map);

    ENTRY *entry = 0;
    for (entry = 0, i = iterations - 1;
	 iterator.next (entry) != 0;
	 iterator.advance (), i--)
      ACE_DEBUG ((LM_DEBUG, "%d ", i));
    ACE_DEBUG ((LM_DEBUG, "\n"));
  }
#endif /* 1 */

#if 1
  {
    MAP_MANAGER::ENTRY item;
	i = 0;
    for (MAP_MANAGER::iterator iter = map.begin ();
         iter != map.end (); i++, iter ++)
      {
        item = *iter;
        ACE_DEBUG ((LM_DEBUG, "%d ", i));
      }
    ACE_DEBUG ((LM_DEBUG, "\n"));
  }
#else
  {
    MAP_MANAGER::ITERATOR iterator (map);

    MAP_MANAGER::ENTRY *entry = 0;
    for (entry = 0, i = 0;
	 iterator.next (entry) != 0;
	 iterator.advance (), i++)
      ACE_DEBUG ((LM_DEBUG, "%d ", i));
    ACE_DEBUG ((LM_DEBUG, "\n"));
  }
#endif /* 1 */

#if 1
  {
    ENTRY item;
	i = iterations - 1;
    for (MAP_MANAGER::reverse_iterator iter = map.rbegin ();
         iter != map.rend (); i--, iter ++)
      {
        item = *iter;
        ACE_DEBUG ((LM_DEBUG, "%d ", i));
      }
    ACE_DEBUG ((LM_DEBUG, "\n"));
  }
#else
  {
    MAP_MANAGER::REVERSE_ITERATOR iterator (map);

    MAP_MANAGER::ENTRY *entry = 0;
    for (entry = 0, i = iterations - 1;
	 iterator.next (entry) != 0;
	 iterator.advance (), i--)
      ACE_DEBUG ((LM_DEBUG, "%d ", i));
    ACE_DEBUG ((LM_DEBUG, "\n"));
  }
#endif /* 1 */

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
	  const char *test_name)
{
  ACE_Profile_Timer timer;
  timer.start ();

  (*ptf) (table_size, iterations);

  timer.stop ();

  ACE_Profile_Timer::ACE_Elapsed_Time et;

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG, "time to test a %d item map for %d iterations using %s\
n",
	      table_size, iterations, test_name));
  ACE_DEBUG ((LM_DEBUG, "real time = %f secs, user time = %f secs, system time = %f secs\n",
	    et.real_time, et.user_time, et.system_time));
  ACE_DEBUG ((LM_DEBUG, "time per call = %f usecs\n",
	      (et.real_time / double (iterations)) * 1000000));
}

int
main (int argc, char *argv[])
{
  ACE_START_TEST ("Map_Manager_Test");

  size_t table_size = ACE_MAX_ITERATIONS / 2;
  size_t iterations = ACE_MAX_ITERATIONS;

  if (argc > 1)
    table_size = ACE_OS::atoi (argv[1]);
  if (argc > 2)
    iterations = ACE_OS::atoi (argv[2]);

  run_test (&test_hash_map_manager, table_size, iterations, "Hash_Map_Manager");
  run_test (&test_map_manager, table_size, iterations, "Map_Manager");

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
main (int argc, char *argv[])
{
  ACE_START_TEST ("Map_Manager_Test");

  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_ERROR ((LM_ERROR, "Template specializations not supported on this platform\n"));

  ACE_END_TEST;
  return 0;
}

#endif /* ACE_HAS_TEMPLATE_SPECIALIZATION */
