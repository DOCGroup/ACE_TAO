/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Malloc_T.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt and Irfan Pyarali
 */
//=============================================================================

#ifndef ACE_MALLOC_T_H
#define ACE_MALLOC_T_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Threads/Synch.h"
#include "ace/Memory/Malloc.h"               /* Need ACE_Control_Block */
#include "ace/Memory/Malloc_Allocator.h"
#include "ace/Utils/Templates/Free_List.h"

/**
 * @class ACE_Cached_Mem_Pool_Node
 *
 * @brief <ACE_Cached_Mem_Pool_Node> keeps unused memory within a free
 * list.
 *
 * The length of a piece of unused memory must be greater than
 * sizeof (void*).  This makes sense because we'll waste even
 * more memory if we keep them in a separate data structure.
 * This class should really be placed within the next class
 * <ACE_Cached_Allocator>.  But this can't be done due to C++
 * compiler portability problems.
 */
template <class T>
class ACE_Cached_Mem_Pool_Node
{
public:
  /// return the address of free memory.
  T *addr (void);

  /// get the next ACE_Cached_Mem_Pool_Node in a list.
  ACE_Cached_Mem_Pool_Node<T> *get_next (void);

  /// set the next ACE_Cached_Mem_Pool_Node.
  void set_next (ACE_Cached_Mem_Pool_Node<T> *ptr);

private:
  /**
   * Since memory is not used when placed in a free list,
   * we can use it to maintain the structure of  free list.
   * I was using union to hide the fact of overlapping memory
   * usage.  However, that cause problem on MSVC.  So, I now turn
   * back to hack this with casting.
   */
  ACE_Cached_Mem_Pool_Node<T> *next_;
};

/**
 * @class ACE_Cached_Allocator
 *
 * @brief Create a cached memory poll with <n_chunks> chunks each with
 * sizeof (TYPE) size.
 *
 * This class enables caching of dynamically allocated,
 * fixed-sized classes.  Notice that the <code>sizeof (TYPE)</code>
 * must be greater than or equal to <code> sizeof (void*) </code> for
 * this to work properly.
 *
 * @sa ACE_Cached_Mem_Pool_Node
 */
template <class T, class ACE_LOCK>
class ACE_Cached_Allocator : public ACE_New_Allocator
{
public:
  /// Create a cached memory poll with <n_chunks> chunks
  /// each with sizeof (TYPE) size.
  ACE_Cached_Allocator (size_t n_chunks);

  /// clear things up.
  ~ACE_Cached_Allocator (void);

  /**
   * Get a chunk of memory from free list cache.  Note that <nbytes> is
   * only checked to make sure that it's <= to sizeof T, and is
   * otherwise ignored since <malloc> always returns a pointer to an
   * item of sizeof (T).
   */
  void *malloc (size_t nbytes = sizeof (T));

  /**
   * Get a chunk of memory from free list cache, giving them
   * <initial_value>.  Note that <nbytes> is only checked to make sure
   * that it's <= to sizeof T, and is otherwise ignored since <malloc>
   * always returns a pointer to an item of sizeof (T).
   */
  virtual void *calloc (size_t nbytes,
                        char initial_value = '\0');

  /// This method is a no-op and just returns 0 since the free list
  /// only works with fixed sized entities.
  virtual void *calloc (size_t n_elem,
                        size_t elem_size,
                        char initial_value = '\0');

  /// Return a chunk of memory back to free list cache.
  void free (void *);

private:
  /// Remember how we allocate the memory in the first place so
  /// we can clear things up later.
  char *pool_;

  /// Maintain a cached memory free list.
  ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<T>, ACE_LOCK> free_list_;
};

/**
 * @class ACE_Allocator_Adapter
 *
 * @brief This class is an Adapter that allows the <ACE_Allocator> to
 * use the <Malloc> class below.
 */
template <class MALLOC>
class ACE_Allocator_Adapter : public ACE_Allocator
{
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
  ACE_Allocator_Adapter (const char *pool_name = 0);

