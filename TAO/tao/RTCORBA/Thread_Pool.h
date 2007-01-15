// -*- C++ -*-

//=============================================================================
/**
 *  @file    Thread_Pool.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali
 *  @author Johnny Willemsen
 */
// ===================================================================

#ifndef TAO_THREAD_POOL_H
#define TAO_THREAD_POOL_H

#include /**/ "ace/pre.h"
#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#define TAO_RTCORBA_SAFE_INCLUDE
#include "tao/RTCORBA/RTCORBAC.h"
#undef TAO_RTCORBA_SAFE_INCLUDE

#include "ace/Hash_Map_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/New_Leader_Generator.h"
#include "ace/Task.h"
#include "ace/Null_Mutex.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Thread_Lane;

/**
 * @class TAO_RT_New_Leader_Generator
 *
 * @brief Class for creating dynamic threads.
 *
 * \nosubgrouping
 *
 **/
class TAO_RTCORBA_Export TAO_RT_New_Leader_Generator
  : public TAO_New_Leader_Generator
{
public:

  /// Constructor.
  TAO_RT_New_Leader_Generator (TAO_Thread_Lane &lane);

  /// Leader/Follower class uses this method to notify the system that
  /// we are out of leaders.
  void no_leaders_available (void);

private:

  /// Lane associated with this leader generator.
  TAO_Thread_Lane &lane_;
};

/**
 * @class TAO_Thread_Pool_Threads
 *
 * @brief Class representing a static thread running in a thread lane.
 *
 * \nosubgrouping
 *
 **/
class TAO_Thread_Pool_Threads : public ACE_Task_Base
{
public:

  /// Constructor.
  TAO_Thread_Pool_Threads (TAO_Thread_Lane &lane);

  /// Method executed when a thread is spawned.
  int svc (void);

  /// Accessor to the lane to which this thread belongs to.
  TAO_Thread_Lane &lane (void) const;

  /// Set TSS resources for the current thread.
  static void set_tss_resources (TAO_ORB_Core &orb_core,
                                 TAO_Thread_Lane &thread_lane);

protected:
  /// Do the real work
  virtual int run (TAO_ORB_Core &orb_core ACE_ENV_ARG_PARAMETER);

  /// Lane to which this thread belongs to.
  TAO_Thread_Lane &lane_;
};

/**
 * @class TAO_Dynamic_Thread_Pool_Threads
 *
 * @brief Class representing a dynamic thread running in a thread lane.
 *
 * \nosubgrouping
 *
 **/
class TAO_Dynamic_Thread_Pool_Threads : public TAO_Thread_Pool_Threads
{
public:
  /// Constructor.
  TAO_Dynamic_Thread_Pool_Threads (TAO_Thread_Lane &lane);

protected:
  /// Do the real work
  virtual int run (TAO_ORB_Core &orb_core ACE_ENV_ARG_PARAMETER);
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
                   CORBA::ULong dynamic_threads,
                   ACE_Time_Value const &dynamic_thread_idle_timeout
                   ACE_ENV_ARG_DECL);

  /// Destructor.
  ~TAO_Thread_Lane (void);

  /// Open the lane.
  void open (ACE_ENV_SINGLE_ARG_DECL);

  /// Finalize the resources.
  void finalize (void);

  /// Shutdown the reactor.
  void shutdown_reactor (void);

  /// Wait for threads to exit.
  void wait (void);

  /// Does @a mprofile belong to us?
  int is_collocated (const TAO_MProfile &mprofile);

  /// Create the static threads - only called once.
  int create_static_threads (void);

  /// Mark that this lane is shutting down, we then don't create any
  /// dynamic threads anymore. When the pool is shutting down the leader
  /// follower loop is called which can cause a request to create a
  /// new dynamic thread but we shouldn't create a new one.
  void shutting_down (void);

  /// Called by the TAO_RT_New_Leader_Generator to request a new dynamic
  /// thread.
  /**
   * It can be that no thread can be created because the number of
   * threads is equal to the maximum we can have or the Thread Lane
   * is shutting down.
   * @retval true A new thread is created
   * @retval false No thread could be created
   */
  bool new_dynamic_thread (void);

  /// @name Accessors
  // @{

  TAO_Thread_Pool &pool (void) const;
  CORBA::ULong id (void) const;

  CORBA::Short lane_priority (void) const;
  CORBA::ULong static_threads (void) const;
  CORBA::ULong dynamic_threads (void) const;

  CORBA::ULong current_threads (void) const;

  CORBA::Short native_priority (void) const;

  TAO_Thread_Lane_Resources &resources (void);

  ACE_Time_Value const &dynamic_thread_idle_timeout (void) const;
  // @}

