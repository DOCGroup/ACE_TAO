/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Optimal_Cache_Map_Manager.h
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef OPTIMAL_CACHE_MAP_MANAGER_T_H
#define OPTIMAL_CACHE_MAP_MANAGER_T_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define  ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Pair_T.h"
#include "ace/Hash_Map_Manager_T.h"

// Forward declaration.
class ACE_Allocator;

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY>
class ACE_Optimal_Cache_Map_Iterator;

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY>
class ACE_Optimal_Cache_Map_Reverse_Iterator;

template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY>
class ACE_Optimal_Cache_Map_Manager
{
  // = TITLE
  //     Defines a abstraction which will purge entries from a map.
  //     The map considered is the ACE_Hash_Map_Manager.
  //
  // = DESCRIPTION

  //     The Optimal_Cache_Map_Manager will manage the map it contains
  //     and provide purging on demand from the map. The strategy for
  //     caching is decided by the user and provided to the Cache
  //     Manager.  The Cache Manager acts as a agent and communicates
  //     between the Map and the Strategy for purging entries from the
  //     map.  To tap the optimal methods like find(key,value,entry)
  //     present in the ACE_Hash_Map_Manager,
  //     Optimal_Cache_Map_Manager provides extra functionality on top
  //     of the Cache_Map_Manager.
  //
  //     No locking mechanism provided since locking at this level
  //     isnt efficient.  Locking has to be provided by the
  //     application.
public:

  // = Traits.
  typedef KEY key_type;
  typedef VALUE mapped_type;

  typedef ACE_TYPENAME CACHING_STRATEGY::ATTRIBUTES ATTRIBUTES;
  typedef ACE_Pair<VALUE, ATTRIBUTES> CACHE_VALUE;
  // The actual value mapped to the key in the map. The <attributes>
  // are used by the strategy and is transparent to the user of this
  // class.

  typedef ACE_Hash_Map_Manager_Ex<KEY, CACHE_VALUE, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> MAP;
  typedef ACE_Hash_Map_Entry<KEY, CACHE_VALUE> CACHE_ENTRY;

  friend class ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>;
  friend class ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY>;

  // = ACE-style iterator typedefs.
  typedef ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> ITERATOR;
  typedef ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> REVERSE_ITERATOR;

   // = STL-style iterator typedefs.
  typedef ITERATOR iterator;
  typedef REVERSE_ITERATOR reverse_iterator;

  // = Initialization and termination methods.
  ACE_Optimal_Cache_Map_Manager (ACE_Allocator *alloc = 0,
                                 ATTRIBUTES attributes = 0,
                                 int purge_percent = 10);
  // Initialize a <Optimal_Cache_Map_Manager> with the <ACE_DEFAULT_MAP_SIZE>.

  ACE_Optimal_Cache_Map_Manager (size_t size,
                                 ACE_Allocator *alloc = 0,
                                 ATTRIBUTES attributes = 0,
                                 int purge_percent = 10 );
  // Initialize a <Optimal_Cache_Map_Manager> with <size> entries.

  ~ACE_Optimal_Cache_Map_Manager (void);
  // Close down a <Optimal_Cache_Map_Manager> and release dynamically allocated
  // resources.

  int open (size_t length = ACE_DEFAULT_MAP_SIZE,
            ACE_Allocator *alloc = 0);
  // Initialise a cache with size <length>.

  int close (void);
  // Close down a cache and release dynamically allocated resources.

  int bind (const KEY &key,
            const VALUE &value);
  // Associate <key> with <value>.  If <key> is already in the
  // MAP then the ENTRY is not changed.  Returns 0 if a new entry is
  // bound successfully, returns 1 if an attempt is made to bind an
  // existing entry, and returns -1 if failures occur.

  int bind (const KEY &key,
            const VALUE &value,
            ACE_Hash_Map_Entry<KEY, ACE_Pair<VALUE, ATTRIBUTES> > *&entry);
  // Same as a normal bind, except the cache entry is also passed back
  // to the caller.  The entry in this case will either be the newly
  // created entry, or the existing one.

  int find (const KEY &key,
            VALUE &value);
  // Loopkup entry<key,value> in the cache.

  int find (const KEY &key);
  // Is <key> in the cache?

  int find (const KEY &key,
            ACE_Hash_Map_Entry<KEY, ACE_Pair<VALUE, ATTRIBUTES> > *&entry);
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
              ACE_Hash_Map_Entry<KEY, ACE_Pair<VALUE, ATTRIBUTES> > *&entry);
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
               ACE_Hash_Map_Entry<KEY, ACE_Pair<VALUE, ATTRIBUTES> > *&entry);
  // Same as a normal trybind, except the cache entry is also passed
  // back to the caller.  The entry in this case will either be the
  // newly created entry, or the existing one.

  int unbind (const KEY &key);
  // Remove <key> from the cache.

  int unbind (const KEY &key,
              VALUE &value);
  // Remove <key> from the cache, and return the <value> associated with
  // <key>.

  int unbind (ACE_Hash_Map_Entry<KEY, ACE_Pair<VALUE, ATTRIBUTES> > *entry);
  // Remove entry from map.

  int purge (void);
  // Remove entries from the cache depending upon the strategy.

  size_t current_size (void);
  // Return the current size of the cache.

  size_t total_size (void);
  // Return the total size of the cache.

  void dump (void) const;
  // Dumps the state of the object.

  // = STL styled iterator factory functions.

  ITERATOR begin (void);
  ITERATOR end (void);
  // Return forward iterator.

  REVERSE_ITERATOR rbegin (void);
  REVERSE_ITERATOR rend (void);
  // Return reverse iterator.

