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

#if !defined (ACE_MAP_MANAGER_H)
#define ACE_MAP_MANAGER_H

#include "ace/ACE.h"

// Forward declaration.
class ACE_Allocator;

template <class EXT_ID, class INT_ID>
struct ACE_Map_Entry
  // = TITLE
  //     An entry in the Map.
{
  EXT_ID ext_id_;
  // Key used to look up an entry.

  INT_ID int_id_;
  // The contents of the entry itself.

  int is_free_;
  // Keeps track whether entry is free or not.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

// Forward decl.
template <class EXT_ID, class INT_ID, class LOCK>
class ACE_Map_Iterator;

// Forward decl.
template <class EXT_ID, class INT_ID, class LOCK>
class ACE_Map_Reverse_Iterator;

template <class EXT_ID, class INT_ID, class LOCK>
class ACE_Map_Manager
  // = TITLE
  //     Define a map abstraction (useful for managing connections and
  //     sessions).
  //     
  // = DESCRIPTION
  //     This implementation of a map uses an array.  It should
  //     be enhanced to use a hash table...  
  //     This class uses an ACE_Allocator to allocate memory
  //     The user can make this a persistant class by providing an 
  //     ACE_Allocator with a persistable memory pool
{
friend class ACE_Map_Iterator<EXT_ID, INT_ID, LOCK>;
friend class ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, LOCK>;
public:
  // = Traits.
  typedef ACE_Map_Entry<EXT_ID, INT_ID> ENTRY;
  typedef ACE_Map_Iterator<EXT_ID, INT_ID, LOCK> ITERATOR;
  typedef ACE_Map_Reverse_Iterator<EXT_ID, INT_ID, LOCK> REVERSE_ITERATOR;

  // = Initialization and termination methods.
  ACE_Map_Manager (ACE_Allocator *allocator = 0);
  // Initialize a <Map_Manager> with the <ACE_DEFAULT_MAP_SIZE>.

  ACE_Map_Manager (size_t size, 
		   ACE_Allocator *allocator = 0);
  // Initialize a <Map_Manager> with <size> entries.

  int open (size_t length = ACE_DEFAULT_MAP_SIZE, 
	    ACE_Allocator *allocator = 0);
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
  // return 0, returns -1 if not found.

  int find (const EXT_ID &ext_id);
  // Returns 0 if the <ext_id> is in the mapping, otherwise -1.

  int unbind (const EXT_ID &ext_id);
  // Unbind (remove) the <ext_id> from the map.  Don't return the
  // <int_id> to the caller (this is useful for collections where the
  // <int_id>s are *not* dynamically allocated...)

  int unbind (const EXT_ID &ext_id, INT_ID &int_id);
  // Break any association of <ext_id>.  Returns the value of <int_id> in
  // case the caller needs to deallocate memory.

  size_t current_size (void);
  // Return the current size of the map.

  size_t total_size (void);
  // Return the total size of the map.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  
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

  LOCK lock_; 
  // Synchronization variable for the MT_SAFE <ACE_Map_Manager>.

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
};

template <class EXT_ID, class INT_ID, class LOCK>
class ACE_Map_Iterator
  // = TITLE
  //     Iterator for the ACE_Map_Manager.
  //
  // = DESCRIPTION
{
public:
  // = Initialization method.
  ACE_Map_Iterator (ACE_Map_Manager <EXT_ID, INT_ID, LOCK> &mm);

  // = Iteration methods.

  int next (ACE_Map_Entry<EXT_ID, INT_ID> *&next_entry);
  // Pass back the next <entry> that hasn't been seen in the Set.
  // Returns 0 when all items have been seen, else 1.

  int advance (void);
  // Move forward by one element in the set.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Map_Manager <EXT_ID, INT_ID, LOCK> &map_man_;
  // Map we are iterating over.

  ssize_t next_;
  // Keeps track of how far we've advanced...
};

template <class EXT_ID, class INT_ID, class LOCK>
class ACE_Map_Reverse_Iterator
  // = TITLE
  //     Reverse Iterator for the ACE_Map_Manager.
  //
  // = DESCRIPTION
{
public:
  // = Initialization method.
  ACE_Map_Reverse_Iterator (ACE_Map_Manager <EXT_ID, INT_ID, LOCK> &mm);

  // = Iteration methods.

  int next (ACE_Map_Entry<EXT_ID, INT_ID> *&next_entry);
  // Pass back the <entry> that hasn't been seen in the Set.
  // Returns 0 when all items have been seen, else 1.

  int advance (void);
  // Move forward by one element in the set.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Map_Manager <EXT_ID, INT_ID, LOCK> &map_man_;
  // Map we are iterating over.

  ssize_t next_;
  // Keeps track of how far we've advanced...
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