  ACE_Allocator_Adapter (const char *pool_name,
                         const char *lock_name,
                         MEMORY_POOL_OPTIONS options = 0)
      : allocator_ (ACE_TEXT_CHAR_TO_TCHAR (pool_name),
                    ACE_TEXT_CHAR_TO_TCHAR (lock_name),
                    options)
    {
      ACE_TRACE ("ACE_Allocator_Adapter<MALLOC>::ACE_Allocator_Adapter");
    }
  // Constructor (this has to be inline to avoid bugs with some C++ compilers.

#if defined (ACE_HAS_WCHAR)
  ACE_Allocator_Adapter (const wchar_t *pool_name);

  ACE_Allocator_Adapter (const wchar_t *pool_name,
                         const wchar_t *lock_name,
                         MEMORY_POOL_OPTIONS options = 0)
      : allocator_ (ACE_TEXT_WCHAR_TO_TCHAR (pool_name),
                    ACE_TEXT_WCHAR_TO_TCHAR (lock_name),
                    options)
    {
      ACE_TRACE ("ACE_Allocator_Adapter<MALLOC>::ACE_Allocator_Adapter");
    }
  // Constructor (this has to be inline to avoid bugs with some C++ compilers.
#endif /* ACE_HAS_WCHAR */

  /// Destructor.
  virtual ~ACE_Allocator_Adapter (void);

  // = Memory Management

  /// Allocate <nbytes>, but don't give them any initial value.
  virtual void *malloc (size_t nbytes);

  /// Allocate <nbytes>, giving them all an <initial_value>.
  virtual void *calloc (size_t nbytes, char initial_value = '\0');

  /// Allocate <n_elem> each of size <elem_size>, giving them
  /// <initial_value>.
  virtual void *calloc (size_t n_elem,
                        size_t elem_size,
                        char initial_value = '\0');

  /// Free <ptr> (must have been allocated by <ACE_Allocator::malloc>).
  virtual void free (void *ptr);

  /// Remove any resources associated with this memory manager.
  virtual int remove (void);

  // = Map manager like functions

  /**
   * Associate <name> with <pointer>.  If <duplicates> == 0 then do
   * not allow duplicate <name>/<pointer> associations, else if
   * <duplicates> != 0 then allow duplicate <name>/<pointer>
   * assocations.  Returns 0 if successfully binds (1) a previously
   * unbound <name> or (2) <duplicates> != 0, returns 1 if trying to
   * bind a previously bound <name> and <duplicates> == 0, else
   * returns -1 if a resource failure occurs.
   */
  virtual int bind (const char *name, void *pointer, int duplicates = 0);

  /**
   * Associate <name> with <pointer>.  Does not allow duplicate
   * <name>/<pointer> associations.  Returns 0 if successfully binds
   * (1) a previously unbound <name>, 1 if trying to bind a previously
   * bound <name>, or returns -1 if a resource failure occurs.  When
   * this call returns <pointer>'s value will always reference the
   * void * that <name> is associated with.  Thus, if the caller needs
   * to use <pointer> (e.g., to free it) a copy must be maintained by
   * the caller.
   */
  virtual int trybind (const char *name, void *&pointer);

  /// Locate <name> and pass out parameter via pointer.  If found,
  /// return 0, Returns -1 if <name> isn't found.
  virtual int find (const char *name, void *&pointer);

  /// Returns 0 if the name is in the mapping and -1 if not.
  virtual int find (const char *name);

  /// Unbind (remove) the name from the map.  Don't return the pointer
  /// to the caller
  virtual int unbind (const char *name);

  /// Break any association of name.  Returns the value of pointer in
  /// case the caller needs to deallocate memory.
  virtual int unbind (const char *name, void *&pointer);

  // = Protection and "sync" (i.e., flushing data to backing store).

  /**
   * Sync <len> bytes of the memory region to the backing store
   * starting at <this->base_addr_>.  If <len> == -1 then sync the
   * whole region.
   */
  virtual int sync (ssize_t len = -1, int flags = MS_SYNC);

  /// Sync <len> bytes of the memory region to the backing store
  /// starting at <addr_>.
  virtual int sync (void *addr, size_t len, int flags = MS_SYNC);

  /**
   * Change the protection of the pages of the mapped region to <prot>
   * starting at <this->base_addr_> up to <len> bytes.  If <len> == -1
   * then change protection of all pages in the mapped region.
   */
  virtual int protect (ssize_t len = -1, int prot = PROT_RDWR);

