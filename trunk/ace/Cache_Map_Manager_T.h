/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Cache_Map_Manager.h
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef CACHE_MAP_MANAGER_T_H
#define CACHE_MAP_MANAGER_T_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define  ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Pair_T.h"

// Forward declaration.
class ACE_Allocator;

template <class KEY, class VALUE, class IMPLEMENTATION, class CACHING_STRATEGY, class ATTRIBUTES>
class ACE_Cache_Map_Iterator;

template <class KEY, class VALUE, class REVERSE_IMPLEMENTATION, class CACHING_STRATEGY, class ATTRIBUTES>
class ACE_Cache_Map_Reverse_Iterator;

template <class KEY, class VALUE, class MAP, class CACHING_STRATEGY, class ATTRIBUTES>
class ACE_Cache_Map_Manager
{
  // = TITLE
  //     Defines a abstraction which will purge entries from a map.
  //
  // = DESCRIPTION
  //     The Cache_Map_Manager will manage the map it contains
  //     and provide purging on demand from the map. The strategy for
  //     caching is decided by the user and provided to the Cache
  //     Manager.  The Cache Manager acts as a agent and communicates
  //     between the Map and the Strategy for purging entries from the
  //     map.
  //
  //     No locking mechanism provided since locking at this level
  //     isnt efficient.  Locking has to be provided by the
  //     application.
public:

  // = Traits.
  typedef KEY key_type;
  typedef VALUE mapped_type;
  typedef ACE_TYPENAME MAP::ITERATOR IMPLEMENTATION;
  typedef ACE_TYPENAME MAP::REVERSE_ITERATOR REVERSE_IMPLEMENTATION;
  typedef ACE_Pair<VALUE, ATTRIBUTES> CACHE_VALUE;
  // The actual value mapped to the key in the map. The <attributes>
  // are used by the strategy and is transparent to the user of this
  // class.

  friend class ACE_Cache_Map_Iterator<KEY, VALUE, IMPLEMENTATION,  CACHING_STRATEGY, ATTRIBUTES>;
  friend class ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_IMPLEMENTATION,  CACHING_STRATEGY, ATTRIBUTES>;

  // = ACE-style iterator typedefs.
  typedef ACE_Cache_Map_Iterator<KEY, VALUE, IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> ITERATOR;
  typedef ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> REVERSE_ITERATOR;

   // = STL-style iterator typedefs.
  typedef ITERATOR iterator;
  typedef REVERSE_ITERATOR reverse_iterator;

  // = Initialization and termination methods.

  ACE_Cache_Map_Manager (size_t size = ACE_DEFAULT_MAP_SIZE,
                         ACE_Allocator *alloc = 0,
                         CACHING_STRATEGY *caching_s = 0,
                         int delete_caching_strategy = 1);
  // Initialize a <Cache_Map_Manager> with <size> entries.
  // By default the caching strategy is allocated and deallocated by
  // the class but if needed it can be changed as per the users need.
  // The <delete_on_destruction> flag simply tells the class whether
  // the ownership is given to the class or not.

  virtual ~ACE_Cache_Map_Manager (void);
  // Close down a <Cache_Map_Manager> and release dynamically allocated
  // resources.

  int open (size_t length = ACE_DEFAULT_MAP_SIZE,
            ACE_Allocator *alloc = 0,
            CACHING_STRATEGY *caching_s = 0,
            int delete_caching_strategy = 1);
  // Initialise a cache with size <length> and set the caching_strategy.

  int close (void);
  // Close down a cache and release dynamically allocated resources.

  int bind (const KEY &key,
            const VALUE &value);
  // Associate <key> with <value>.  If <key> is already in the
  // MAP then the ENTRY is not changed.  Returns 0 if a new entry is
  // bound successfully, returns 1 if an attempt is made to bind an
  // existing entry, and returns -1 if failures occur.

  int find (const KEY &key,
            VALUE &value);
  // Loopkup entry<key,value> in the cache.

  int find (const KEY &key);
  // Is <key> in the cache?

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

  int trybind (const KEY &key,
               VALUE &value);
  // Associate <key> with <value> if and only if <key> is not in the
  // cache.  If <key> is already in the cache, then the <value> parameter
  // is overwritten with the existing value in the cache. Returns 0 if a
  // new <key>/<value> association is created.  Returns 1 if an
  // attempt is made to bind an existing entry.  This function fails
  // for maps that do not allow user specified keys.

  int unbind (const KEY &key);
  // Remove <key> from the cache.

  int unbind (const KEY &key,
              VALUE &value);
  // Remove <key> from the cache, and return the <value> associated with
  // <key>.

  int purge (MAP &map);
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

  MAP &map (void);
  // The map managed by the Cache_Map_Manager.

  CACHING_STRATEGY *caching_strategy (void);
  // The caching strategy used on the cache.

protected:

  MAP map_;
  // The underlying map which needs to be cached.

  CACHING_STRATEGY *caching_strategy_;
  // The strategy to be followed for caching entries in the map.

