/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Hash_Cache_Map_Manager.h
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef HASH_CACHE_MAP_MANAGER_T_H
#define HASH_CACHE_MAP_MANAGER_T_H

#include "ace/Hash_Map_Manager_T.h"
#include "ace/Cache_Map_Manager_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define  ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declaration.
class ACE_Allocator;

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY, class ATTRIBUTES>
class ACE_Hash_Cache_Map_Manager : public ACE_Cache_Map_Manager< KEY, VALUE, 
                                   ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTRIBUTES>, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex>,
                                   ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTRIBUTES>, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex>,
                                   ACE_Hash_Map_Reverse_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTRIBUTES>, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex>,
                                   CACHING_STRATEGY, ATTRIBUTES>
{
  // = TITLE
  //     Defines a abstraction which will purge entries from a map.
  //     The map considered is the ACE_Hash_Map_Manager_Ex.
  //
  // = DESCRIPTION
  //     The Hash_Cache_Map_Manager will manage the map it contains
  //     and provide purging on demand from the map. The strategy for
  //     caching is decided by the user and provided to the Cache
  //     Manager.  The Cache Manager acts as a agent and communicates
  //     between the Map and the Strategy for purging entries from the
  //     map.  To tap the optimal methods like find(key,value,entry)
  //     present in the ACE_Hash_Map_Manager,
  //     Hash_Cache_Map_Manager provides extra functionality on top
  //     of the Cache_Map_Manager.
  //
  //     No locking mechanism provided since locking at this level
  //     isnt efficient.  Locking has to be provided by the
  //     application.
 public:

  typedef ACE_Pair<VALUE, ATTRIBUTES> CACHE_VALUE;
  typedef ACE_Hash_Map_Manager_Ex<KEY, CACHE_VALUE, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> HASH_MAP;
  typedef ACE_Hash_Map_Entry<KEY, CACHE_VALUE> CACHE_ENTRY;
  typedef KEY key_type;
  typedef VALUE mapped_type;
  // The actual value mapped to the key in the map. The <attributes>
  // are used by the strategy and is transparent to the user of this
  // class.

  // = Initialization and termination methods.
  ACE_Hash_Cache_Map_Manager (size_t size = ACE_DEFAULT_MAP_SIZE,
                              ACE_Allocator *alloc = 0,
                              CACHING_STRATEGY *caching_s = 0,
                              int delete_caching_strategy = 1);
  // Initialize a <Hash_Cache_Map_Manager> with <size> entries.
  // By default the caching strategy is allocated and deallocated by
  // the class but if needed it can be changed as per the users need.
  // The <delete_on_destruction> flag simply tells the class whether
  // the ownership is given to the class or not.
  
  ~ACE_Hash_Cache_Map_Manager (void);
  // Close down a <Cache_Map_Manager> and release dynamically allocated
  // resources.

  int bind (const KEY &key,
            const VALUE &value);
  // Associate <key> with <value>.  If <key> is already in the
  // MAP then the ENTRY is not changed.  Returns 0 if a new entry is
  // bound successfully, returns 1 if an attempt is made to bind an
  // existing entry, and returns -1 if failures occur.

  int bind (const KEY &key,
            const VALUE &value,
            CACHE_ENTRY *&entry);
  // Same as a normal bind, except the cache entry is also passed back
  // to the caller.  The entry in this case will either be the newly
  // created entry, or the existing one.

  int find (const KEY &key,
            VALUE &value);
  // Loopkup entry<key,value> in the cache.

  int find (const KEY &key);
  // Is <key> in the cache?

  int find (const KEY &key,
            CACHE_ENTRY *&entry);
  // Obtain the entry when the find succeeds.

  int rebind (const KEY &key,
              const VALUE &value);
  // Reassociate the <key> with <value>. If the <key> already exists
  // in the cache then returns 1, on a new bind returns 0 and returns
  // -1 in case of any failures.

  int rebind (const KEY &key,
              const VALUE &value,
              VALUE &old_value);
  // Reassociate <key> with <value>, storing the old value into the
  // "out" parameter <old_value>.  The function fails if <key> is not
  // in the cache for caches that do not allow user specified keys.
  // However, for caches that allow user specified keys, if the key is
  // not in the cache, a new <key>/<value> association is created.

  int rebind (const KEY &key,
              const VALUE &value,
              KEY &old_key,
              VALUE &old_value);
  // Reassociate <key> with <value>, storing the old key and value
  // into the "out" parameters <old_key> and <old_value>.  The
  // function fails if <key> is not in the cache for caches that do not
  // allow user specified keys.  However, for caches that allow user
  // specified keys, if the key is not in the cache, a new <key>/<value>
  // association is created.

  int rebind (const KEY &key,
              const VALUE &value,
              CACHE_ENTRY *&entry);
  // Same as a normal rebind, except the cache entry is also passed back
  // to the caller.  The entry in this case will either be the newly
  // created entry, or the existing one.

  int trybind (const KEY &key,
               VALUE &value);
  // Associate <key> with <value> if and only if <key> is not in the
  // cache.  If <key> is already in the cache, then the <value> parameter
  // is overwritten with the existing value in the cache. Returns 0 if a
  // new <key>/<value> association is created.  Returns 1 if an
  // attempt is made to bind an existing entry.  This function fails
  // for maps that do not allow user specified keys.

  int trybind (const KEY &key,
               VALUE &value,
               CACHE_ENTRY *&entry);
  // Same as a normal trybind, except the cache entry is also passed
  // back to the caller.  The entry in this case will either be the
  // newly created entry, or the existing one.

  int unbind (const KEY &key);
  // Remove <key> from the cache.

  int unbind (const KEY &key,
              VALUE &value);
  // Remove <key> from the cache, and return the <value> associated with
  // <key>.

  int unbind (CACHE_ENTRY *entry);
  // Remove entry from map.

protected:

  typedef ACE_Cache_Map_Manager<KEY, VALUE, 
                                ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTRIBUTES>, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex>,
                                ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTRIBUTES>, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex>,
                                ACE_Hash_Map_Reverse_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTRIBUTES>, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex>,
                                CACHING_STRATEGY, ATTRIBUTES>
    ACE_HCMM_BASE;
  // Base class.
};


#if defined (__ACE_INLINE__)
#include "ace/Hash_Cache_Map_Manager_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Hash_Cache_Map_Manager_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ace/Hash_Cache_Map_Manager_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* HASH_CACHE_MAP_MANAGER_T_H */
