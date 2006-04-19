// -*- C++ -*-

//=============================================================================
/**
 *  @file    Hash_Map_Manager_T.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_HASH_MAP_MANAGER_T_H
#define ACE_HASH_MAP_MANAGER_T_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Default_Constants.h"
#include "ace/Functor_T.h"
#include "ace/Log_Msg.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

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

// Forward decl.
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_Iterator_Base_Ex;

// Forward decl.
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_Const_Iterator_Base_Ex;

// Forward decl.
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_Iterator_Ex;

// Forward decl.
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_Const_Iterator_Ex;

// Forward decl.
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_Reverse_Iterator_Ex;

// Forward decl.
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_Bucket_Iterator;

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
 * This class uses an ACE_Allocator to allocate memory.  The
 * user can make this a persistent class by providing an
 * ACE_Allocator with a persistable memory pool.
 */
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_Manager_Ex
{
public:
  friend class ACE_Hash_Map_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>;
  friend class ACE_Hash_Map_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>;
  friend class ACE_Hash_Map_Const_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>;
  friend class ACE_Hash_Map_Const_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>;
  friend class ACE_Hash_Map_Reverse_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>;
  friend class ACE_Hash_Map_Bucket_Iterator<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>;

  typedef EXT_ID
          KEY;
  typedef INT_ID
          VALUE;
  typedef ACE_LOCK lock_type;
  typedef ACE_Hash_Map_Entry<EXT_ID, INT_ID>
          ENTRY;

  // = ACE-style iterator typedefs.
  typedef ACE_Hash_Map_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>
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

  /// Returns the current number of ACE_Hash_Map_Entry objects in the
  /// hash table.
  size_t current_size (void) const;

  /// Return the size of the array that's used to point to the
  /// linked lists of ACE_Hash_Map_Entry objects in the hash table.
  size_t total_size (void) const;

  /**
   * Returns a reference to the underlying <ACE_LOCK>.  This makes it
   * possible to acquire the lock explicitly, which can be useful in
   * some cases if you instantiate the <ACE_Atomic_Op> with an
   * <ACE_Recursive_Mutex> or <ACE_Process_Mutex>, or if you need to
   * guard the state of an iterator.
   * @note The right name would be <lock>, but HP/C++ will choke on that!
   */
  ACE_LOCK &mutex (void);

  /// Dump the state of an object.
  void dump (void) const;

  // = STL styled iterator factory functions.

  /// Return forward iterator.
  ACE_Hash_Map_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> begin (void);
  ACE_Hash_Map_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> end (void);

  /// Return reverse iterator.
  ACE_Hash_Map_Reverse_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> rbegin (void);
  ACE_Hash_Map_Reverse_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> rend (void);

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
   * Resize the map.  Must be called with locks held.
   * @note This method should never be called more than once or else all the
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

protected:
  /// Returns the <ACE_Hash_Map_Entry> that corresponds to <ext_id>.
  int shared_find (const EXT_ID &ext_id,
                   ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry,
                   size_t &loc);

  /// Accessor of the underlying table
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *table (void);

  /// Accessor of the current size attribute
  size_t cur_size (void) const;

private:
  /**
   * Array of <ACE_Hash_Map_Entry> *s, each of which points to an
   * <ACE_Hash_Map_Entry> that serves as the beginning of a linked
   * list of <EXT_ID>s that hash to that bucket.
   */
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *table_;

  /// Total size of the hash table.
  size_t total_size_;

  /// Current number of entries in the table
  /// @note That this can be larger than <total_size_> due to the
  /// bucket chaining).
  size_t cur_size_;

  // = Disallow these operations.
  // Hack alert... Sun C++ 5.4 can't hack explicit templates with this
  // in place. Please remove this when taking out explicit templates.
#if !(defined (__SUNPRO_CC) && (__SUNPRO_CC <= 0x540) && \
      defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID,  HASH_KEY, COMPARE_KEYS, ACE_LOCK> &))
#endif /* __SUNPRO_CC <= 0x540 && !ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

  ACE_UNIMPLEMENTED_FUNC (ACE_Hash_Map_Manager_Ex (const ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID,  HASH_KEY, COMPARE_KEYS, ACE_LOCK> &))
};

