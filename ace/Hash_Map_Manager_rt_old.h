
// -*- C++ -*-

//=============================================================================
/**
 *  @file    Hash_Map_Manager_rt.h
 *
 *  $Id$
 *
 *  @author Jeremy M. Nolle <jmn3@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_HASH_MAP_MANAGER_RT_H
#define ACE_HASH_MAP_MANAGER_RT_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Default_Constants.h"
#include "ace/Functor.h"
#include "ace/Log_Msg.h"


/**
 * @class ACE_Hash_Map_Entry
 *
 * @brief Define an entry in the hash table.
 */
template <class EXT_ID, class INT_ID>
class ACE_Hash_Map_Entry
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  ACE_Hash_Map_Entry (const EXT_ID &ext_id,
                      const INT_ID &int_id,
                      ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next = 0,
                      ACE_Hash_Map_Entry<EXT_ID, INT_ID> *prev = 0);

  /// Constructor.
  ACE_Hash_Map_Entry (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next,
                      ACE_Hash_Map_Entry<EXT_ID, INT_ID> *prev);

  # if ! defined (ACE_HAS_BROKEN_NOOP_DTORS)
  /// Destructor.
  ~ACE_Hash_Map_Entry (void);
  #endif /* ! defined (ACE_HAS_BROKEN_NOOP_DTORS) */

  /// Key used to look up an entry.
  EXT_ID ext_id_;

  /// The contents of the entry itself.
  INT_ID int_id_;

  /// Pointer to the next item in the bucket of overflow nodes.
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next_;

  /// Pointer to the prev item in the bucket of overflow nodes.
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *prev_;

  /// Dump the state of an object.
  void dump (void) const;
};

class Hash_Map_Manager_rt  
{
public:
/*  typedef EXT_ID
          KEY;
  typedef INT_ID
          VALUE;
  typedef ACE_Hash_Map_Entry<EXT_ID, INT_ID>
          ENTRY;
*/
  Hash_Map_Manager_rt();
  
  /// Destructor.
  ~Hash_Map_Manager_rt();


};

// Forward decl.
class ACE_Allocator;