  /// Change the protection of the pages of the mapped region to <prot>
  /// starting at <addr> up to <len> bytes.
  virtual int protect (void *addr, size_t len, int prot = PROT_RDWR);

  /// Returns the underlying allocator.
  ALLOCATOR &alloc (void);

#if defined (ACE_HAS_MALLOC_STATS)
  /// Dump statistics of how malloc is behaving.
  virtual void print_stats (void) const;
#endif /* ACE_HAS_MALLOC_STATS */

  /// Dump the state of the object.
  virtual void dump (void) const;

private:
  /// ALLOCATOR instance, which is owned by the adapter.
  ALLOCATOR allocator_;
};

/**
 * @class ACE_Static_Allocator
 *
 * @brief Defines a class that provided a highly optimized memory
 * management scheme for allocating memory statically.
 *
 * This class allocates a fixed-size <POOL_SIZE> of memory and
 * uses the <ACE_Static_Allocator_Base> class implementations of
 * <malloc> and <calloc> to optimize memory allocation from this
 * pool.
 */
template <size_t POOL_SIZE>
class ACE_Static_Allocator : public ACE_Static_Allocator_Base
{
public:
  ACE_Static_Allocator (void)
    : ACE_Static_Allocator_Base (pool_, POOL_SIZE)
  {
    // This function <{must}> be inlined!!!
  }

private:
  /// Pool contents.
  char pool_[POOL_SIZE];
};

// Forward declaration.
template <ACE_MEM_POOL_1, class ACE_LOCK, class ACE_CB>
class ACE_Malloc_LIFO_Iterator_T;

// Ensure backwards compatibility...
#define ACE_Malloc_Iterator ACE_Malloc_LIFO_Iterator

// Forward declaration.
template <ACE_MEM_POOL_1, class ACE_LOCK, class ACE_CB>
class ACE_Malloc_FIFO_Iterator_T;

/**
 * @class ACE_Malloc_T
 *
 * @brief Define a C++ class that uses parameterized types to provide
 * an extensible mechanism for encapsulating various of dynamic
 * memory management strategies.
 *
 * This class can be configured flexibly with different
 * MEMORY_POOL strategies and different types of ACE_LOCK
 * strategies.
 */
template <ACE_MEM_POOL_1, class ACE_LOCK, class ACE_CB>
class ACE_Malloc_T
{
public:
  friend class ACE_Malloc_LIFO_Iterator_T<ACE_MEM_POOL_2, ACE_LOCK, ACE_CB>;
  friend class ACE_Malloc_FIFO_Iterator_T<ACE_MEM_POOL_2, ACE_LOCK, ACE_CB>;
  typedef ACE_MEM_POOL MEMORY_POOL;
  typedef ACE_MEM_POOL_OPTIONS MEMORY_POOL_OPTIONS;
  typedef ACE_TYPENAME ACE_CB::ACE_Name_Node NAME_NODE;
  typedef ACE_TYPENAME ACE_CB::ACE_Malloc_Header MALLOC_HEADER;

  // = Initialization and termination methods.
  /**
   * Initialize ACE_Malloc.  This constructor passes <pool_name> to
   * initialize the memory pool, and uses <ACE::basename> to
   * automatically extract out the name used for the underlying lock
   * name (if necessary).
   */
  ACE_Malloc_T (const ACE_TCHAR *pool_name = 0);

  /**
   * Initialize ACE_Malloc.  This constructor passes <pool_name> to
   * initialize the memory pool, and uses <lock_name> to automatically
   * extract out the name used for the underlying lock name (if
   * necessary).  In addition, <options> is passed through to
   * initialize the underlying memory pool.
   */
  ACE_Malloc_T (const ACE_TCHAR *pool_name,
                const ACE_TCHAR *lock_name,
                const ACE_MEM_POOL_OPTIONS *options = 0);

#if !defined (ACE_HAS_TEMPLATE_TYPEDEFS)
  /// This is necessary to work around template bugs with certain C++
  /// compilers.
  ACE_Malloc_T (const ACE_TCHAR *pool_name,
                const ACE_TCHAR *lock_name,
                const void *options = 0);
#endif /* ACE_HAS_TEMPLATE_TYPEDEFS */

