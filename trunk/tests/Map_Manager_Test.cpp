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

#include "ace/Map_Manager.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Profile_Timer.h"
#include "ace/Synch.h"
#include "test_config.h"
#include "Map_Manager_Test_Key.h"

typedef ACE_Null_Mutex MUTEX;
typedef size_t VALUE;

typedef ACE_Map_Manager <KEY, VALUE, MUTEX> MAP_MANAGER;
typedef ACE_Map_Iterator <KEY, VALUE, MUTEX> ITERATOR;
typedef ACE_Map_Reverse_Iterator <KEY, VALUE, MUTEX> REVERSE_ITERATOR;
typedef ACE_Map_Entry <KEY, VALUE> ENTRY;
typedef ACE_Hash_Map_Manager <KEY, VALUE, MUTEX> HASH_MAP_MANAGER;
typedef ACE_Hash_Map_Iterator <KEY, VALUE, MUTEX> HASH_ITERATOR;
typedef ACE_Hash_Map_Entry <KEY, VALUE> HASH_ENTRY;

static void 
test_hash_map_manager (size_t table_size, size_t iterations)
{
  HASH_MAP_MANAGER map (table_size);
  VALUE i;
  VALUE j;

  for (i = 0; i < iterations; i++)
    ACE_ASSERT (map.bind (KEY (i), i) != -1);

  {
    HASH_ITERATOR iterator (map);  

    for (HASH_ENTRY *entry = 0;
	 iterator.next (entry) != 0;
	 iterator.advance ())
      continue;
  }

  {
    HASH_MAP_MANAGER::ITERATOR iterator (map);

    for (HASH_MAP_MANAGER::ENTRY *entry = 0;
	 iterator.next (entry) != 0;
	 iterator.advance ())
      continue;
  }

  for (i = 0; i < iterations; i++)
    {
      ACE_ASSERT (map.find (KEY (i), j) != -1);
      ACE_ASSERT (i == j);
    }

  for (i = 0; i < iterations; i++)
    ACE_ASSERT (map.unbind (KEY (i)) != -1);

  ACE_ASSERT (map.current_size () == 0);
}

static void
test_map_manager (size_t table_size, size_t iterations)
{
  MAP_MANAGER map (table_size);
  VALUE i;
  VALUE j;

  for (i = 0; i < iterations; i++)
    ACE_ASSERT (map.bind (KEY (i), i) != -1);

  {
    ITERATOR iterator (map);  

    for (ENTRY *entry = 0;
	 iterator.next (entry) != 0;
	 iterator.advance ())
      continue;
  }

  {
    REVERSE_ITERATOR iterator (map);

    for (ENTRY *entry = 0;
	 iterator.next (entry) != 0;
	 iterator.advance ())
      continue;
  }

  {
    MAP_MANAGER::ITERATOR iterator (map);

    for (MAP_MANAGER::ENTRY *entry = 0;
	 iterator.next (entry) != 0;
	 iterator.advance ())
      continue;
  }

  {
    MAP_MANAGER::REVERSE_ITERATOR iterator (map);

    for (MAP_MANAGER::ENTRY *entry = 0;
	 iterator.next (entry) != 0;
	 iterator.advance ())
      continue;
  }

  for (i = 0; i < iterations; i++)
    {
      ACE_ASSERT (map.find (KEY (i), j) != -1);
      ACE_ASSERT (i == j);
    }

  for (i = 0; i < iterations; i++)
    ACE_ASSERT (map.unbind (KEY (i)) != -1);

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

  ACE_DEBUG ((LM_DEBUG, "time to test a %d item map for %d iterations using %s\n", 
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

  size_t table_size = ACE_MAX_ITERATIONS;
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

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Hash_Map_Manager<KEY, VALUE, MUTEX>;
template class ACE_Hash_Map_Iterator<KEY, VALUE, MUTEX>;
template class ACE_Hash_Map_Entry<KEY, VALUE>;
template class ACE_Map_Manager<KEY, VALUE, MUTEX>;
template class ACE_Map_Iterator<KEY, VALUE, MUTEX>;
template class ACE_Map_Reverse_Iterator<KEY, VALUE, MUTEX>;
template class ACE_Map_Entry<KEY, VALUE>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
