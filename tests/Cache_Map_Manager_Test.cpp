// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Cache_Map_Manager_Test.cpp
//
// = DESCRIPTION
//     This is a simple test of the <ACE_Cache_Map_Manager> and
//     <ACE_Hash_Cache_Map_Manager> that illustrates how to use the forward
//     and reverse iterators as well as the purging and caching features.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
// ============================================================================

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "test_config.h"
#include "ace/Hash_Cache_Map_Manager_T.h"
#include "ace/Map_Manager.h"
#include "ace/Caching_Strategies_T.h"
#include "ace/Functor.h"
#include "ace/Pair_T.h"
#include "ace/Synch.h"

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */


typedef size_t KEY;
typedef size_t VALUE;
typedef int ATTR;
typedef ACE_Pair<VALUE, ATTR> CACHE_VALUE;
typedef ACE_Hash<KEY> HASH_KEY;
typedef ACE_Equal_To<KEY> COMPARE_KEYS;

typedef ACE_Hash_Map_Manager_Ex<KEY, CACHE_VALUE, ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex> 
        HASH_MAP_MANAGER;
typedef ACE_Hash_Map_Iterator_Ex<KEY, CACHE_VALUE, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>
        HASH_MAP_ITER;
typedef ACE_Hash_Map_Reverse_Iterator_Ex<KEY, CACHE_VALUE, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>
        HASH_MAP_REV_ITER;
  
typedef ACE_Svc_Caching_Strategy_Utility<KEY, CACHE_VALUE,HASH_MAP_MANAGER, HASH_MAP_ITER, ATTR> 
        HASH_CACHING_STRATEGY_UTILITY;

typedef ACE_LRU_Caching_Strategy <KEY, CACHE_VALUE,HASH_MAP_MANAGER,ATTR, HASH_CACHING_STRATEGY_UTILITY>
        LRU_HASH_MAP;

// Using the Hash_Map.
typedef ACE_Cache_Map_Manager<KEY, VALUE, HASH_MAP_MANAGER,HASH_MAP_ITER, HASH_MAP_REV_ITER, LRU_HASH_MAP, ATTR>
        CACHE_1; 

// Test out the special Hash_Map version of the Cache_Map_Manager,
// i.e. the Hash_Cache_Map_Manager
typedef ACE_Hash_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, LRU_HASH_MAP, ATTR>
      HASH_CACHE_1;


void run_iterator_cache1 (CACHE_1 &cache,
                          size_t iterations)
{
  ACE_UNUSED_ARG (cache);
  ACE_UNUSED_ARG (iterations);
  size_t counter = 0;
  
  // Forward iteration...trying prefix ++
  {
    counter = 0;
    CACHE_1::iterator end = cache.end ();
    
    for (CACHE_1::iterator iter = cache.begin ();
         iter != end;
         ++iter)
      {
        // Debugging info.
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("(%d|%d|%d)"),
                    counter,
                    (*iter).first (), 
                    (*iter).second ()));
        
        ACE_ASSERT ((*iter).first () == (*iter).second ());
        ++counter;
      }
    
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("\n")));
    ACE_ASSERT (counter == iterations);
    
  }
}

void run_iterator_hash_cache1 (HASH_CACHE_1 &cache,
                               size_t iterations)
{
  ACE_UNUSED_ARG (iterations);
  size_t counter = 0;
  
  // Forward iteration...trying prefix ++
  {
    counter = 0;
    HASH_CACHE_1::iterator end = cache.end ();
    
    for (HASH_CACHE_1::iterator iter = cache.begin ();
         iter != end;
         ++iter)
      {
        // Debugging info.
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("(%d|%d|%d)"),
                    counter,
                    (*iter).first (), 
                    (*iter).second ()));
        
        ACE_ASSERT ((*iter).first () == (*iter).second ());
        ++counter;
      }
    
    ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("\n")));
    ACE_ASSERT (counter == iterations);
    
  }
}

