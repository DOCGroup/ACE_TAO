/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    MEM_SAP.h
//
// = AUTHOR
//    Nanbor Wang
//
// ============================================================================

#ifndef ACE_MEM_SAP_H
#define ACE_MEM_SAP_H

#include "ace/Malloc.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_MEM_SAP
{
  // = TITLE
  //     Defines the methods of shared memory management for
  //     shared memory transport.
public:
  // = Initialization and termination methods.

#if defined (ACE_HAS_POSITION_INDEPENDENT_MALLOC)
  typedef ACE_Malloc_T<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex, ACE_PI_Control_Block> MALLOC;
#else
  typedef ACE_Malloc_T<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex, ACE_Control_Block> MALLOC;
#endif /* ACE_HAS_POSITION_INDEPENDENT_MALLOC */
  typedef ACE_MMAP_Memory_Pool_Options MALLOC_OPTIONS;
  // I'll just hardcode this for mmap for now.

  ~ACE_MEM_SAP (void);
  // Destructor.

  void * acquire_buffer (const ssize_t size);
  // request a buffer of size <size>.  Return 0 if the <shm_malloc_> is
  // not initialized.

  int release_buffer (void *buf);
  // release a buffer pointed by <buf>.  Return -1 if the <shm_malloc_>
  // is not initialized.

  off_t set_buf_len (void *buf,
                     size_t n);
  // Set the length of buf (containing information) to <n> bytes.
  // Return the offset of the <buf> relative to the base address.
  // <buf> must be acquired by <get_buffer> method.  Return -1 if the
  // <shm_malloc_> is not initialized.

  ssize_t get_buf_len (const off_t off, void *&buf);
  // Convert the buffer offset <off> to absolute address to <buf>.
  // Return the size of valid information containing in the <buf>,
  // -1 if <shm_malloc_> is not initialized.

  int remove (void);
  // Remove the shared resouce (mmap file) used by us.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:

  // = Class initializing methods to create/connect to a shared memory pool.

  int create_shm_malloc (const char *name,
                         MALLOC_OPTIONS *options = 0);
  // Create a new shm_malloc object.  Return 0 if succeed and -1 otherwise.
  // This method should only be called from an acceptor class that wants to
  // create a new memory pool for inter process communication.

  int close_shm_malloc (const int remove = 0);
  // Close down the share memory pool.  If <remove> != 0, then the mmap file
  // will also get removed.

  MALLOC *shm_malloc_;
  // Data exchange channel.

  ACE_MEM_SAP (void);
  // Constructor.  Prevent this class from being instantiated.
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/MEM_SAP.i"
#endif

#endif /* ACE_SOCK_IO_H */
