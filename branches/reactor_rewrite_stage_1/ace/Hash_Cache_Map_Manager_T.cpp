// $Id$

#ifndef ACE_HASH_CACHE_MAP_MANAGER_T_C
#define ACE_HASH_CACHE_MAP_MANAGER_T_C

#include "ace/Hash_Cache_Map_Manager_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if !defined (__ACE_INLINE__)
#include "ace/Hash_Cache_Map_Manager_T.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ace, Hash_Cache_Map_Manager_T, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Hash_Cache_Map_Manager)

#define T_1 class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY, class ATTRIBUTES
#define T_2 KEY, VALUE,  HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY, ATTRIBUTES

template <T_1>
ACE_Hash_Cache_Map_Manager<T_2>::ACE_Hash_Cache_Map_Manager (CACHING_STRATEGY &caching_s,
                                                             size_t size,
                                                             ACE_Allocator *alloc)
  : ACE_HCMM_BASE (caching_s,
                   size,
                   alloc)
{
}

template <T_1>
ACE_Hash_Cache_Map_Manager<T_2>::~ACE_Hash_Cache_Map_Manager (void)
{
}

template <T_1> int
ACE_Hash_Cache_Map_Manager<T_2>:: bind (const KEY &key,
                                        const VALUE &value,
                                        ACE_Hash_Map_Entry<KEY, ACE_Pair<VALUE, ATTRIBUTES> > *&entry)
{
  // Insert a entry which has the <key> and the <cache_value> which is
  // the combination of the <value> and the attributes of the caching
  // strategy.
  CACHE_VALUE cache_value (value,
                           this->caching_strategy_.attributes ());

  int bind_result = this->map_.bind (key,
                                     cache_value,
                                     entry);

  if (bind_result != -1)
    {

      int result = this->caching_strategy_.notify_bind (bind_result,
                                                        cache_value.second ());

      if (result == -1)
        {

          this->map_.unbind (key);

          // Unless the notification goes thru the bind operation is
          // not complete.
          bind_result = -1;

        }
    }

  return bind_result;
}

template <T_1> int
ACE_Hash_Cache_Map_Manager<T_2>::rebind (const KEY &key,
                                         const VALUE &value,
                                         ACE_Hash_Map_Entry<KEY, ACE_Pair<VALUE, ATTRIBUTES> > *&entry)
{
  CACHE_VALUE cache_value (value,
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

          // Make sure the unbind operation is done only when the
          // notification fails after a bind which is denoted by
          // rebind_result = 0
          if (rebind_result == 0)
            this->map_.unbind (key);

          // Unless the notification goes thru the rebind operation is
          // not complete.
          rebind_result = -1;

        }

    }

  return rebind_result;
}

template <T_1> int
ACE_Hash_Cache_Map_Manager<T_2>::trybind (const KEY &key,
                                          VALUE &value,
                                          ACE_Hash_Map_Entry<KEY, ACE_Pair<VALUE, ATTRIBUTES> > *&entry)
{
  CACHE_VALUE cache_value (value,
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
            this->map_.unbind (key);

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

template <T_1> int
ACE_Hash_Cache_Map_Manager<T_2>::find (const KEY &key,
                                       ACE_Hash_Map_Entry<KEY, ACE_Pair<VALUE, ATTRIBUTES> > *&entry)
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
        find_result = 0;

    }

  return find_result;
}

template <T_1> int
ACE_Hash_Cache_Map_Manager<T_2>::find (const KEY &key,
                                       VALUE &value)
{
  CACHE_ENTRY *entry = 0;

  int result = this->find (key,
                           entry);

  if (result != -1)
    {
      value = entry->int_id_.first ();
    }

  return result;
}

template <T_1> int
ACE_Hash_Cache_Map_Manager<T_2>::find (const KEY &key)
{
  CACHE_ENTRY *entry = 0;

  return this->find (key,
                     entry);
}

template <T_1> int
ACE_Hash_Cache_Map_Manager<T_2>::unbind (ACE_Hash_Map_Entry<KEY, ACE_Pair<VALUE, ATTRIBUTES> > *entry)
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

#undef T_1
#undef T_2

#endif /* ACE_HASH_CACHE_MAP_MANAGER_T_C */
