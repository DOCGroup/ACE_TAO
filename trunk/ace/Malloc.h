/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Malloc.h 
//
// = AUTHOR
//    Doug Schmidt and Irfan Pyarali
// 
// ============================================================================

#if !defined (ACE_MALLOC_H)
#define ACE_MALLOC_H

#include "ace/ACE.h"

class ACE_Export ACE_Allocator
{
  // = TITLE
  //    Interface for a dynamic memory allocator that uses inheritance
  //    and dynamic binding to provide extensible mechanisms for
  //    allocating and deallocating memory.
public:
  // = Memory Management
  
  static ACE_Allocator *instance (void);
  // Get pointer to a default <ACE_Allocator>.

  static ACE_Allocator *instance (ACE_Allocator *);
  // Set pointer to a process-wide <ACE_Allocator> and return existing
  // pointer.

  static void close_singleton (void);
  // Delete the dynamically allocated Singleton

  virtual ~ACE_Allocator (void);
  // Virtual destructor

  virtual void *malloc (size_t nbytes) = 0;
  // Allocate <nbytes>, but don't give them any initial value.

  virtual void *calloc (size_t nbytes, char initial_value = '\0') = 0;
  // Allocate <nbytes>, giving them <initial_value>.

  virtual void free (void *ptr) = 0;
  // Free <ptr> (must have been allocated by <ACE_Allocator::malloc>).

  virtual int remove (void) = 0;
  // Remove any resources associated with this memory manager.

  // = Map manager like functions

  virtual int bind (const char *name, void *pointer, int duplicates = 0) = 0;
  // Associate <name> with <pointer>.  If <duplicates> == 0 then do
  // not allow duplicate <name>/<pointer> associations, else if
  // <duplicates> != 0 then allow duplicate <name>/<pointer>
  // assocations.  Returns 0 if successfully binds (1) a previously
  // unbound <name> or (2) <duplicates> != 0, returns 1 if trying to
  // bind a previously bound <name> and <duplicates> == 0, else
  // returns -1 if a resource failure occurs.  

  virtual int trybind (const char *name, void *&pointer) = 0;
  // Associate <name> with <pointer>.  Does not allow duplicate
  // <name>/<pointer> associations.  Returns 0 if successfully binds
  // (1) a previously unbound <name>, 1 if trying to bind a previously
  // bound <name>, or returns -1 if a resource failure occurs.  When
  // this call returns <pointer>'s value will always reference the
  // void * that <name> is associated with.  Thus, if the caller needs
  // to use <pointer> (e.g., to free it) a copy must be maintained by
  // the caller.

  virtual int find (const char *name, void *&pointer) = 0;
  // Locate <name> and pass out parameter via pointer.  If found,
  // return 0, Returns -1 if failure occurs.

  virtual int find (const char *name) = 0;
  // returns 0 if the name is in the mapping. -1, otherwise.

  virtual int unbind (const char *name) = 0;
  // Unbind (remove) the name from the map.  Don't return the pointer
  // to the caller

  virtual int unbind (const char *name, void *&pointer) = 0;
  // Break any association of name.  Returns the value of pointer in
  // case the caller needs to deallocate memory.

  // = Protection and "sync" (i.e., flushing memory to persistent
  // backing store).

  virtual int sync (ssize_t len = -1, int flags = MS_SYNC) = 0;
  // Sync <len> bytes of the memory region to the backing store
  // starting at <this->base_addr_>.  If <len> == -1 then sync the
  // whole region.

  virtual int sync (void *addr, size_t len, int flags = MS_SYNC) = 0;
  // Sync <len> bytes of the memory region to the backing store
  // starting at <addr_>.

  virtual int protect (ssize_t len = -1, int prot = PROT_RDWR) = 0;
  // Change the protection of the pages of the mapped region to <prot>
  // starting at <this->base_addr_> up to <len> bytes.  If <len> == -1
  // then change protection of all pages in the mapped region.