void run_reverse_iterator_cache1 (CACHE_1 &cache,
                                  size_t iterations)
{
  ACE_UNUSED_ARG (cache);
  size_t counter = 0;
  
  // Reverse iteration...trying prefix ++
  {
    counter = iterations;
    CACHE_1::reverse_iterator  rend = cache.rend ();
    
    for (CACHE_1::reverse_iterator iter = cache.rbegin ();
         iter != rend;
         ++iter)
      {
        ACE_ASSERT ((*iter).first () == (*iter).second ());
        
        // Debugging info.
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("(%d|%d|%d)"),
                    counter,
                    (*iter).first (),
                    (*iter).second ()));
        --counter;
      }
    
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("\n")));
    ACE_ASSERT (counter == 0);
    
  }
}

void run_reverse_iterator_hash_cache1 (HASH_CACHE_1 &cache,
                                       size_t iterations)
{
  ACE_UNUSED_ARG (cache);
  size_t counter = 0;
  
  // Reverse iteration...trying prefix ++
  {
    counter = iterations;
    HASH_CACHE_1::reverse_iterator  rend = cache.rend ();
    
    for (HASH_CACHE_1::reverse_iterator iter = cache.rbegin ();
         iter != rend;
         ++iter)
      {
        ACE_ASSERT ((*iter).first () == (*iter).second ());
        
        // Debugging info.
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("(%d|%d|%d)"),
                    counter,
                    (*iter).first (),
                    (*iter).second ()));
        --counter;
      }
    
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("\n")));
    ACE_ASSERT (counter == 0);
    
  }
}

void find_test_cache1 (CACHE_1 &cache,
                       KEY *verify_keys,
                       size_t total_entries,
                       size_t times,
                       size_t random_seed)
{
  ACE_UNUSED_ARG (cache);
  ACE_UNUSED_ARG (verify_keys);
  
  ACE_DEBUG ((LM_DEBUG, "find\n"));
  ACE_OS::srand (random_seed);
  
  size_t order = 0;
  for (VALUE i = 0; i < times; ++i)
    {
      if (random_seed != 0)
            order = (int) (ACE_OS::rand () % total_entries);
      else
        order = i;
          
      VALUE j = 0;
      
      ACE_ASSERT (cache.find (verify_keys [order], j) != -1);
      ACE_ASSERT (j == verify_keys [order]);
      
      ACE_DEBUG ((LM_DEBUG,
                  "%d|%d   ",
                  verify_keys [order],
                  j));
      ACE_UNUSED_ARG (j);
    }
 
  ACE_DEBUG ((LM_DEBUG, "\n"));
}

void find_test_hash_cache1 (HASH_CACHE_1 &cache,
                            KEY *verify_keys,
                            size_t total_entries,
                            size_t times,
                            size_t random_seed)
{
  ACE_UNUSED_ARG (cache);
  ACE_UNUSED_ARG (verify_keys);
  
  ACE_DEBUG ((LM_DEBUG, "find\n"));
  ACE_OS::srand (random_seed);
  
  size_t order = 0;
  for (VALUE i = 0; i < times; ++i)
    {
      if (random_seed != 0)
        order = (int) (ACE_OS::rand () % total_entries);
      else
        order = i;
      
      VALUE j = 0;
          
      ACE_ASSERT (cache.find (verify_keys [order], j) != -1);
      ACE_ASSERT (j == verify_keys [order]);
      
      ACE_DEBUG ((LM_DEBUG,
                  "%d|%d   ",
                      verify_keys [order],
                  j));
      ACE_UNUSED_ARG (j);
    }
  
  ACE_DEBUG ((LM_DEBUG, "\n"));
}

