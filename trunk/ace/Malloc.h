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
#include "ace/pre.h"
#define ACE_MALLOC_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Malloc_Base.h"

#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)
#include "ace/Based_Pointer_T.h"
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */

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
#define ACE_MALLOC_STATS(X) X
#else
#define ACE_MALLOC_STATS(X)
#endif /* ACE_HAS_MALLOC_STATS */

#if !defined (ACE_MALLOC_PADDING)
// ACE_MALLOC_PADDING allows you to insure that allocated regions are
// at least <ACE_MALLOC_PADDING> bytes long.  It is especially useful
// when you want areas to be at least a page long, or 32K long, or
// something like that.  It doesn't guarantee alignment to an address
// multiple, like 8-byte data alignment, etc.  The allocated area's
// padding to your selected size is done with an added array of long[]
// and your compiler will decide how to align things in memory.

#define ACE_MALLOC_PADDING 1
#endif /* ACE_MALLOC_PADDING */

#if !defined (ACE_MALLOC_ALIGN)
// Align the malloc header size to a multiple of a double.
#define ACE_MALLOC_ALIGN (sizeof (double))
#endif /* ACE_MALLOC_ALIGN */

// ACE_MALLOC_HEADER_SIZE is the normalized malloc header size.
#define ACE_MALLOC_HEADER_SIZE (ACE_MALLOC_PADDING % ACE_MALLOC_ALIGN == 0 \
                                ? ACE_MALLOC_PADDING \
                                : (((ACE_MALLOC_PADDING / ACE_MALLOC_ALIGN) + 1) \
                                   * ACE_MALLOC_ALIGN))

class ACE_Export ACE_Control_Block
{
  // = TITLE
  //    This information is stored in memory allocated by the <Memory_Pool>.
  //
  // = DESCRIPTION
  //    This class defines the "old" control block class for use in
  //    ACE_Malloc_T.  This control block implementation is
  //    considerable more efficient than the "position independent"
  //    one below (ACE_PI_Control_Block) but if you are going to use
  //    it to construct a ACE_Malloc_T and access the memory from
  //    several different processes, you must "map" the underlying
  //    memory pool to the same address.
public:

  class ACE_Export ACE_Malloc_Header
  {
    // = TITLE
    //    This is the control block header.  It's used by <ACE_Malloc>
    //    to keep track of each chunk of data when it's in the free
    //    list or in use.
  public:
    ACE_Malloc_Header (void);

    ACE_Malloc_Header *next_block_;
    // Points to next block if on free list.

    static void init_ptr (ACE_Malloc_Header **ptr,
                          ACE_Malloc_Header *init,
                          void *base_addr);
    // Initialize a malloc header pointer.

    size_t size_;
    // Size of this header control block.

#if defined (ACE_MALLOC_PADDING_SIZE) && (ACE_MALLOC_PADDING_SIZE == 0)
    // No padding required.
#else
# if !defined (ACE_MALLOC_PADDING_SIZE)
#   define ACE_MALLOC_PADDING_SIZE ((int) (ACE_MALLOC_HEADER_SIZE - \
                                    (sizeof (ACE_Malloc_Header*) + sizeof (size_t)))\
                                    / (int) sizeof (long))
# endif /* !ACE_MALLOC_PADDING_SIZE */
    long padding_[ACE_MALLOC_PADDING_SIZE < 1 ? 1 : ACE_MALLOC_PADDING_SIZE];
#endif /* ACE_MALLOC_PADDING_SIZE && ACE_MALLOC_PADDING_SIZE == 0 */

    void dump (void) const;
    // Dump the state of the object.
  };

  class ACE_Export ACE_Name_Node
  {
    // = TITLE
    //    This class supports "named memory regions" within <ACE_Malloc>.
    //
    // = DESCRIPTION
    //   Internally, the named memory regions are stored as a
    //   doubly-linked list within the <Memory_Pool>.  This makes
    //   it easy to iterate over the items in the list in both FIFO
    //   and LIFO order.
  public:
    // = Initialization methods.
    ACE_Name_Node (const char *name,
                   char *name_ptr,
                   char *pointer,
                   ACE_Name_Node *head);
    // Constructor.

    ACE_Name_Node (const ACE_Name_Node &);
    // Copy constructor.

    ACE_Name_Node (void);
    // Constructor.

    ~ACE_Name_Node (void);
    // Constructor.

    static void init_ptr (ACE_Name_Node **ptr,
                          ACE_Name_Node *init,
                          void *base_addr);
    // Initialize a name node pointer.

    const char *name (void) const;
    // Return a pointer to the name of this node.

    void name (const char *);
    // Assign a name;

    char *name_;
    // Name of the Node.

    char *pointer_;
    // Pointer to the contents.

    ACE_Name_Node *next_;
    // Pointer to the next node in the doubly-linked list.

    ACE_Name_Node *prev_;
    // Pointer to the previous node in the doubly-linked list.

    void dump (void) const;
    // Dump the state of the object.
  };

  static void print_alignment_info (void);
  // Print out a bunch of size info for debugging.

  ACE_Name_Node *name_head_;
  // Head of the linked list of Name Nodes.

  ACE_Malloc_Header *freep_;
  // Current head of the freelist.

  char lock_name_[MAXNAMELEN];
  // Name of lock thats ensures mutual exclusion.

