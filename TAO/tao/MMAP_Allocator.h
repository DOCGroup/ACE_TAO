// -*- C++ -*-

//=============================================================================
/**
 *  @file    MMAP_Allocator.h
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_MMAP_ALLOCATOR_H
#define TAO_MMAP_ALLOCATOR_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if TAO_HAS_SENDFILE == 1

#include "ace/Malloc_T.h"
#include "ace/MMAP_Memory_Pool.h"
#include "ace/Thread_Mutex.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// An ACE_Lite_MMAP_Memory_Pool is used since sync()ing is not
// needed.  All memory allocated by the TAO_MMAP_Allocator is meant
// for strictly transient data, not persistent.
typedef ACE_Allocator_Adapter <ACE_Malloc<ACE_LITE_MMAP_MEMORY_POOL, TAO_SYNCH_MUTEX> > TAO_MMAP_Allocator_Base;

/**
 * @class TAO_MMAP_Allocator
 *
 * @brief mmap-based allocator.
 *
 * This class utilizes and
 */
class TAO_MMAP_Allocator
  : public TAO_MMAP_Allocator_Base
{
public:
  /// Constructor
  TAO_MMAP_Allocator ();

  /// Destructor.
  virtual ~TAO_MMAP_Allocator ();

  /// Return backing store handle.
  ACE_HANDLE handle ();

  /// Return offset in backing store file for memory address @a p. If @a p
  /// is not coming from this allocator -1 is returned.
  off_t offset (void * p);

private:
  TAO_MMAP_Allocator (TAO_MMAP_Allocator const &) = delete;
  void operator= (TAO_MMAP_Allocator const &) = delete;
  TAO_MMAP_Allocator (TAO_MMAP_Allocator &&) = delete;
  void operator= (TAO_MMAP_Allocator &&) = delete;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_SENDFILE==1 */

#include /**/ "ace/post.h"

#endif  /* TAO_MMAP_ALLOCATOR_H */
