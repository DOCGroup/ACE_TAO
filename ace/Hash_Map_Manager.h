/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Hash_Map_Manager.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_HASH_MAP_MANAGER_H)
#define ACE_HASH_MAP_MANAGER_H

#include "ace/OS.h"

class ACE_Allocator;

template <class EXT_ID, class INT_ID>
class ACE_Hash_Map_Entry
  // = TITLE
  //     Define an entry in the hash table.
{
public:
  // = Initialization and termination methods.
  ACE_Hash_Map_Entry (void);
  // Default constructor.

  ACE_Hash_Map_Entry (const EXT_ID &ext_id,
		      const INT_ID &int_id,
		      ACE_Hash_Map_Entry<EXT_ID, INT_ID> *ptr = 0);
  // Constructor.

  ~ACE_Hash_Map_Entry (void);
  // Destructor.

  EXT_ID ext_id_;
  // Key used to look up an entry.

  INT_ID int_id_;
  // The contents of the entry itself.

  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next_;
  // Pointer to the next item in the bucket of overflow nodes.

  void dump (void) const;
  // Dump the state of an object.
};

// Forward decl.
template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Hash_Map_Iterator;

template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Hash_Map_Manager 
  // = TITLE
  //     Define a map abstraction that associates <EXT_ID>s with
  //     <INT_ID>s.  
  //     
  // = DESCRIPTION
  //     This implementation of a map uses a hash table.  Therefore,
  //     this class expects that the <EXT_ID> contains a method called
  //     <hash>.  In addition, the <EXT_ID> must support <operator==>
  //     (both of these constraints can be alleviated via template
  //     specialization).  This class uses an ACE_Allocator to
  //     allocate memory The user can make this a persistant class by
  //     providing an ACE_Allocator with a persistable memory pool
{
  friend class ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>;
public:
  typedef ACE_Hash_Map_Entry<EXT_ID, INT_ID> ENTRY;
  typedef ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> ITERATOR;

  // = Initialization and termination methods.

  ACE_Hash_Map_Manager (size_t size, 
			ACE_Allocator *alloc = 0);
  // Initialize a <Hash_Map_Manager> with size <length>.

  ACE_Hash_Map_Manager (ACE_Allocator *alloc = 0);
  // Initialize a <Hash_Map_Manager> with default size.

  int open (size_t length = ACE_DEFAULT_MAP_SIZE, 
	    ACE_Allocator *alloc = 0);
  // Initialize a <Hash_Map_Manager> with size <length>.

  int close (void);
  // Close down a <Hash_Map_Manager> and release dynamically allocated
  // resources.

  ~ACE_Hash_Map_Manager (void);
  // Initialize a <Hash_Map_Manager> with size <length>.

  int trybind (const EXT_ID &ext_id,
	       INT_ID &int_id);
  // Associate <ext_id> with <int_id> if and only if <ext_id> is not
  // in the map.  If <ext_id> is already in the map then the <int_id>
  // parameter is assigned the existing value in the map.  Returns 0
  // if a new entry is bound successfully, returns 1 if an attempt is
  // made to bind an existing entry, and returns -1 if failures occur.

  int bind (const EXT_ID &item,
	    const INT_ID &int_id);
  // Associate <ext_id> with <int_id>.  If <ext_id> is already in the
  // map then the <ACE_Hash_Map_Entry> is not changed.  Returns 0 if a
  // new entry is bound successfully, returns 1 if an attempt is made
  // to bind an existing entry, and returns -1 if failures occur.

  int rebind (const EXT_ID &ext_id, 
	      const INT_ID &int_id,
	      EXT_ID &old_ext_id, 
	      INT_ID &old_int_id);
  // Associate <ext_id> with <int_id>.  If <ext_id> is not in the map
  // then behaves just like <bind>.  Otherwise, store the old values
  // of <ext_id> and <int_id> into the "out" parameters and rebind the
  // new parameters.  This is very useful if you need to have an
  // atomic way of updating <ACE_Hash_Map_Entrys> and you also need full
  // control over memory allocation.  Returns 0 if a new entry is
  // bound successfully, returns 1 if an existing entry was rebound,
  // and returns -1 if failures occur.

  int find (const EXT_ID &item,
	    INT_ID &int_id);
  // Locate <ext_id> and pass out parameter via <int_id>.  If found,
  // return 0, returns -1 if not found.

  int find (const EXT_ID &ext_id);
  // Returns 0 if the <ext_id> is in the mapping, otherwise -1.

  int unbind (const EXT_ID &ext_id);
  // Unbind (remove) the <ext_id> from the map.  Don't return the
  // <int_id> to the caller (this is useful for collections where the
  // <int_id>s are *not* dynamically allocated...)

  int unbind (const EXT_ID &ext_id,
	      INT_ID &int_id);
  // Break any association of <ext_id>.  Returns the value of <int_id>
  // in case the caller needs to deallocate memory.

  size_t current_size (void);
  // Return the current size of the map.

  size_t total_size (void);
  // Return the total size of the map.

  void dump (void) const;
  // Dump the state of an object.

protected:
  // = The following methods do the actual work.

  int equal (const EXT_ID &id1, const EXT_ID &id2);
  // Returns 1 if <id1> == <id2>, else 0.  This is defined as a
  // separate method to facilitate template specialization.

  u_long hash (const EXT_ID &ext_id);
  // Compute the hash value of the <ext_id>.  This is defined as a
  // separate method to facilitate template specialization.

  // = These methods assume locks are held by private methods.
  
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

private:
  int shared_find (const EXT_ID &ext_id,
                   ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry,
                   u_long &loc);
  int shared_find (const EXT_ID &ext_id,
                   ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry,
                   ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&prev,
                   u_long &loc);
  // Returns the <ACE_Hash_Map_Entry> that corresponds to <ext_id>.
  // prev points to the previous entry.

  ACE_Hash_Map_Entry<EXT_ID, INT_ID> **table_;
  // Array of <ACE_Hash_Map_Entry> *s, each of which points to the
  // beginning of a linked list of <EXT_ID>s that hash to that bucket.

  size_t total_size_;
  // Total size of the hash table.

  size_t cur_size_;
  // Current number of elements in the table.

  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *sentinel_;
  // Sentinel node that improves lookup time.
  EXT_ID sentinel_ext_id_fakenull_;
  // Cleanup sentinel when it is through.
};

template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Hash_Map_Iterator
  // = TITLE
  //     Iterator for the ACE_Hash_Map_Manager.
  //
  // = DESCRIPTION
{
public:
  // = Initialization method.
  ACE_Hash_Map_Iterator (ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK> &mm);

  // = Iteration methods.

  int next (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&next_entry);
  // Pass back the next <entry> that hasn't been seen in the Set.
  // Returns 0 when all items have been seen, else 1.

  int done (void) const;
  // Returns 1 when all items have been seen, else 0.

  int advance (void);
  // Move forward by one element in the set.  Returns 0 when all the
  // items in the set have been seen, else 1.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK> &map_man_;
  // Map we are iterating over.

  size_t index_;
  // Keeps track of how far we've advanced in the table.

  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next_;
  // Keeps track of how far we've advanced in a linked list in each
  // table slot.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Hash_Map_Manager.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Hash_Map_Manager.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_HASH_MAP_MANAGER_H */
