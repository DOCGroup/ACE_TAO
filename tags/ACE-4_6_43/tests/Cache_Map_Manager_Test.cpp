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
//     This is a test of the <ACE_Cache_Map_Manager> and
//     <ACE_Hash_Cache_Map_Manager> that illustrates how to use the
//     forward and reverse iterators, as well as the purging and
//     caching features.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
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
#include "ace/Get_Opt.h"

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

typedef ACE_Hash_Map_Manager_Ex<KEY, CACHE_VALUE, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>
	HASH_MAP_MANAGER;

typedef ACE_Hash_Map_Iterator_Ex<KEY, CACHE_VALUE, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>
	HASH_MAP_MANAGER_ITERATOR;

typedef ACE_Map_Manager<KEY, CACHE_VALUE, ACE_Null_Mutex>
	MAP_MANAGER;

typedef ACE_Map_Iterator<KEY, CACHE_VALUE, ACE_Null_Mutex>
	MAP_MANAGER_ITERATOR;

typedef ACE_Map_Reverse_Iterator<KEY, CACHE_VALUE, ACE_Null_Mutex>
	MAP_MANAGER_REVERSE_ITERATOR;

typedef ACE_Pair_Caching_Utility<KEY, CACHE_VALUE, HASH_MAP_MANAGER, HASH_MAP_MANAGER_ITERATOR, ATTR>
	HASH_MAP_CACHING_UTILITY;

typedef ACE_Pair_Caching_Utility<KEY, CACHE_VALUE, MAP_MANAGER, MAP_MANAGER_ITERATOR, ATTR>
	MAP_CACHING_UTILITY;

typedef ACE_LRU_Caching_Strategy<KEY, CACHE_VALUE, HASH_MAP_MANAGER, ATTR, HASH_MAP_CACHING_UTILITY>
	HASH_MAP_LRU;

typedef ACE_LRU_Caching_Strategy<KEY, CACHE_VALUE, MAP_MANAGER, ATTR, MAP_CACHING_UTILITY>
	MAP_LRU;

typedef ACE_Hash_Cache_Map_Manager<KEY, VALUE, ACE_Hash<KEY>, ACE_Equal_To<KEY>, HASH_MAP_LRU, ATTR>
	HASH_MAP_CACHE;

typedef ACE_Cache_Map_Manager<KEY, VALUE, MAP_MANAGER, MAP_MANAGER_ITERATOR, MAP_MANAGER_REVERSE_ITERATOR, MAP_LRU, ATTR>
	MAP_CACHE;

static size_t iterations = ACE_MAX_ITERATIONS;
static size_t no_of_lookups = iterations / 2;
static int randomize_lookups = 1;
static int purge_percent = 10;

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
		  ASYS_TEXT ("(%d|%d)"),
		  (*iter).first (),
		  (*iter).second ()));

      ACE_ASSERT ((*iter).first () == (*iter).second ());
      ++counter;
    }

  ACE_DEBUG ((LM_DEBUG,
	      ASYS_TEXT ("\n")));

  ACE_UNUSED_ARG (iterations);
  ACE_ASSERT (counter == iterations);
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
		  ASYS_TEXT ("(%d|%d)"),
		  (*iter).first (),
		  (*iter).second ()));

      ACE_ASSERT ((*iter).first () == (*iter).second ());
      ++counter;
    }

  ACE_DEBUG ((LM_DEBUG,
	      ASYS_TEXT ("\n")));

  ACE_UNUSED_ARG (iterations);
  ACE_ASSERT (counter == iterations);
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
      ACE_ASSERT ((*iter).first () == (*iter).second ());

      // Debugging info.
      ACE_DEBUG ((LM_DEBUG,
		  ASYS_TEXT ("(%d|%d)"),
		  (*iter).first (),
		  (*iter).second ()));
      --counter;
    }

  ACE_DEBUG ((LM_DEBUG,
	      ASYS_TEXT ("\n")));
  ACE_ASSERT (counter == 0);
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
      ACE_ASSERT ((*iter).first () == (*iter).second ());

      // Debugging info.
      ACE_DEBUG ((LM_DEBUG,
		  ASYS_TEXT ("(%d|%d)"),
		  (*iter).first (),
		  (*iter).second ()));
      --counter;
    }

  ACE_DEBUG ((LM_DEBUG,
	      ASYS_TEXT ("\n")));
  ACE_ASSERT (counter == 0);
}

