// -*- C++ -*-

//=============================================================================
/**
 *  @file    Cache_Map_Manager_T.h
 *
 *  $Id$
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_CACHE_MAP_MANAGER_T_H
#define ACE_CACHE_MAP_MANAGER_T_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Default_Constants.h"
#include "ace/Global_Macros.h"
#include "ace/Pair_T.h"

// Forward declaration.
class ACE_Allocator;

#if !defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)

#define ACE_Cache_Map_Iterator ACMI
#define ACE_Cache_Map_Reverse_Iterator ACMRI

template <class KEY, class VALUE, class IMPLEMENTATION, class CACHING_STRATEGY, class ATTRIBUTES>
class ACE_Cache_Map_Iterator;

template <class KEY, class VALUE, class REVERSE_IMPLEMENTATION, class CACHING_STRATEGY, class ATTRIBUTES>
class ACE_Cache_Map_Reverse_Iterator;

#define ACE_T1 class KEY, class VALUE, class MAP, class ITERATOR_IMPL, class REVERSE_ITERATOR_IMPL, class CACHING_STRATEGY, class ATTRIBUTES
#define ACE_T2 KEY, VALUE, MAP, ITERATOR_IMPL, REVERSE_ITERATOR_IMPL, CACHING_STRATEGY, ATTRIBUTES

#else

#define ACE_T1 class KEY, class VALUE, class MAP, class CACHING_STRATEGY, class ATTRIBUTES
#define ACE_T2 KEY, VALUE, MAP, CACHING_STRATEGY, ATTRIBUTES

#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

// For linkers that cant grok long names.
#define ACE_Cache_Map_Manager ACMM

/**
 * @class ACE_Cache_Map_Manager
 *
 * @brief Defines a abstraction that will purge entries from a map.
 *
 * The <ACE_Cache_Map_Manager> will manage the map it contains
 * and provide purging on demand from the map. The strategy for
 * caching is decided by the user and provided to the Cache
 * Manager.  The Cache Manager acts as a agent and communicates
 * between the Map and the Strategy for purging entries from the
 * map.
 * No locking mechanism provided since locking at this level
 * isn't efficient.  Locking has to be provided by the
 * application.
 */
template <ACE_T1>
class ACE_Cache_Map_Manager
{
public:

  // = Traits.
  typedef KEY key_type;
  typedef VALUE mapped_type;
  typedef MAP map_type;
  typedef CACHING_STRATEGY caching_strategy_type;

#if !defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)

  typedef ITERATOR_IMPL ITERATOR_IMPLEMENTATION;
  typedef REVERSE_ITERATOR_IMPL REVERSE_ITERATOR_IMPLEMENTATION;

  friend class ACE_Cache_Map_Iterator<KEY, VALUE, ITERATOR_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES>;
  friend class ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_ITERATOR_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES>;

  // = ACE-style iterator typedefs.
  typedef ACE_Cache_Map_Iterator<KEY, VALUE, ITERATOR_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES>
          ITERATOR;
  typedef ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_ITERATOR_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES>
          REVERSE_ITERATOR;

   // = STL-style iterator typedefs.
  typedef ITERATOR
          iterator;
  typedef REVERSE_ITERATOR
          reverse_iterator;

#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

  /**
   * The actual value mapped to the key in the map. The <attributes>
   * are used by the strategy and is transparent to the user of this
   * class.
   */
  typedef ACE_Pair<VALUE, ATTRIBUTES> CACHE_VALUE;

  // = Initialization and termination methods.

  /// Initialize a <Cache_Map_Manager> with <caching_strategy> and
  /// <size> entries.
  ACE_Cache_Map_Manager (CACHING_STRATEGY &caching_strategy,
                         size_t size = ACE_DEFAULT_MAP_SIZE,
                         ACE_Allocator *alloc = 0);

  /// Close down a <Cache_Map_Manager> and release dynamically allocated
  /// resources.
  virtual ~ACE_Cache_Map_Manager (void);

  /// Initialize a cache with size <length>.
  int open (size_t length = ACE_DEFAULT_MAP_SIZE,
            ACE_Allocator *alloc = 0);

  /// Close down a cache and release dynamically allocated resources.
  int close (void);

  /**
   * Associate <key> with <value>.  If <key> is already in the MAP
   * then the ENTRY is not changed.  Returns 0 if a new entry is bound
   * successfully, returns 1 if an attempt is made to bind an existing
   * entry, and returns -1 if failures occur.
   */
  int bind (const KEY &key,
            const VALUE &value);