/**
 * @class ACE_Hash_Map_Manager_Ex
 *
 * @brief Define a map abstraction that efficiently associates
 * <EXT_ID>s with <INT_ID>s.
 *
 * This implementation of a map uses a hash table.  Key hashing
 * is achieved through the HASH_KEY object and key comparison is
 * achieved through the COMPARE_KEYS object.
 * This class uses an <ACE_Allocator> to allocate memory.  The
 * user can make this a persistent class by providing an
 * <ACE_Allocator> with a persistable memory pool.
 */
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_Manager_Ex
{
public:
  //friend class ACE_Hash_Map_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>;
  //friend class ACE_Hash_Map_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>;
  //friend class ACE_Hash_Map_Const_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>;
  //friend class ACE_Hash_Map_Const_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>;
  //friend class ACE_Hash_Map_Reverse_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>;
  //friend class ACE_Hash_Map_Bucket_Iterator<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>;

  typedef EXT_ID
          KEY;
  typedef INT_ID
          VALUE;
  typedef ACE_Hash_Map_Entry<EXT_ID, INT_ID>
          ENTRY;

  // = ACE-style iterator typedefs.
  /*typedef ACE_Hash_Map_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>
          ITERATOR;
  typedef ACE_Hash_Map_Const_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>
          CONST_ITERATOR;
  typedef ACE_Hash_Map_Reverse_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>
          REVERSE_ITERATOR;

  // = STL-style iterator typedefs.
  typedef ACE_Hash_Map_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>
          iterator;
  typedef ACE_Hash_Map_Const_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>
          const_iterator;
  typedef ACE_Hash_Map_Reverse_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>
          reverse_iterator;
*/
  // = Initialization and termination methods.

  /// Initialize a <Hash_Map_Manager_Ex> with default size.
  ACE_Hash_Map_Manager_Ex (ACE_Allocator *alloc = 0);

  /// Initialize a <Hash_Map_Manager_Ex> with size <length>.
  ACE_Hash_Map_Manager_Ex (size_t size,
                            ACE_Allocator *alloc = 0);

  /// Initialize a <Hash_Map_Manager_Ex> with <size> elements.
  int open (size_t size = ACE_DEFAULT_MAP_SIZE,
            ACE_Allocator *alloc = 0);

  /// Close down a <Hash_Map_Manager_Ex> and release dynamically allocated
  /// resources.
  int close (void);

  /// Removes all the entries in <Map_Manager_Ex>.
  int unbind_all (void);

  /// Cleanup the <Hash_Map_Manager_Ex>.
  ~ACE_Hash_Map_Manager_Ex (void);

  /**
   * Associate <ext_id> with <int_id>.  If <ext_id> is already in the
   * map then the <ACE_Hash_Map_Entry> is not changed.  Returns 0 if a
   * new entry is bound successfully, returns 1 if an attempt is made
   * to bind an existing entry, and returns -1 if failures occur.
   */
  int bind (const EXT_ID &item,
            const INT_ID &int_id);

  /**
   * Same as a normal bind, except the map entry is also passed back
   * to the caller.  The entry in this case will either be the newly
   * created entry, or the existing one.
   */
  int bind (const EXT_ID &ext_id,
            const INT_ID &int_id,
            ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry);

  /**
   * Associate <ext_id> with <int_id> if and only if <ext_id> is not
   * in the map.  If <ext_id> is already in the map then the <int_id>
   * parameter is assigned the existing value in the map.  Returns 0
   * if a new entry is bound successfully, returns 1 if an attempt is
   * made to bind an existing entry, and returns -1 if failures occur.
   */
  int trybind (const EXT_ID &ext_id,
               INT_ID &int_id);

  /**
   * Same as a normal trybind, except the map entry is also passed
   * back to the caller.  The entry in this case will either be the
   * newly created entry, or the existing one.
   */
  int trybind (const EXT_ID &ext_id,
               INT_ID &int_id,
               ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry);

  /**
   * Reassociate <ext_id> with <int_id>.  If <ext_id> is not in the
   * map then behaves just like <bind>.  Returns 0 if a new entry is
   * bound successfully, returns 1 if an existing entry was rebound,
   * and returns -1 if failures occur.
   */
  int rebind (const EXT_ID &ext_id,
              const INT_ID &int_id);

  /**
   * Same as a normal rebind, except the map entry is also passed back
   * to the caller.  The entry in this case will either be the newly
   * created entry, or the existing one.
   */
  int rebind (const EXT_ID &ext_id,
              const INT_ID &int_id,
              ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry);

  /**
   * Associate <ext_id> with <int_id>.  If <ext_id> is not in the map
   * then behaves just like <bind>.  Otherwise, store the old value of
   * <int_id> into the "out" parameter and rebind the new parameters.
   * Returns 0 if a new entry is bound successfully, returns 1 if an
   * existing entry was rebound, and returns -1 if failures occur.
   */
  int rebind (const EXT_ID &ext_id,
              const INT_ID &int_id,
              INT_ID &old_int_id);

  /**
   * Same as a normal rebind, except the map entry is also passed back
   * to the caller.  The entry in this case will either be the newly
   * created entry, or the existing one.
   */
  int rebind (const EXT_ID &ext_id,
              const INT_ID &int_id,
              INT_ID &old_int_id,
              ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry);

  /**
   * Associate <ext_id> with <int_id>.  If <ext_id> is not in the map
   * then behaves just like <bind>.  Otherwise, store the old values
   * of <ext_id> and <int_id> into the "out" parameters and rebind the
   * new parameters.  This is very useful if you need to have an
   * atomic way of updating <ACE_Hash_Map_Entrys> and you also need
   * full control over memory allocation.  Returns 0 if a new entry is
   * bound successfully, returns 1 if an existing entry was rebound,
   * and returns -1 if failures occur.
   */
  int rebind (const EXT_ID &ext_id,
              const INT_ID &int_id,
              EXT_ID &old_ext_id,
              INT_ID &old_int_id);

  /**
   * Same as a normal rebind, except the map entry is also passed back
   * to the caller.  The entry in this case will either be the newly
   * created entry, or the existing one.
   */
  int rebind (const EXT_ID &ext_id,
              const INT_ID &int_id,
              EXT_ID &old_ext_id,
              INT_ID &old_int_id,
              ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry);

  /// Locate <ext_id> and pass out parameter via <int_id>.
  /// Return 0 if found, returns -1 if not found.
  int find (const EXT_ID &ext_id,
            INT_ID &int_id) const;

  /// Returns 0 if the <ext_id> is in the mapping, otherwise -1.
  int find (const EXT_ID &ext_id) const;

  /// Locate <ext_id> and pass out parameter via <entry>.  If found,
  /// return 0, returns -1 if not found.
  int find (const EXT_ID &ext_id,
            ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry) const;

  /**
   * Unbind (remove) the <ext_id> from the map.  Don't return the
   * <int_id> to the caller (this is useful for collections where the
   * <int_id>s are *not* dynamically allocated...)
   */
  int unbind (const EXT_ID &ext_id);

  /// Break any association of <ext_id>.  Returns the value of <int_id>
  /// in case the caller needs to deallocate memory. Return 0 if the
  /// unbind was successfully, and returns -1 if failures occur.
  int unbind (const EXT_ID &ext_id,
              INT_ID &int_id);

  /// Remove entry from map. Return 0 if the unbind was successfully,
  /// and returns -1 if failures occur.
  int unbind (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *entry);

  /// Return the current size of the map.
  size_t current_size (void) const;

  /// Return the total size of the map.
  size_t total_size (void) const;

  /**
   * Returns a reference to the underlying <ACE_LOCK>.  This makes it
   * possible to acquire the lock explicitly, which can be useful in
   * some cases if you instantiate the <ACE_Atomic_Op> with an
   * <ACE_Recursive_Mutex> or <ACE_Process_Mutex>, or if you need to
   * guard the state of an iterator.  NOTE: the right name would be
   * <lock>, but HP/C++ will choke on that!
   */
  ACE_LOCK &mutex (void);

  /// Dump the state of an object.
  void dump (void) const;

  // = STL styled iterator factory functions.

  /*/// Return forward iterator.
  ACE_Hash_Map_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> begin (void);
  ACE_Hash_Map_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> end (void);

  /// Return reverse iterator.
  ACE_Hash_Map_Reverse_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> rbegin (void);
  ACE_Hash_Map_Reverse_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> rend (void);
*/
protected:
  // = The following methods do the actual work.

  /// Returns 1 if <id1> == <id2>, else 0.  This is defined as a
  /// separate method to facilitate template specialization.
  int equal (const EXT_ID &id1, const EXT_ID &id2);

  /// Compute the hash value of the <ext_id>.  This is defined as a
  /// separate method to facilitate template specialization.
  u_long hash (const EXT_ID &ext_id);

  // = These methods assume locks are held by private methods.

  /// Performs bind.  Must be called with locks held.
  int bind_i (const EXT_ID &ext_id,
              const INT_ID &int_id);

  /// Performs bind.  Must be called with locks held.
  int bind_i (const EXT_ID &ext_id,
              const INT_ID &int_id,
              ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry);

  /// Performs trybind.  Must be called with locks held.
  int trybind_i (const EXT_ID &ext_id,
                 INT_ID &int_id);

  /// Performs trybind.  Must be called with locks held.
  int trybind_i (const EXT_ID &ext_id,
                 INT_ID &int_id,
                 ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry);

  /// Performs rebind.  Must be called with locks held.
  int rebind_i (const EXT_ID &ext_id,
                const INT_ID &int_id);

  /// Performs rebind.  Must be called with locks held.
  int rebind_i (const EXT_ID &ext_id,
                const INT_ID &int_id,
                ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry);

  /// Performs rebind.  Must be called with locks held.
  int rebind_i (const EXT_ID &ext_id,
                const INT_ID &int_id,
                INT_ID &old_int_id);

  /// Performs rebind.  Must be called with locks held.
  int rebind_i (const EXT_ID &ext_id,
                const INT_ID &int_id,
                INT_ID &old_int_id,
                ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry);

  /// Performs rebind.  Must be called with locks held.
  int rebind_i (const EXT_ID &ext_id,
                const INT_ID &int_id,
                EXT_ID &old_ext_id,
                INT_ID &old_int_id);

  /// Performs rebind.  Must be called with locks held.
  int rebind_i (const EXT_ID &ext_id,
                const INT_ID &int_id,
                EXT_ID &old_ext_id,
                INT_ID &old_int_id,
                ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry);

  /// Performs a find of <int_id> using <ext_id> as the key.  Must be
  /// called with locks held.
  int find_i (const EXT_ID &ext_id,
              INT_ID &int_id);

  /// Performs a find using <ext_id> as the key.  Must be called with
  /// locks held.
  int find_i (const EXT_ID &ext_id);

  /// Performs a find using <ext_id> as the key.  Must be called with
  /// locks held.
  int find_i (const EXT_ID &ext_id,
              ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry);

  /// Performs unbind.  Must be called with locks held.
  int unbind_i (const EXT_ID &ext_id,
                INT_ID &int_id);

  /// Performs unbind.  Must be called with locks held.
  int unbind_i (const EXT_ID &ext_id);

  /// Performs unbind.  Must be called with locks held.
  int unbind_i (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *entry);

  /**
   * Resize the map.  Must be called with locks held.  Note, that this
   * method should never be called more than once or else all the
   * hashing will get screwed up as the size will change.
   */
  int create_buckets (size_t size);

  /// Close down a <Map_Manager_Ex>.  Must be called with
  /// locks held.
  int close_i (void);

  /// Removes all the entries in <Map_Manager_Ex>.  Must be called with
  /// locks held.
  int unbind_all_i (void);

  /// Pointer to a memory allocator.
  ACE_Allocator *allocator_;

  /// Synchronization variable for the MT_SAFE <ACE_Hash_Map_Manager_Ex>.
  ACE_LOCK lock_;

  /// Function object used for hashing keys.
  HASH_KEY hash_key_;

  /// Function object used for comparing keys.
  COMPARE_KEYS compare_keys_;

private:
  /// Returns the <ACE_Hash_Map_Entry> that corresponds to <ext_id>.
  int shared_find (const EXT_ID &ext_id,
                   ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry,
                   u_long &loc);

  /**
   * Array of <ACE_Hash_Map_Entry> *s, each of which points to an
   * <ACE_Hash_Map_Entry> that serves as the beginning of a linked
   * list of <EXT_ID>s that hash to that bucket.
   */
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *table_;

  /// Total size of the hash table.
  size_t total_size_;

  /// Current number of entries in the table (note that this can be
  /// larger than <total_size_> due to the bucket chaining).
  size_t cur_size_;

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID,  HASH_KEY, COMPARE_KEYS, ACE_LOCK> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Hash_Map_Manager_Ex (const ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID,  HASH_KEY, COMPARE_KEYS, ACE_LOCK> &))
};



#if defined (__ACE_INLINE__)
// Include ace/Hash_Map_Manager_T.i on all platforms excluding SunCC.
// This nonsense is necessary since SunCC (version 4.2) cannot inline
// the code in ace/Hash_Map_Manager_T.i (with the fast option).
# if !(defined (__SUNPRO_CC) && (__SUNPRO_CC == 0x420))
#  include "ace/Hash_Map_Manager_rt.i"
# endif /* ! __SUNPRO_CC */
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_HASH_MAP_MANAGER_RT_H */
