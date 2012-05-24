
//=============================================================================
/**
 *  @file    Cache_Map_Manager_Test.cpp
 *
 *  $Id$
 *
 *   This is a test of the <ACE_Cache_Map_Manager> and
 *   <ACE_Hash_Cache_Map_Manager> that illustrates how to use the
 *   forward and reverse iterators, as well as the purging and
 *   caching features.
 *
 *
 *  @author Kirthika Parameswaran  <kirthika@cs.wustl.edu>
 */
//=============================================================================


#include "ace/OS_NS_string.h"

#include "ace/OS_Memory.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_time.h"
#include "test_config.h"
#include "ace/Hash_Cache_Map_Manager_T.h"
#include "ace/Map_Manager.h"
#include "ace/Caching_Strategies_T.h"
#include "ace/Functor.h"
#include "ace/Get_Opt.h"
#include "Cache_Map_Manager_Test.h"     // Hash_Key class defined in here

typedef size_t KEY;
typedef size_t VALUE;
typedef int ATTR;
typedef std::pair<VALUE, ATTR> CACHE_VALUE;
typedef ACE_Equal_To<KEY> COMPARE_KEYS;

typedef ACE_Hash_Map_Manager_Ex<KEY, CACHE_VALUE, Hash_Key, ACE_Equal_To<KEY>, ACE_Null_Mutex>
        HASH_MAP_MANAGER;
typedef ACE_Hash_Map_Iterator_Ex<KEY, CACHE_VALUE, Hash_Key, ACE_Equal_To<KEY>, ACE_Null_Mutex>
        HASH_MAP_ITERATOR;
typedef ACE_Hash_Map_Reverse_Iterator_Ex<KEY, CACHE_VALUE, Hash_Key, ACE_Equal_To<KEY>, ACE_Null_Mutex>
        HASH_MAP_REVERSE_ITERATOR;

typedef ACE_Map_Manager<KEY, CACHE_VALUE, ACE_Null_Mutex>
        MAP_MANAGER;
typedef ACE_Map_Iterator<KEY, CACHE_VALUE, ACE_Null_Mutex>
        MAP_ITERATOR;
typedef ACE_Map_Reverse_Iterator<KEY, CACHE_VALUE, ACE_Null_Mutex>
        MAP_REVERSE_ITERATOR;

typedef ACE_Cleanup_Strategy<KEY, CACHE_VALUE, HASH_MAP_MANAGER>
        HASH_MAP_CLEANUP;

typedef ACE_Cleanup_Strategy<KEY, CACHE_VALUE, MAP_MANAGER>
        MAP_CLEANUP;

typedef ACE_Pair_Caching_Utility<KEY, CACHE_VALUE, HASH_MAP_MANAGER, HASH_MAP_ITERATOR, ATTR>
        HASH_MAP_CACHING_UTILITY;

typedef ACE_Pair_Caching_Utility<KEY, CACHE_VALUE, MAP_MANAGER, MAP_ITERATOR, ATTR>
        MAP_CACHING_UTILITY;

// = Hash_Map_Manager related
typedef ACE_Caching_Strategy<ATTR, HASH_MAP_CACHING_UTILITY>
        HASH_MAP_CACHING_STRATEGY;
typedef ACE_LRU_Caching_Strategy<ATTR, HASH_MAP_CACHING_UTILITY>
        HASH_MAP_LRU;
typedef ACE_LFU_Caching_Strategy<ATTR, HASH_MAP_CACHING_UTILITY>
        HASH_MAP_LFU;
typedef ACE_FIFO_Caching_Strategy<ATTR, HASH_MAP_CACHING_UTILITY>
        HASH_MAP_FIFO;
typedef ACE_Null_Caching_Strategy<ATTR, HASH_MAP_CACHING_UTILITY>
        HASH_MAP_NULL;
typedef ACE_Caching_Strategy_Adapter<ATTR, HASH_MAP_CACHING_UTILITY, HASH_MAP_LRU>
        HASH_MAP_LRU_ADAPTER;