void purge_test_cache1 (CACHE_1 &cache,
                        HASH_MAP_MANAGER &map,
                        KEY *verify_keys,
                        size_t iterations)
{
  ACE_UNUSED_ARG (cache);
  ACE_UNUSED_ARG (map);
  ACE_UNUSED_ARG (verify_keys);
  
  size_t purge_percent = 10;
  
  // Find the numeber of entries whihc will get purged.
  double val = (double) purge_percent / 100;
  size_t no_of_entries = (size_t) (val * iterations + 0.5);
  
  // Verify whether the purging happens properly. For that we will
  // first sort all the entries by their attributes and store them.
  // Then we purge and check that the top <no_of_entries> have
  // disappeared from the map or not!

  ATTR *attr = new ATTR[iterations];
  
  ACE_UNUSED_ARG (attr);
  
  HASH_MAP_MANAGER::iterator end = map.end ();
  
  size_t counter = 0;
  for (HASH_MAP_MANAGER::iterator iter = map.begin ();
       iter != end;
       ++iter)
    {
          verify_keys[counter] = (*iter).ext_id_;
          attr[counter] = (*iter).int_id_.second ();
          
          ACE_DEBUG ((LM_DEBUG, 
                      "key=%d, attr=%d, index=%d\n",
                      verify_keys[counter], 
                      attr[counter],
                      counter));
          ++counter;
    }
  
  // Sort in ascending order on the attributes.
  VALUE i;
  for (i = 0; i < (iterations - 1); ++i)
    {
          for (VALUE j = (i + 1); j < iterations; ++j)
            {
              KEY tmp; ATTR tmp1;
              if (attr[i] > attr[j])
                {
                  tmp = verify_keys[i];
                  tmp1 = attr[i];
                  verify_keys[i] = verify_keys[j];
                  attr[i] = attr[j];
                  verify_keys[j] = tmp;
                  attr[j] = tmp1;
                }
            }
    }
  
  ACE_DEBUG ((LM_DEBUG, "In attr sorted order \n"));
  for (i = 0; i < iterations; ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "key=%d  ", verify_keys[i]));
    }
  ACE_DEBUG ((LM_DEBUG, " \n"));
  ACE_ASSERT (cache.purge () != -1);
      
  // Check that the top no_of_entries in the verify_keys array
  // are missing from the cache denoting successfull purging.
  for (i = 0; i < no_of_entries; ++i)
    {
      
      VALUE j;
      ACE_ASSERT (cache.find (verify_keys [i], j) == -1); 
          ACE_UNUSED_ARG (j);
    }
  
  delete[] attr;
}

void purge_test_hash_cache1 (HASH_CACHE_1 &cache,
                             HASH_MAP_MANAGER &map,
                             KEY *verify_keys,
                             size_t iterations)
{
  ACE_UNUSED_ARG (cache);
  ACE_UNUSED_ARG (map);
  ACE_UNUSED_ARG (verify_keys);
  
  unsigned int purge_percent = 10;
  
  // Find the numeber of entries whihc will get purged.
  double val = (double) purge_percent / 100;
  size_t no_of_entries = (size_t) (val * iterations + 0.5);
  
  // Verify whether the purging happens properly. For that we will
  // first sort all the entries by their attributes and store them.
  // Then we purge and check that the top <no_of_entries> have
  // disappeared from the map or not!

  ATTR *attr = new ATTR[iterations];
  
  ACE_UNUSED_ARG (attr);
  
  HASH_MAP_MANAGER::iterator end = map.end ();
  
  size_t counter = 0;
  for (HASH_MAP_MANAGER::iterator iter = map.begin ();
       iter != end;
       ++iter)
    {
          verify_keys[counter] = (*iter).ext_id_;
          attr[counter] = (*iter).int_id_.second ();
          
          ACE_DEBUG ((LM_DEBUG, 
                      "key=%d, attr=%d, index=%d\n",
                      verify_keys[counter], 
                      attr[counter],
                      counter));
          ++counter;
    }
  
  // Sort in ascending order on the attributes.
  VALUE i;
  for (i = 0; i < (iterations - 1); ++i)
    {
          for (VALUE j = (i + 1); j < iterations; ++j)
            {
              KEY tmp; ATTR tmp1;
              if (attr[i] > attr[j])
                {
                  tmp = verify_keys[i];
                  tmp1 = attr[i];
                  verify_keys[i] = verify_keys[j];
                  attr[i] = attr[j];
                  verify_keys[j] = tmp;
                  attr[j] = tmp1;
                }
            }
    }
  
  ACE_DEBUG ((LM_DEBUG, "In attr sorted order \n"));
  for (i = 0; i < iterations; ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "key=%d  ", verify_keys[i]));
    }
  ACE_DEBUG ((LM_DEBUG, " \n"));
  ACE_ASSERT (cache.purge () != -1);
      
  // Check that the top no_of_entries in the verify_keys array
  // are missing from the cache denoting successfull purging.
  for (i = 0; i < no_of_entries; ++i)
    {
      
      VALUE j;
      ACE_ASSERT (cache.find (verify_keys [i], j) == -1); 
          ACE_UNUSED_ARG (j);
    }
  
  delete[] attr;
}