#if defined (ACE_HAS_MALLOC_STATS)
  // Keep statistics about ACE_Malloc state and performance.
  ACE_Malloc_Stats malloc_stats_;
#define ACE_CONTROL_BLOCK_SIZE ((int)(sizeof (ACE_Name_Node*) \
                                      + sizeof (ACE_Malloc_Header*) \
                                      + MAXNAMELEN  \
                                      + sizeof (ACE_Malloc_Stats)))
#else
#define ACE_CONTROL_BLOCK_SIZE ((int)(sizeof (ACE_Name_Node*) \
                                      + sizeof (ACE_Malloc_Header*) \
                                      + MAXNAMELEN))
#endif /* ACE_HAS_MALLOC_STATS */

// Notice the casting to int for <sizeof> otherwise unsigned int
// arithmetic is used and some awful things may happen.
#if defined (ACE_CONTROL_BLOCK_ALIGN_LONGS) && (ACE_CONTROL_BLOCK_ALIGN_LONGS == 0)
  // No padding required in control block.
#else
# if !defined (ACE_CONTROL_BLOCK_ALIGN_LONGS)
#   define ACE_CONTROL_BLOCK_ALIGN_LONGS \
            ((ACE_CONTROL_BLOCK_SIZE % ACE_MALLOC_ALIGN != 0 \
              ? ACE_MALLOC_ALIGN - (ACE_CONTROL_BLOCK_SIZE % ACE_MALLOC_ALIGN) \
              : ACE_MALLOC_ALIGN) / int (sizeof (long)))
# endif /* !ACE_CONTROL_BLOCK_ALIGN_LONGS */
  long align_[ACE_CONTROL_BLOCK_ALIGN_LONGS < 1 ? 1 : ACE_CONTROL_BLOCK_ALIGN_LONGS];
  // Force alignment.
#endif /* ACE_CONTROL_BLOCK_ALIGN_LONGS && ACE_CONTROL_BLOCK_ALIGN_LONGS == 0 */

  ACE_Malloc_Header base_;
  // Dummy node used to anchor the freelist.  This needs to come last...

  void dump (void) const;
  // Dump the state of the object.
};

#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)

// prepare for position independent malloc
class ACE_Export ACE_PI_Control_Block
{
  // = TITLE
  //    This information is stored in memory allocated by the <Memory_Pool>.
  //
  // = DESCRIPTION
  //    This class implements the control block structure that can be
  //    used in a "position indepent" fashion, i.e., you don't need to
  //    "map" the underlying memory pool to the same address in
  //    processes sharing the memory.  The tradoff of this flexibility
  //    is more expensive malloc/free operations.
public:
  class ACE_Malloc_Header;
  class ACE_Name_Node;

  typedef ACE_Based_Pointer<ACE_Malloc_Header> MALLOC_HEADER_PTR;
  typedef ACE_Based_Pointer<ACE_Name_Node> NAME_NODE_PTR;
  typedef ACE_Based_Pointer_Basic<char> CHAR_PTR;

  class ACE_Export ACE_Malloc_Header
  {
    // = TITLE
    //    This is the control block header.  It's used by <ACE_Malloc>
    //    to keep track of each chunk of data when it's in the free
    //    list or in use.
  public:
    ACE_Malloc_Header (void);

    MALLOC_HEADER_PTR next_block_;
    // Points to next block if on free list.

    static void init_ptr (MALLOC_HEADER_PTR *ptr,
                          ACE_Malloc_Header *init,
                          void *base_addr);
    // Initialize a malloc header pointer.

    size_t size_;
    // Size of this header control block.

#if defined (ACE_PI_MALLOC_PADDING_SIZE) && (ACE_PI_MALLOC_PADDING_SIZE == 0)
    // No padding required for PI_Malloc_Header.
#else
# if !defined (ACE_PI_MALLOC_PADDING_SIZE)
#   define ACE_PI_MALLOC_PADDING_SIZE ((int) (ACE_MALLOC_HEADER_SIZE - \
                                       (sizeof (MALLOC_HEADER_PTR) + sizeof (size_t)))\
                                       / (int) sizeof (long))