typedef ACE_Caching_Strategy_Adapter<ATTR, HASH_MAP_CACHING_UTILITY, HASH_MAP_LFU>
        HASH_MAP_LFU_ADAPTER;
typedef ACE_Caching_Strategy_Adapter<ATTR, HASH_MAP_CACHING_UTILITY, HASH_MAP_FIFO>
        HASH_MAP_FIFO_ADAPTER;
typedef ACE_Caching_Strategy_Adapter<ATTR, HASH_MAP_CACHING_UTILITY, HASH_MAP_NULL>
        HASH_MAP_NULL_ADAPTER;

// = Map_Manager related
typedef ACE_Caching_Strategy<ATTR, MAP_CACHING_UTILITY>
        MAP_CACHING_STRATEGY;
typedef ACE_LRU_Caching_Strategy<ATTR, MAP_CACHING_UTILITY>
        MAP_LRU;
typedef ACE_LFU_Caching_Strategy<ATTR, MAP_CACHING_UTILITY>
        MAP_LFU;
typedef ACE_FIFO_Caching_Strategy<ATTR, MAP_CACHING_UTILITY>
        MAP_FIFO;
typedef ACE_Null_Caching_Strategy<ATTR, MAP_CACHING_UTILITY>
        MAP_NULL;
typedef ACE_Caching_Strategy_Adapter<ATTR, MAP_CACHING_UTILITY, MAP_LRU>
        MAP_LRU_ADAPTER;
typedef ACE_Caching_Strategy_Adapter<ATTR, MAP_CACHING_UTILITY, MAP_LFU>
        MAP_LFU_ADAPTER;
typedef ACE_Caching_Strategy_Adapter<ATTR, MAP_CACHING_UTILITY, MAP_FIFO>
        MAP_FIFO_ADAPTER;
typedef ACE_Caching_Strategy_Adapter<ATTR, MAP_CACHING_UTILITY, MAP_NULL>
        MAP_NULL_ADAPTER;

typedef ACE_Hash_Cache_Map_Manager<KEY, VALUE, Hash_Key, ACE_Equal_To<KEY>, HASH_MAP_CACHING_STRATEGY, ATTR>
        HASH_MAP_CACHE;
typedef ACE_Cache_Map_Manager<KEY, VALUE, MAP_MANAGER, MAP_ITERATOR, MAP_REVERSE_ITERATOR, MAP_CACHING_STRATEGY, ATTR>
        MAP_CACHE;

enum Caching_Strategy_Type
{
  ACE_LFU,
  ACE_FIFO,
  ACE_LRU,
  ACE_NULL,
  ACE_ALL
};

static size_t iterations = ACE_MAX_ITERATIONS;
static size_t no_of_lookups = iterations / 2;
static int randomize_lookups = 1;
static int purge_percent = 10;
static int debug = 0;
static Caching_Strategy_Type caching_strategy_type = ACE_ALL;
static KEY *lookup_array = 0;

static void
run_iterator_cache (MAP_CACHE &cache)
{
  size_t iterations = cache.current_size ();
  size_t counter = 0;
  MAP_CACHE::iterator end = cache.end ();

  for (MAP_CACHE::iterator iter = cache.begin ();
       iter != end;
       ++iter)
    {
      // Debugging info.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%d|%d)"),
                  (*iter).first (),
                  (*iter).second ()));

      ACE_TEST_ASSERT ((*iter).first () == (*iter).second ());
      ++counter;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));

  ACE_UNUSED_ARG (iterations);
  ACE_TEST_ASSERT (counter == iterations);
}

