// $Id$

#ifndef ACE_OPTIMAL_CACHE_MAP_MANAGER_T_C
#define ACE_OPTIMAL_CACHE_MAP_MANAGER_T_C

#define ACE_BUILD_DLL

#include "Optimal_Cache_Map_Manager_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Malloc.h"
#include "ace/Service_Config.h"

#if !defined (__ACE_INLINE__)
#include "Optimal_Cache_Map_Manager_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Web_Crawler, Optimal_Cache_Map_Manager_T, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Pair)

ACE_ALLOC_HOOK_DEFINE(ACE_Reference_Pair)

ACE_ALLOC_HOOK_DEFINE(ACE_Optimal_Cache_Map_Manager)

ACE_ALLOC_HOOK_DEFINE(ACE_Optimal_Cache_Map_Iterator)

ACE_ALLOC_HOOK_DEFINE(ACE_Optimal_Cache_Map_Reverse_Iterator)
  
template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> int
ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::bind (const KEY &key,
                                                                                           const VALUE &value)
{
  // Insert a entry which has the <key> and the <cache_value> which is
  // the combination of the <value> and the attributes of the caching strategy.  
  ACE_Pair<VALUE, ATTRIBUTES> cache_value (value,
                                           this->caching_strategy_.attributes ());
  
  int bind_result =this->map_.bind (key,
                                    cache_value);
  if (bind_result != -1)
    {
      int result = this->caching_strategy_.notify_bind (bind_result, 
                                                        cache_value.second ());
      if (result == -1)
        {
          this->map_.unbind (key,
                             cache_value);
          // Unless the notification goes thru the bind operation is not complete.
          bind_result = -1;
        }
    }
  return bind_result;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> int
ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>:: bind (const KEY &key,
                                                                                            const VALUE &value,
                                                                                            ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::CACHE_ENTRY *&entry)
{
  // Insert a entry which has the <key> and the <cache_value> which is
  // the combination of the <value> and the attributes of the caching strategy.  
  ACE_Pair<VALUE, ATTRIBUTES> cache_value (value,
                                           this->caching_strategy_.attributes ());
  
  int bind_result =this->map_.bind (key,
                                    cache_value,
                                    entry);
  if (bind_result != -1)
    {
      int result = this->caching_strategy_.notify_bind (bind_result, 
                                                        cache_value.second ());

      if (result == -1)
        {
          this->map_.unbind (key,
                             cache_value);
          // Unless the notification goes thru the bind operation is not complete.
          bind_result = -1;
        }
    }
  return bind_result;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> int
ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::rebind (const KEY &key,
                                                                                             const VALUE &value)
{
  ACE_Pair<VALUE, ATTRIBUTES> cache_value (value,
                                           this->caching_strategy_.attributes ());
  int rebind_result = this->map_.rebind (key,
                                         cache_value);
  if (rebind_result != -1)
    {
      int result = this->caching_strategy_.notify_rebind (rebind_result, 
                                                          cache_value.second ());
      if (result == -1)
        {
          this->map_.unbind (key,
                             cache_value);
          // Unless the notification goes thru the rebind operation is not complete.
          rebind_result = -1;
        }
    }
  return rebind_result; 
}


template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> int
ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::rebind (const KEY &key,
                                                                                             const VALUE &value,
                                                                                             VALUE &old_value)
{
  ACE_Pair<VALUE, ATTRIBUTES> cache_value (value,
                                           this->caching_strategy_.attributes ());
  ACE_Pair<VALUE, ATTRIBUTES> old_cache_value (old_value,
                                               this->caching_strategy_.attributes ());
  int rebind_result = this->map_.rebind (key,
                                         cache_value,
                                         old_cache_value);
  if (rebind_result != -1)
    {
      int result = this->caching_strategy_.notify_rebind (rebind_result, 
                                                          cache_value.second ());
                                      
      if (result == -1)
        {
          this->map_.unbind (key,
                             cache_value);
          // Unless the notification goes thru the rebind operation is not complete.
          rebind_result = -1;
        }
      else
        old_value = old_cache_value.first ();
    }
  return rebind_result; 
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> int
ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::rebind (const KEY &key,
                                                                                             const VALUE &value,
                                                                                             KEY &old_key,
                                                                                             VALUE &old_value)
{
  ACE_Pair<VALUE, ATTRIBUTES> cache_value (value,
                                           this->caching_strategy_.attributes ());
  ACE_Pair<VALUE, ATTRIBUTES> old_cache_value (old_value,
                                               this->caching_strategy_.attributes ());
  int rebind_result = this->map_.rebind (key,
                                         cache_value,
                                         old_key,
                                         old_cache_value);
  if (rebind_result != -1)
    {
      int result = this->caching_strategy_.notify_rebind (rebind_result, 
                                                          cache_value.second ());
                                      
      if (result == -1)
        {
          this->map_.unbind (key,
                             cache_value);
          // Unless the notification goes thru the rebind operation is not complete.
          rebind_result = -1;
        }
      else
        old_value = old_cache_value.first ();
    }
  return rebind_result; 
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> int
ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::rebind (const KEY &key,
                                                                                             const VALUE &value,
                                                                                             ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::CACHE_ENTRY *&entry)
{
  ACE_Pair<VALUE, ATTRIBUTES> cache_value (value,
                           this->caching_strategy_.attributes ());
  int rebind_result = this->map_.rebind (key,
                                         cache_value,
                                         entry);
  if (rebind_result != -1)
    {
      int result = this->caching_strategy_.notify_rebind (rebind_result, 
                                                          cache_value.second ());
      if (result == -1)
        {
          this->map_.unbind (key,
                             cache_value);
          // Unless the notification goes thru the rebind operation is not complete.
          rebind_result = -1;
        }
    }
  return rebind_result; 
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> int
ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::trybind (const KEY &key,
                                                                                              VALUE &value)
{
  ACE_Pair<VALUE, ATTRIBUTES> cache_value (value, 
                           this->caching_strategy_.attributes ());
  int trybind_result = this->map_.trybind (key,
                                           cache_value);
  if (trybind_result != -1)
    {
      int result = this->caching_strategy_.notify_trybind (trybind_result,
                                                           cache_value.second ());
      if (result == -1)
        {
          // If the entry has got inserted into the map, it is removed
          // due to failure.
          if (trybind_result == 0)
            this->map_.unbind (key,
                               cache_value);
          trybind_result = -1;
        }
      else
        {
          // If an attempt is made to bind an existing entry the value
          // is overwritten with the value from the map.
          if (trybind_result == 1)
            value = cache_value.first ();
        } 
      
    }
  return trybind_result;
}
template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> int
ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::trybind (const KEY &key,
                                                                                              VALUE &value,
                                                                                              ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::CACHE_ENTRY *&entry)
{
  ACE_Pair<VALUE, ATTRIBUTES> cache_value (value, 
                           this->caching_strategy_.attributes ());
  int trybind_result = this->map_.trybind (key,
                                           cache_value,
                                           entry);
  if (trybind_result != -1)
    {
      int result = this->caching_strategy_.notify_trybind (trybind_result,
                                                           cache_value.second ());
      if (result == -1)
        {
          // If the entry has got inserted into the map, it is removed
          // due to failure.
          if (trybind_result == 0)
            this->map_.unbind (key,
                               cache_value);
          trybind_result = -1;
        }
      else
        {
          // If an attempt is made to bind an existing entry the value
          // is overwritten with the value from the map.
          if (trybind_result == 1)
            value = cache_value.first ();
        } 
      
    }
  return trybind_result;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> int
ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::find (const KEY& key,
                                                                                           VALUE& value)
{
  // Lookup the key and populate the <value>.
  ACE_Pair<VALUE, ATTRIBUTES> cache_value;
                          
  int find_result = this->map_.find (key,
                                     cache_value);
  if (find_result != -1)
  {
    int result = this->caching_strategy_.notify_find (find_result,
                                                      cache_value.second ());
    // Unless the find and notification operations go thru, this
    // method is not successful.
    if (result == -1)
      find_result = -1;
    else
      {
        // Since the <cache_value> has now changed after the
        // notification, we need to bind to the map again.
        int rebind_result = this->map_.rebind (key,
                                               cache_value);
        if (rebind_result == -1)
          find_result = -1;
        else
          value = cache_value.first ();
      }
  }
  return find_result;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> int
ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::find (const KEY& key)
{
  // Lookup the key and populate the <value>.
  ACE_Pair<VALUE, ATTRIBUTES> cache_value;
                          
  int find_result = this->map_.find (key,
                                     cache_value);
  if (find_result != -1)
  {
    int result = this->caching_strategy_.notify_find (find_result,
                                                      cache_value.second ());
    // Unless the find and notification operations go thru, this
    // method is not successful.
    if (result == -1)
      find_result = -1;
    else
      {
        // Since the <cache_value> has now changed after the
        // notification, we need to bind to the map again.
        int rebind_result = this->map_.rebind (key,
                                               cache_value);
        if (rebind_result == -1)
          find_result = -1;
      }
  }
  return find_result;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> int
ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::find (const KEY& key,
                                                                                           ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::CACHE_ENTRY *&entry)
{
  // Lookup the key and populate the <value>.
  int find_result = this->map_.find (key,
                                     entry);
  if (find_result != -1)
  {
    int result = this->caching_strategy_.notify_find (find_result,
                                                      entry->int_id_.second ());
    // Unless the find and notification operations go thru, this
    // method is not successful.
    if (result == -1)
      find_result = -1;
    else
      {
        // Since the <cache_value> has now changed after the
        // notification, we need to bind to the map again.
        int rebind_result = this->map_.rebind (key,
                                               entry->int_id_);
        if (rebind_result == -1)
          find_result = -1;
      }
  }
  return find_result;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> int
ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::unbind (const KEY& key)
{
 // Remove the entry from the cache.
  ACE_Pair<VALUE, ATTRIBUTES> cache_value;
  int unbind_result = this->map_.unbind (key,
                                         cache_value);
  
  if (unbind_result != -1)
    {
      int result = this->caching_strategy_.notify_unbind (unbind_result,
                                                          cache_value.second ());

      if (result == -1)
        unbind_result = -1;
    }
  return unbind_result; 
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> int
ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::unbind (const KEY& key,
                                                                                             VALUE& value)
{
  // Remove the entry from the cache.
  ACE_Pair<VALUE, ATTRIBUTES> cache_value;

  int unbind_result = this->map_.unbind (key,
                                         cache_value);
                                        
  if (unbind_result != -1)
    {
      int result = this->caching_strategy_.notify_unbind (unbind_result,
                                                          cache_value.second ());
      if (result == -1)
        unbind_result = -1;
      else
        value = cache_value.first ();
    }
  return unbind_result;
}

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> int
ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::unbind (ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::CACHE_ENTRY *entry)
{
  // Remove the entry from the cache.
  int unbind_result = this->map_.unbind (entry);
                                        
                                        
  if (unbind_result != -1)
    {
      int result = this->caching_strategy_.notify_unbind (unbind_result,
                                                          entry->int_id_.second ());
      if (result == -1)
        unbind_result = -1;
    }
  return unbind_result;
}
                                                                                                 
template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY> void
ACE_Optimal_Cache_Map_Manager<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>::dump (void) const
{
  this->map_.dump ();
  this->caching_strategy_.dump ();
}

#endif /* ACE_OPTIMAL_CACHE_MAP_MANAGER_T_C */
