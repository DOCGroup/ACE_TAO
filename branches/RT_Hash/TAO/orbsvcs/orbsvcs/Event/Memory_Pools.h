/* -*- C++ -*- */
//=============================================================================
/**
 *  @file Memory_Pools.h
 *
 *  $Id$
 *
 *  @author Tim Harrison (harrison@cs.wustl.edu)
 */
//=============================================================================


#ifndef ACE_MEMORY_POOLS_H
#define ACE_MEMORY_POOLS_H
#include "ace/pre.h"

#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Event_Channel.h"
#include "Dispatching_Modules.h"
#include "ace/Malloc.h"

// ************************************************************

#if defined (ACE_HAS_THREAD_SPECIFIC_STORAGE) || defined (ACE_HAS_TSS_EMULATION) || !defined (ACE_HAS_THREADS)
  typedef ACE_Null_Mutex ACE_MEMORY_POOL_MUTEX;
#else
  // Use the same object for each thread.  Therefore, we have to use
  // real synchronization.
  typedef TAO_SYNCH_MUTEX ACE_MEMORY_POOL_MUTEX;
#endif /* ACE_HAS_THREAD_SPECIFIC_STORAGE || ACE_HAS_TSS_EMULATION || !ACE_HAS_THREADS */

typedef char
        ACE_ES_Dispatch_Request_Chunk[sizeof (ACE_ES_Dispatch_Request)];

typedef ACE_Cached_Allocator<ACE_ES_Dispatch_Request_Chunk, ACE_MEMORY_POOL_MUTEX>
        _ACE_Dispatch_Request_Allocator;

/**
 * @class ACE_ES_Dispatch_Request_Allocator
 *
 * @brief Dispatch Request Allocator
 *
 * This just sets the size of the Event Container memory pool.
 */
class TAO_RTOLDEvent_Export ACE_ES_Dispatch_Request_Allocator : public _ACE_Dispatch_Request_Allocator
{
public:
  ACE_ES_Dispatch_Request_Allocator (void) :
    _ACE_Dispatch_Request_Allocator (ACE_ES_DISPATCH_REQUEST_MEMORY_POOL) {}
};

// ************************************************************

typedef ACE_Malloc<ACE_LOCAL_MEMORY_POOL, TAO_SYNCH_MUTEX> ACE_ES_Event_Allocator;

/**
 * @class ACE_ES_Memory_Pools
 *
 * @brief Event Service Memory Pools.
 *
 * These have to be static in order to be accessed by operator
 * news, right?
 */
class TAO_RTOLDEvent_Export ACE_ES_Memory_Pools
{
public:
  /**
   * This can be called by every thread that will access these memory
   * pools to preallocate the thread specific allocators.  It is not
   * mandatory.
   */
  static int thr_init (void);

  static void *new_Dispatch_Request (void);
  static void delete_Dispatch_Request (void *);

  typedef ACE_Singleton<ACE_ES_Dispatch_Request_Allocator,TAO_SYNCH_MUTEX> Dispatch_Request_;
};

#if defined (__ACE_INLINE__)
#include "Memory_Pools.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_MEMORY_POOLS_H */
