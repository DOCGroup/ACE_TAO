//=============================================================================
/**
 *  @file    Thread_Pool.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali
 */
// ===================================================================

#ifndef TAO_THREAD_POOL_H
#define TAO_THREAD_POOL_H

#include "ace/pre.h"
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "RTCORBA.h"
#include "tao/ORB.h"
#include "ace/Hash_Map_Manager.h"

class TAO_Thread_Lane;

/**
 * @class TAO_Thread_Pool_Threads
 *
 * @brief Class representing a thread running in a thread lane.
 *
 * \nosubgrouping
 *
 **/
class TAO_RTCORBA_Export TAO_Thread_Pool_Threads : public ACE_Task_Base
{
public:

  /// Constructor.
  TAO_Thread_Pool_Threads (TAO_Thread_Lane &lane,
                           ACE_Thread_Manager &tm);

  /// Method executed when a thread is spawned.
  int svc (void);

  /// Accessor to the lane to which this thread belongs to.
  TAO_Thread_Lane &lane (void) const;

private:

  /// Lane to which this thread belongs to.
  TAO_Thread_Lane &lane_;
};

class TAO_Thread_Pool;

/**
 * @class TAO_Thread_Lane
 *
 * @brief Class representing the thread lane inside a thread pool.
 *
 * \nosubgrouping
 *
 **/
class TAO_RTCORBA_Export TAO_Thread_Lane
{
public:

  /// Constructor.
  TAO_Thread_Lane (TAO_Thread_Pool &pool,
                   CORBA::ULong id,
                   CORBA::Short lane_priority,
                   CORBA::ULong static_threads,
                   CORBA::ULong dynamic_threads);

  /// Create the static threads - only called once.
  int create_static_threads (void);

  /// Create <number_of_threads> of dynamic threads.  Can be called
  /// multiple times.
  int create_dynamic_threads (CORBA::ULong number_of_threads);

  /// @name Accessors
  // @{

  CORBA::Short lane_priority (void) const;
  CORBA::ULong static_threads (void) const;
  CORBA::ULong dynamic_threads (void) const;

  CORBA::ULong id (void) const;

  TAO_Thread_Pool &thread_pool (void) const;
  TAO_Thread_Pool_Threads &threads (void);

  // @}

private:

  TAO_Thread_Pool &thread_pool_;
  CORBA::ULong id_;

  CORBA::Short lane_priority_;
  CORBA::ULong static_threads_;
  CORBA::ULong dynamic_threads_;

  TAO_Thread_Pool_Threads threads_;
};

/**
 * @class TAO_Thread_Pool
 *
 * @brief Class representing the thread pool inside a thread pool
 * manager.
 *
 * \nosubgrouping
 *
 **/
class TAO_RTCORBA_Export TAO_Thread_Pool
{
  friend class TAO_Thread_Lane;

public:

  /// Constructor (for pools without lanes).
  TAO_Thread_Pool (CORBA::ULong id,
                   CORBA::ULong stack_size,
                   CORBA::ULong static_threads,
                   CORBA::ULong dynamic_threads,
                   CORBA::Short default_priority,
                   CORBA::Boolean allow_request_buffering,
                   CORBA::ULong max_buffered_requests,
                   CORBA::ULong max_request_buffer_size,
                   CORBA::ORB_ptr orb);

  /// Constructor (for pools with lanes).
  TAO_Thread_Pool (CORBA::ULong id,
                   CORBA::ULong stack_size,
                   const RTCORBA::ThreadpoolLanes &lanes,
                   CORBA::Boolean allow_borrowing,
                   CORBA::Boolean allow_request_buffering,
                   CORBA::ULong max_buffered_requests,
                   CORBA::ULong max_request_buffer_size,
                   CORBA::ORB_ptr orb);

  /// Destructor.
  ~TAO_Thread_Pool (void);

  /// Create the static threads - only called once.
  int create_static_threads (void);

  /// @name Accessors
  // @{

  CORBA::ULong id (void) const;

  CORBA::ULong stack_size (void) const;
  CORBA::Boolean allow_borrowing (void) const;
  CORBA::Boolean allow_request_buffering (void) const;
  CORBA::ULong max_buffered_requests (void) const;
  CORBA::ULong max_request_buffer_size (void) const;