// All the combinations of the Maps, caching_startegies, utilities and
// cleanup strategies can be tested to the hilt using this method. The
// <randomize> option allows the user to decide whether the searches
// on the map should be done with a different seed per iteration.
void functionality_test_cache1 (size_t table_size,
                                size_t iterations,
                                size_t no_of_lookups,
                                size_t randomize)
{
  CACHE_1 cache(table_size);
  
  VALUE i;
  KEY *keys = new KEY[iterations];
  KEY *verify_keys = new KEY[iterations];
  
  ACE_UNUSED_ARG (keys);
  ACE_UNUSED_ARG (verify_keys);
  // Fill some values into the keys.
  for (i = 0; i < iterations; ++i)
        {
          keys[i] = i;
          verify_keys[i] = keys [i];
        }
  
  // Add it to the map now.
   size_t counter = 0;
  for (i = 0; i < iterations; ++i)
    {
      ACE_ASSERT (cache.bind (keys[i], i) == 0);
      ACE_DEBUG ((LM_DEBUG, "keys[i]=%d verify_keys[i]=%d\n", keys[i],verify_keys[i])); 
      ++counter;
      ACE_ASSERT (cache.current_size () == counter);
    }
  
  // Lets check out the iterators - forward and reverse.
  run_iterator_cache1 (cache,
                iterations);
  
  run_reverse_iterator_cache1 (cache,
                        iterations);
  
  // Let search now, this will affect the attributes of the entries,
  // thereby affecting the purging of the entries.
  size_t random_seed = 0;
  for (i = 0; i < no_of_lookups; ++i)
    {
      size_t total_entries = iterations;
      int times = iterations / 2;
      
      if (randomize != 0)
        random_seed = (unsigned int) ACE_OS::time (0); 
      
      find_test_cache1 (cache,
                        verify_keys,
                        total_entries,
                        times,
                        random_seed);
    }
  
  // Now we clear the cache. Heres where the strategy somes into play.
  // The utiltity helps in actually performing the purging and the
  // cleanup strategy will remove the entries.
  ACE_DEBUG ((LM_DEBUG, "cache size before purging  %d\n",
              cache.current_size ()));
  purge_test_cache1 (cache,
                     cache.map (),
                     verify_keys,
                     iterations);
  ACE_DEBUG ((LM_DEBUG, "cache size after purging  %d\n",
              cache.current_size ()));
  delete[] keys;
  delete[] verify_keys;

}

void functionality_test_hash_cache1 (size_t table_size,
                                     size_t iterations,
                                     size_t no_of_lookups,
                                     size_t randomize)
{
  HASH_CACHE_1 cache(table_size);
  
  VALUE i;
  KEY *keys = new KEY[iterations];
  KEY *verify_keys = new KEY[iterations];
  
  ACE_UNUSED_ARG (keys);
  ACE_UNUSED_ARG (verify_keys);
  // Fill some values into the keys.
  for (i = 0; i < iterations; ++i)
    {
      keys[i] = i;
      verify_keys[i] = keys [i];
    }
  
  // Add it to the map now.
  size_t counter = 0;
  for (i = 0; i < iterations; ++i)
    {
      ACE_ASSERT (cache.bind (keys[i], i) == 0);
      ACE_DEBUG ((LM_DEBUG, "keys[i]=%d verify_keys[i]=%d\n", keys[i],verify_keys[i])); 
      ++counter;
      ACE_ASSERT (cache.current_size () == counter);
    }
  
  // Lets check out the iterators - forward and reverse.
  run_iterator_hash_cache1 (cache,
                            iterations);
  
  run_reverse_iterator_hash_cache1 (cache,
                                    iterations);
  
  // Let search now, this will affect the attributes of the entries,
  // thereby affecting the purging of the entries.
  size_t random_seed = 0;
  for (i = 0; i < no_of_lookups; ++i)
    {
      size_t total_entries = iterations;
      int times = iterations / 2;
      
      if (randomize != 0)
        random_seed = (unsigned int) ACE_OS::time (0); 
      
      find_test_hash_cache1 (cache,
                             verify_keys,
                             total_entries,
                             times,
                             random_seed);
    }
  
  // Now we clear the cache. Heres where the strategy somes into play.
  // The utiltity helps in actually performing the purging and the
  // cleanup strategy will remove the entries.
  ACE_DEBUG ((LM_DEBUG, "cache size before purging  %d\n",
              cache.current_size ()));
  purge_test_hash_cache1 (cache,
                          cache.map (),
                          verify_keys,
                          iterations);
  ACE_DEBUG ((LM_DEBUG, "cache size after purging  %d\n",
              cache.current_size ()));
  delete[] keys;
  delete[] verify_keys;
  
}