  virtual int protect (void *addr, size_t len, int prot = PROT_RDWR) = 0;
  // Change the protection of the pages of the mapped region to <prot>
  // starting at <addr> up to <len> bytes.

#if defined (ACE_HAS_MALLOC_STATS)
  virtual void print_stats (void) const = 0;
  // Dump statistics of how malloc is behaving.
#endif /* ACE_HAS_MALLOC_STATS */ 

  virtual void dump (void) const = 0;
  // Dump the state of the object.
private:

  static ACE_Allocator *allocator_;
  // Pointer to a process-wide <ACE_Allocator> instance.

  static int delete_allocator_;
  // Must delete the <allocator_> if non-0.
};

#if defined (ACE_HAS_MALLOC_STATS)
#include "ace/Synch_T.h"
#if defined (ACE_HAS_THREADS)
#define ACE_PROCESS_MUTEX ACE_Process_Mutex
#else
#include "ace/SV_Semaphore_Simple.h"
#define ACE_PROCESS_MUTEX ACE_SV_Semaphore_Simple
#endif /* ACE_HAS_THREADS */

typedef ACE_Atomic_Op<ACE_PROCESS_MUTEX, int> ACE_INT;

struct ACE_Export ACE_Malloc_Stats
// TITLE
//    This keeps stats on the usage of the memory manager.
{
  ACE_Malloc_Stats (void);
  void dump (void) const;

  ACE_INT nchunks_; 
  // Coarse-grained unit of allocation. 

  ACE_INT nblocks_; 
  // Fine-grained unit of allocation. 

  ACE_INT ninuse_;  
  // Number of blocks in use 
};
#define AMS(X) X
#else
#define AMS(X) 
#endif /* ACE_HAS_MALLOC_STATS */

// ACE_MALLOC_ALIGN allows you to insure that allocated regions are at least
// <ACE_MALLOC_ALIGN> bytes long.  It is especially useful when you want
// areas to be at least a page long, or 32K long, or something like that.
// It doesn't guarantee alignment to an address multiple, like 8-byte
// data alignment, etc.  The allocated area's padding to your selected size
// is done with an added array of long[] and your compiler will decide how
// to align things in memory.
//
// The default ACE_MALLOC_ALIGN is 'long', which will probably add a long
// of padding - it doesn't have any real affect.  If you want to use this
// feature, define ACE_MALLOC_ALIGN in your config.h file and use a signed
// integer number of bytes you want.  For example:
//    #define ACE_MALLOC_ALIGN ((int)4096)

#if !defined (ACE_MALLOC_ALIGN)
#define ACE_MALLOC_ALIGN ((int)(sizeof (long)))
#endif /* ACE_MALLOC_ALIGN */


union ACE_Export ACE_Malloc_Header
// TITLE
//    This is a block header.
{
  struct ACE_Malloc_Control_Block
  {
    ACE_Malloc_Header *next_block_;
    // Points to next block if on free list.
    size_t size_; 
    // Size of this block.
  } s_;

  long align_[ACE_MALLOC_ALIGN/sizeof (long)];
  // Force alignment.
};

class ACE_Export ACE_Name_Node 
{
  // = TITLE
  //    This is stored as a linked list within the Memory_Pool  
  //    to allow "named memory chunks."
public:
  // = Initialization methods.
  ACE_Name_Node (const char *name, void *, ACE_Name_Node *);
  ACE_Name_Node (void);

  char *name_;
  // Name of the Node.

  void *pointer_;
  // Pointer to the contents.

  ACE_Name_Node *next_;
  // Pointer to the next node in the chain.

  void dump (void) const;
  // Dump the state of the object.
};