/**
 * @class ACE_Hash_Map_Iterator_Base_Ex
 *
 * @brief Base iterator for the <ACE_Hash_Map_Manager_Ex>
 *
 * This class factors out common code from its templatized
 * subclasses.
 */
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_Iterator_Base_Ex
{
public:
  // = Initialization method.
  /// Contructor.  If head != 0, the iterator constructed is positioned
  /// at the head of the map, it is positioned at the end otherwise.
  ACE_Hash_Map_Iterator_Base_Ex (ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> &mm,
                                 int head);

  // = ITERATION methods.

  /// Pass back the next <entry> that hasn't been seen in the Set.
  /// Returns 0 when all items have been seen, else 1.
  int next (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&next_entry) const;

  /// Returns 1 when all items have been seen, else 0.
  int done (void) const;

  /// Returns a reference to the interal element <this> is pointing to.
  ACE_Hash_Map_Entry<EXT_ID, INT_ID>& operator* (void) const;

  /// Returns reference the Hash_Map_Manager_Ex that is being iterated
  /// over.
  ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>& map (void);

  /// Check if two iterators point to the same position
  bool operator== (const ACE_Hash_Map_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> &) const;
  bool operator!= (const ACE_Hash_Map_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> &) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// Move forward by one element in the set.  Returns 0 when there's
  /// no more item in the set after the current items, else 1.
  int forward_i (void);

  /// Move backward by one element in the set.  Returns 0 when there's
  /// no more item in the set before the current item, else 1.
  int reverse_i (void);

  /// Dump the state of an object.
  void dump_i (void) const;

  /// Map we are iterating over.
  ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> *map_man_;

  /// Keeps track of how far we've advanced in the table.
  ssize_t index_;

  /// Keeps track of how far we've advanced in a linked list in each
  /// table slot.
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next_;
};

/**
 * @class ACE_Hash_Map_Const_Iterator_Base_Ex
 *
 * @brief Base const iterator for the <ACE_Hash_Map_Manager_Ex>
 *
 * This class factors out common code from its templatized
 * subclasses.
 */
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_Const_Iterator_Base_Ex
{
public:
  // = Initialization method.
  /// Contructor.  If head != 0, the iterator constructed is positioned
  /// at the head of the map, it is positioned at the end otherwise.
  ACE_Hash_Map_Const_Iterator_Base_Ex (const ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> &mm,
                                       int head);

  // = ITERATION methods.

  /// Pass back the next <entry> that hasn't been seen in the Set.
  /// Returns 0 when all items have been seen, else 1.
  int next (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&next_entry) const;

  /// Returns 1 when all items have been seen, else 0.
  int done (void) const;

  /// Returns a reference to the interal element <this> is pointing to.
  ACE_Hash_Map_Entry<EXT_ID, INT_ID>& operator* (void) const;

  /// Returns reference the Hash_Map_Manager_Ex that is being iterated
  /// over.
  const ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>& map (void);

  /// Check if two iterators point to the same position
  bool operator== (const ACE_Hash_Map_Const_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> &) const;
  bool operator!= (const ACE_Hash_Map_Const_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> &) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  /// Move forward by one element in the set.  Returns 0 when there's
  /// no more item in the set after the current items, else 1.
  int forward_i (void);

  /// Move backward by one element in the set.  Returns 0 when there's
  /// no more item in the set before the current item, else 1.
  int reverse_i (void);

  /// Dump the state of an object.
  void dump_i (void) const;

  /// Map we are iterating over.
  const ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> *map_man_;

  /// Keeps track of how far we've advanced in the table.
  ssize_t index_;

  /// Keeps track of how far we've advanced in a linked list in each
  /// table slot.
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next_;
};