static void
find_test_cache (MAP_CACHE &cache)
{
  ACE_DEBUG ((LM_DEBUG, "find\n"));

  size_t total_entries = cache.current_size ();

  for (size_t i = 0; i < no_of_lookups; ++i)
    {
      KEY key = i;
      if (randomize_lookups != 0)
	key = ACE_OS::rand () % total_entries;

      VALUE j;
      int result = cache.find (key, j);

      ACE_ASSERT (result != -1);
      ACE_ASSERT (j == key);

      ACE_DEBUG ((LM_DEBUG,
		  "%d  ",
		  j));
    }

  ACE_DEBUG ((LM_DEBUG, "\n"));
}

static void 
find_test_hash_cache (HASH_MAP_CACHE &cache)
{
  ACE_DEBUG ((LM_DEBUG, "find\n"));

  size_t total_entries = cache.current_size ();

  for (size_t i = 0; i < no_of_lookups; ++i)
    {
      KEY key = i;
      if (randomize_lookups != 0)
	key = ACE_OS::rand () % total_entries;

      VALUE j;
      int result = cache.find (key, j);

      ACE_ASSERT (result != -1);
      ACE_ASSERT (j == key);

      ACE_DEBUG ((LM_DEBUG,
		  "%d  ",
		  j));
    }

  ACE_DEBUG ((LM_DEBUG, "\n"));
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
  ACE_ASSERT (result != -1);

  size_t resultant_size = current_map_size - entries_to_remove;
  ACE_UNUSED_ARG (resultant_size);

  // Make sure the purge took out the appropriate number of entries.
  ACE_ASSERT (cache.current_size () == resultant_size);
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
  ACE_ASSERT (result != -1);

  size_t resultant_size = current_map_size - entries_to_remove;
  ACE_UNUSED_ARG (resultant_size);

  // Make sure the purge took out the appropriate number of entries.
  ACE_ASSERT (cache.current_size () == resultant_size);
}

static void
functionality_test_cache (void)
{
  MAP_CACHE cache;
  KEY i = 0;
  VALUE j = 0;

  // Add it to the map now.
  size_t counter = 0;
  for (;
       i < iterations;
       ++i, ++j)
    {
      int result = cache.bind (i, j);
      ACE_ASSERT (result != -1);
      ACE_DEBUG ((LM_DEBUG, "keys[%d]=%d value=[%d]=%d\n",
		  i, i, j, j));
      ++counter;
      ACE_ASSERT (cache.current_size () == counter);
    }

  run_iterator_cache (cache);
  run_reverse_iterator_cache (cache);

  find_test_cache (cache);
  
  ACE_DEBUG ((LM_DEBUG,
	     "Number of entries in cache before purging : %d\n", 
	     cache.current_size ()));	  
  purge_test_cache (cache);
  ACE_DEBUG ((LM_DEBUG,
	      "Number of entries in cache after purging : %d\n", 
	      cache.current_size ()));	  

  run_iterator_cache (cache);
  run_reverse_iterator_cache (cache);
}

