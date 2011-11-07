// -*- C++ -*-

//=============================================================================
/**
 *  @file    Random_File.h
 *
 *  $Id$
 *
 *  This class implements a a random-access file containing
 *  fixed-size blocks.
 *
 *  @author Jonathan Pollack <pollack_j@ociweb.com>
 */
//=============================================================================

#ifndef RANDOM_FILE_H
#define RANDOM_FILE_H
#include /**/ "ace/pre.h"
#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/notify_serv_export.h"
#include "tao/Versioned_Namespace.h"
#include "ace/FILE.h"
#include "ace/streams.h"

#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_Notify
{

/**
 * \brief A random file class.
 *
 * Derived from ACE_FILE, this class provides access to a
 * file of fixed-size blocks.
 *
 */
class TAO_Notify_Serv_Export Random_File : public ACE_FILE
{
public:
  /// The constructor.
  Random_File();

  /// The destructor, which closes the open file.
  ~Random_File();

  /// Open a file with default permissions.
  bool open(const ACE_TCHAR* filename, size_t block_size = 512);

  /// Accessor for the block size.
  /// Note signed size_t is used to be compatible with
  /// ACE_FILE.
  size_t block_size() const;

  /// Return the current file size, in number of blocks.
  ACE_OFF_T size() const;

  /// Write a block to our file, potentially as an "atomic" write.
  /// If the atomic argument is true, then the operating system's
  /// write-through cache for this file is flushed both before and
  /// after the write.
  /// The flush before ensures that any record pointers in this block
  /// will point to records that actually appear in the file.
  /// The flush after provides the caller with a guarantee that
  /// the data will appear in the file even if the system fails
  /// immediately after this method returns.
  bool write(const size_t block_number, void* buffer, bool atomic = false);

  /// Read a block from our file.
  bool read(const size_t block_number, void* buffer);

private:
  /// Seek to a given block number, used by reads and writes.
  bool seek(const size_t block_number);

  /// Synchronize the file to disk, used to implement atomic.
  bool sync();

private:
  size_t block_size_;
  mutable TAO_SYNCH_MUTEX lock_;
};

} /* namespace TAO_Notify */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* RANDOM_FILE_H */
