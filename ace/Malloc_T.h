/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Malloc_T.h
//
// = AUTHOR
//    Doug Schmidt and Irfan Pyarali
//
// ============================================================================

#ifndef ACE_MALLOC_T_H
#define ACE_MALLOC_T_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"
#include "ace/Malloc.h"
#include "ace/Free_List.h"

template <class T>
class ACE_Cached_Mem_Pool_Node
{
  // = TITLE
  //    <ACE_Cached_Mem_Pool_Node> keeps unused memory within a free
  //    list.
  //
  // = DESCRIPTION
  //    The length of a piece of unused memory must be greater than
  //    sizeof (void*).  This makes sense because we'll waste even
  //    more memory if we keep them in a separate data structure.
  //    This class should really be placed within the next class
  //    <ACE_Cached_Allocator>.  But this can't be done due to C++
  //    compiler portability problems.
public:
  T *addr (void);
  // return the address of free memory.

  ACE_Cached_Mem_Pool_Node<T> *get_next (void);
  // get the next ACE_Cached_Mem_Pool_Node in a list.

  void set_next (ACE_Cached_Mem_Pool_Node<T> *ptr);
  // set the next ACE_Cached_Mem_Pool_Node.

private:
  ACE_Cached_Mem_Pool_Node<T> *next_;
  // Since memory is not used when placed in a free list,
  // we can use it to maintain the structure of  free list.
  // I was using union to hide the fact of overlapping memory
  // usage.  However, that cause problem on MSVC.  So, I now turn
  // back to hack this with casting.
};

template <class T, class ACE_LOCK>
class ACE_Cached_Allocator : public ACE_New_Allocator
{
  // = TITLE
  //   Create a cached memory poll with <n_chunks> chunks each with
  //   sizeof (TYPE) size.
  //
  // = DESCRIPTION
  //   This class enables caching of dynamically allocated,
  //   fixed-sized classes.
public:
  ACE_Cached_Allocator (size_t n_chunks);
  // Create a cached memory poll with <n_chunks> chunks
  // each with sizeof (TYPE) size.

  ~ACE_Cached_Allocator (void);
  // clear things up.

  void* malloc (size_t);
  // get a chunk of memory from free store.

  void free (void *);
  // return a chunk of memory back to free store.

private:
  T *pool_;
  // remember how we allocate the memory in the first place so
  // we can clear things up later.

  ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<T>, ACE_LOCK> free_list_;
  // Maintain a cached memory free list.
};

template <class MALLOC>
class ACE_Allocator_Adapter : public ACE_Allocator
{
  // = TITLE
  //    This class is an Adapter that allows the <ACE_Allocator> to
  //    use the <Malloc> class below.
public:
  // Trait.
  typedef MALLOC ALLOCATOR;

#if defined (ACE_HAS_TEMPLATE_TYPEDEFS)
  // The following code will break C++ compilers that don't support
  // template typedefs correctly.
  typedef const ACE_TYPENAME MALLOC::MEMORY_POOL_OPTIONS *MEMORY_POOL_OPTIONS;
#else
  typedef const void *MEMORY_POOL_OPTIONS;
#endif /* ACE_HAS_TEMPLATE_TYPEDEFS */

  // = Initialization.
  ACE_Allocator_Adapter (LPCTSTR pool_name = 0);

  ACE_Allocator_Adapter (LPCTSTR pool_name,
                         LPCTSTR lock_name,
                         MEMORY_POOL_OPTIONS options = 0)
      : allocator_ (pool_name, lock_name, options)
    {
      ACE_TRACE ("ACE_Allocator_Adapter<MALLOC>::ACE_Allocator_Adapter");
    }
  // Constructor (this has to be inline to avoid bugs with some C++ compilers.

  virtual ~ACE_Allocator_Adapter (void);
  // Destructor.

  // = Memory Management

  virtual void *malloc (size_t nbytes);
  // Allocate <nbytes>, but don't give them any initial value.

  virtual void *calloc (size_t nbytes, char initial_value = '\0');
  // Allocate <nbytes>, giving them all an <initial_value>.