static void
functionality_test_hash_cache (void)
{
  HASH_MAP_CACHE cache;
  KEY i = 0;
  VALUE j = 0;

  // Add it to the map now.
  size_t counter = 0;
  for (;
       i < iterations;
       ++i, ++j)
    {
      int result = cache.bind (i, j);
      ACE_ASSERT (result != -1);
      ACE_DEBUG ((LM_DEBUG,
		  "keys[%d]=%d value=[%d]=%d\n",
		  i, i, j, j));
      ++counter;
      ACE_ASSERT (cache.current_size () == counter);
    }

  run_iterator_hash_cache (cache);
  run_reverse_iterator_hash_cache (cache);

  find_test_hash_cache (cache);

  ACE_DEBUG ((LM_DEBUG,
	      "Number of entries in cache before purging : %d\n", 
	      cache.current_size ()));		   
  purge_test_hash_cache (cache);
  ACE_DEBUG ((LM_DEBUG,
	      "Number of entries in cache after purging : %d\n", 
	      cache.current_size ()));

  run_iterator_hash_cache (cache);
  run_reverse_iterator_hash_cache (cache);
}

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "r:i:f:p:");

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 'i':
	iterations = atoi (get_opt.optarg);
	break;
      case 'f':
	no_of_lookups = atoi (get_opt.optarg);
	break;
      case 'r':
	randomize_lookups = atoi (get_opt.optarg);
	break;
      case 'p':
	purge_percent = atoi (get_opt.optarg);
	break;
      case '?':
      case 'h':
      default:
	ACE_ERROR ((LM_ERROR,
		    "usage: %s "
		    "[-r (randomize lookups)] "
		    "[-i (iterations)] "
		    "[-p (purge percent)] "
		    "[-f (number of lookups)] \n",
		    argv[0]));
	return -1;
      }

  return 0;
}

int
main (int argc, ASYS_TCHAR *argv[])
{
  int result = parse_args (argc, argv);
  if (result != 0)
    return result;

  ACE_START_TEST (ASYS_TEXT ("Cache_Map_Manager_Test"));
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::VERBOSE_LITE);

  ACE_OS::srand (ACE_static_cast (size_t, ACE_OS::time (0)));

  functionality_test_cache ();
  functionality_test_hash_cache ();

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::VERBOSE_LITE);
  ACE_END_TEST;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Pair<VALUE, ATTR>;
template class ACE_Reference_Pair<KEY, VALUE>;
template class ACE_Equal_To<KEY>;

template class ACE_Hash_Map_Manager_Ex<KEY, CACHE_VALUE, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<KEY, CACHE_VALUE, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<KEY, CACHE_VALUE, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<KEY, CACHE_VALUE, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<KEY, CACHE_VALUE>;

template class ACE_Map_Manager<KEY, CACHE_VALUE, ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<KEY, CACHE_VALUE, ACE_Null_Mutex>;
template class ACE_Map_Iterator<KEY, CACHE_VALUE, ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<KEY, CACHE_VALUE, ACE_Null_Mutex>;
template class ACE_Map_Entry<KEY, CACHE_VALUE>;

template class ACE_Pair_Caching_Utility<KEY, CACHE_VALUE, HASH_MAP_MANAGER, HASH_MAP_MANAGER::iterator, ATTR>;
template class ACE_Pair_Caching_Utility<KEY, CACHE_VALUE, MAP_MANAGER, MAP_MANAGER::iterator, ATTR>;

template class ACE_LRU_Caching_Strategy<KEY, CACHE_VALUE, HASH_MAP_MANAGER, ATTR, HASH_MAP_CACHING_UTILITY>;
template class ACE_LRU_Caching_Strategy<KEY, CACHE_VALUE, MAP_MANAGER, ATTR, MAP_CACHING_UTILITY>;

template class ACE_Default_Cleanup_Strategy<KEY, ACE_Pair<KEY, ATTR>, HASH_MAP_MANAGER>;
template class ACE_Default_Cleanup_Strategy<KEY, ACE_Pair<KEY, ATTR>, MAP_MANAGER>;

template class ACE_Cleanup_Strategy<KEY, ACE_Pair<KEY, ATTR>, HASH_MAP_MANAGER>;
template class ACE_Cleanup_Strategy<KEY, ACE_Pair<KEY, ATTR>, MAP_MANAGER>;

template class ACE_Cache_Map_Manager<KEY, VALUE, MAP_MANAGER, MAP_MANAGER::iterator, MAP_MANAGER::reverse_iterator, MAP_LRU, ATTR>;
template class ACE_Cache_Map_Iterator<KEY, VALUE, MAP_MANAGER::iterator, MAP_LRU, ATTR>;
template class ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, MAP_MANAGER::reverse_iterator, MAP_LRU, ATTR>;