private:

  /// Validate lane's priority and map it to a native value.
  void validate_and_map_priority (ACE_ENV_SINGLE_ARG_DECL);

  int create_threads_i (TAO_Thread_Pool_Threads &thread_pool,
                        CORBA::ULong number_of_threads,
                        long thread_flags);

  /// Create @a number_of_threads of dynamic threads.  Can be called
  /// multiple times.
  int create_dynamic_threads (CORBA::ULong number_of_threads);

  /// The Thread Pool to which this lane belongs.
  TAO_Thread_Pool &pool_;

  /// The id of this lane
  CORBA::ULong const id_;

  CORBA::Short lane_priority_;

  /// This boolean is set when we are shutting down, then we will not create
  /// any new dynamic threads
  bool shutdown_;

  /// Number of static threads
  CORBA::ULong const static_threads_number_;

  /// Maximum number of threads we are allowed to create
  CORBA::ULong const dynamic_threads_number_;

  /// Array with all static threads
  TAO_Thread_Pool_Threads static_threads_;

  /// Array with all dynamic threads
  TAO_Dynamic_Thread_Pool_Threads dynamic_threads_;

  TAO_RT_New_Leader_Generator new_thread_generator_;

  TAO_Thread_Lane_Resources resources_;

  CORBA::Short native_priority_;

  ACE_Time_Value const dynamic_thread_idle_timeout_;

  /// Lock to guard all members of the lane
  mutable ACE_SYNCH_MUTEX lock_;
};

class TAO_Thread_Pool_Manager;

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
public:

  /// Constructor (for pools without lanes).
  TAO_Thread_Pool (TAO_Thread_Pool_Manager &manager,
                   CORBA::ULong id,
                   CORBA::ULong stack_size,
                   CORBA::ULong static_threads,
                   CORBA::ULong dynamic_threads,
                   CORBA::Short default_priority,
                   CORBA::Boolean allow_request_buffering,
                   CORBA::ULong max_buffered_requests,
                   CORBA::ULong max_request_buffer_size,
                   ACE_Time_Value const &dynamic_thread_idle_timeout
                   ACE_ENV_ARG_DECL);

  /// Constructor (for pools with lanes).
  TAO_Thread_Pool (TAO_Thread_Pool_Manager &manager,
                   CORBA::ULong id,
                   CORBA::ULong stack_size,
                   const RTCORBA::ThreadpoolLanes &lanes,
                   CORBA::Boolean allow_borrowing,
                   CORBA::Boolean allow_request_buffering,
                   CORBA::ULong max_buffered_requests,
                   CORBA::ULong max_request_buffer_size,
                   ACE_Time_Value const &dynamic_thread_idle_timeout
                   ACE_ENV_ARG_DECL);

  /// Destructor.
  ~TAO_Thread_Pool (void);

  /// Open the pool.
  void open (ACE_ENV_SINGLE_ARG_DECL);

  /// Finalize the resources.
  void finalize (void);

  /// Shutdown the reactor.
  void shutdown_reactor (void);

  /// Wait for threads to exit.
  void wait (void);

  /// Mark this thread pool that we are shutting down.
  void shutting_down (void);

  /// Does @a mprofile belong to us?
  int is_collocated (const TAO_MProfile &mprofile);

  /// Create the static threads - only called once.
  int create_static_threads (void);

  /// Check if this thread pool has (explicit) lanes.
  bool with_lanes (void) const;

  /// @name Accessors
  // @{

  TAO_Thread_Pool_Manager &manager (void) const;
  CORBA::ULong id (void) const;

  CORBA::ULong stack_size (void) const;
  CORBA::Boolean allow_borrowing (void) const;
  CORBA::Boolean allow_request_buffering (void) const;
  CORBA::ULong max_buffered_requests (void) const;
  CORBA::ULong max_request_buffer_size (void) const;

  TAO_Thread_Lane **lanes (void);
  CORBA::ULong number_of_lanes (void) const;

  // @}

