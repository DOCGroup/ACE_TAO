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

#ifndef ACE_MALLOC_H
#define ACE_MALLOC_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Malloc_Base.h"

#if defined (ACE_HAS_POSITION_INDEPENDENT_MALLOC)
#include "ace/Based_Pointer_T.h"
#endif /* ACE_HAS_POSITION_INDEPENDENT_MALLOC */

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

#if !defined (ACE_MALLOC_PADDING)
// ACE_MALLOC_PADDING allows you to insure that allocated regions are
// at least <ACE_MALLOC_PADDING> bytes long.  It is especially useful
// when you want areas to be at least a page long, or 32K long, or
// something like that.  It doesn't guarantee alignment to an address
// multiple, like 8-byte data alignment, etc.  The allocated area's
// padding to your selected size is done with an added array of long[]
// and your compiler will decide how to align things in memory.
//
// If you want to use this feature, define ACE_MALLOC_PADDING in your
// config.h file and use a signed integer number of bytes you want, e.g.:
//
// #define ACE_MALLOC_PADDING ((int) 4096)

#define ACE_MALLOC_PADDING 1
#endif /* ACE_MALLOC_PADDING */

class ACE_Export ACE_Malloc_Header
{
  // = TITLE
  //    This is the control block header.  It's used by <ACE_Malloc>
  //    to keep track of each chunk of data when it's in the free
  //    list or in use.
public:
#if defined (ACE_HAS_POSITION_INDEPENDENT_MALLOC)
# define ACE_POINTER_CAST(PTR) ((PTR).addr ())
  typedef ACE_Based_Pointer<ACE_Malloc_Header> HEADER_PTR;
#else 
# define ACE_POINTER_CAST(PTR) ((PTR))
  typedef ACE_Malloc_Header *HEADER_PTR;
#endif /* ACE_HAS_POSITION_INDEPENDENT_MALLOC */

  class ACE_Malloc_Control_Block
  {
  public:
    HEADER_PTR next_block_;
    // Points to next block if on free list.

    size_t size_;
    // Size of this block.
  } s_;

#if (ACE_MALLOC_PADDING > 1)
#define ACE_MALLOC_PADDING_SIZE ((ACE_MALLOC_PADDING - \
                                  (sizeof (ACE_Malloc_Control_Block)) / sizeof (long)))
  long padding_[ACE_MALLOC_PADDING_SIZE < 1 : ACE_MALLOC_PADDING_SIZE];
#endif /* ACE_MALLOC_PADDING > 0 */
};

class ACE_Export ACE_Name_Node
{
  // = TITLE
  //    This class supports "named memory regions" within <ACE_Malloc>.
  //
  // = DESCRIPTION
  //   Internally, the named memory regions are stored as a linked
  //   list within the <Memory_Pool>.
public:
  // = Initialization methods.
  ACE_Name_Node (const char *name,
                 char *,
                 ACE_Name_Node *);
  // Constructor.

  ACE_Name_Node (const ACE_Name_Node &);
  // Copy constructor.

  ACE_Name_Node (void);
  // Constructor.

  ~ACE_Name_Node (void);
  // Constructor.

  const char *name (void) const;
  // Return a pointer to the name of this node.

  void name (const char *);
  // Assign a name;

  ACE_Based_Pointer_Basic<char> name_;
  // Name of the Node.

  ACE_Based_Pointer_Basic<char> pointer_;
  // Pointer to the contents.

  ACE_Based_Pointer<ACE_Name_Node> next_;
  // Pointer to the next node in the chain.

  void dump (void) const;
  // Dump the state of the object.
};

class ACE_Export ACE_Control_Block
{
  // = TITLE
  //    This information is stored in memory allocated by the <Memory_Pool>.
  //
  // = DESCRIPTION
  //    This class should be local to class ACE_Malloc, but some older
  //    C++ compilers don't like nested classes in templates...
public:
  ACE_Based_Pointer<ACE_Name_Node> name_head_;
  // Head of the linked list of Name Nodes.

  ACE_Malloc_Header::HEADER_PTR freep_;
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
#define ACE_CONTROL_BLOCK_SIZE ((int)(sizeof (ACE_Name_Node *) \
                                      + sizeof (ACE_Malloc_Header *) \
                                      + MAXNAMELEN))
#endif /* ACE_HAS_MALLOC_STATS */

// Notice the casting to int for sizeof() otherwise unsigned int
// arithmetic is used and some awful things may happen.
#define ACE_CONTROL_BLOCK_ALIGN_LONGS ((ACE_CONTROL_BLOCK_SIZE % ACE_MALLOC_PADDING != 0 \
                                        ? ACE_MALLOC_PADDING - (ACE_CONTROL_BLOCK_SIZE) \
                                        : ACE_MALLOC_PADDING) / int (sizeof (long)))

  long align_[ACE_CONTROL_BLOCK_ALIGN_LONGS < 1 ? 1 : ACE_CONTROL_BLOCK_ALIGN_LONGS];
  // Force alignment.

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
  //     ACE_Allocator_Adapter <ACE_Malloc <ACE_LOCAL_MEMORY_POOL,
  //     MUTEX> > This will allow you to use the added functionality
  //     of bind/find/etc. while using the new/delete operators.
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

private:
  // DO NOT ADD ANY STATE (DATA MEMBERS) TO THIS CLASS!!!!
  // See the ACE_Allocator::instance () implementation for explanation.
};

class ACE_Export ACE_Static_Allocator_Base : public ACE_Allocator
{
  // = TITLE
  //     Defines a class that provided a highly optimized memory
  //     management scheme for allocating memory statically.
  //
  // = DESCRIPTION
  //     This class manages a fixed-size <POOL_SIZE> of memory.  Every
  //     time <malloc>/<calloc> is called, it simply moves an internal
  //     index forward and returns a pointer to the requested chunk.
  //     All memory is allocated statically (typically via the
  //     <ACE_Static_Allocator> template) and <free> is a no-op.  This
  //     behavior is useful for use-cases where all the memory
  //     allocation needs are known in advance and no deletions ever
  //     occur.
public:
  ACE_Static_Allocator_Base (char *buffer, size_t size);
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

protected:
  ACE_Static_Allocator_Base (void);
  // Don't allow direct instantiations of this class.

  char *buffer_;
  // Pointer to the buffer.

  size_t size_;
  // Size of the buffer.

  size_t offset_;
  // Pointer to the current offset in the <buffer_>.
};

#if defined (__ACE_INLINE__)
#include "ace/Malloc.i"
#endif /* __ACE_INLINE__ */

// Include the ACE_Malloc templates and ACE_Memory_Pool classes at this point.
#include "ace/Malloc_T.h"
#include "ace/Memory_Pool.h"

#endif /* ACE_MALLOC_H */