static void
run_iterator_hash_cache (HASH_MAP_CACHE &cache)
{
  size_t iterations = cache.current_size ();
  size_t counter = 0;
  HASH_MAP_CACHE::iterator end = cache.end ();

  for (HASH_MAP_CACHE::iterator iter = cache.begin ();
       iter != end;
       ++iter)
    {
      // Debugging info.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%d|%d)"),
                  (*iter).first (),
                  (*iter).second ()));

      ACE_TEST_ASSERT ((*iter).first () == (*iter).second ());
      ++counter;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));

  ACE_UNUSED_ARG (iterations);
  ACE_TEST_ASSERT (counter == iterations);
}

static void
run_reverse_iterator_cache (MAP_CACHE &cache)
{
  size_t counter = cache.current_size ();
  MAP_CACHE::reverse_iterator  rend = cache.rend ();

  for (MAP_CACHE::reverse_iterator iter = cache.rbegin ();
       iter != rend;
       ++iter)
    {
      ACE_TEST_ASSERT ((*iter).first () == (*iter).second ());

      // Debugging info.
      if (debug)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%d|%d)"),
                    (*iter).first (),
                    (*iter).second ()));
      --counter;
    }

  if (debug)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));

  ACE_TEST_ASSERT (counter == 0);
}

static void
run_reverse_iterator_hash_cache (HASH_MAP_CACHE &cache)
{
  size_t counter = cache.current_size ();
  HASH_MAP_CACHE::reverse_iterator rend = cache.rend ();

  for (HASH_MAP_CACHE::reverse_iterator iter = cache.rbegin ();
       iter != rend;
       ++iter)
    {
      ACE_TEST_ASSERT ((*iter).first () == (*iter).second ());

      // Debugging info.
      if (debug)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%d|%d)"),
                    (*iter).first (),
                    (*iter).second ()));
      --counter;
    }

  if (debug)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));

  ACE_TEST_ASSERT (counter == 0);
}

static void
find_test_cache (MAP_CACHE &cache)
{
  for (size_t i = 0; i < no_of_lookups; ++i)
    {
      VALUE j = 0;
      int result = cache.find (lookup_array[i], j);
      result = cache.find (lookup_array[i]);
      ACE_TEST_ASSERT (result != -1);
      ACE_TEST_ASSERT (j == lookup_array[i]);

      if (debug)
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d  "), j));

      // Once more, with only the key. Test Bugzilla #4029.
      result = cache.find (lookup_array[i]);
      ACE_TEST_ASSERT (result != -1);

      ACE_UNUSED_ARG (result);
    }

  if (debug)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));
}

static void
find_test_hash_cache (HASH_MAP_CACHE &cache)
{
  for (size_t i = 0; i < no_of_lookups; ++i)
    {
      VALUE j = 0;
      int result = cache.find (lookup_array[i], j);

      ACE_TEST_ASSERT (result != -1);
      ACE_TEST_ASSERT (j == lookup_array[i]);

      if (debug)
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d  "), j));

      ACE_UNUSED_ARG (result);
    }

  if (debug)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\n")));
}

static void
purge_test_cache (MAP_CACHE &cache)
{
  // Get the number of entries in the container.
  size_t current_map_size = cache.current_size ();

  // Find the number of entries which will get purged.
  size_t entries_to_remove = size_t ((double (purge_percent) / 100 * current_map_size) + 0.5);

  // Tell the caching strategy how much to purge.
  cache.caching_strategy ().purge_percent (purge_percent);

  // Purge from cache.
  int result = cache.purge ();
  ACE_TEST_ASSERT (result != -1);
  ACE_UNUSED_ARG (result);

  size_t resultant_size = 0;
  if (caching_strategy_type == ACE_NULL)
    resultant_size = current_map_size;
  else
    resultant_size = current_map_size - entries_to_remove;

  // Make sure the purge took out the appropriate number of entries.
  ACE_TEST_ASSERT (cache.current_size () == resultant_size);
  ACE_UNUSED_ARG (resultant_size);
}

