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
{
  // = TITLE
  //     Define an entry in the hash table.
public:
  // = Initialization and termination methods.
  ACE_Hash_Map_Entry (const EXT_ID &ext_id,
		      const INT_ID &int_id,
		      ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next = 0,
                      ACE_Hash_Map_Entry<EXT_ID, INT_ID> *prev = 0);
  // Constructor.

  ACE_Hash_Map_Entry (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next,
                      ACE_Hash_Map_Entry<EXT_ID, INT_ID> *prev);
  // Constructor.

  ~ACE_Hash_Map_Entry (void);
  // Destructor.

  EXT_ID ext_id_;
  // Key used to look up an entry.

  INT_ID int_id_;
  // The contents of the entry itself.

  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next_;
  // Pointer to the next item in the bucket of overflow nodes.

  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *prev_;
  // Pointer to the prev item in the bucket of overflow nodes.

  void dump (void) const;
  // Dump the state of an object.
};

// Forward decl.
template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Hash_Map_Iterator_Base;

// Forward decl.
template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Hash_Map_Iterator;

// Forward decl.
template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Hash_Map_Reverse_Iterator;

template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Hash_Map_Manager 
{
  // = TITLE
  //     Define a map abstraction that efficiently associates
  //     <EXT_ID>s with <INT_ID>s.
  //     
  // = DESCRIPTION
  //     This implementation of a map uses a hash table.  Therefore,
  //     this class expects that the <EXT_ID> contains a method called
  //     <hash>.  In addition, the <EXT_ID> must support <operator==>.
  //     Both of these constraints can be alleviated via template
  //     specialization, as shown in the $ACE_ROOT/tests/Conn_Test.cpp
  //     test.
  //
  //     This class uses an <ACE_Allocator> to allocate memory.  The
  //     user can make this a persistent class by providing an
  //     <ACE_Allocator> with a persistable memory pool.
public:
  friend class ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>;
  friend class ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>;
  friend class ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>;

  typedef EXT_ID KEY;
  typedef INT_ID VALUE;
  typedef ACE_Hash_Map_Entry<EXT_ID, INT_ID> 
          ENTRY;
  typedef ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> 
          ITERATOR;
  typedef ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> 
          REVERSE_ITERATOR;
  // = STL-style iterator typedefs.
  typedef ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> 
          iterator;
  typedef ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> 
          reverse_iterator;

  // = Initialization and termination methods.

  ACE_Hash_Map_Manager (ACE_Allocator *alloc = 0);
  // Initialize a <Hash_Map_Manager> with default size.

  ACE_Hash_Map_Manager (size_t size, 
			ACE_Allocator *alloc = 0);
  // Initialize a <Hash_Map_Manager> with size <length>.

  int open (size_t size = ACE_DEFAULT_MAP_SIZE, 
	    ACE_Allocator *alloc = 0);
  // Initialize a <Hash_Map_Manager> with <size> elements.

  int close (void);
  // Close down a <Hash_Map_Manager> and release dynamically allocated
  // resources.

  ~ACE_Hash_Map_Manager (void);
  // Initialize a <Hash_Map_Manager> with size <length>.

  int bind (const EXT_ID &item,
	    const INT_ID &int_id);
  // Associate <ext_id> with <int_id>.  If <ext_id> is already in the
  // map then the <ACE_Hash_Map_Entry> is not changed.  Returns 0 if a
  // new entry is bound successfully, returns 1 if an attempt is made
  // to bind an existing entry, and returns -1 if failures occur.

  int bind (const EXT_ID &ext_id,
            const INT_ID &int_id,
            ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry);
  // Same as a normal bind, except the map entry is also passed back
  // to the caller.  The entry in this case will either be the newly
  // created entry, or the existing one.

  int trybind (const EXT_ID &ext_id,
	       INT_ID &int_id);
  // Associate <ext_id> with <int_id> if and only if <ext_id> is not
  // in the map.  If <ext_id> is already in the map then the <int_id>
  // parameter is assigned the existing value in the map.  Returns 0
  // if a new entry is bound successfully, returns 1 if an attempt is
  // made to bind an existing entry, and returns -1 if failures occur.

  int trybind (const EXT_ID &ext_id,
	       INT_ID &int_id,
               ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry);
  // Same as a normal trybind, except the map entry is also passed
  // back to the caller.  The entry in this case will either be the
  // newly created entry, or the existing one.

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

  int rebind (const EXT_ID &ext_id, 
	      const INT_ID &int_id,
	      EXT_ID &old_ext_id, 
	      INT_ID &old_int_id,
              ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry);
  // Same as a normal rebind, except the map entry is also passed back
  // to the caller.  The entry in this case will either be the newly
  // created entry, or the existing one.

  int find (const EXT_ID &ext_id,
	    INT_ID &int_id);
  // Locate <ext_id> and pass out parameter via <int_id>.  If found,
  // return 0, returns -1 if not found.

  int find (const EXT_ID &ext_id);
  // Returns 0 if the <ext_id> is in the mapping, otherwise -1.

  int find (const EXT_ID &ext_id,
	    ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry);
  // Locate <ext_id> and pass out parameter via <entry>.  If found,
  // return 0, returns -1 if not found.

  int unbind (const EXT_ID &ext_id);
  // Unbind (remove) the <ext_id> from the map.  Don't return the
  // <int_id> to the caller (this is useful for collections where the
  // <int_id>s are *not* dynamically allocated...)

  int unbind (const EXT_ID &ext_id,
	      INT_ID &int_id);
  // Break any association of <ext_id>.  Returns the value of <int_id>
  // in case the caller needs to deallocate memory.

  int unbind (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *entry);
  // Remove entry from map.

  size_t current_size (void);
  // Return the current size of the map.

  size_t total_size (void);
  // Return the total size of the map.

  void dump (void) const;
  // Dump the state of an object.

  // = STL styled iterator factory functions.

  ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> begin (void);
  ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> end (void);
  // Return forward iterator.

  ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> rbegin (void);
  ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> rend (void);
  // Return reverse iterator.

protected:
  // = The following methods do the actual work.

  int equal (const EXT_ID &id1, const EXT_ID &id2);
  // Returns 1 if <id1> == <id2>, else 0.  This is defined as a
  // separate method to facilitate template specialization.

  u_long hash (const EXT_ID &ext_id);
  // Compute the hash value of the <ext_id>.  This is defined as a
  // separate method to facilitate template specialization.

  // = These methods assume locks are held by private methods.
  
  int bind_i (const EXT_ID &ext_id, 
              const INT_ID &int_id);
  // Performs bind.  Must be called with locks held.

  int bind_i (const EXT_ID &ext_id,
              const INT_ID &int_id,
              ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry);
  // Performs bind.  Must be called with locks held.

  int trybind_i (const EXT_ID &ext_id, 
                 INT_ID &int_id);
  // Performs trybind.  Must be called with locks held.

  int trybind_i (const EXT_ID &ext_id,
                 INT_ID &int_id,
                 ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry);
  // Performs trybind.  Must be called with locks held.
  
  int rebind_i (const EXT_ID &ext_id, 
                const INT_ID &int_id,
		EXT_ID &old_ext_id, 
                INT_ID &old_int_id);
  // Performs rebind.  Must be called with locks held.

  int rebind_i (const EXT_ID &ext_id, 
                const INT_ID &int_id,
                EXT_ID &old_ext_id, 
                INT_ID &old_int_id,
                ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry);
  // Performs rebind.  Must be called with locks held.

  int find_i (const EXT_ID &ext_id, 
              INT_ID &int_id);
  // Performs a find of <int_id> using <ext_id> as the key.  Must be
  // called with locks held.

  int find_i (const EXT_ID &ext_id);
  // Performs a find using <ext_id> as the key.  Must be called with
  // locks held.

  int find_i (const EXT_ID &ext_id,
              ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry);
  // Performs a find using <ext_id> as the key.  Must be called with
  // locks held.

  int unbind_i (const EXT_ID &ext_id, 
                INT_ID &int_id);
  // Performs unbind.  Must be called with locks held.

  int unbind_i (const EXT_ID &ext_id);
  // Performs unbind.  Must be called with locks held.

  int unbind_i (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *entry);
  // Performs unbind.  Must be called with locks held.

  int create_buckets (size_t size);
  // Resize the map.  Must be called with locks held.  Note, that this
  // method should never be called more than once or else all the
  // hashing will get screwed up as the size will change.

  int close_i (void);
  // Close down a <Map_Manager>.  Must be called with
  // locks held.

  ACE_Allocator *allocator_;
  // Pointer to a memory allocator.

  ACE_LOCK lock_; 
  // Synchronization variable for the MT_SAFE <ACE_Hash_Map_Manager>.

private:
  int shared_find (const EXT_ID &ext_id,
                   ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&entry,
                   u_long &loc);
  // Returns the <ACE_Hash_Map_Entry> that corresponds to <ext_id>.

  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *table_;
  // Array of <ACE_Hash_Map_Entry> *s, each of which points to an
  // <ACE_Hash_Map_Entry> that serves as the beginning of a linked
  // list of <EXT_ID>s that hash to that bucket.

  size_t total_size_;
  // Total size of the hash table.

  size_t cur_size_;
  // Current number of entries in the table (note that this can be
  // larger than <total_size_> due to the bucket chaining).
};

