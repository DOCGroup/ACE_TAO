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

#include "ace/Memory/PI_Malloc.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// MEM_SAP requries position independent pointers to work
#if (ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1)

#include "ace/Threads/Process_Mutex.h"

class ACE_MEM_SAP;
class ACE_Reactive_MEM_IO;
class ACE_MT_MEM_IO;
class ACE_MEM_IO;

// Internal data structure
// MEM_SAP uses to queue up
// data.
class ACE_Export ACE_MEM_SAP_Node
{
public:
//    friend class ACE_MEM_SAP;
//    friend class ACE_Reactive_MEM_IO;
//    friend class ACE_MT_MEM_IO;
//    friend class ACE_MEM_IO;

  typedef ACE_Based_Pointer<ACE_MEM_SAP_Node> ACE_MEM_SAP_NODE_PTR;

  // Initialize the node with its capacity.
  ACE_MEM_SAP_Node (size_t cap);

  // Get the size of the data we hold.
  size_t size (void) const;

  // Get the capacity of this block of data.
  size_t capacity (void) const;

  // Get the pointer to the block of data we hold.
  void *data (void);

  // The maximum size of this memory block.
  size_t capacity_;

  // The actualy size used.
  size_t size_;

  ACE_MEM_SAP_NODE_PTR next_;
};

/**
 * @Class ACE_MEM_SAP
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
  virtual ~ACE_MEM_SAP (void);

  /**
   * Initialize the MEM_SAP object.
   */
  virtual int init (ACE_HANDLE handle,
                    const ACE_TCHAR *name,
                    MALLOC_OPTIONS *options) = 0;

  /**
   * Finalizing the MEM_SAP object.  This method doesn't invoke
   * the <remove> method.
   */
  virtual int fini ();

  /**
   * Fetch location of next available data into <recv_buffer_>.
   * As this operation read the address of the data off the socket
   * using ACE::recv, <timeout> only applies to ACE::recv.
   */
  virtual int recv_buf (ACE_MEM_SAP_Node *&buf,
                        int flags,
                        const ACE_Time_Value *timeout) = 0;

  /**
   * Wait to to <timeout> amount of time to send <buf>.  If <send>
   * times out a -1 is returned with <errno == ETIME>.  If it succeeds
   * the number of bytes sent is returned.  */
  virtual int send_buf (ACE_MEM_SAP_Node *buf,
                        int flags,
                        const ACE_Time_Value *timeout) = 0;

  /// request a buffer of size <size>.  Return 0 if the <shm_malloc_> is
  /// not initialized.
  ACE_MEM_SAP_Node *acquire_buffer (const ssize_t size);

  /// release a buffer pointed by <buf>.  Return -1 if the <shm_malloc_>
  /// is not initialized.
  int release_buffer (ACE_MEM_SAP_Node *buf);

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
                         MALLOC_OPTIONS *options);

  /// Close down the share memory pool.  Clean up the
  /// mmap file if we are the last one using it.
  int close_shm_malloc (void);

  ACE_HANDLE handle_;

  /// Data exchange channel.
  MALLOC_TYPE *shm_malloc_;

  /// Constructor.  Prevent this class from being instantiated.
  ACE_MEM_SAP (void);
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Memory/MEM_SAP.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#endif /* ACE_HAS_POSITION_INDEPENDENT_POINTERS == 1 */

#include "ace/post.h"
#endif /* ACE_SOCK_IO_H */
