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
//    Irfan Pyarali
//
// ============================================================================

#include "test_config.h"
#include "ace/Containers.h"
#include "ace/Map_T.h"
#include "ace/Profile_Timer.h"
#include "ace/Synch.h"

ACE_RCSID(tests, Map_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

// Key data type.
typedef ACE_Array<char> KEY;

////////////////////////////////////////////////////////////////////////////////

class Key_Generator
{
  // = TITLE  
  //     Defines a key generator.
  //
  // = DESCRIPTION
  //     This class is used in adapters of maps that do not produce keys.
public:
  
  Key_Generator (void)
    : counter_ (0)
    {
    }

  int operator() (KEY &key)
    {
      // Keep original information in the key intact.
      size_t original_size = key.size ();

      // Size of this counter key.
      size_t counter_key_size = sizeof this->counter_;

      // Resize to accommodate both the original data and the new key.
      key.size (counter_key_size + original_size);

      // Add new key data.
      ACE_OS::memcpy (&key[original_size],
                      &++this->counter_,
                      sizeof this->counter_);
      
      // Success.
      return 0;
    }

private:  
  u_long counter_;
};

////////////////////////////////////////////////////////////////////////////////

class Hash_Key
{
public:
  u_long operator () (const KEY &key) const
    {
      // Recover system generated part of key.
      u_long value;            
      size_t counter_key_size = sizeof (u_long);

      // Copy system key from user key.
      ACE_OS::memcpy (&value,
                      &key[key.size () - counter_key_size],
                      sizeof value);      
      
      // Return the system key value as the hash value.
      return value;
    } 
};

////////////////////////////////////////////////////////////////////////////////

class Key_Adapter
{
public:

  int encode (const KEY &original_key,
              const ACE_Active_Map_Manager_Key &active_key,
              KEY &modified_key)
    {
      // Keep original information in the key intact.
      modified_key = original_key;
      size_t original_size = modified_key.size ();

      // Size of active key.
      size_t active_key_size = active_key.size ();

      // Resize to accommodate both the original data and the new active key.
      modified_key.size (active_key_size + original_size);

      // Copy active key data into user key.
      active_key.encode (&modified_key[original_size]);

      // Success.
      return 0;
    }

  int decode (const KEY &modified_key,
              ACE_Active_Map_Manager_Key &active_key)
    {
      // Read the active key data from the back of the key.
      size_t active_key_size = active_key.size ();      
      size_t original_size = modified_key.size () - active_key_size;

      // Read off value of index and generation.
      active_key.decode (&modified_key[original_size]);

      // Success.
      return 0;
    }

  int decode (const KEY &modified_key,
              KEY &original_key)
    {
      // Read the original user key data from the front of the
      // modified key.
      size_t active_key_size = ACE_Active_Map_Manager_Key::size ();      

      // Copy all the data.
      original_key = modified_key;

      // Resize to ignore active key data.
      original_key.size (original_key.size () - active_key_size);

      // Success.
      return 0;
    }
};

////////////////////////////////////////////////////////////////////////////////

// Value type.
typedef size_t VALUE;

// Generic map type.
typedef ACE_Map<KEY, VALUE> MAP;

// Manager Manager adapter.
typedef ACE_Map_Manager_Adapter<KEY, VALUE, Key_Generator> MAP_MANAGER_ADAPTER;

// Hash Manager Manager adapter.
typedef ACE_Hash_Map_Manager_Ex_Adapter<KEY, VALUE, Hash_Key, ACE_Equal_To<KEY>, Key_Generator> HASH_MAP_MANAGER_ADAPTER;

// Active Manager Manager adapter.
typedef ACE_Active_Map_Manager_Adapter<KEY, VALUE, Key_Adapter> ACTIVE_MAP_MANAGER_ADAPTER;

static void
functionality_test (MAP &map, 
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
      ACE_ASSERT (map.bind_modify_key (i, modified_keys[i]) == 0);
      ++counter;
      ACE_ASSERT (map.current_size () == counter);
    }
    
  // Forward iteration...
  {
    counter = 0;
    MAP::iterator end = map.end ();

    for (MAP::iterator iter = map.begin ();
         iter != end;
         ++iter, ++counter)
      {
        MAP::value_type entry = *iter;

        // Recover original key.
        KEY original_key;
        ACE_ASSERT (map.recover_key (entry.first (),
                                     original_key) == 0);
        
        // Make sure recovering keys work.
        ACE_ASSERT (original_keys[entry.second ()] == original_key);

        // Obtain value from key.
        VALUE original_value;
        ACE_OS::memcpy (&original_value,
                        &original_key[0],
                        sizeof original_value);

        // Debugging info.
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("(%d|%d|%d)"),
                    counter,
                    original_value,
                    entry.second ()));
      }

    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("\n")));
    ACE_ASSERT (counter == iterations);
  }

  // Reverse iteration...
  {
    counter = iterations;    
    MAP::reverse_iterator end = map.rend ();
    
    for (MAP::reverse_iterator iter = map.rbegin ();
         iter != end;
         ++iter)
      {
        --counter;
        MAP::value_type entry = *iter;

        // Recover original key.
        KEY original_key;
        ACE_ASSERT (map.recover_key (entry.first (),
                                     original_key) == 0);
        
        // Make sure recovering keys work.
        ACE_ASSERT (original_keys[entry.second ()] == original_key);

        // Obtain value from key.
        VALUE original_value;
        ACE_OS::memcpy (&original_value,
                        &original_key[0],
                        sizeof original_value);

        // Debugging info.
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("(%d|%d|%d)"),
                    counter,
                    original_value,
                    entry.second ()));
      }
    
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("\n")));
    ACE_ASSERT (counter == 0);
  }

  // Search using the modified keys.
  for (i = 0; i < iterations; ++i)
    {
      VALUE j;
      ACE_ASSERT (map.find (modified_keys[i], j) != -1);
      ACE_ASSERT (i == j);
    }

  // Rmoved keys from map.
  counter = iterations;
  for (i = 0; i < iterations; ++i)
    {
      ACE_ASSERT (map.unbind (modified_keys[i]) != -1);
      --counter;
      ACE_ASSERT (map.current_size () == counter);
    }

  // Cleanup.
  delete[] modified_keys;
  delete[] original_keys;
}