template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Hash_Map_Iterator_Base
{
  // = TITLE
  //     Iterator for the ACE_Hash_Map_Manager.
public:
  // = Initialization method.
  ACE_Hash_Map_Iterator_Base (ACE_Hash_Map_Manager <EXT_ID, INT_ID, ACE_LOCK> &mm,
			      int head);
  // Contructor.  If head != 0, the iterator constructed is positioned
  // at the head of the map, it is positioned at the end otherwise.

  // = ITERATION methods.

  int next (ACE_Hash_Map_Entry<EXT_ID, INT_ID> *&next_entry);
  // Pass back the next <entry> that hasn't been seen in the Set.
  // Returns 0 when all items have been seen, else 1.

  int done (void) const;
  // Returns 1 when all items have been seen, else 0.

  ACE_Hash_Map_Entry<EXT_ID, INT_ID>& operator* (void);
  // Returns a reference to the interal element <this> is pointing
  // to. 
  
  ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK>& map (void);
  // Returns reference the Hash_Map_Manager that is being iterated
  // over. 

  int operator== (const ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK> &) const;
  int operator!= (const ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK> &) const;
  // Check if two iterators point to the same position

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  int forward_i (void);
  // Move forward by one element in the set.  Returns 0 when there's
  // no more item in the set after the current items, else 1.
  
  int reverse_i (void);
  // Move backward by one element in the set.  Returns 0 when there's
  // no more item in the set before the current item, else 1.
  
  void dump_i (void) const;
  // Dump the state of an object.
  
  ACE_Hash_Map_Manager <EXT_ID, INT_ID, ACE_LOCK> *map_man_;
  // Map we are iterating over.
  
  ssize_t index_;
  // Keeps track of how far we've advanced in the table.
  
  ACE_Hash_Map_Entry<EXT_ID, INT_ID> *next_;
  // Keeps track of how far we've advanced in a linked list in each
  // table slot.
};