  /// Destructor
  ~ACE_Malloc_T (void);

  /// Get Reference counter.
  int ref_counter (void);

  /// Release ref counter.
  int release (void);

  /// Releases resources allocated by <ACE_Malloc>.
  int remove (void);

  // = Memory management

  /// Allocate <nbytes>, but don't give them any initial value.
  void *malloc (size_t nbytes);

  /// Allocate <nbytes>, giving them <initial_value>.
  void *calloc (size_t nbytes, char initial_value = '\0');

  /// Allocate <n_elem> each of size <elem_size>, giving them
  /// <initial_value>.
  void *calloc (size_t n_elem,
                size_t elem_size,
                char initial_value = '\0');

  /// Deallocate memory pointed to by <ptr>, which must have been
  /// allocated previously by <this->malloc>.
  void  free (void *ptr);

  /// Returns a reference to the underlying memory pool.
  MEMORY_POOL &memory_pool (void);

  // = Map manager like functions

  /**
   * Associate <name> with <pointer>.  If <duplicates> == 0 then do
   * not allow duplicate <name>/<pointer> associations, else if
   * <duplicates> != 0 then allow duplicate <name>/<pointer>
   * assocations.  Returns 0 if successfully binds (1) a previously
   * unbound <name> or (2) <duplicates> != 0, returns 1 if trying to
   * bind a previously bound <name> and <duplicates> == 0, else
   * returns -1 if a resource failure occurs.
   */
  int bind (const char *name, void *pointer, int duplicates = 0);

  /**
   * Associate <name> with <pointer>.  Does not allow duplicate
   * <name>/<pointer> associations.  Returns 0 if successfully binds
   * (1) a previously unbound <name>, 1 if trying to bind a previously
   * bound <name>, or returns -1 if a resource failure occurs.  When
   * this call returns <pointer>'s value will always reference the
   * void * that <name> is associated with.  Thus, if the caller needs
   * to use <pointer> (e.g., to free it) a copy must be maintained by
   * the caller.
   */
  int trybind (const char *name, void *&pointer);

  /// Locate <name> and pass out parameter via <pointer>.  If found,
  /// return 0, returns -1 if failure occurs.
  int find (const char *name, void *&pointer);

  /// Returns 0 if <name> is in the mapping. -1, otherwise.
  int find (const char *name);

  /**
   * Unbind (remove) the name from the map.  Don't return the pointer
   * to the caller.  If you want to remove all occurrences of <name>
   * you'll need to call this method multiple times until it fails...
   */
  int unbind (const char *name);

  /**
   * Unbind (remove) one association of <name> to <pointer>.  Returns
   * the value of pointer in case the caller needs to deallocate
   * memory.  If you want to remove all occurrences of <name> you'll
   * need to call this method multiple times until it fails...
   */
  int unbind (const char *name, void *&pointer);

  // = Protection and "sync" (i.e., flushing data to backing store).

  /**
   * Sync <len> bytes of the memory region to the backing store
   * starting at <this->base_addr_>.  If <len> == -1 then sync the
   * whole region.
   */
  int sync (ssize_t len = -1, int flags = MS_SYNC);

  /// Sync <len> bytes of the memory region to the backing store
  /// starting at <addr_>.
  int sync (void *addr, size_t len, int flags = MS_SYNC);

  /**
   * Change the protection of the pages of the mapped region to <prot>
   * starting at <this->base_addr_> up to <len> bytes.  If <len> == -1
   * then change protection of all pages in the mapped region.
   */
  int protect (ssize_t len = -1, int prot = PROT_RDWR);

  /// Change the protection of the pages of the mapped region to <prot>
  /// starting at <addr> up to <len> bytes.
  int protect (void *addr, size_t len, int prot = PROT_RDWR);

  /**
   * Returns a count of the number of available chunks that can hold
   * <size> byte allocations.  Function can be used to determine if you
   * have reached a water mark. This implies a fixed amount of allocated
   * memory.
   *
   * @param size - the chunk size of that you would like a count of
   * @return function returns the number of chunks of the given size
   *          that would fit in the currently allocated memory.
   */
  ssize_t avail_chunks (size_t size) const;

#if defined (ACE_HAS_MALLOC_STATS)
  /// Dump statistics of how malloc is behaving.
  void print_stats (void) const;
#endif /* ACE_HAS_MALLOC_STATS */