  ACE_Thread_Manager &thread_manager (void);

  CORBA::ORB_ptr orb (void) const;
  TAO_Thread_Lane **lanes (void);
  CORBA::ULong number_of_lanes (void) const;

  // @}

private:

  CORBA::ULong id_;

  CORBA::ULong stack_size_;
  CORBA::Boolean allow_borrowing_;
  CORBA::Boolean allow_request_buffering_;
  CORBA::ULong max_buffered_requests_;
  CORBA::ULong max_request_buffer_size_;

  TAO_Thread_Lane **lanes_;
  CORBA::ULong number_of_lanes_;

  ACE_Thread_Manager &thread_manager_;

  CORBA::ORB_var orb_;
};

/**
 * @class TAO_Thread_Pool_Manager
 *
 * @brief Class for managing thread pools.
 *
 * \nosubgrouping
 *
 **/
class TAO_RTCORBA_Export TAO_Thread_Pool_Manager
{
public:

  /// Constructor.
  TAO_Thread_Pool_Manager (TAO_ORB_Core *orb_core);

  /// Destructor.
  ~TAO_Thread_Pool_Manager (void);

  /// Create the static threads - only called once.
  int
  create_static_threads (void);

  /// Create a threadpool without lanes.
  RTCORBA::ThreadpoolId
  create_threadpool (CORBA::ULong stacksize,
                     CORBA::ULong static_threads,
                     CORBA::ULong dynamic_threads,
                     RTCORBA::Priority default_priority,
                     CORBA::Boolean allow_request_buffering,
                     CORBA::ULong max_buffered_requests,
                     CORBA::ULong max_request_buffer_size,
                     CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Create a threadpool with lanes.
  RTCORBA::ThreadpoolId
  create_threadpool_with_lanes (CORBA::ULong stacksize,
                                const RTCORBA::ThreadpoolLanes & lanes,
                                CORBA::Boolean allow_borrowing,
                                CORBA::Boolean allow_request_buffering,
                                CORBA::ULong max_buffered_requests,
                                CORBA::ULong max_request_buffer_size,
                                CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Destroy a threadpool.
  void
  destroy_threadpool (RTCORBA::ThreadpoolId threadpool,
                      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RTCORBA::RTORB::InvalidThreadpool));

  /// Collection of thread pools.
  typedef ACE_Hash_Map_Manager<RTCORBA::ThreadpoolId, TAO_Thread_Pool *, ACE_Null_Mutex> THREAD_POOLS;

  /// Access the thread pools managed by this class.
  THREAD_POOLS &thread_pools (void);

private:

  /// @name Helpers
  // @{

  RTCORBA::ThreadpoolId
  create_threadpool_i (CORBA::ULong stacksize,
                       CORBA::ULong static_threads,
                       CORBA::ULong dynamic_threads,
                       RTCORBA::Priority default_priority,
                       CORBA::Boolean allow_request_buffering,
                       CORBA::ULong max_buffered_requests,
                       CORBA::ULong max_request_buffer_size,
                       CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  RTCORBA::ThreadpoolId
  create_threadpool_with_lanes_i (CORBA::ULong stacksize,
                                  const RTCORBA::ThreadpoolLanes & lanes,
                                  CORBA::Boolean allow_borrowing,
                                  CORBA::Boolean allow_request_buffering,
                                  CORBA::ULong max_buffered_requests,
                                  CORBA::ULong max_request_buffer_size,
                                  CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void destroy_threadpool_i (RTCORBA::ThreadpoolId threadpool,
                             CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RTCORBA::RTORB::InvalidThreadpool));

  RTCORBA::ThreadpoolId
  create_threadpool_helper (TAO_Thread_Pool *thread_pool,
                            CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // @}

  TAO_ORB_Core *orb_core_;

  THREAD_POOLS thread_pools_;
  RTCORBA::ThreadpoolId thread_pool_id_counter_;
  ACE_SYNCH_MUTEX lock_;
};

#if defined (__ACE_INLINE__)
# include "Thread_Pool.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif /* TAO_THREAD_POOL_H */