static void
insert_test (MAP &map, 
             size_t iterations,
             KEY *keys)
{
  // Add to the map, allowing keys to be created by the map.
  size_t counter = 0;
  for (VALUE i = 0; i < iterations; ++i)
    {
      ACE_ASSERT (map.bind_create_key (i, keys[i]) == 0);
      ++counter;
      ACE_ASSERT (map.current_size () == counter);
    }
}

static void
find_test (MAP &map, 
           size_t iterations,
           KEY *keys)
{
  // Find system generated keys.
  for (VALUE i = 0; i < iterations; ++i)
    {
      VALUE j;
      ACE_ASSERT (map.find (keys[i], j) != -1);
      ACE_ASSERT (i == j);
    }
}

static void
unbind_test (MAP &map, 
             size_t iterations,
             KEY *keys)
{
  // Remove system generated keys.
  size_t counter = iterations;
  for (VALUE i = 0; i < iterations; ++i)
    {
      ACE_ASSERT (map.unbind (keys[i]) != -1);
      --counter;
      ACE_ASSERT (map.current_size () == counter);
    }
}

static void
performance_test (void (*ptf) (MAP &, size_t, KEY *),
                  MAP &map,
                  size_t iterations,
                  KEY *keys,
                  size_t table_size,
                  const ASYS_TCHAR *test_name)
{
  ACE_Profile_Timer timer;
  timer.start ();

  (*ptf) (map, iterations, keys);

  timer.stop ();

  ACE_Profile_Timer::ACE_Elapsed_Time et;

  timer.elapsed_time (et);

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("time to run %s of size %d for %d iterations\n"),
              test_name,
              table_size,
              iterations));

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("real time = %f secs, user time = %f secs, system time = %f secs\n"),
              et.real_time,
              et.user_time,
              et.system_time));

  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("time per call = %f usecs\n"),
              (et.real_time / ACE_timer_t (iterations)) * 1000000));
}