protected:

  ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTRIBUTES>, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex>  &map (void);
  // The map managed by the Optimal_Cache_Map_Manager.

  CACHING_STRATEGY &caching_strategy (void);
  // The caching strategy used on the cache.

  ACE_Hash_Map_Manager_Ex<KEY, ACE_Pair<VALUE, ATTRIBUTES>, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex>  map_;
  // The underlying map which needs to be cached.

  CACHING_STRATEGY caching_strategy_;
  // The strategy to be followed for caching entries in the map.
};
////////////////////////////////////////////////////////////////////////////////
template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY>
class ACE_Optimal_Cache_Map_Iterator
{
  // = TITLE
  //     Defines a iterator for the Optimal_Cache_Map_Manager.
  //
  // = DESCRIPTION
  //     Implementation to be provided by the iterator of the map
  //     managed by the ACE_Optimal_Cache_Map_Manager.

public:

  // = Traits.
  typedef ACE_TYPENAME CACHING_STRATEGY::ATTRIBUTES ATTRIBUTES;
  typedef ACE_Pair <VALUE, ATTRIBUTES> CACHE_VALUE;
  // The actual value mapped to the key in the cache. The <attributes>
  // are used by the strategy and is transperant to the cache user.

  typedef ACE_Reference_Pair<KEY, VALUE> value_type;
  typedef ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTRIBUTES>, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> IMPLEMENTATION;
  // = Initialisation and termination methods.

  ACE_Optimal_Cache_Map_Iterator (const ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTRIBUTES>, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> &iterator_impl);

  ACE_Optimal_Cache_Map_Iterator (const ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &rhs);
  // Copy constructor.

  ~ACE_Optimal_Cache_Map_Iterator (void);

  // = Iteration methods.

  ACE_Optimal_Cache_Map_Iterator &operator= (const ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &rhs);
  // assignment operator.

  int operator== (const ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &rhs);
  int operator!= (const ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &rhs);
  // Comparision operators.

  ACE_Reference_Pair<KEY, VALUE> operator* (void) const;
  // Returns a reference to the internal element <this> is pointing to.

  // = STL styled iteration, compare, and reference functions.

  ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &operator++ (void);
  // Prefix advance

  ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> operator++ (int);
  // Postfix advance.

  ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &operator-- (void);
  // Prefix reverse.

  ACE_Optimal_Cache_Map_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> operator-- (int);
  // Postfix reverse.

  ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTRIBUTES>, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> iterator_implementation (void) const;
  // Returns the iterator of the internal map in the custody of the Optimal_Cache_Map_Manager.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_Hash_Map_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTRIBUTES>, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> iterator_implementation_;
  // The actual iterator which iterates internally on the map
  // belonging to the Optimal_Cache_Map_Manager.
};

////////////////////////////////////////////////////////////////////////////////
template <class KEY, class VALUE, class HASH_KEY, class COMPARE_KEYS, class CACHING_STRATEGY>
class ACE_Optimal_Cache_Map_Reverse_Iterator
{
  // = TITLE
  //     Defines a reverse iterator for the Optimal_Cache_Map_Manager.
  //
  // = DESCRIPTION
  //     Implementation to be provided by the reverse iterator of the map
  //     managed by thr Optimal_Cache_Map_manager.

public:

  // = Traits.
  typedef ACE_TYPENAME CACHING_STRATEGY::ATTRIBUTES ATTRIBUTES;
  typedef ACE_Pair <VALUE, ATTRIBUTES> CACHE_VALUE;
  // The actual value mapped to the key in the cache. The <attributes>
  // are used by the strategy and is transperant to the cache user.

  typedef ACE_Reference_Pair<KEY, VALUE> value_type;
  typedef ACE_Hash_Map_Reverse_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTRIBUTES>, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> REVERSE_IMPLEMENTATION;
  // = Initialisation and termination methods.

  ACE_Optimal_Cache_Map_Reverse_Iterator (const REVERSE_IMPLEMENTATION &iterator_impl);

  ACE_Optimal_Cache_Map_Reverse_Iterator (const ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &rhs);
  // Copy constructor.

  ~ACE_Optimal_Cache_Map_Reverse_Iterator (void);

  // = Iteration methods.

  ACE_Optimal_Cache_Map_Reverse_Iterator &operator= (const ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &rhs);
  // Assignment operator.

  int operator== (const ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &rhs);
  int operator!= (const ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &rhs);
  // Comparision operators.

  ACE_Reference_Pair<KEY, VALUE> operator* (void) const;
  // Returns a reference to the internal element <this> is pointing to.

  // = STL styled iteration, compare, and reference functions.

  ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &operator++ (void);
  // Prefix advance

  ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> operator++ (int);
  // Postfix advance.

  ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> &operator-- (void);
  // Prefix reverse.

  ACE_Optimal_Cache_Map_Reverse_Iterator<KEY, VALUE, HASH_KEY, COMPARE_KEYS, CACHING_STRATEGY> operator-- (int);
  // Postfix reverse.

  ACE_Hash_Map_Reverse_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTRIBUTES>, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> iterator_implementation (void) const;
  // Returns the iterator of the internal map in the custody of the Optimal_Cache_Map_Manager.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_Hash_Map_Reverse_Iterator_Ex<KEY, ACE_Pair<VALUE, ATTRIBUTES>, HASH_KEY, COMPARE_KEYS, ACE_Null_Mutex> reverse_iterator_implementation_;
  // The actual iterator which iterates internally on the map
  // belonging to the Optimal_Cache_Map_Manager.
};

#if defined (__ACE_INLINE__)
#include "Optimal_Cache_Map_Manager_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Optimal_Cache_Map_Manager_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation "Optimal_Cache_Map_Manager_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */


#endif /* OPTIMAL_CACHE_MAP_MANAGER_T_H */