/**
 * @class ACE_Hash_Map_Iterator_Ex
 *
 * @brief Forward iterator for the <ACE_Hash_Map_Manager_Ex>.
 *
 * This class does not perform any internal locking of the
 * <ACE_Hash_Map_Manager_Ex> it is iterating upon since locking is
 * inherently inefficient and/or error-prone within an STL-style
 * iterator.  If you require locking, you can explicitly use an
 * ACE_Guard or ACE_Read_Guard on the <ACE_Hash_Map_Manager_Ex>'s
 * internal lock, which is accessible via its <mutex> method.
 */
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_Iterator_Ex : public ACE_Hash_Map_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>
{
public:
  // = Initialization method.
  ACE_Hash_Map_Iterator_Ex (ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> &mm,
                            int tail = 0);

  // = Iteration methods.
  /// Move forward by one element in the set.  Returns 0 when all the
  /// items in the set have been seen, else 1.
  int advance (void);

  /// Dump the state of an object.
  void dump (void) const;

  // = STL styled iteration, compare, and reference functions.

  /// Prefix advance.
  ACE_Hash_Map_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> &operator++ (void);

  /// Postfix advance.
  ACE_Hash_Map_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> operator++ (int);

  /// Prefix reverse.
  ACE_Hash_Map_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> &operator-- (void);

  /// Postfix reverse.
  ACE_Hash_Map_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> operator-- (int);

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;
};

/**
 * @class ACE_Hash_Map_Const_Iterator_Ex
 *
 * @brief Const forward iterator for the <ACE_Hash_Map_Manager_Ex>.
 *
 * This class does not perform any internal locking of the
 * <ACE_Hash_Map_Manager_Ex> it is iterating upon since locking is
 * inherently inefficient and/or error-prone within an STL-style
 * iterator.  If you require locking, you can explicitly use an
 * ACE_Guard or ACE_Read_Guard on the <ACE_Hash_Map_Manager_Ex>'s
 * internal lock, which is accessible via its <mutex> method.
 */
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_Const_Iterator_Ex : public ACE_Hash_Map_Const_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>
{
public:
  // = Initialization method.
  ACE_Hash_Map_Const_Iterator_Ex (const ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> &mm,
                                  int tail = 0);

  // = Iteration methods.
  /// Move forward by one element in the set.  Returns 0 when all the
  /// items in the set have been seen, else 1.
  int advance (void);

  /// Dump the state of an object.
  void dump (void) const;

  // = STL styled iteration, compare, and reference functions.

  /// Prefix advance.
  ACE_Hash_Map_Const_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> &operator++ (void);

  /// Postfix advance.
  ACE_Hash_Map_Const_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> operator++ (int);

  /// Prefix reverse.
  ACE_Hash_Map_Const_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> &operator-- (void);

  /// Postfix reverse.
  ACE_Hash_Map_Const_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> operator-- (int);

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;
};

/**
 * @class ACE_Hash_Map_Bucket_Iterator
 *
 * @brief Forward iterator for the <ACE_Hash_Map_Manager_Ex> which
 * only traverses a particular bucket.  The particular bucket is
 * specified by the <EXT_ID> parameter specified in the constructor.
 *
 * This class does not perform any internal locking of the
 * <ACE_Hash_Map_Manager_Ex> it is iterating upon since locking is
 * inherently inefficient and/or error-prone within an STL-style
 * iterator.  If you require locking, you can explicitly use an
 * ACE_Guard or ACE_Read_Guard on the <ACE_Hash_Map_Manager_Ex>'s
 * internal lock, which is accessible via its <mutex> method.
 *
 * Note that a creation method for this new iterator cannot be added
 * to the hash map, since this would require adding explicit template
 * instantiations for bucket iterators on platforms with broken
 * templates.
 */
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_Bucket_Iterator
{
public:
  // = Initialization method.
  ACE_Hash_Map_Bucket_Iterator (ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> &mm,
                                const EXT_ID &ext_id,
                                int tail = 0);

  // = STL styled iteration, compare, and reference functions.

  /// Prefix advance.
  ACE_Hash_Map_Bucket_Iterator<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> &operator++ (void);

  /// Postfix advance.
  ACE_Hash_Map_Bucket_Iterator<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> operator++ (int);

  /// Prefix reverse.
  ACE_Hash_Map_Bucket_Iterator<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> &operator-- (void);

  /// Postfix reverse.
  ACE_Hash_Map_Bucket_Iterator<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> operator-- (int);

  /// Returns a reference to the interal element <this> is pointing to.
  ACE_Hash_Map_Entry<EXT_ID, INT_ID>& operator* (void) const;

  /// Returns reference the Hash_Map_Manager_Ex that is being iterated
  /// over.
  ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>& map (void);

  /// Check if two iterators point to the same position
  bool operator== (const ACE_Hash_Map_Bucket_Iterator<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> &) const;
  bool operator!= (const ACE_Hash_Map_Bucket_Iterator<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> &) const;

protected:
  /// Move forward by one element in the set.  Returns 0 when there's
  /// no more item in the set after the current items, else 1.
  int forward_i (void);

  /// Move backward by one element in the set.  Returns 0 when there's
  /// no more item in the set before the current item, else 1.
  int reverse_i (void);

  /// Map we are iterating over.
  ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> *map_man_;

  /// Keeps track of how far we've advanced in the table.
  ssize_t index_;

  /// Keeps track of how far we've advanced in a linked list in each
  /// table slot.
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next_;
};