  virtual void free (void *ptr);
  // Free <ptr> (must have been allocated by <ACE_Allocator::malloc>).

  virtual int remove (void);
  // Remove any resources associated with this memory manager.

  // = Map manager like functions

  virtual int bind (const char *name, void *pointer, int duplicates = 0);
  // Associate <name> with <pointer>.  If <duplicates> == 0 then do
  // not allow duplicate <name>/<pointer> associations, else if
  // <duplicates> != 0 then allow duplicate <name>/<pointer>
  // assocations.  Returns 0 if successfully binds (1) a previously
  // unbound <name> or (2) <duplicates> != 0, returns 1 if trying to
  // bind a previously bound <name> and <duplicates> == 0, else
  // returns -1 if a resource failure occurs.

  virtual int trybind (const char *name, void *&pointer);
  // Associate <name> with <pointer>.  Does not allow duplicate
  // <name>/<pointer> associations.  Returns 0 if successfully binds
  // (1) a previously unbound <name>, 1 if trying to bind a previously
  // bound <name>, or returns -1 if a resource failure occurs.  When
  // this call returns <pointer>'s value will always reference the
  // void * that <name> is associated with.  Thus, if the caller needs
  // to use <pointer> (e.g., to free it) a copy must be maintained by
  // the caller.

  virtual int find (const char *name, void *&pointer);
  // Locate <name> and pass out parameter via pointer.  If found,
  // return 0, Returns -1 if failure occurs.

  virtual int find (const char *name);
  // returns 0 if the name is in the mapping. -1, otherwise.

  virtual int unbind (const char *name);
  // Unbind (remove) the name from the map.  Don't return the pointer
  // to the caller

  virtual int unbind (const char *name, void *&pointer);
  // Break any association of name.  Returns the value of pointer in
  // case the caller needs to deallocate memory.

  // = Protection and "sync" (i.e., flushing data to backing store).

  virtual int sync (ssize_t len = -1, int flags = MS_SYNC);
  // Sync <len> bytes of the memory region to the backing store
  // starting at <this->base_addr_>.  If <len> == -1 then sync the
  // whole region.

  virtual int sync (void *addr, size_t len, int flags = MS_SYNC);
  // Sync <len> bytes of the memory region to the backing store
  // starting at <addr_>.

  virtual int protect (ssize_t len = -1, int prot = PROT_RDWR);
  // Change the protection of the pages of the mapped region to <prot>
  // starting at <this->base_addr_> up to <len> bytes.  If <len> == -1
  // then change protection of all pages in the mapped region.

  virtual int protect (void *addr, size_t len, int prot = PROT_RDWR);
  // Change the protection of the pages of the mapped region to <prot>
  // starting at <addr> up to <len> bytes.

  ALLOCATOR &alloc (void);
  // Returns the underlying allocator.

#if defined (ACE_HAS_MALLOC_STATS)
  virtual void print_stats (void) const;
  // Dump statistics of how malloc is behaving.
#endif /* ACE_HAS_MALLOC_STATS */

  virtual void dump (void) const;
  // Dump the state of the object.

private:
  ALLOCATOR allocator_;
  // ALLOCATOR instance, which is owned by the adapter.
};

template <size_t POOL_SIZE>
class ACE_Static_Allocator : public ACE_Static_Allocator_Base
{
  // = TITLE
  //     Defines a class that provided a highly optimized memory
  //     management scheme for allocating memory statically.
  //
  // = DESCRIPTION
  //     This class allocates a fixed-size <POOL_SIZE> of memory and
  //     uses the <ACE_Static_Allocator_Base> class implementations of
  //     <malloc> and <calloc> to optimize memory allocation from this
  //     pool.
public:
  ACE_Static_Allocator (void)
    : ACE_Static_Allocator_Base (pool_, POOL_SIZE)
  {
    // This function <{must}> be inlined!!!
  }

private:
  char pool_[POOL_SIZE];
  // Pool contents.
};

// Forward declaration.
template <ACE_MEM_POOL_1, class ACE_LOCK>
class ACE_Malloc_Iterator;