template class ACE_Cache_Map_Manager<KEY, VALUE, HASH_MAP_MANAGER, HASH_MAP_MANAGER::iterator, HASH_MAP_MANAGER::reverse_iterator, HASH_MAP_LRU, ATTR>;
template class ACE_Cache_Map_Iterator<KEY, VALUE, HASH_MAP_MANAGER::iterator, HASH_MAP_LRU, ATTR>;
template class ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_MAP_MANAGER::reverse_iterator, HASH_MAP_LRU, ATTR>;
template class ACE_Hash_Cache_Map_Manager<KEY, VALUE, ACE_Hash<KEY>, ACE_Equal_To<KEY>, HASH_MAP_LRU, ATTR>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Pair<VALUE, ATTR>
#pragma instantiate ACE_Reference_Pair<KEY, VALUE>
#pragma instantiate ACE_Equal_To<KEY>

#pragma instantiate ACE_Hash_Map_Manager_Ex<KEY, CACHE_VALUE, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<KEY, CACHE_VALUE, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<KEY, CACHE_VALUE, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<KEY, CACHE_VALUE, ACE_Hash<KEY>, ACE_Equal_To<KEY>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<KEY, CACHE_VALUE>

#pragma instantiate ACE_Map_Manager<KEY, CACHE_VALUE, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<KEY, CACHE_VALUE, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<KEY, CACHE_VALUE, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<KEY, CACHE_VALUE, ACE_Null_Mutex>
#pragma instantiate ACE_Map_Entry<KEY, CACHE_VALUE>

#pragma instantiate ACE_Pair_Caching_Utility<KEY, CACHE_VALUE, HASH_MAP_MANAGER, HASH_MAP_MANAGER::iterator, ATTR>
#pragma instantiate ACE_Pair_Caching_Utility<KEY, CACHE_VALUE, MAP_MANAGER, MAP_MANAGER::iterator, ATTR>

#pragma instantiate ACE_LRU_Caching_Strategy<KEY, CACHE_VALUE, HASH_MAP_MANAGER, ATTR, HASH_MAP_CACHING_UTILITY>
#pragma instantiate ACE_LRU_Caching_Strategy<KEY, CACHE_VALUE, MAP_MANAGER, ATTR, MAP_CACHING_UTILITY>

#pragma instantiate ACE_Default_Cleanup_Strategy<KEY, ACE_Pair<KEY, ATTR>, HASH_MAP_MANAGER>
#pragma instantiate ACE_Default_Cleanup_Strategy<KEY, ACE_Pair<KEY, ATTR>, MAP_MANAGER>

#pragma instantiate ACE_Cleanup_Strategy<KEY, ACE_Pair<KEY, ATTR>, HASH_MAP_MANAGER>
#pragma instantiate ACE_Cleanup_Strategy<KEY, ACE_Pair<KEY, ATTR>, MAP_MANAGER>

#pragma instantiate ACE_Cache_Map_Manager<KEY, VALUE, MAP_MANAGER, MAP_MANAGER::iterator, MAP_MANAGER::reverse_iterator, MAP_LRU, ATTR>
#pragma instantiate ACE_Cache_Map_Iterator<KEY, VALUE, MAP_MANAGER::iterator, MAP_LRU, ATTR>
#pragma instantiate ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, MAP_MANAGER::reverse_iterator, MAP_LRU, ATTR>

#pragma instantiate ACE_Cache_Map_Manager<KEY, VALUE, HASH_MAP_MANAGER, HASH_MAP_MANAGER::iterator, HASH_MAP_MANAGER::reverse_iterator, HASH_MAP_LRU, ATTR>
#pragma instantiate ACE_Cache_Map_Iterator<KEY, VALUE, HASH_MAP_MANAGER::iterator, HASH_MAP_LRU, ATTR>
#pragma instantiate ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_MAP_MANAGER::reverse_iterator, HASH_MAP_LRU, ATTR>
#pragma instantiate ACE_Hash_Cache_Map_Manager<KEY, VALUE, ACE_Hash<KEY>, ACE_Equal_To<KEY>, HASH_MAP_LRU, ATTR>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