/**
 * @class ACE_Hash_Map_Reverse_Iterator_Ex
 *
 * @brief Reverse iterator for the <ACE_Hash_Map_Manager_Ex>.
 *
 * This class does not perform any internal locking of the
 * <ACE_Hash_Map_Manager_Ex> it is iterating upon since locking is
 * inherently inefficient and/or error-prone within an STL-style
 * iterator.  If you require locking, you can explicitly use an
 * ACE_Guard or ACE_Read_Guard on the <ACE_Hash_Map_Manager_Ex>'s
 * internal lock, which is accessible via its <mutex> method.
 */
template <class EXT_ID, class INT_ID, class HASH_KEY, class COMPARE_KEYS, class ACE_LOCK>
class ACE_Hash_Map_Reverse_Iterator_Ex : public ACE_Hash_Map_Iterator_Base_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK>
{
public:
  // = Initialization method.
  ACE_Hash_Map_Reverse_Iterator_Ex (ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> &mm,
                                    int head = 0);

  // = Iteration methods.
  /// Move forward by one element in the set.  Returns 0 when all the
  /// items in the set have been seen, else 1.
  int advance (void);

  /// Dump the state of an object.
  void dump (void) const;

  // = STL styled iteration, compare, and reference functions.

  /// Prefix reverse.
  ACE_Hash_Map_Reverse_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> &operator++ (void);

  /// Postfix reverse.
  ACE_Hash_Map_Reverse_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> operator++ (int);

  /// Prefix advance.
  ACE_Hash_Map_Reverse_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> &operator-- (void);

  /// Postfix advance.
  ACE_Hash_Map_Reverse_Iterator_Ex<EXT_ID, INT_ID, HASH_KEY, COMPARE_KEYS, ACE_LOCK> operator-- (int);

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;
};

/**
 * @class ACE_Hash_Map_Manager
 *
 * @brief Wrapper for backward compatibility.
 *
 * This implementation of a map uses a hash table.  This class
 * expects that the <EXT_ID> contains a method called <hash>.
 * In addition, the <EXT_ID> must support <operator==>.  Both of
 * these constraints can be alleviated via template
 * specialization, as shown in the $ACE_ROOT/tests/Conn_Test.cpp
 * test.
 *
 * <b> Requirements and Performance Characteristics</b>
 *   - Internal Structure
 *       Hash Table
 *   - Duplicates allowed?
 *       No
 *   - Random access allowed?
 *       Yes
 *   - Search speed
 *       O(1)
 *   - Insert/replace speed
 *       O(1), can be longer if the hash map has to resize
 *   - Iterator still valid after change to container?
 *       Yes
 *   - Frees memory for removed elements?
 *       Yes
 *   - Items inserted by
 *       Value
 *   - Requirements for key type
 *       -# Default constructor
 *       -# Copy constructor
 *       -# operator=
 *       -# operator==
 *   - Requirements for object type
 *       -# Default constructor
 *       -# Copy constructor
 *       -# operator=
 *       -# operator<
 */
template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Hash_Map_Manager : public ACE_Hash_Map_Manager_Ex<EXT_ID, INT_ID, ACE_Hash<EXT_ID>, ACE_Equal_To<EXT_ID>, ACE_LOCK>
{
public:
  /// Initialize a <Hash_Map_Manager> with default size.
  ACE_Hash_Map_Manager (ACE_Allocator *alloc = 0);

  /// Initialize a <Hash_Map_Manager> with size <length>.
  ACE_Hash_Map_Manager (size_t size,
                        ACE_Allocator *alloc = 0);

  // = The following two are necessary for template specialization of
  // ACE_Hash_Map_Manager to work.
  int equal (const EXT_ID &id1, const EXT_ID &id2);
  u_long hash (const EXT_ID &ext_id);
};