  /// Returns a pointer to the lock used to provide mutual exclusion to
  /// an <ACE_Malloc> allocator.
  ACE_LOCK &mutex (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

  /// Return cb_ptr value.
  void *base_addr (void);

private:
  /// Initialize the Malloc pool.
  int open (void);

  /// Associate <name> with <pointer>.  Assumes that locks are held by
  /// callers.
  int shared_bind (const char *name,
                   void *pointer);

  /**
   * Try to locate <name>.  If found, return the associated
   * <ACE_Name_Node>, else returns 0 if can't find the <name>.
   * Assumes that locks are held by callers.  Remember to cast the
   * return value to ACE_CB::ACE_Name_Node*.
   */
  void *shared_find (const char *name);

  /// Allocate memory.  Assumes that locks are held by callers.
  void *shared_malloc (size_t nbytes);

  /// Deallocate memory.  Assumes that locks are held by callers.
  void shared_free (void *ptr);

  /// Pointer to the control block that is stored in memory controlled
  /// by <MEMORY_POOL>.
  ACE_CB *cb_ptr_;

  /// Pool of memory used by <ACE_Malloc> to manage its freestore.
  MEMORY_POOL memory_pool_;

  /// Lock that ensures mutual exclusion for the <MEMORY_POOL>.
  ACE_LOCK lock_;
};

/**
 * @class ACE_Malloc_LIFO_Iterator_T
 *
 * @brief LIFO iterator for names stored in Malloc'd memory.
 *
 * Does not support deletions while iteration is occurring.
 */
template <ACE_MEM_POOL_1, class ACE_LOCK, class ACE_CB>
class ACE_Malloc_LIFO_Iterator_T
{
public:
  typedef ACE_TYPENAME ACE_CB::ACE_Name_Node NAME_NODE;
  typedef ACE_TYPENAME ACE_CB::ACE_Malloc_Header MALLOC_HEADER;

  // = Initialization method.
  /// if <name> = 0 it will iterate through everything else only
  /// through those entries whose <name> match.
  ACE_Malloc_LIFO_Iterator_T (ACE_Malloc_T<ACE_MEM_POOL_2, ACE_LOCK, ACE_CB> &malloc,
                              const char *name = 0);

  ~ACE_Malloc_LIFO_Iterator_T (void);

  // = Iteration methods.

  /// Returns 1 when all items have been seen, else 0.
  int done (void) const;

  /// Pass back the next <entry> in the set that hasn't yet been
  /// visited.  Returns 0 when all items have been seen, else 1.
  int next (void *&next_entry);

  /**
   * Pass back the next <entry> (and the <name> associated with it) in
   * the set that hasn't yet been visited.  Returns 0 when all items
   * have been seen, else 1.
   */
  int next (void *&next_entry,
            const char *&name);

  /// Move forward by one element in the set.  Returns 0 when all the
  /// items in the set have been seen, else 1.
  int advance (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Malloc we are iterating over.
  ACE_Malloc_T<ACE_MEM_POOL_2, ACE_LOCK, ACE_CB> &malloc_;

  /// Keeps track of how far we've advanced...
  NAME_NODE *curr_;

  /// Lock Malloc for the lifetime of the iterator.
  ACE_Read_Guard<ACE_LOCK> guard_;

  /// Name that we are searching for.
  const char *name_;
};

/**
 * @class ACE_Malloc_FIFO_Iterator_T
 *
 * @brief FIFO iterator for names stored in Malloc'd memory.
 *
 * Does not support deletions while iteration is occurring.
 */
template <ACE_MEM_POOL_1, class ACE_LOCK, class ACE_CB>
class ACE_Malloc_FIFO_Iterator_T
{
public:
  typedef ACE_TYPENAME ACE_CB::ACE_Name_Node NAME_NODE;
  typedef ACE_TYPENAME ACE_CB::ACE_Malloc_Header MALLOC_HEADER;