static void
purge_test_hash_cache (HASH_MAP_CACHE &cache)
{
  // Get the number of entries in the container.
  size_t current_map_size = cache.current_size ();

  // Find the number of entries which will get purged.
  size_t entries_to_remove = size_t ((double (purge_percent) / 100 * current_map_size) + 0.5);

  // Tell the caching strategy how much to purge.
  cache.caching_strategy ().purge_percent (purge_percent);

  // Purge from cache.
  int result = cache.purge ();
  ACE_TEST_ASSERT (result != -1);
  ACE_UNUSED_ARG (result);

  size_t resultant_size = 0;
  if (caching_strategy_type == ACE_NULL)
    resultant_size = current_map_size;
  else
    resultant_size = current_map_size - entries_to_remove;

  // Make sure the purge took out the appropriate number of entries.
  ACE_TEST_ASSERT (cache.current_size () == resultant_size);
  ACE_UNUSED_ARG (resultant_size);
}

static void
functionality_test_cache (MAP_CACHING_STRATEGY &caching_strategy)
{
  MAP_CACHE cache (caching_strategy);
  KEY i = 0;
  VALUE j = 0;

  // Add it to the map now.
  for (size_t counter = 0;
       i < iterations;
       ++i, ++j)
    {
      int result = cache.bind (i, j);
      ACE_TEST_ASSERT (result == 0);
      ACE_UNUSED_ARG (result);

      if (debug)
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("keys[%d]=%d value=[%d]=%d\n"),
                    i, i, j, j));
      ++counter;
      ACE_TEST_ASSERT (cache.current_size () == counter);
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Number of entries in cache before purging: %d\n"),
              cache.current_size ()));

  run_iterator_cache (cache);
  run_reverse_iterator_cache (cache);

  find_test_cache (cache);

  purge_test_cache (cache);

  run_iterator_cache (cache);
  run_reverse_iterator_cache (cache);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Number of entries in cache  after purging: %d\n"),
              cache.current_size ()));
}

static void
functionality_test_hash_cache (HASH_MAP_CACHING_STRATEGY &caching_strategy)
{
  HASH_MAP_CACHE cache (caching_strategy);
  KEY i = 0;
  VALUE j = 0;

  // Add it to the map now.
  for (size_t counter = 0;
       i < iterations;
       ++i, ++j)
    {
      int result = cache.bind (i, j);
      ACE_TEST_ASSERT (result == 0);
      ACE_UNUSED_ARG (result);

      if (debug)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("keys[%d]=%d value=[%d]=%d\n"),
                    i, i, j, j));
      ++counter;
      ACE_TEST_ASSERT (cache.current_size () == counter);
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Number of entries in cache before purging: %d\n"),
              cache.current_size ()));

  run_iterator_hash_cache (cache);
  run_reverse_iterator_hash_cache (cache);

  find_test_hash_cache (cache);

  purge_test_hash_cache (cache);

  run_iterator_hash_cache (cache);
  run_reverse_iterator_hash_cache (cache);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Number of entries in cache  after purging: %d\n"),
              cache.current_size ()));
}