template <ACE_MEM_POOL_1, class ACE_LOCK>
class ACE_Malloc
{
  // = TITLE
  //     Define a C++ class that uses parameterized types to provide
  //     an extensible mechanism for encapsulating various of dynamic
  //     memory management strategies.
  //
  // = DESCRIPTION
  //     This class can be configured flexibly with different
  //     MEMORY_POOL strategies and different types of ACE_LOCK
  //     strategies.
public:
  friend class ACE_Malloc_Iterator<ACE_MEM_POOL_2, ACE_LOCK>;
  typedef ACE_MEM_POOL MEMORY_POOL;
  typedef ACE_MEM_POOL_OPTIONS MEMORY_POOL_OPTIONS;

  // = Initialization and termination methods.
  ACE_Malloc (LPCTSTR pool_name = 0);
  // Initialize ACE_Malloc.  This constructor passes <pool_name> to
  // initialize the memory pool, and uses <ACE::basename> to
  // automatically extract out the name used for the underlying lock
  // name (if necessary).

  ACE_Malloc (LPCTSTR pool_name,
              LPCTSTR lock_name,
              const ACE_MEM_POOL_OPTIONS *options = 0);
  // Initialize ACE_Malloc.  This constructor passes <pool_name> to
  // initialize the memory pool, and uses <lock_name> to automatically
  // extract out the name used for the underlying lock name (if
  // necessary).  In addition, <options> is passed through to
  // initialize the underlying memory pool.

#if !defined (ACE_HAS_TEMPLATE_TYPEDEFS)
  ACE_Malloc (LPCTSTR pool_name,
              LPCTSTR lock_name,
              const void *options = 0);
  // This is necessary to work around template bugs with certain C++
  // compilers.
#endif /* ACE_HAS_TEMPLATE_TYPEDEFS */

  ~ACE_Malloc (void);
  // Destructor

  int remove (void);
  // Releases resources allocated by ACE_Malloc.

  // = Memory management

  void *malloc (size_t nbytes);
  // Allocate <nbytes>, but don't give them any initial value.

  void *calloc (size_t nbytes, char initial_value = '\0');
  // Allocate <nbytes>, giving them <initial_value>.

  void  free (void *ptr);
  // Deallocate memory pointed to by <ptr>, which must have been
  // allocated previously by <this->malloc>.

  MEMORY_POOL &memory_pool (void);
  // Returns a reference to the underlying memory pool.

  // = Map manager like functions

  int bind (const char *name, void *pointer, int duplicates = 0);
  // Associate <name> with <pointer>.  If <duplicates> == 0 then do
  // not allow duplicate <name>/<pointer> associations, else if
  // <duplicates> != 0 then allow duplicate <name>/<pointer>
  // assocations.  Returns 0 if successfully binds (1) a previously
  // unbound <name> or (2) <duplicates> != 0, returns 1 if trying to
  // bind a previously bound <name> and <duplicates> == 0, else
  // returns -1 if a resource failure occurs.

  int trybind (const char *name, void *&pointer);
  // Associate <name> with <pointer>.  Does not allow duplicate
  // <name>/<pointer> associations.  Returns 0 if successfully binds
  // (1) a previously unbound <name>, 1 if trying to bind a previously
  // bound <name>, or returns -1 if a resource failure occurs.  When
  // this call returns <pointer>'s value will always reference the
  // void * that <name> is associated with.  Thus, if the caller needs
  // to use <pointer> (e.g., to free it) a copy must be maintained by
  // the caller.

  int find (const char *name, void *&pointer);
  // Locate <name> and pass out parameter via <pointer>.  If found,
  // return 0, returns -1 if failure occurs.

  int find (const char *name);
  // Returns 0 if <name> is in the mapping. -1, otherwise.

  int unbind (const char *name);
  // Unbind (remove) the name from the map.  Don't return the pointer
  // to the caller.  If you want to remove all occurrences of <name>
  // you'll need to call this method multiple times until it fails...

  int unbind (const char *name, void *&pointer);
  // Unbind (remove) one association of <name> to <pointer>.  Returns
  // the value of pointer in case the caller needs to deallocate
  // memory.  If you want to remove all occurrences of <name> you'll
  // need to call this method multiple times until it fails...