template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Hash_Map_Iterator
  : public ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>
{
  // = TITLE
  //     Iterator for the ACE_Hash_Map_Manager.
  //
  // = DESCRIPTION
public:
  // = Initialization method.
  ACE_Hash_Map_Iterator (ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK> &mm,
			 int tail = 0);

  // = Iteration methods.

  int advance (void);
  // Move forward by one element in the set.  Returns 0 when all the
  // items in the set have been seen, else 1.
  
  void dump (void) const;
  // Dump the state of an object.

  // = STL styled iteration, compare, and reference functions.

  ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> operator++ (void);
  // Postfix advance.

  ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>& operator++ (int);
  // Prefix advance.

  ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK> operator-- (void);
  // Postfix advance.

  ACE_Hash_Map_Iterator<EXT_ID, INT_ID, ACE_LOCK>& operator-- (int);
  // Prefix advance.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

template <class EXT_ID, class INT_ID, class ACE_LOCK>
class ACE_Hash_Map_Reverse_Iterator
  : public ACE_Hash_Map_Iterator_Base<EXT_ID, INT_ID, ACE_LOCK>
{
  // = TITLE
  //     Iterator for the ACE_Hash_Map_Manager.
  //
  // = DESCRIPTION
public:
  // = Initialization method.
  ACE_Hash_Map_Reverse_Iterator (ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_LOCK> &mm,
				 int head = 0);

  // = Iteration methods.

  int advance (void);
  // Move forward by one element in the set.  Returns 0 when all the
  // items in the set have been seen, else 1.
  
  void dump (void) const;
  // Dump the state of an object.

  // = STL styled iteration, compare, and reference functions.

  ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> operator++ (void);
  // Postfix advance.

  ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>& operator++ (int);
  // Prefix advance.

  ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK> operator-- (void);
  // Postfix advance.

  ACE_Hash_Map_Reverse_Iterator<EXT_ID, INT_ID, ACE_LOCK>& operator-- (int);
  // Prefix advance.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Hash_Map_Manager.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Hash_Map_Manager.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_HASH_MAP_MANAGER_H */

