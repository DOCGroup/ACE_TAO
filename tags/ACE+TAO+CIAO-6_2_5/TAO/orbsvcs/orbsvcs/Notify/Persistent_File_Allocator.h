// -*- C++ -*-

//=============================================================================
/**
 *  @file    Persistent_File_Allocator.h
 *
 *  $Id$
 *
 *  A Persistent_File_Allocator manages a free list and allocates and
 *  deallocates blocks from a Random_File.  There should be only one
 *  Persistent_File_Allocator for each Random_File.
 *
 *  @author Jonathan Pollack <pollack_j@ociweb.com>
 */
//=============================================================================

#ifndef PERSISTENT_FILE_ALLOCATOR_H
#define PERSISTENT_FILE_ALLOCATOR_H
#include /**/ "ace/pre.h"
#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/notify_serv_export.h"
#include "orbsvcs/Notify/Random_File.h"
#include "orbsvcs/Notify/Bit_Vector.h"
#include "ace/Containers_T.h"
#include "ace/Unbounded_Queue.h"
#include "ace/Thread_Manager.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{

/// \brief An interface to allow callbacks on completion of persistent storage
/// requests.
class TAO_Notify_Serv_Export Persistent_Callback
{
public:
  virtual ~Persistent_Callback();
  /// \brief Called by a Persistent_File_Allocator when a write request has
  /// completed.
  virtual void persist_complete() = 0;
};

/**
 * \brief A class to represent a block on disk.
 *
 * Contains the raw data to be written on disk as well as
 * positioning information, synchronization information, and a pointer
 * to a callback.
 */
class TAO_Notify_Serv_Export Persistent_Storage_Block
{
public:
  /// The constructor.  Initializes the callback to NULL.
  Persistent_Storage_Block(
    const size_t block_number,
    const size_t block_size);
  /// The copy constructor.  Makes a deep copy of the passed in PSB.
  Persistent_Storage_Block(const Persistent_Storage_Block& psb);
  /// The destructor.
  ~Persistent_Storage_Block();

  /// Set our block to not have any data at all - a no-op.  This can be
  /// used to implement a checkpoint in the write stream.
  void set_no_write();
  /// Find out whether we have data to be written.
  bool get_no_write();

  /// Set our block to be written as a near-atomic operation.
  void set_sync();
  /// Find out whether this block should be written near-atomically.
  bool get_sync() const;

  /// Find out our physical block number.
  size_t block_number() const;

  /// Return our data to the user.
  unsigned char* data() const;
  /// Set our data pointer, and optionally delete it.
  void reassign_data(unsigned char* newptr, bool delete_old = false);

  /// Return block number and relinquish ownership.
  size_t detach ();

  /// Set our callback.
  void set_callback(Persistent_Callback* callback);
  /// Get our callback.
  Persistent_Callback* get_callback() const;

  /// Set ownership of this PSB.
  void set_allocator_owns(bool allocator_owns = true);
  /// Get ownership status of this PSB.
  bool get_allocator_owns() const;

private:
  /// Our raw data.
  unsigned char* data_;
  /// The block number corresponding to our data.
  size_t block_number_;
  /// Are we a no-op with just a callback?
  bool no_write_;
  /// Write in near-atomic fashion.
  bool sync_;
  /// The size of our block.
  size_t block_size_;
  /// Our optional callback function, to be used in such things as state
  /// transitions.
  Persistent_Callback* callback_;
  /// Does the allocator obtain ownership of our block?
  bool allocator_owns_;
};

/**
 * \brief A class that manages the details of persistent storage.
 *
 * Maintains a free list, write queue, allocations of new
 * blocks, reads, and writes.  This class also manages a thread that performs
 * background updating of a Random_File.
 * @todo this is too much for one class to do.  It should be refactored.
 * @todo we shouldn't arbitrarily use a thread.
 */
class TAO_Notify_Serv_Export Persistent_File_Allocator
{
public:
  /// The constructor.
  Persistent_File_Allocator();
  /// The destructor.
  ~Persistent_File_Allocator();

  bool open (const ACE_TCHAR* filename,
    const size_t block_size = 512);

  /// \brief Wait for pending I/O and terminate our work thread.
  void shutdown();

  /// Allocate a new Persistent_Storage_Block and initialize it to an unused
  /// block of storage.
  Persistent_Storage_Block* allocate();

  /// \brief Allocate a new Persistent_Storage_Block at a given address
  Persistent_Storage_Block* allocate_at(size_t block_number);

  /// \brief Allocate a PSB that is marked to not persist
  Persistent_Storage_Block* allocate_nowrite();

  /// \brief Mark a block as used, removing it from the free list.
  void used(size_t block_number);

  /// \brief Mark a block number as able to be used again.
  void free(size_t block_number);

  /// \brief Access block size.
  size_t block_size() const;

  /// \brief Read data into a PSB.
  ///
  /// Data will come either from the queue of blocks to be written, or
  /// it will be read from the file if there are no queued write requests for
  /// this block.
  bool read(Persistent_Storage_Block* psb);

  /// \brief Write this block to the file,
  ///
  /// Add the Persistent_Storage_Block to our write queue and let the
  /// worker thread handle writing this to the Random_File.
  bool write(Persistent_Storage_Block* psb);

  /// for information (unit test) only.
  ACE_OFF_T file_size () const;

private:
  /// Free a previously assigned block.
  void free_block(const size_t block_number);
  /// Find and allocate a free block.
  bool allocate_block(size_t& block_number);

  /// Used during thread startup to cast us back to ourselves and call the
  /// run() method.
  static ACE_THR_FUNC_RETURN thr_func(void * arg);
  /// Wait for pending I/O to complete and shut our worker thread down safely.
  void shutdown_thread();
  /// The worker's execution thread.
  void run();

private:
  ACE_Thread_Manager thread_manager_;
  Random_File pstore_;
  Bit_Vector free_blocks_;
  ACE_Unbounded_Queue<Persistent_Storage_Block*> block_queue_;
  TAO_SYNCH_MUTEX lock_;
  TAO_SYNCH_MUTEX free_blocks_lock_;
  TAO_SYNCH_MUTEX queue_lock_;
  bool terminate_thread_;
  bool thread_active_;
  ACE_SYNCH_CONDITION wake_up_thread_;
};

} /* namespace TAO_Notify */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* PERSISTENT_FILE_ALLOCATOR_H */