  // = Protection and "sync" (i.e., flushing data to backing store).

  int sync (ssize_t len = -1, int flags = MS_SYNC);
  // Sync <len> bytes of the memory region to the backing store
  // starting at <this->base_addr_>.  If <len> == -1 then sync the
  // whole region.

  int sync (void *addr, size_t len, int flags = MS_SYNC);
  // Sync <len> bytes of the memory region to the backing store
  // starting at <addr_>.

  int protect (ssize_t len = -1, int prot = PROT_RDWR);
  // Change the protection of the pages of the mapped region to <prot>
  // starting at <this->base_addr_> up to <len> bytes.  If <len> == -1
  // then change protection of all pages in the mapped region.

  int protect (void *addr, size_t len, int prot = PROT_RDWR);
  // Change the protection of the pages of the mapped region to <prot>
  // starting at <addr> up to <len> bytes.

  ssize_t avail_chunks (size_t size) const;
  // Returns a count of the number of available chunks that can hold
  // <size> byte allocations.  Function can be used to determine if you
  // have reached a water mark. This implies a fixed amount of allocated
  // memory.
  //
  // @param size - the chunk size of that you would like a count of
  // @return function returns the number of chunks of the given size
  //          that would fit in the currently allocated memory.

#if defined (ACE_HAS_MALLOC_STATS)
  void print_stats (void) const;
  // Dump statistics of how malloc is behaving.
#endif /* ACE_HAS_MALLOC_STATS */

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  int open (void);
  // Initialize the Malloc pool.

  int shared_bind (const char *name, void *pointer);
  // Associate <name> with <pointer>.  Assumes that locks are held by
  // callers.

  ACE_Name_Node *shared_find (const char *name);
  // Try to locate <name>.  If found, return the associated
  // <ACE_Name_Node>, else returns 0 if can't find the <name>.
  // Assumes that locks are held by callers.

  void *shared_malloc (size_t nbytes);
  // Allocate memory.  Assumes that locks are held by callers.

  void shared_free (void *ptr);
  // Deallocate memory.  Assumes that locks are held by callers.

  ACE_Control_Block *cb_ptr_;
  // Pointer to the control block (stored in memory controlled by
  // MEMORY_POOL).

  MEMORY_POOL memory_pool_;
  // Pool of memory used by ACE_Malloc

  ACE_LOCK lock_;
  // Local that ensures mutual exclusion.
};

template <ACE_MEM_POOL_1, class ACE_LOCK>
class ACE_Malloc_Iterator
{
  // = TITLE
  //     Iterator for names stored in Malloc'd memory.
  //
  // = DESCRIPTION
  //     Does not allows deletions while iteration is occurring.
public:
  // = Initialization method.
  ACE_Malloc_Iterator (ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK> &malloc,
                       const char *name = 0);
  // if <name> = 0 it will iterate through everything else only
  // through those entries whose <name> match.

  ~ACE_Malloc_Iterator (void);

  // = Iteration methods.

  int next (void *&next_entry);
  // Pass back the next <entry> in the set that hasn't yet been
  // visited.  Returns 0 when all items have been seen, else 1.

  int done (void) const;
  // Returns 1 when all items have been seen, else 0.

  int next (void *&next_entry, char *&name);
  // Pass back the next <entry> (and the <name> associated with it) in
  // the set that hasn't yet been visited.  Returns 0 when all items
  // have been seen, else 1.

  int advance (void);
  // Move forward by one element in the set.  Returns 0 when all the
  // items in the set have been seen, else 1.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK> &malloc_;
  // Malloc we are iterating over.

  ACE_Name_Node *curr_;
  // Keeps track of how far we've advanced...

  ACE_Read_Guard<ACE_LOCK> guard_;
  // Lock Malloc for the lifetime of the iterator.

  const char *name_;
  // Name that we are searching for.
};

#if defined (__ACE_INLINE__)
#include "ace/Malloc_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Malloc_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Malloc_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_MALLOC_H */