  /**
   * Lookup entry<key,value> in the cache. If it is not found, returns -1.
   * If the <key> is located in the MAP object, the CACHING_STRATEGY is
   * notified of it via notify_find (int result, ATTRIBUTES &attribute).
   * If notify_find also returns 0 (success), then this function returns
   * 0 (success) and sets the cached value in <value>.
   */
  int find (const KEY &key,
            VALUE &value);

  /**
   * Lookup entry<key,value> in the cache. If it is not found, returns -1.
   * If the <key> is located in the MAP object, the CACHING_STRATEGY is
   * notified of it via notify_find (int result, ATTRIBUTES &attribute).
   * If notify_find also returns 0 (success), then this function returns
   * 0 (success).
   */
  int find (const KEY &key);

  /**
   * Reassociate the <key> with <value>. If the <key> already exists
   * in the cache then returns 1, on a new bind returns 0 and returns
   * -1 in case of any failures.
   */
  int rebind (const KEY &key,
              const VALUE &value);

  /**
   * Reassociate <key> with <value>, storing the old value into the
   * "out" parameter <old_value>.  The function fails if <key> is not
   * in the cache for caches that do not allow user specified keys.
   * However, for caches that allow user specified keys, if the key is
   * not in the cache, a new <key>/<value> association is created.
   */
  int rebind (const KEY &key,
              const VALUE &value,
              VALUE &old_value);

  /**
   * Reassociate <key> with <value>, storing the old key and value
   * into the "out" parameters <old_key> and <old_value>.  The
   * function fails if <key> is not in the cache for caches that do
   * not allow user specified keys.  However, for caches that allow
   * user specified keys, if the key is not in the cache, a new
   * <key>/<value> association is created.
   */
  int rebind (const KEY &key,
              const VALUE &value,
              KEY &old_key,
              VALUE &old_value);

  /**
   * Associate <key> with <value> if and only if <key> is not in the
   * cache.  If <key> is already in the cache, then the <value>
   * parameter is overwritten with the existing value in the
   * cache. Returns 0 if a new <key>/<value> association is created.
   * Returns 1 if an attempt is made to bind an existing entry.  This
   * function fails for maps that do not allow user specified keys.
   */
  int trybind (const KEY &key,
               VALUE &value);

  /// Remove <key> from the cache.
  int unbind (const KEY &key);

  /// Remove <key> from the cache, and return the <value> associated with
  /// <key>.
  int unbind (const KEY &key,
              VALUE &value);

  /// Remove entries from the cache depending upon the strategy.
  int purge (void);

  /// Return the current size of the cache.
  size_t current_size (void) const;

  /// Return the total size of the cache.
  size_t total_size (void) const;

  /// Dumps the state of the object.
  void dump (void) const;

#if !defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)

  // = STL styled iterator factory functions.

  /// Return forward iterator.
  ITERATOR begin (void);
  ITERATOR end (void);

  /// Return reverse iterator.
  REVERSE_ITERATOR rbegin (void);
  REVERSE_ITERATOR rend (void);

#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

  /// The map managed by the Cache_Map_Manager.
  MAP &map (void);

  /// The caching strategy used on the cache.
  CACHING_STRATEGY &caching_strategy (void);

protected:

  /// The underlying map which needs to be cached.
  MAP map_;

  /// The strategy to be followed for caching entries in the map.
  CACHING_STRATEGY &caching_strategy_;

private:

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Cache_Map_Manager<ACE_T2> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Cache_Map_Manager (const ACE_Cache_Map_Manager<ACE_T2> &))

};

#if !defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)

/**
 * @class ACE_Cache_Map_Iterator
 *
 * @brief Defines a iterator for the Cache_Map_Manager.
 *
 * Implementation to be provided by the iterator of the map
 * managed by the ACE_Cache_Map_Manager.
 */
template <class KEY, class VALUE, class IMPLEMENTATION, class CACHING_STRATEGY, class ATTRIBUTES>
class ACE_Cache_Map_Iterator
{

public:

  // = Traits.
  /// The actual value mapped to the key in the cache. The <attributes>
  /// are used by the strategy and is transperant to the cache user.
  typedef ACE_Reference_Pair<KEY, VALUE>
          value_type;
  typedef ACE_Pair <VALUE, ATTRIBUTES>
          CACHE_VALUE;

  // = Initialisation and termination methods.

  ACE_Cache_Map_Iterator (const IMPLEMENTATION &iterator_impl);

  /// Copy constructor.
  ACE_Cache_Map_Iterator (const ACE_Cache_Map_Iterator<KEY, VALUE, IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &rhs);

