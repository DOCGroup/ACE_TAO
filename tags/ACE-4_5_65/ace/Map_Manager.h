/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Map_Manager.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_MAP_MANAGER_H
#define ACE_MAP_MANAGER_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"

// Forward declaration.
class ACE_Allocator;

template <class EXT_ID, class INT_ID>
class ACE_Map_Entry
{
  // = TITLE
  //     An entry in the Map.
public:
  EXT_ID ext_id_;
  // Key used to look up an entry.

  INT_ID int_id_;
  // The contents of the entry itself.

  int is_free_;
  // Keeps track whether entry is free or not.

# if ! defined (ACE_HAS_BROKEN_NOOP_DTORS)
  ~ACE_Map_Entry (void);
  // We need this destructor to keep some compilers from complaining.
  // It's just a no-op, however.
# endif /* ! defined (ACE_HAS_BROKEN_NOOP_DTORS) */

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

// Forward decl.
template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Map_Iterator_Base;

// Forward decl.
template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Map_Iterator;

// Forward decl.
template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Map_Reverse_Iterator;

template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Map_Manager
{
  // = TITLE
  //     Define a map abstraction that associates <EXT_ID>s with
  //     <INT_ID>s.
  //
  // = DESCRIPTION
  //     The <EXT_ID> must support <operator==>.  This constraint can
  //     be alleviated via template specialization, as shown in the
  //     $ACE_ROOT/tests/Conn_Test.cpp test.
  //
  //     This class uses an <ACE_Allocator> to allocate memory.  The
  //     user can make this a persistant class by providing an
  //     <ACE_Allocator> with a persistable memory pool.
  //
  //     This implementation of a map uses an array, which is searched
  //     linearly.  For more efficient searching you should use the
  //     <ACE_Hash_Map_Manager>.
public:
  friend class ACE_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>;
  friend class ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>;
  friend class ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>;

  // = Traits.
  typedef EXT_ID KEY;
  typedef INT_ID VALUE;
  typedef ACE_Map_Entry<EXT_ID, INT_ID> ENTRY;
  typedef ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> ITERATOR;
  typedef ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> REVERSE_ITERATOR;

  typedef ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> iterator;
  typedef ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> reverse_iterator;

  // = Initialization and termination methods.
  ACE_Map_Manager (ACE_Allocator *alloc = 0);
  // Initialize a <Map_Manager> with the <ACE_DEFAULT_MAP_SIZE>.

  ACE_Map_Manager (size_t size,
                   ACE_Allocator *alloc = 0);
  // Initialize a <Map_Manager> with <size> entries.

  int open (size_t length = ACE_DEFAULT_MAP_SIZE,
            ACE_Allocator *alloc = 0);
  // Initialize a <Map_Manager> with size <length>.

  int close (void);
  // Close down a <Map_Manager> and release dynamically allocated
  // resources.

  ~ACE_Map_Manager (void);
  // Close down a <Map_Manager> and release dynamically allocated
  // resources.

  int trybind (const EXT_ID &ext_id,
               INT_ID &int_id);
  // Associate <ext_id> with <int_id> if and only if <ext_id> is not
  // in the map.  If <ext_id> is already in the map then the <int_id>
  // parameter is overwritten with the existing value in the map
  // Returns 0 if a new entry is bound successfully, returns 1 if an
  // attempt is made to bind an existing entry, and returns -1 if
  // failures occur.

  int bind (const EXT_ID &ext_id,
            const INT_ID &int_id);
  // Associate <ext_id> with <int_id>.  If <ext_id> is already in the
  // map then the <Map_Entry> is not changed.  Returns 0 if a new
  // entry is bound successfully, returns 1 if an attempt is made to
  // bind an existing entry, and returns -1 if failures occur.

  int rebind (const EXT_ID &ext_id,
              const INT_ID &int_id,
              EXT_ID &old_ext_id,
              INT_ID &old_int_id);
  // Associate <ext_id> with <int_id>.  If <ext_id> is not in the
  // map then behaves just like <bind>.  Otherwise, store the old
  // values of <ext_id> and <int_id> into the "out" parameters and
  // rebind the new parameters.  This is very useful if you need to
  // have an atomic way of updating <Map_Entries> and you also need
  // full control over memory allocation.  Returns 0 if a new entry is
  // bound successfully, returns 1 if an existing entry was rebound,
  // and returns -1 if failures occur.

  int find (const EXT_ID &ext_id, INT_ID &int_id);
  // Locate <ext_id> and pass out parameter via <int_id>.  If found,
  // returns and non-negative integer; returns -1 if not found.

  int find (const EXT_ID &ext_id);
  // Returns a non-negative integer if the <ext_id> is in the mapping, otherwise -1.

  int unbind (const EXT_ID &ext_id);
  // Unbind (remove) the <ext_id> from the map.  Don't return the
  // <int_id> to the caller (this is useful for collections where the
  // <int_id>s are *not* dynamically allocated...)  Returns 0 if
  // successful, else -1.

  int unbind (const EXT_ID &ext_id, INT_ID &int_id);
  // Break any association of <ext_id>.  Returns the value of <int_id>
  // in case the caller needs to deallocate memory.  Returns 0 if
  // successful, else -1.

  size_t current_size (void);
  // Return the current size of the map.

  size_t total_size (void);
  // Return the total size of the map.

  ACE_LOCK &mutex (void);
  // Returns a reference to the underlying <ACE_LOCK>.  This makes it
  // possible to acquire the lock explicitly, which can be useful in
  // some cases if you instantiate the <ACE_Atomic_Op> with an
  // <ACE_Recursive_Mutex> or <ACE_Process_Mutex>, or if you need to
  // guard the state of an iterator.  NOTE: the right name would be
  // <lock>, but HP/C++ will choke on that!

  void dump (void) const;
  // Dump the state of an object.

  // = STL styled iterator factory functions.

  ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> begin (void);
  ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> end (void);
  // Return forward iterator.

  ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> rbegin (void);
  ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> rend (void);
  // Return reverse iterator.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:

  void free_search_structure (void);
  // Explicitly call the destructors and free up the search_structure_.

  ACE_Map_Entry<EXT_ID, INT_ID> *search_structure_;
  // Implementation of the Map (should use hashing instead of
  // array...).

  // = The following methods do the actual work.

  // These methods assume that the locks are held by the private
  // methods.

  int bind_i (const EXT_ID &ext_id, const INT_ID &int_id);
  // Performs the binding of <ext_id> to <int_id>.  Must be
  // called with locks held.

  int rebind_i (const EXT_ID &ext_id, const INT_ID &int_id,
                EXT_ID &old_ext_id, INT_ID &old_int_id);
  // Performs a rebinding of <ext_it> to <int_id>.  Must be called
  // with locks held.

  int find_i (const EXT_ID &ext_id, INT_ID &int_id);
  // Performs a find of <int_id> using <ext_id> as the key.  Must be
  // called with locks held.

  int find_i (const EXT_ID &ext_id);
  // Performs a find using <ext_id> as the key.  Must be called with
  // locks held.

  int unbind_i (const EXT_ID &ext_id, INT_ID &int_id);
  // Performs an unbind of <int_id> using <ext_id> as the key.  Must
  // be called with locks held.

  int unbind_i (const EXT_ID &ext_id);
  // Performs an unbind using <ext_id> as the key.  Must be called
  // with locks held.

  int trybind_i (const EXT_ID &ext_id, INT_ID &int_id);
  // Performs a conditional bind of <int_id> using <ext_id> as the
  // key.  Must be called with locks held.

  int resize_i (size_t size);
  // Resize the map.  Must be called with locks held.

  int close_i (void);
  // Close down a <Map_Manager>.  Must be called with
  // locks held.

  ACE_Allocator *allocator_;
  // Pointer to a memory allocator.

  ACE_LOCK lock_;
  // Synchronization variable for the MT_SAFE <ACE_Map_Manager>.

  int equal (const EXT_ID &id1, const EXT_ID &id2);
  // Returns 1 if <id1> == <id2>, else 0.  This is defined as a
  // separate method to facilitate template specialization.

private:

  int shared_find (const EXT_ID &ext_id, int &first_free);
  // Locate an entry, keeping track of the first free slot.  Must be
  // called with locks held.

  int shared_find (const EXT_ID &ext_id);
  // Locate an entry.  Must be called with locks held.

  int shared_bind (const EXT_ID &ext_id, const INT_ID &int_id, int first_free);
  // Bind an entry.  Must be called with locks held.

  int shared_unbind (const EXT_ID &ext_id);
  // Unbind (remove) the <ext_id> from the map.  Keeps track of where
  // the <ext_id> was found so that this->unbind (<ext_id>, <int_id>)
  // can return it to the caller.  Must be called with locks held.

  size_t total_size_;
  // Total number of elements in this->search_structure_.

  size_t cur_size_;
  // Index of highest active elementin this->search_structure_.

  // = Disallow these operations.
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK> &))
  ACE_UNIMPLEMENTED_FUNC (ACE_Map_Manager (const ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK> &))
};

