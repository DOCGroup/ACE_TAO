// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Map_Test.cpp
//
// = DESCRIPTION
//     This is a simple test of the <ACE_Map> and illustrates how to
//     use the forward and reverse iterators.
//
// = AUTHOR
//    Irfan Pyarali <irfan@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "Map_Test.h"
#include "ace/Map_T.h"
#include "ace/Profile_Timer.h"



#undef THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
#define THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL(X) \
  ((X)                                                          \
   ? static_cast<void>(0)                                       \
   : ACE_VERSIONED_NAMESPACE_NAME::__ace_assert(__FILE__, __LINE__, ACE_TEXT_CHAR_TO_TCHAR (#X)))

// Value type.
typedef size_t VALUE;

// Generic map type.
typedef ACE_Map<KEY, VALUE> TEST_MAP;

// Manager Manager adapter.
typedef ACE_Map_Manager_Adapter<KEY, VALUE, Key_Generator> MAP_MANAGER_ADAPTER;

// Hash Manager Manager adapter.
typedef ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, Hash_Key, ACE_Equal_To<KEY>, Key_Generator> HASH_MAP_MANAGER_ADAPTER;

// Active Manager Manager adapter.
typedef ACE_Active_Map_Manager_Adapter<KEY, VALUE, Key_Adapter> ACTIVE_MAP_MANAGER_ADAPTER;

static void
functionality_test (TEST_MAP &map,
                    size_t iterations)
{
  size_t counter;
  VALUE i;
  KEY *original_keys = new KEY[iterations];
  KEY *modified_keys = new KEY[iterations];

  // Setup the keys to have some initial data.
  for (i = 0;
       i < iterations;
       ++i)
    {
      original_keys[i].size (sizeof i / sizeof (KEY::TYPE));
      ACE_OS::memcpy (&original_keys[i][0],
                      &i,
                      sizeof i);
    }

  // Make a copy of the keys so that we can compare with the original
  // keys later.
  for (i = 0; i < iterations; ++i)
    {
      modified_keys[i] = original_keys[i];
    }

  // Add to the map, allowing keys to be modified.
  counter = 0;
  for (i = 0; i < iterations; ++i)
    {
      THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (map.bind_modify_key (i, modified_keys[i]) == 0);
      ++counter;
      THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (map.current_size () == counter);
    }

  // Forward iteration...
  {
    counter = 0;
    TEST_MAP::iterator end = map.end ();

    for (TEST_MAP::iterator iter = map.begin ();
         iter != end;
         ++iter, ++counter)
      {
        TEST_MAP::value_type entry = *iter;

        // Recover original key.
        KEY original_key;
        THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (map.recover_key (entry.first (),
                                     original_key) == 0);

        // Make sure recovering keys work.
        THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (original_keys[entry.second ()] == original_key);

        // Obtain value from key.
        VALUE original_value;
        ACE_OS::memcpy (&original_value,
                        &original_key[0],
                        sizeof original_value);

        // Debugging info.
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%d|%d|%d)"),
                    counter,
                    original_value,
                    entry.second ()));
      }

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\n")));
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (counter == iterations);
  }

  // Reverse iteration...
  {
    counter = iterations;
    TEST_MAP::reverse_iterator end = map.rend ();

    for (TEST_MAP::reverse_iterator iter = map.rbegin ();
         iter != end;
         ++iter)
      {
        --counter;
        TEST_MAP::value_type entry = *iter;

        // Recover original key.
        KEY original_key;
        THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (map.recover_key (entry.first (),
                                     original_key) == 0);

        // Make sure recovering keys work.
        THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (original_keys[entry.second ()] == original_key);

        // Obtain value from key.
        VALUE original_value;
        ACE_OS::memcpy (&original_value,
                        &original_key[0],
                        sizeof original_value);

        // Debugging info.
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%d|%d|%d)"),
                    counter,
                    original_value,
                    entry.second ()));
      }

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("\n")));
    THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (counter == 0);
  }

  // Search using the modified keys.
  for (i = 0; i < iterations; ++i)
    {
      VALUE j;
      THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (map.find (modified_keys[i], j) != -1);
      THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (i == j);
    }

  // Rmoved keys from map.
  counter = iterations;
  for (i = 0; i < iterations; ++i)
    {
      THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (map.unbind (modified_keys[i]) != -1);
      --counter;
      THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (map.current_size () == counter);
    }

  // Cleanup.
  delete[] modified_keys;
  delete[] original_keys;
}