  virtual ~ACE_Cache_Map_Iterator (void);

  // = Iteration methods.

  /// assignment operator.
  ACE_Cache_Map_Iterator <KEY, VALUE, IMPLEMENTATION,
                          CACHING_STRATEGY, ATTRIBUTES> &operator=
      (const ACE_Cache_Map_Iterator<KEY, VALUE, IMPLEMENTATION,
                                    CACHING_STRATEGY, ATTRIBUTES> &rhs);

  /// Comparision operators.
  int operator== (const ACE_Cache_Map_Iterator<KEY, VALUE, IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &rhs) const;
  int operator!= (const ACE_Cache_Map_Iterator<KEY, VALUE, IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &rhs) const;

  /// Returns a reference to the internal element <this> is pointing
  /// to.
  ACE_Reference_Pair<KEY, VALUE> operator* (void) const;

  // = STL styled iteration, compare, and reference functions.

  /// Prefix advance
  ACE_Cache_Map_Iterator<KEY, VALUE, IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &operator++ (void);

  /// Postfix advance.
  ACE_Cache_Map_Iterator<KEY, VALUE, IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> operator++ (int);

  /// Prefix reverse.
  ACE_Cache_Map_Iterator<KEY, VALUE, IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &operator-- (void);

  /// Postfix reverse.
  ACE_Cache_Map_Iterator<KEY, VALUE, IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> operator-- (int);

  /// Returns the iterator of the internal map in the custody of the
  /// Cache_Map_Manager.
  IMPLEMENTATION &iterator_implementation (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// The actual iterator which iterates internally on the map
  /// belonging to the Cache_Map_Manager.
  IMPLEMENTATION iterator_implementation_;
};

/**
 * @class ACE_Cache_Map_Reverse_Iterator
 *
 * @brief Defines a reverse iterator for the Cache_Map_Manager.
 *
 * Implementation to be provided by the reverse iterator of the map
 * managed by thr Cache_Map_manager.
 */
template <class KEY, class VALUE, class REVERSE_IMPLEMENTATION, class CACHING_STRATEGY, class ATTRIBUTES>
class ACE_Cache_Map_Reverse_Iterator
{
public:

  // = Traits.
  /// The actual value mapped to the key in the cache. The <attributes>
  /// are used by the strategy and is transperant to the cache user.
  typedef ACE_Reference_Pair<KEY, VALUE> value_type;
  typedef ACE_Pair <VALUE, ATTRIBUTES> CACHE_VALUE;

  // = Initialisation and termination methods.

  ACE_Cache_Map_Reverse_Iterator (const REVERSE_IMPLEMENTATION &iterator_impl);

  /// Copy constructor.
  ACE_Cache_Map_Reverse_Iterator (const ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &rhs);

  ~ACE_Cache_Map_Reverse_Iterator (void);

  // = Iteration methods.

  /// Assignment operator.
  ACE_Cache_Map_Reverse_Iterator <KEY, VALUE, REVERSE_IMPLEMENTATION,
                                  CACHING_STRATEGY, ATTRIBUTES> &operator=
     (const ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_IMPLEMENTATION,
                                           CACHING_STRATEGY, ATTRIBUTES> &rhs);

  /// Comparision operators.
  int operator== (const ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &rhs) const;
  int operator!= (const ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &rhs) const;

  /// Returns a reference to the internal element <this> is pointing
  /// to.
  ACE_Reference_Pair<KEY, VALUE> operator* (void) const;

  // = STL styled iteration, compare, and reference functions.

  /// Prefix advance
  ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &operator++ (void);

  /// Postfix advance.
  ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> operator++ (int);

  /// Prefix reverse.
  ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> &operator-- (void);

  /// Postfix reverse.
  ACE_Cache_Map_Reverse_Iterator<KEY, VALUE, REVERSE_IMPLEMENTATION, CACHING_STRATEGY, ATTRIBUTES> operator-- (int);

  /// Returns the iterator of the internal map in the custody of the
  /// Cache_Map_Manager.
  REVERSE_IMPLEMENTATION &iterator_implementation (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// The actual iterator which iterates internally on the map
  /// belonging to the Cache_Map_Manager.
  REVERSE_IMPLEMENTATION reverse_iterator_implementation_;
};

#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

#undef ACE_T1
#undef ACE_T2

#if defined (__ACE_INLINE__)
#include "ace/Cache_Map_Manager_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Cache_Map_Manager_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Cache_Map_Manager_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* ACE_CACHE_MAP_MANAGER_T_H */