template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Map_Iterator_Base
{
  // = TITLE
  //     Iterator for the <ACE_Map_Manager>.
  //
  // = DESCRIPTION
  //     This class factors out common code from its templatized
  //     subclasses.
public:
  // = Initialization method.
  ACE_Map_Iterator_Base (ACE_Map_Manager <EXT_ID, INT_ID, ACE_LOCK> &mm,
                         int head);
  // Contructor.  If head != 0, the iterator constructed is positioned
  // at the head of the map, it is positioned at the end otherwise.

  // = Iteration methods.

  int next (ACE_Map_Entry<EXT_ID, INT_ID> *&next_entry);
  // Pass back the next <entry> that hasn't been seen in the Set.
  // Returns 0 when all items have been seen, else 1.

  int done (void) const;
  // Returns 1 when all items have been seen, else 0.

  ACE_Map_Entry<EXT_ID, INT_ID>& operator* (void);
  // Returns a reference to the interal element <this> is pointing to.

  ACE_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>& map (void);
  // Returns reference the Map_Manager that is being iterated
  // over.

  int operator== (const ACE_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK> &) const;
  int operator!= (const ACE_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK> &) const;
  // Check if two iterators point to the same position

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  int forward_i (void);
  // Move forward by one element in the set.  Returns 0 when there's
  // no more item in the set after the current items, else 1.

  int reverse_i (void);
  // Move backware by one element in the set.  Returns 0 when there's
  // no more item in the set before the current item, else 1.

  void dump_i (void) const;
  // Dump the state of an object.

  ACE_Map_Manager <EXT_ID, INT_ID, ACE_LOCK> *map_man_;
  // Map we are iterating over.

  ssize_t next_;
  // Keeps track of how far we've advanced...
};