/**
 * @class ACE_Hash_Map_Iterator
 *
 * @brief Wrapper for backward compatibility.
 */
template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Hash_Map_Iterator : public ACE_Hash_Map_Iterator_Ex<EXT_ID, INT_ID, ACE_Hash<EXT_ID>, ACE_Equal_To<EXT_ID>, ACE_LOCK>
{
public:
  // = Initialization method.
  /// Construct from map
  ACE_Hash_Map_Iterator (ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK> &mm,
                         int tail = 0);

  /// Construct from base
  ACE_Hash_Map_Iterator (const ACE_Hash_Map_Iterator_Ex<EXT_ID, INT_ID, ACE_Hash<EXT_ID>, ACE_Equal_To<EXT_ID>, ACE_LOCK> &base);

  /// Assignment from base
  ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> &
  operator= (const ACE_Hash_Map_Iterator_Ex<EXT_ID, INT_ID, ACE_Hash<EXT_ID>, ACE_Equal_To<EXT_ID>, ACE_LOCK> &base);
};

/**
 * @class ACE_Hash_Map_Const_Iterator
 *
 * @brief Wrapper for backward compatibility.
 */
template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Hash_Map_Const_Iterator : public ACE_Hash_Map_Const_Iterator_Ex<EXT_ID, INT_ID, ACE_Hash<EXT_ID>, ACE_Equal_To<EXT_ID>, ACE_LOCK>
{
public:
  // = Initialization method.
  /// Construct from map
  ACE_Hash_Map_Const_Iterator (const ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK> &mm,
                               int tail = 0);

  /// Construct from base
  ACE_Hash_Map_Const_Iterator (const ACE_Hash_Map_Const_Iterator_Ex<EXT_ID, INT_ID, ACE_Hash<EXT_ID>, ACE_Equal_To<EXT_ID>, ACE_LOCK> &base);

  /// Assignment from base
  ACE_Hash_Map_Const_Iterator<EXT_ID, INT_ID, ACE_LOCK> &
  operator= (const ACE_Hash_Map_Const_Iterator_Ex<EXT_ID, INT_ID, ACE_Hash<EXT_ID>, ACE_Equal_To<EXT_ID>, ACE_LOCK> &base);
};

/**
 * @class ACE_Hash_Map_Reverse_Iterator
 *
 * @brief Wrapper for backward compatibility.
 */
template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Hash_Map_Reverse_Iterator : public ACE_Hash_Map_Reverse_Iterator_Ex<EXT_ID, INT_ID, ACE_Hash<EXT_ID>, ACE_Equal_To<EXT_ID>, ACE_LOCK>
{
public:
  // = Initialization method.
  ACE_Hash_Map_Reverse_Iterator (ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK> &mm,
                                 int head = 0);

  /// Construct from base
  ACE_Hash_Map_Reverse_Iterator (const ACE_Hash_Map_Reverse_Iterator_Ex<EXT_ID, INT_ID, ACE_Hash<EXT_ID>, ACE_Equal_To<EXT_ID>, ACE_LOCK> &base);

  /// Assignment from base
  ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> &
  operator= (const ACE_Hash_Map_Reverse_Iterator_Ex<EXT_ID, INT_ID, ACE_Hash<EXT_ID>, ACE_Equal_To<EXT_ID>, ACE_LOCK> &base);
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
// Include ace/Hash_Map_Manager_T.i on all platforms excluding SunCC.
// This nonsense is necessary since SunCC (version 4.2) cannot inline
// the code in ace/Hash_Map_Manager_T.i (with the fast option).
# if !(defined (__SUNPRO_CC) && (__SUNPRO_CC == 0x420))
#  include "ace/Hash_Map_Manager_T.inl"
# endif /* ! __SUNPRO_CC */
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Hash_Map_Manager_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Hash_Map_Manager_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_HASH_MAP_MANAGER_T_H */