static void
insert_test (TEST_MAP &map,
             size_t iterations,
             KEY *keys)
{
  // Add to the map, allowing keys to be created by the map.
  size_t counter = 0;
  for (VALUE i = 0; i < iterations; ++i)
    {
      THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (map.bind_create_key (i, keys[i]) == 0);
      ++counter;
      THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (map.current_size () == counter);
    }
}

static void
find_test (TEST_MAP &map,
           size_t iterations,
           KEY *keys)
{
  // Find system generated keys.
  for (VALUE i = 0; i < iterations; ++i)
    {
      VALUE j;
      THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (map.find (keys[i], j) != -1);
      THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (i == j);
    }
}

static void
unbind_test (TEST_MAP &map,
             size_t iterations,
             KEY *keys)
{
  // Remove system generated keys.
  size_t counter = iterations;
  for (VALUE i = 0; i < iterations; ++i)
    {
      THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (map.unbind (keys[i]) != -1);
      --counter;
      THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL (map.current_size () == counter);
    }
}

static void
performance_test (void (*ptf) (TEST_MAP &, size_t, KEY *),
                  TEST_MAP &map,
                  size_t iterations,
                  KEY *keys,
                  size_t table_size,
                  const ACE_TCHAR *test_name)
{
  ACE_Profile_Timer timer;
  timer.start ();

  (*ptf) (map, iterations, keys);

  timer.stop ();

  ACE_Profile_Timer::ACE_Elapsed_Time et;

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("time to run %s of size %d for %d iterations\n"),
              test_name,
              table_size,
              iterations));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("real time = %f secs, user time = %f secs, system time = %f secs\n"),
              et.real_time,
              et.user_time,
              et.system_time));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("time per call = %f usecs\n"),
              (et.real_time / ACE_timer_t (iterations)) * 1000000));
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Map_Test"));
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::VERBOSE_LITE);

  size_t table_size = ACE_MAX_ITERATIONS / 2;
  size_t iterations = ACE_MAX_ITERATIONS;
  size_t functionality_tests = 1;

  if (argc > 1)
    functionality_tests = ACE_OS::atoi (argv[1]);

  if (argc > 2)
    table_size = ACE_OS::atoi (argv[2]);

  if (argc > 3)
    iterations = ACE_OS::atoi (argv[3]);

  MAP_MANAGER_ADAPTER map1 (table_size);
  HASH_MAP_MANAGER_ADAPTER map2 (table_size);
  ACTIVE_MAP_MANAGER_ADAPTER map3 (table_size);

  if (functionality_tests)
    {
      // Functionality test of the maps.
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nMap Manager functionality test\n")));
      functionality_test (map1, iterations);

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nHash Map Manager functionality test\n")));
      functionality_test (map2, iterations);

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nActive Map Manager functionality test\n")));
      functionality_test (map3, iterations);

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));
    }

  // Performance test of the maps.
  KEY *keys = new KEY[iterations];

  // Map Manager
  performance_test (&insert_test,
                    map1,
                    iterations,
                    keys,
                    table_size,
                    ACE_TEXT ("Map Manager (insert test)"));
  performance_test (&find_test,
                    map1,
                    iterations,
                    keys,
                    table_size,
                    ACE_TEXT ("Map Manager (find test)"));
  performance_test (&unbind_test,
                    map1,
                    iterations,
                    keys,
                    table_size,
                    ACE_TEXT ("Map Manager (unbind test)"));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));

  // Hash Map Manager
  performance_test (&insert_test,
                    map2,
                    iterations,
                    keys,
                    table_size,
                    ACE_TEXT ("Hash Map Manager (insert test)"));
  performance_test (&find_test,
                    map2,
                    iterations,
                    keys,
                    table_size,
                    ACE_TEXT ("Hash Map Manager (find test)"));
  performance_test (&unbind_test,
                    map2,
                    iterations,
                    keys,
                    table_size,
                    ACE_TEXT ("Hash Map Manager (unbind test)"));

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));

  // Active Map Manager
  performance_test (&insert_test,
                    map3,
                    iterations,
                    keys,
                    table_size,
                    ACE_TEXT ("Active Map Manager (insert test)"));
  performance_test (&find_test,
                    map3,
                    iterations,
                    keys,
                    table_size,
                    ACE_TEXT ("Active Map Manager (find test)"));
  performance_test (&unbind_test,
                    map3,
                    iterations,
                    keys,
                    table_size,
                    ACE_TEXT ("Active Map Manager (unbind test)"));

  delete[] keys;

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::VERBOSE_LITE);
  ACE_END_TEST;

  return 0;
}
#undef THIS_IS_NOT_AN_ASSERT_IT_IS_A_NON_DEBUG_TEST_AS_WELL