template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Map_Iterator : public ACE_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>
{
  // = TITLE
  //     Forward iterator for the <ACE_Map_Manager>.
  //
  // = DESCRIPTION
  //     This class does not perform any internal locking of the
  //     <ACE_Map_Manager> it is iterating upon since locking is
  //     inherently inefficient and/or error-prone within an STL-style
  //     iterator.  If you require locking, you can explicitly use an
  //     <ACE_Guard> or <ACE_Read_Guard> on the <ACE_Map_Manager>'s
  //     internal lock, which is accessible via its <mutex> method.
public:
  // = Initialization method.
  ACE_Map_Iterator (ACE_Map_Manager <EXT_ID, INT_ID, ACE_LOCK> &mm,
                    int tail = 0);

  // = Iteration methods.

  int advance (void);
  // Move forward by one element in the set.   Returns 0 when all the
  // items in the set have been seen, else 1.

  void dump (void) const;
  // Dump the state of an object.

  // = STL styled iteration, compare, and reference functions.

  ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> &operator++ (void);
  // Prefix advance.

  ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> operator++ (int);
  // Postfix advance.

  ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> &operator-- (void);
  // Prefix reverse.

  ACE_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> operator-- (int);
  // Postfix reverse.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Map_Reverse_Iterator : public ACE_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>
{
  // = TITLE
  //     Reverse Iterator for the <ACE_Map_Manager>.
  //
  // = DESCRIPTION
  //     This class does not perform any internal locking of the
  //     <ACE_Map_Manager> it is iterating upon since locking is
  //     inherently inefficient and/or error-prone within an STL-style
  //     iterator.  If you require locking, you can explicitly use an
  //     <ACE_Guard> or <ACE_Read_Guard> on the <ACE_Map_Manager>'s
  //     internal lock, which is accessible via its <mutex> method.
public:
  // = Initialization method.
  ACE_Map_Reverse_Iterator (ACE_Map_Manager <EXT_ID, INT_ID, ACE_LOCK> &mm,
                            int head = 0);

  // = Iteration methods.

  int advance (void);
  // Move forward by one element in the set.  Returns 0 when all the
  // items in the set have been seen, else 1.

  void dump (void) const;
  // Dump the state of an object.

  // = STL styled iteration, compare, and reference functions.

  ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> &operator++ (void);
  // Prefix reverse.

  ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> operator++ (int);
  // Postfix reverse.

  ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> &operator-- (void);
  // Prefix advance.

  ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> operator-- (int);
  // Postfix advance.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#if defined (__ACE_INLINE__)
#include "ace/Map_Manager.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Map_Manager.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Map_Manager.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_MAP_MANAGER_H */