//===================START OF TEST===================================

int
main (int argc, ASYS_TCHAR *argv[])
{
  ACE_START_TEST (ASYS_TEXT ("Cache_Map_Manager_Test"));
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::VERBOSE_LITE);

  size_t table_size = ACE_MAX_ITERATIONS / 2;
  int iterations = ACE_MAX_ITERATIONS;
  int no_of_lookups = 3;
  unsigned int randomize = 1;
  
  if (argc > 1)
   no_of_lookups = ACE_OS::atoi (argv[1]);
  
  if (argc > 2)
    table_size = ACE_OS::atoi (argv[2]);
  
  if (argc > 3)
    iterations = ACE_OS::atoi (argv[3]);
  
  if (argc > 4)
    randomize = ACE_OS::atoi (argv[4]);

 
  functionality_test_cache1(table_size,
                            iterations,
                            no_of_lookups,
                            randomize);

  ACE_DEBUG ((LM_DEBUG, "Success cache_1!\n"));

  functionality_test_hash_cache1(table_size,
                                 iterations,
                                 no_of_lookups,
                                 randomize);
  
  ACE_DEBUG ((LM_DEBUG, "Success hash_cache_1!\n"));
  ACE_LOG_MSG->set_flags (ACE_Log_Msg::VERBOSE_LITE);
  ACE_END_TEST;

  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Pair<VALUE, ATTR>;
template class ACE_Reference_Pair<KEY, VALUE>;
template class ACE_Equal_To<KEY>;

template class ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>; 
template class ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<KEY, ACE_Pair<VALUE,ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<KEY, ACE_Pair<VALUE,  ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager<KEY, ACE_Pair<VALUE, ATTR>, ACE_Null_Mutex>; 
template class ACE_Hash_Map_Iterator<KEY, ACE_Pair<VALUE, ATTR>, ACE_Null_Mutex>; 
template class ACE_Hash_Map_Reverse_Iterator<KEY, ACE_Pair<VALUE, ATTR>, ACE_Null_Mutex>; 
template class ACE_Hash_Map_Entry<KEY, ACE_Pair<VALUE, ATTR>  >;
template class ACE_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE, ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ATTR>;
template class ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE, ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>;
template class ACE_LRU_Caching_Strategy <KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR,  ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>  >;
template class ACE_FIFO_Caching_Strategy <KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR, ACE_Svc_Caching_Strategy_Utility<KEY,  ACE_Pair<VALUE, ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE,  ATTR>, ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>   >;
template class ACE_LFU_Caching_Strategy <KEY, ACE_Pair<VALUE,ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR, ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE, ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>, ATTR>       >;

template class ACE_Default_Cleanup_Strategy<KEY, ACE_Pair<KEY, ATTR>, ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex> >;
template class ACE_Cleanup_Strategy<KEY, ACE_Pair<KEY, ATTR>, ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<KEY, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex> >;

template class ACE_Hash_Cache_Map_Manager<KEY, VALUE, ACE_Hash<KEY>,  ACE_Equal_To<KEY>, ACE_LRU_Caching_Strategy <KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR,  ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>  >, ATTR>;
template class ACE_Hash_Cache_Map_Manager<KEY, VALUE, ACE_Hash<KEY>,  ACE_Equal_To<KEY>, ACE_LFU_Caching_Strategy <KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR,  ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>  >, ATTR>;

template class ACE_Cache_Map_Manager<KEY, VALUE, ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ACE_Hash_Map_Reverse_Iterator_Ex<KEY, ACE_Pair<VALUE,ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ACE_LRU_Caching_Strategy <KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR,  ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>  >,ATTR>;
template class ACE_Cache_Map_Manager<KEY, VALUE, ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ACE_Hash_Map_Reverse_Iterator_Ex<KEY, ACE_Pair<VALUE,ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ACE_LFU_Caching_Strategy <KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR,  ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>  >,ATTR>;