private:

  TAO_Thread_Pool_Manager &manager_;
  CORBA::ULong id_;

  CORBA::ULong stack_size_;
  CORBA::Boolean allow_borrowing_;
  CORBA::Boolean allow_request_buffering_;
  CORBA::ULong max_buffered_requests_;
  CORBA::ULong max_request_buffer_size_;
  ACE_Time_Value const dynamic_thread_idle_timeout_;

  TAO_Thread_Lane **lanes_;
  CORBA::ULong number_of_lanes_;
  bool with_lanes_;
};

class TAO_ORB_Core;

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
  TAO_Thread_Pool_Manager (TAO_ORB_Core &orb_core);

  /// Destructor.
  ~TAO_Thread_Pool_Manager (void);

  /// Finalize the resources.
  void finalize (void);

  /// Shutdown the reactor.
  void shutdown_reactor (void);

  /// Wait for threads to exit.
  void wait (void);

  /// Does @a mprofile belong to us?
  int is_collocated (const TAO_MProfile &mprofile);

  /// Create a threadpool without lanes.
  RTCORBA::ThreadpoolId
  create_threadpool (CORBA::ULong stacksize,
                     CORBA::ULong static_threads,
                     CORBA::ULong dynamic_threads,
                     RTCORBA::Priority default_priority,
                     CORBA::Boolean allow_request_buffering,
                     CORBA::ULong max_buffered_requests,
                     CORBA::ULong max_request_buffer_size,
                     ACE_Time_Value const &dynamic_thread_idle_timeout
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Create a threadpool with lanes.
  RTCORBA::ThreadpoolId
  create_threadpool_with_lanes (CORBA::ULong stacksize,
                                const RTCORBA::ThreadpoolLanes & lanes,
                                CORBA::Boolean allow_borrowing,
                                CORBA::Boolean allow_request_buffering,
                                CORBA::ULong max_buffered_requests,
                                CORBA::ULong max_request_buffer_size,
                                ACE_Time_Value const &dynamic_thread_idle_timeout
                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Destroy a threadpool.
  void destroy_threadpool (RTCORBA::ThreadpoolId threadpool
                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RTCORBA::RTORB::InvalidThreadpool));

  TAO_Thread_Pool *get_threadpool (RTCORBA::ThreadpoolId thread_pool_id ACE_ENV_ARG_DECL);

  /// Collection of thread pools.
  typedef ACE_Hash_Map_Manager<RTCORBA::ThreadpoolId, TAO_Thread_Pool *, ACE_Null_Mutex> THREAD_POOLS;

  /// @name Accessors
  // @{

  TAO_ORB_Core &orb_core (void) const;

  // @}

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
                       ACE_Time_Value const &dynamic_thread_idle_timeout
                       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  RTCORBA::ThreadpoolId
  create_threadpool_with_lanes_i (CORBA::ULong stacksize,
                                  const RTCORBA::ThreadpoolLanes & lanes,
                                  CORBA::Boolean allow_borrowing,
                                  CORBA::Boolean allow_request_buffering,
                                  CORBA::ULong max_buffered_requests,
                                  CORBA::ULong max_request_buffer_size,
                                  ACE_Time_Value const &dynamic_thread_idle_timeout
                                  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  RTCORBA::ThreadpoolId
  create_threadpool_helper (TAO_Thread_Pool *thread_pool
                            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // @}

private:

  TAO_ORB_Core &orb_core_;

  THREAD_POOLS thread_pools_;
  RTCORBA::ThreadpoolId thread_pool_id_counter_;
  ACE_SYNCH_MUTEX lock_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/RTCORBA/Thread_Pool.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"

#endif /* TAO_THREAD_POOL_H */