  // = Initialization method.
  /// if <name> = 0 it will iterate through everything else only
  /// through those entries whose <name> match.
  ACE_Malloc_FIFO_Iterator_T (ACE_Malloc_T<ACE_MEM_POOL_2, ACE_LOCK, ACE_CB> &malloc,
                              const char *name = 0);

  ~ACE_Malloc_FIFO_Iterator_T (void);

  // = Iteration methods.

  /// Returns 1 when all items have been seen, else 0.
  int done (void) const;

  /// Pass back the next <entry> in the set that hasn't yet been
  /// visited.  Returns 0 when all items have been seen, else 1.
  int next (void *&next_entry);

  /**
   * Pass back the next <entry> (and the <name> associated with it) in
   * the set that hasn't yet been visited.  Returns 0 when all items
   * have been seen, else 1.
   */
  int next (void *&next_entry,
            const char *&name);

  /// Move forward by one element in the set.  Returns 0 when all the
  /// items in the set have been seen, else 1.
  int advance (void);

  /// Go to the starting element that was inserted first. Returns 0
  /// when there is no item in the set, else 1.
  int start (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Malloc we are iterating over.
  ACE_Malloc_T<ACE_MEM_POOL_2, ACE_LOCK, ACE_CB> &malloc_;

  /// Keeps track of how far we've advanced...
  NAME_NODE *curr_;

  /// Lock Malloc for the lifetime of the iterator.
  ACE_Read_Guard<ACE_LOCK> guard_;

  /// Name that we are searching for.
  const char *name_;
};

template <ACE_MEM_POOL_1, class ACE_LOCK>
class ACE_Malloc : public ACE_Malloc_T<ACE_MEM_POOL_2, ACE_LOCK, ACE_Control_Block>
{
public:
  // = Initialization and termination methods.
  /**
   * Initialize ACE_Malloc.  This constructor passes <pool_name> to
   * initialize the memory pool, and uses <ACE::basename> to
   * automatically extract out the name used for the underlying lock
   * name (if necessary).
   */
  ACE_Malloc (const ACE_TCHAR *pool_name = 0);

  /**
   * Initialize ACE_Malloc.  This constructor passes <pool_name> to
   * initialize the memory pool, and uses <lock_name> to automatically
   * extract out the name used for the underlying lock name (if
   * necessary).  In addition, <options> is passed through to
   * initialize the underlying memory pool.
   */
  ACE_Malloc (const ACE_TCHAR *pool_name,
              const ACE_TCHAR *lock_name,
              const ACE_MEM_POOL_OPTIONS *options = 0);

#if !defined (ACE_HAS_TEMPLATE_TYPEDEFS)
  /// This is necessary to work around template bugs with certain C++
  /// compilers.
  ACE_Malloc (const ACE_TCHAR *pool_name,
              const ACE_TCHAR *lock_name,
              const void *options = 0);
#endif /* ACE_HAS_TEMPLATE_TYPEDEFS */
};

template <ACE_MEM_POOL_1, class ACE_LOCK>
class ACE_Malloc_LIFO_Iterator : public ACE_Malloc_LIFO_Iterator_T<ACE_MEM_POOL_2, ACE_LOCK, ACE_Control_Block>
{
public:
  // = Initialization method.
  /// if <name> = 0 it will iterate through everything else only
  /// through those entries whose <name> match.
  ACE_Malloc_LIFO_Iterator (ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK> &malloc,
                            const char *name = 0);
};

template <ACE_MEM_POOL_1, class ACE_LOCK>
class ACE_Malloc_FIFO_Iterator : public ACE_Malloc_FIFO_Iterator_T<ACE_MEM_POOL_2, ACE_LOCK, ACE_Control_Block>
{
public:
  // = Initialization method.
  /// if <name> = 0 it will iterate through everything else only
  /// through those entries whose <name> match.
  ACE_Malloc_FIFO_Iterator (ACE_Malloc<ACE_MEM_POOL_2, ACE_LOCK> &malloc,
                            const char *name = 0);
};

#if defined (__ACE_INLINE__)
#include "ace/Memory/Malloc_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Memory/Malloc_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Malloc_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* ACE_MALLOC_H */