class ACE_Export ACE_Control_Block
{
  // = TITLE
  //    This information is stored in memory allocated by the MEMORY_POOL.
  //
  // = DESCRIPTION
  //    This class should be local to class ACE_Malloc, but cfront and
  //    G++ don't like nested classes in templates...
public:
  ACE_Name_Node *name_head_;
  // Head of the linked list of Name Nodes.

  ACE_Malloc_Header *freep_;
  // Current head of the freelist. 

  char lock_name_[MAXNAMELEN];
  // Name of lock thats ensures mutual exclusion. 

#if defined (ACE_HAS_MALLOC_STATS)
  // Keep statistics about ACE_Malloc state and performance.
  ACE_Malloc_Stats malloc_stats_;
#define ACE_CONTROL_BLOCK_SIZE ((int)(sizeof (ACE_Name_Node *) \
				      + sizeof (ACE_Malloc_Header *) \
				      + MAXNAMELEN  \
				      + sizeof (ACE_Malloc_Stats)))
#else
#define ACE_CONTROL_BLOCK_SIZE ((int)(sizeof(ACE_Name_Node *) \
				      + sizeof (ACE_Malloc_Header *) \
				      + MAXNAMELEN))
#endif /* ACE_HAS_MALLOC_STATS */

// Notice the casting to int for sizeof() otherwise unsigned int
// arithmetic is used and some awful things may happen.
#define ACE_CONTROL_BLOCK_ALIGN_LONGS ((ACE_CONTROL_BLOCK_SIZE % ACE_MALLOC_ALIGN != 0 \
					? ACE_MALLOC_ALIGN - (ACE_CONTROL_BLOCK_SIZE) \
					: ACE_MALLOC_ALIGN) / int(sizeof(long)))

  long align_[ACE_CONTROL_BLOCK_ALIGN_LONGS < 1 ? 1 : ACE_CONTROL_BLOCK_ALIGN_LONGS];

  ACE_Malloc_Header base_;
  // Dummy node used to anchor the freelist. 

  void dump (void) const;
  // Dump the state of the object.
};

class ACE_Export ACE_New_Allocator : public ACE_Allocator
{
  // = TITLE
  //     Defines a class that provided a simple implementation of
  //     memory allocation.
  //
  // = DESCRIPTION
  //     This class uses the new/delete operators to allocate and free
  //     up memory.  Please note that the only methods that are
  //     supported are malloc and free. All other methods are no-ops.
  //     If you require this functionality, please use:
  //     ACE_Allocator_Adapter <ACE_Malloc <ACE_LOCAL_MEMORY_POOL, MUTEX>>
  //     This will allow you to use the added functionality of
  //     bind/find/etc. while using the new/delete operators.
public:
  virtual void *malloc (size_t nbytes); 
  virtual void *calloc (size_t nbytes, char initial_value = '\0');
  virtual void free (void *ptr);
  virtual int remove (void);
  virtual int bind (const char *name, void *pointer, int duplicates = 0);
  virtual int trybind (const char *name, void *&pointer);
  virtual int find (const char *name, void *&pointer);
  virtual int find (const char *name);
  virtual int unbind (const char *name);
  virtual int unbind (const char *name, void *&pointer);
  virtual int sync (ssize_t len = -1, int flags = MS_SYNC);
  virtual int sync (void *addr, size_t len, int flags = MS_SYNC);
  virtual int protect (ssize_t len = -1, int prot = PROT_RDWR); 
  virtual int protect (void *addr, size_t len, int prot = PROT_RDWR);
#if defined (ACE_HAS_MALLOC_STATS)
  virtual void print_stats (void) const;
#endif /* ACE_HAS_MALLOC_STATS */ 
  virtual void dump (void) const;
};

#if defined (__ACE_INLINE__)
#include "ace/Malloc.i"
#endif /* __ACE_INLINE__ */

// Include the ACE_Malloc templates and ACE_Memory_Pool classes at this point.
#include "ace/Malloc_T.h"
#include "ace/Memory_Pool.h"

#endif /* ACE_MALLOC_H */