template class ACE_Cache_Map_Iterator<KEY, VALUE, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ACE_LRU_Caching_Strategy <KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR,  ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>  >,ATTR>;
template class ACE_Cache_Map_Iterator<KEY, VALUE, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ACE_LFU_Caching_Strategy <KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR,  ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>  >,ATTR>;

template class ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, ACE_Hash_Map_Reverse_Iterator_Ex<KEY, ACE_Pair<VALUE,ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ACE_LRU_Caching_Strategy <KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR,  ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>  >,ATTR>;
template class ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, ACE_Hash_Map_Reverse_Iterator_Ex<KEY, ACE_Pair<VALUE,ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ACE_LFU_Caching_Strategy <KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR,  ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>  >,ATTR>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Pair<VALUE, ATTR>
#pragma instantiate ACE_Reference_Pair<KEY, VALUE>
#pragma instantiate ACE_Equal_To<KEY>

#pragma instantiate ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<KEY, ACE_Pair<VALUE,ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<KEY, ACE_Pair<VALUE,  ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager<KEY, ACE_Pair<VALUE, ATTR>, ACE_Null_Mutex> 
#pragma instantiate ACE_Hash_Map_Iterator<KEY, ACE_Pair<VALUE, ATTR>, ACE_Null_Mutex> 
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<KEY, ACE_Pair<VALUE, ATTR>, ACE_Null_Mutex> 
#pragma instantiate ACE_Hash_Map_Entry<KEY, ACE_Pair<VALUE, ATTR>  >
#pragma instantiate ACE_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE, ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ATTR>
#pragma instantiate ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE, ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>
#pragma instantiate ACE_LRU_Caching_Strategy <KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR,  ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>  >
#pragma instantiate ACE_FIFO_Caching_Strategy <KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR, ACE_Svc_Caching_Strategy_Utility<KEY,  ACE_Pair<VALUE, ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE,  ATTR>, ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>   >
#pragma instantiate ACE_LFU_Caching_Strategy <KEY, ACE_Pair<VALUE,ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR, ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE, ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>, ATTR>       >

#pragma instantiate ACE_Default_Cleanup_Strategy<KEY, ACE_Pair<KEY, ATTR>, ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex> >
#pragma instantiate ACE_Cleanup_Strategy<KEY, ACE_Pair<KEY, ATTR>, ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<KEY, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex> >

#pragma instantiate ACE_Hash_Cache_Map_Manager<KEY, VALUE, ACE_Hash<KEY>,  ACE_Equal_To<KEY>, ACE_LRU_Caching_Strategy <KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR,  ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>  >, ATTR>
#pragma instantiate ACE_Hash_Cache_Map_Manager<KEY, VALUE, ACE_Hash<KEY>,  ACE_Equal_To<KEY>, ACE_LFU_Caching_Strategy <KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR,  ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>  >, ATTR>

#pragma instantiate ACE_Cache_Map_Manager<KEY, VALUE, ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ACE_Hash_Map_Reverse_Iterator_Ex<KEY, ACE_Pair<VALUE,ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ACE_LRU_Caching_Strategy <KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR,  ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>  >,ATTR>
#pragma instantiate ACE_Cache_Map_Manager<KEY, VALUE, ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ACE_Hash_Map_Reverse_Iterator_Ex<KEY, ACE_Pair<VALUE,ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ACE_LFU_Caching_Strategy <KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR,  ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>  >,ATTR>

#pragma instantiate ACE_Cache_Map_Iterator<KEY, VALUE, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ACE_LRU_Caching_Strategy <KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR,  ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>  >,ATTR>
#pragma instantiate ACE_Cache_Map_Iterator<KEY, VALUE, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ACE_LFU_Caching_Strategy <KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR,  ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>  >,ATTR>

#pragma instantiate ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, ACE_Hash_Map_Reverse_Iterator_Ex<KEY, ACE_Pair<VALUE,ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ACE_LRU_Caching_Strategy <KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR,  ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>  >,ATTR>
#pragma instantiate ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, ACE_Hash_Map_Reverse_Iterator_Ex<KEY, ACE_Pair<VALUE,ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ACE_LFU_Caching_Strategy <KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR,  ACE_Svc_Caching_Strategy_Utility<KEY, ACE_Pair<VALUE,  ATTR>,ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTR>,  ACE_Hash<KEY>,ACE_Equal_To<KEY>, ACE_Null_Mutex>, ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTR>, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>,ATTR>  >,ATTR>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