  int delete_caching_strategy_;
  // This flag denotes whether the ownership lies in the hands of the
  // class or not. Is yes, then it deletes the strategy.
};

////////////////////////////////////////////////////////////////////////////////

template <class KEY, class VALUE, class IMPLEMENTATION, class CACHING_STRATEGY, class ATTRIBUTES>
class ACE_Cache_Map_Iterator
{
  // = TITLE
  //     Defines a iterator for the Cache_Map_Manager.
  //
  // = DESCRIPTION
  //     Implementation to be provided by the iterator of the map
  //     managed by the ACE_Cache_Map_Manager.

public:

  // = Traits.
  typedef ACE_Reference_Pair<KEY, VALUE> value_type;
  typedef ACE_Pair <VALUE, ATTRIBUTES> CACHE_VALUE;
  // The actual value mapped to the key in the cache. The <attributes>
  // are used by the strategy and is transperant to the cache user.

  // = Initialisation and termination methods.

  ACE_Cache_Map_Iterator (const IMPLEMENTATION &iterator_impl);

  ACE_Cache_Map_Iterator (const ACE_Cache_Map_Iterator<KEY, VALUE, IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &rhs);
  // Copy constructor.

  virtual ~ACE_Cache_Map_Iterator (void);

  // = Iteration methods.

  ACE_Cache_Map_Iterator &operator= (const ACE_Cache_Map_Iterator<KEY, VALUE, IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &rhs);
  // assignment operator.

  int operator== (const ACE_Cache_Map_Iterator<KEY, VALUE, IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &rhs) const;
  int operator!= (const ACE_Cache_Map_Iterator<KEY, VALUE, IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &rhs) const;
  // Comparision operators.

  ACE_Reference_Pair<KEY, VALUE> operator* (void) const;
  // Returns a reference to the internal element <this> is pointing
  // to.

  // = STL styled iteration, compare, and reference functions.

  ACE_Cache_Map_Iterator<KEY, VALUE, IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &operator++ (void);
  // Prefix advance

  ACE_Cache_Map_Iterator<KEY, VALUE, IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> operator++ (int);
  // Postfix advance.

  ACE_Cache_Map_Iterator<KEY, VALUE, IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &operator-- (void);
  // Prefix reverse.

  ACE_Cache_Map_Iterator<KEY, VALUE, IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> operator-- (int);
  // Postfix reverse.

  IMPLEMENTATION &iterator_implementation (void);
  // Returns the iterator of the internal map in the custody of the
  // Cache_Map_Manager.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  IMPLEMENTATION iterator_implementation_;
  // The actual iterator which iterates internally on the map
  // belonging to the Cache_Map_Manager.
};

////////////////////////////////////////////////////////////////////////////////
template <class KEY, class VALUE, class REVERSE_IMPLEMENTATION, class CACHING_STRATEGY, class ATTRIBUTES>
class ACE_Cache_Map_Reverse_Iterator
{
  // = TITLE
  //     Defines a reverse iterator for the Cache_Map_Manager.
  //
  // = DESCRIPTION
  //     Implementation to be provided by the reverse iterator of the map
  //     managed by thr Cache_Map_manager.

public:

  // = Traits.
  typedef ACE_Reference_Pair<KEY, VALUE> value_type;
  typedef ACE_Pair <VALUE, ATTRIBUTES> CACHE_VALUE;
  // The actual value mapped to the key in the cache. The <attributes>
  // are used by the strategy and is transperant to the cache user.

  // = Initialisation and termination methods.

  ACE_Cache_Map_Reverse_Iterator (const REVERSE_IMPLEMENTATION &iterator_impl);

  ACE_Cache_Map_Reverse_Iterator (const ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &rhs);
  // Copy constructor.

  ~ACE_Cache_Map_Reverse_Iterator (void);

  // = Iteration methods.

  ACE_Cache_Map_Reverse_Iterator &operator= (const ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &rhs);
  // Assignment operator.

  int operator== (const ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &rhs) const;
  int operator!= (const ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &rhs) const;
  // Comparision operators.

  ACE_Reference_Pair<KEY, VALUE> operator* (void) const;
  // Returns a reference to the internal element <this> is pointing
  // to.

  // = STL styled iteration, compare, and reference functions.

  ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &operator++ (void);
  // Prefix advance

  ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> operator++ (int);
  // Postfix advance.

  ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &operator-- (void);
  // Prefix reverse.

  ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> operator-- (int);
  // Postfix reverse.

  REVERSE_IMPLEMENTATION &iterator_implementation (void);
  // Returns the iterator of the internal map in the custody of the
  // Cache_Map_Manager.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  REVERSE_IMPLEMENTATION reverse_iterator_implementation_;
  // The actual iterator which iterates internally on the map
  // belonging to the Cache_Map_Manager.
};

#if defined (__ACE_INLINE__)
#include "ace/Cache_Map_Manager_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Cache_Map_Manager_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation "ace/Cache_Map_Manager_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* CACHE_MAP_MANAGER_T_H */