void
test_caching_strategy_type (void)
{
  HASH_MAP_CACHING_STRATEGY *hash_map_caching_strategy = 0;
  MAP_CACHING_STRATEGY *map_caching_strategy = 0;

  switch (caching_strategy_type)
    {
    case ACE_NULL:
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nNull_Caching_Strategy\n\n")));
      ACE_NEW (map_caching_strategy,
               MAP_NULL_ADAPTER);
      ACE_NEW (hash_map_caching_strategy,
               HASH_MAP_NULL_ADAPTER);
      break;

    case ACE_LRU:
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nLRU_Caching_Strategy\n\n")));
      ACE_NEW (map_caching_strategy,
               MAP_LRU_ADAPTER);
      ACE_NEW (hash_map_caching_strategy,
               HASH_MAP_LRU_ADAPTER);
      break;

    case ACE_LFU:
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nLFU_Caching_Strategy\n\n")));
      ACE_NEW (map_caching_strategy,
               MAP_LFU_ADAPTER);
      ACE_NEW (hash_map_caching_strategy,
               HASH_MAP_LFU_ADAPTER);
      break;

    case ACE_FIFO:
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nFIFO_Caching_Strategy\n\n")));
      ACE_NEW (map_caching_strategy,
               MAP_FIFO_ADAPTER);
      ACE_NEW (hash_map_caching_strategy,
               HASH_MAP_FIFO_ADAPTER);
      break;

    case ACE_ALL: // Just to remove warnings!
      break;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("map cache\n")));
  functionality_test_cache (*map_caching_strategy);

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nhash map cache\n")));
  functionality_test_hash_cache (*hash_map_caching_strategy);

  delete map_caching_strategy;
  delete hash_map_caching_strategy;
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("c:i:r:f:p:d"));

  int cc;
  while ((cc = get_opt ()) != -1)
    {
    switch (cc)
      {
      case 'c':
        if (ACE_OS::strcmp (get_opt.opt_arg (), ACE_TEXT ("null")) == 0)
          caching_strategy_type = ACE_NULL;
        if (ACE_OS::strcmp (get_opt.opt_arg (), ACE_TEXT ("lru")) == 0)
          caching_strategy_type = ACE_LRU;
        if (ACE_OS::strcmp (get_opt.opt_arg (), ACE_TEXT ("lfu")) == 0)
          caching_strategy_type = ACE_LFU;
        if (ACE_OS::strcmp (get_opt.opt_arg (), ACE_TEXT ("fifo")) == 0)
          caching_strategy_type = ACE_FIFO;
        break;
      case 'i':
        iterations = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'f':
        no_of_lookups = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'r':
        randomize_lookups = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'p':
        purge_percent = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'd':
        debug = 1;
        break;
      case '?':
      case 'h':
      default:
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("usage: %s ")
                    ACE_TEXT ("[-c (caching strategy: lru / lfu / fifo / null [default = all])] ")
                    ACE_TEXT ("[-r (randomize lookups)] ")
                    ACE_TEXT ("[-i (iterations)] ")
                    ACE_TEXT ("[-d (debug, i.e., addition printouts)] ")
                    ACE_TEXT ("[-p (purge percent)] ")
                    ACE_TEXT ("[-f (number of lookups)]\n"),
                    ACE_TEXT ("Cache_Map_Manager_Test")));
        return -1;
      }
    }
  return 0;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  // Validate options.
  int result = parse_args (argc, argv);
  if (result != 0)
    return result;

  // Start the test only if options are valid.
  ACE_START_TEST (ACE_TEXT ("Cache_Map_Manager_Test"));

  // Remove the extra debugging attributes from Log_Msg output.
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::VERBOSE_LITE);

  // Providing random a unique seed.
  ACE_OS::srand (static_cast<u_int> (ACE_OS::time (0)));

  // Create the lookup array.
  ACE_NEW_RETURN (lookup_array,
                  KEY[no_of_lookups],
                  -1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\nLookup sequence: ")));

  // Initialize the lookup array.
  for (size_t k = 0;
       k < no_of_lookups;
       ++k)
    {
      if (randomize_lookups != 0)
        lookup_array[k] = ACE_OS::rand () % iterations;
      else
        lookup_array[k] = k % iterations;

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%d  "),
                  lookup_array[k]));
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n\n")));

  // Do we need to test all the strategies.
  if (caching_strategy_type == ACE_ALL)
    {
      caching_strategy_type = ACE_NULL;
      test_caching_strategy_type ();

      caching_strategy_type = ACE_LRU;
      test_caching_strategy_type ();

      caching_strategy_type = ACE_LFU;
      test_caching_strategy_type ();

      caching_strategy_type = ACE_FIFO;
      test_caching_strategy_type ();
    }
  else
    {
      test_caching_strategy_type ();
    }

  delete[] lookup_array;

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::VERBOSE_LITE);
  ACE_END_TEST;

  return 0;
}

