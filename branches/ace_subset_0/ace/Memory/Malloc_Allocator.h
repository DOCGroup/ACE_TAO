/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   Malloc_Allocator.h
 *
 *  $Id$
 * 
 *  @author Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================

#ifndef MALLOC_ALLOCATOR_H
#define MALLOC_ALLOCATOR_H
#include "ace/pre.h"

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Memory/Malloc_Base.h"

#ifdef ACE_SUBSET_0
#include "ace/Log_Msg.h"
#endif

#if defined (ACE_HAS_MALLOC_STATS)
#include "ace/Threads/Synch_T.h"
#if defined (ACE_HAS_THREADS)
#define ACE_PROCESS_MUTEX ACE_Process_Mutex
#else
#include "ace/IPC/SV_Semaphore_Simple.h"
#define ACE_PROCESS_MUTEX ACE_SV_Semaphore_Simple
#endif /* ACE_HAS_THREADS */

typedef ACE_Atomic_Op<ACE_PROCESS_MUTEX, int> ACE_INT;

/// This keeps stats on the usage of the memory manager.
struct ACE_Export ACE_Malloc_Stats
{
  ACE_Malloc_Stats (void);
  void dump (void) const;

  /// Coarse-grained unit of allocation.
  ACE_INT nchunks_;

  /// Fine-grained unit of allocation.
  ACE_INT nblocks_;

  /// Number of blocks in use
  ACE_INT ninuse_;
};
#define ACE_MALLOC_STATS(X) X
#else
#define ACE_MALLOC_STATS(X)
#endif /* ACE_HAS_MALLOC_STATS */

/**
 * @class ACE_New_Allocator
 *
 * @brief Defines a class that provided a simple implementation of
 * memory allocation.
 *
 * This class uses the new/delete operators to allocate and free up
 * memory.  Please note that the only methods that are supported are
 * <malloc>, <calloc>, and <free>. All other methods are no-ops that
 * return -1 and set <errno> to <ENOTSUP>.  If you require this
 * functionality, please use: ACE_Allocator_Adapter <ACE_Malloc
 * <ACE_LOCAL_MEMORY_POOL, MUTEX> >, which will allow you to use the
 * added functionality of bind/find/etc. while using the new/delete
 * operators.  
 */
class ACE_Export ACE_New_Allocator : public ACE_Allocator
{
public:
  /// These methods are defined.
  virtual void *malloc (size_t nbytes);
  virtual void *calloc (size_t nbytes, char initial_value = '\0');
  virtual void *calloc (size_t n_elem, size_t elem_size, char initial_value = '\0');
  virtual void free (void *ptr);

  /// These methods are no-ops.
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

/**
 * @class ACE_Static_Allocator_Base
 *
 * @brief Defines a class that provided a highly optimized memory
 * management scheme for allocating memory statically.
 *
 * This class manages a fixed-size <POOL_SIZE> of memory.  Every
 * time <malloc>/<calloc> is called, it simply moves an internal
 * index forward and returns a pointer to the requested chunk.
 * All memory is allocated statically (typically via the
 * <ACE_Static_Allocator> template) and <free> is a no-op.  This
 * behavior is useful for use-cases where all the memory
 * allocation needs are known in advance and no deletions ever
 * occur.
 */
class ACE_Export ACE_Static_Allocator_Base : public ACE_Allocator
{
public:
  ACE_Static_Allocator_Base (char *buffer, size_t size);
  virtual void *malloc (size_t nbytes);
  virtual void *calloc (size_t nbytes, char initial_value = '\0');
  virtual void *calloc (size_t n_elem, size_t elem_size, char initial_value = '\0');
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
  /// Don't allow direct instantiations of this class.
  ACE_Static_Allocator_Base (void);

  /// Pointer to the buffer.
  char *buffer_;

  /// Size of the buffer.
  size_t size_;

  /// Pointer to the current offset in the <buffer_>.
  size_t offset_;
};

#if defined (__ACE_INLINE__)
#include "ace/Memory/Malloc_Allocator.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* MALLOC_ALLOCATOR_H */