# endif /* !ACE_PI_MALLOC_PADDING_SIZE */
    long padding_[ACE_PI_MALLOC_PADDING_SIZE < 1 ? 1 : ACE_PI_MALLOC_PADDING_SIZE];
#endif /* ACE_PI_MALLOC_PADDING_SIZE && ACE_PI_MALLOC_PADDING_SIZE == 0 */

    void dump (void) const;
    // Dump the state of the object.

  private:
    ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Malloc_Header &))
  };

  class ACE_Export ACE_Name_Node
  {
    // = TITLE
    //    This class supports "named memory regions" within <ACE_Malloc>.
    //
    // = DESCRIPTION
    //   Internally, the named memory regions are stored as a
    //   doubly-linked list within the <Memory_Pool>.  This makes
    //   it easy to iterate over the items in the list in both FIFO
    //   and LIFO order.
  public:
    // = Initialization methods.
    ACE_Name_Node (const char *name,
                   char *name_ptr,
                   char *pointer,
                   ACE_Name_Node *head);
    // Constructor.

    ACE_Name_Node (const ACE_Name_Node &);
    // Copy constructor.

    ACE_Name_Node (void);
    // Constructor.

    ~ACE_Name_Node (void);
    // Constructor.

    static void init_ptr (NAME_NODE_PTR *ptr,
                          ACE_Name_Node *init,
                          void *base_addr);
    // Initialize a name node pointer.

    const char *name (void) const;
    // Return a pointer to the name of this node.

    void name (const char *);
    // Assign a name;

    CHAR_PTR name_;
    // Name of the Node.

    CHAR_PTR pointer_;
    // Pointer to the contents.

    NAME_NODE_PTR next_;
    // Pointer to the next node in the doubly-linked list.

    NAME_NODE_PTR prev_;
    // Pointer to the previous node in the doubly-linked list.

    void dump (void) const;
    // Dump the state of the object.

  private:
    ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Name_Node &))
  };

  static void print_alignment_info (void);
  // Print out a bunch of size info for debugging.

  NAME_NODE_PTR name_head_;
  // Head of the linked list of Name Nodes.

  MALLOC_HEADER_PTR freep_;
  // Current head of the freelist.

  char lock_name_[MAXNAMELEN];
  // Name of lock thats ensures mutual exclusion.

#if defined (ACE_HAS_MALLOC_STATS)
  // Keep statistics about ACE_Malloc state and performance.
  ACE_Malloc_Stats malloc_stats_;
#define ACE_PI_CONTROL_BLOCK_SIZE ((int)(sizeof (NAME_NODE_PTR) \
                                         + sizeof (MALLOC_HEADER_PTR) \
                                         + MAXNAMELEN  \
                                         + sizeof (ACE_Malloc_Stats)))
#else
#define ACE_PI_CONTROL_BLOCK_SIZE ((int)(sizeof (NAME_NODE_PTR) \
                                         + sizeof (MALLOC_HEADER_PTR) \
                                         + MAXNAMELEN))
#endif /* ACE_HAS_MALLOC_STATS */

#if defined (ACE_PI_CONTROL_BLOCK_ALIGN_LONGS) && (ACE_PI_CONTROL_BLOCK_ALIGN_LONGS == 0)
  // No padding required for PI_Control_Block.
#else
# if !defined (ACE_PI_CONTROL_BLOCK_ALIGN_LONGS)
// Notice the casting to int for <sizeof> otherwise unsigned int
// arithmetic is used and some awful things may happen.
#   define ACE_PI_CONTROL_BLOCK_ALIGN_LONGS \
            ((ACE_PI_CONTROL_BLOCK_SIZE % ACE_MALLOC_ALIGN != 0 \
              ? ACE_MALLOC_ALIGN - (ACE_PI_CONTROL_BLOCK_SIZE % ACE_MALLOC_ALIGN) \
              : ACE_MALLOC_ALIGN) / int (sizeof (long)))
# endif /* !ACE_PI_CONTROL_BLOCK_ALIGN_LONGS */
  long align_[ACE_PI_CONTROL_BLOCK_ALIGN_LONGS < 1 ? 1 : ACE_PI_CONTROL_BLOCK_ALIGN_LONGS];
  // Force alignment.
#endif /* ACE_PI_CONTROL_BLOCK_ALIGN_LONGS && ACE_PI_CONTROL_BLOCK_ALIGN_LONGS == 0 */

  ACE_Malloc_Header base_;
  // Dummy node used to anchor the freelist.  This needs to come last...

  void dump (void) const;
  // Dump the state of the object.

private:
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Control_Block &))
};
#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */

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
  // DO NOT ADD ANY STATE (DATA MEMBERS) TO THIS CLASS!!!!  See the
  // <ACE_Allocator::instance> implementation for explanation.
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

#include "ace/post.h"
#endif /* ACE_MALLOC_H */