int
main (int argc, ASYS_TCHAR *argv[])
{
  ACE_START_TEST (ASYS_TEXT ("Map_Test"));
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
      ACE_DEBUG ((LM_DEBUG, "\nMap Manager functionality test\n"));
      functionality_test (map1, iterations);
      
      ACE_DEBUG ((LM_DEBUG, "\nHash Map Manager functionality test\n"));
      functionality_test (map2, iterations);
      
      ACE_DEBUG ((LM_DEBUG, "\nActive Map Manager functionality test\n"));
      functionality_test (map3, iterations);
      
      ACE_DEBUG ((LM_DEBUG, "\n"));        
    }

  // Performance test of the maps.
  KEY *keys = new KEY[iterations];
      
  // Map Manager
  performance_test (&insert_test, map1, iterations, keys, table_size, "Map Manager (insert test)");                    
  performance_test (&find_test,   map1, iterations, keys, table_size, "Map Manager (find test)");                    
  performance_test (&unbind_test, map1, iterations, keys, table_size, "Map Manager (unbind test)");                    

  ACE_DEBUG ((LM_DEBUG, "\n"));  

  // Hash Map Manager
  performance_test (&insert_test, map2, iterations, keys, table_size, "Hash Map Manager (insert test)");                    
  performance_test (&find_test,   map2, iterations, keys, table_size, "Hash Map Manager (find test)");                    
  performance_test (&unbind_test, map2, iterations, keys, table_size, "Hash Map Manager (unbind test)");                    

  ACE_DEBUG ((LM_DEBUG, "\n"));  

  // Active Map Manager
  performance_test (&insert_test, map3, iterations, keys, table_size, "Active Map Manager (insert test)");                    
  performance_test (&find_test,   map3, iterations, keys, table_size, "Active Map Manager (find test)");                    
  performance_test (&unbind_test, map3, iterations, keys, table_size, "Active Map Manager (unbind test)");                    

  delete[] keys;

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::VERBOSE_LITE);
  ACE_END_TEST;
  
  return 0;
}

#if 0

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Manager_Ex<TYPE, TYPE, HASH_KEY, COMPARE_KEYS, MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<TYPE, TYPE, HASH_KEY, COMPARE_KEYS, MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<TYPE, TYPE, HASH_KEY, COMPARE_KEYS, MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TYPE, TYPE, HASH_KEY, COMPARE_KEYS, MUTEX>;
template class ACE_Hash_Map_Entry<TYPE, TYPE>;
template class ACE_Equal_To<TYPE>;
template class ACE_Map_Manager<TYPE, TYPE, MUTEX>;
template class ACE_Map_Iterator_Base<TYPE, TYPE, MUTEX>;
template class ACE_Map_Iterator<TYPE, TYPE, MUTEX>;
template class ACE_Map_Reverse_Iterator<TYPE, TYPE, MUTEX>;
template class ACE_Map_Entry<TYPE, TYPE>;
template class ACE_Active_Map_Manager<TYPE>;
template class ACE_Map_Manager<ACE_Active_Map_Manager_Key, TYPE, MUTEX>;
template class ACE_Map_Iterator_Base<ACE_Active_Map_Manager_Key, TYPE, MUTEX>;
template class ACE_Map_Iterator<ACE_Active_Map_Manager_Key, TYPE, MUTEX>;
template class ACE_Map_Reverse_Iterator<ACE_Active_Map_Manager_Key, TYPE, MUTEX>;
template class ACE_Map_Entry<ACE_Active_Map_Manager_Key, TYPE>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Manager_Ex<TYPE, TYPE, HASH_KEY, COMPARE_KEYS, MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TYPE, TYPE, HASH_KEY, COMPARE_KEYS, MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TYPE, TYPE, HASH_KEY, COMPARE_KEYS, MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TYPE, TYPE, HASH_KEY, COMPARE_KEYS, MUTEX>
#pragma instantiate ACE_Hash_Map_Entry<TYPE, TYPE>
#pragma instantiate ACE_Equal_To<TYPE>
#pragma instantiate ACE_Map_Manager<TYPE, TYPE, MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<TYPE, TYPE, MUTEX>
#pragma instantiate ACE_Map_Iterator<TYPE, TYPE, MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<TYPE, TYPE, MUTEX>
#pragma instantiate ACE_Map_Entry<TYPE, TYPE>
#pragma instantiate ACE_Active_Map_Manager<TYPE>
#pragma instantiate ACE_Map_Manager<ACE_Active_Map_Manager_Key, TYPE, MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<ACE_Active_Map_Manager_Key, TYPE, MUTEX>
#pragma instantiate ACE_Map_Iterator<ACE_Active_Map_Manager_Key, TYPE, MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_Active_Map_Manager_Key, TYPE, MUTEX>
#pragma instantiate ACE_Map_Entry<ACE_Active_Map_Manager_Key, TYPE>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif
