/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    MEM_SAP.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_MEM_SAP_H
#define ACE_MEM_SAP_H
#include "ace/pre.h"

#include "ace/PI_Malloc.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// MEM_SAP requries position independent pointers to work
#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)

#include "ace/Process_Mutex.h"

/**
 * @class ACE_MEM_SAP
 *
 * @brief Defines the methods of shared memory management for
 * shared memory transport.
 */
class ACE_Export ACE_MEM_SAP
{
public:
  // = Initialization and termination methods.

  typedef ACE_Malloc_T<ACE_MMAP_MEMORY_POOL, ACE_Process_Mutex, ACE_PI_Control_Block> MALLOC_TYPE;
  typedef ACE_MMAP_Memory_Pool_Options MALLOC_OPTIONS;

  /// Destructor.
  ~ACE_MEM_SAP (void);

  /// request a buffer of size <size>.  Return 0 if the <shm_malloc_> is
  /// not initialized.
  void *acquire_buffer (const ssize_t size);

  /// release a buffer pointed by <buf>.  Return -1 if the <shm_malloc_>
  /// is not initialized.
  int release_buffer (void *buf);

  /**
   * Set the length of buf (containing information) to <n> bytes.
   * Return the offset of the <buf> relative to the base address.
   * <buf> must be acquired by <get_buffer> method.  Return -1 if the
   * <shm_malloc_> is not initialized.
   */
  off_t set_buf_len (void *buf,
                     size_t n);

  /**
   * Convert the buffer offset <off> to absolute address to <buf>.
   * Return the size of valid information containing in the <buf>,
   * -1 if <shm_malloc_> is not initialized.
   */
  ssize_t get_buf_len (const off_t off, void *&buf);

  /// Remove the shared resouce (mmap file) used by us.
  int remove (void);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  // = Class initializing methods to create/connect to a shared memory pool.

  /**
   * Create a new shm_malloc object.  Return 0 if succeed and -1
   * otherwise.  This method should only be called from an acceptor
   * class that wants to create a new memory pool for inter process
   * communication.
   */
  int create_shm_malloc (const ACE_TCHAR *name,
                         MALLOC_OPTIONS *options = 0);

  /// Close down the share memory pool.  If <remove> != 0, then the
  /// mmap file will also get removed.
  int close_shm_malloc (const int remove = 0);

  /// Data exchange channel.
  MALLOC_TYPE *shm_malloc_;

  /// Constructor.  Prevent this class from being instantiated.
  ACE_MEM_SAP (void);
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/MEM_SAP.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */

#include "ace/post.h"
#endif /* ACE_SOCK_IO_H */
