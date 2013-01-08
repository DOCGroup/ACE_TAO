// -*- C++ -*-

//=============================================================================
/**
 *  @file DTP_Thread_Pool.h
 *
 *  $Id$
 *
 *  @author Irfan Pyarali
 *  @author Johnny Willemsen
 *  @author Phil Mesnier
 */
// ===================================================================

#ifndef TAO_DTP_THREAD_POOL_H
#define TAO_DTP_THREAD_POOL_H

#include /**/ "ace/pre.h"
#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Dynamic_TP/dynamic_tp_export.h"
#include "tao/Dynamic_TP/Dynamic_TP_Config.h"
#include "tao/New_Leader_Generator.h"
#include "ace/Task.h"
#include "ace/Null_Mutex.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_DTP_Thread_Lane;

/**
 * @class TAO_DTP_New_Leader_Generator
 *
 * @brief Class for creating dynamic threads.
 *
 * \nosubgrouping
 *
 **/
class TAO_Dynamic_TP_Export TAO_DTP_New_Leader_Generator
  : public TAO_New_Leader_Generator
{
public:

  /// Constructor.
  TAO_DTP_New_Leader_Generator (TAO_DTP_Thread_Lane &lane);

  /// Leader/Follower class uses this method to notify the system that
  /// we are out of leaders.
  void no_leaders_available (void);

private:

  /// Lane associated with this leader generator.
  TAO_DTP_Thread_Lane &lane_;
};

/**
 * @class TAO_DTP_Thread_Pool_Threads
 *
 * @brief Class representing a static thread running in a thread lane.
 *
 * \nosubgrouping
 *
 **/
class TAO_DTP_Thread_Pool_Threads : public ACE_Task_Base
{
public:

  /// Constructor.
  TAO_DTP_Thread_Pool_Threads (TAO_DTP_Thread_Lane &lane);

  /// Method executed when a thread is spawned.
  int svc (void);

  /// Accessor to the lane to which this thread belongs to.
  TAO_DTP_Thread_Lane &lane (void) const;

protected:
  /// Do the real work
  virtual int run (TAO_ORB_Core &orb_core);

  /// Lane to which this thread belongs to.
  TAO_DTP_Thread_Lane &lane_;
};

class TAO_DTP_Thread_Pool;

/**
 * @class TAO_DTP_Thread_Lane
 *
 * @brief Class representing the thread lane inside a thread pool.
 *
 * \nosubgrouping
 *
 **/
class TAO_Dynamic_TP_Export TAO_DTP_Thread_Lane
{
public:
  /// Constructor.
  TAO_DTP_Thread_Lane (TAO_DTP_Thread_Pool &pool,
                       CORBA::ULong id,
                       CORBA::ULong minimum_threads,
                       CORBA::ULong initial_threads,
                       CORBA::ULong maximum_threads,
                       ACE_Time_Value const &dynamic_thread_time);

  /// Destructor.
  ~TAO_DTP_Thread_Lane (void);

  /// Open the lane.
  void open (void);

  /// Finalize the resources.
  void finalize (void);

  /// Shutdown the reactor.
  void shutdown_reactor (void);

  /// Wait for threads to exit.
  void wait (void);

  /// Does @a mprofile belong to us?
  int is_collocated (const TAO_MProfile &mprofile);

  /// Create the initial threads - only called once.
  int create_initial_threads (void);

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
  TAO_DTP_Thread_Pool &pool (void) const;
  CORBA::ULong id (void) const;

  CORBA::ULong minimum_threads (void) const;
  CORBA::ULong maximum_threads (void) const;

  CORBA::ULong current_threads (void) const;

  TAO_Thread_Lane_Resources &resources (void);

  ACE_Time_Value const &dynamic_thread_time (void) const;
  // @}

private:
  int create_threads_i (TAO_DTP_Thread_Pool_Threads &thread_pool,
                        CORBA::ULong number_of_threads,
                        long thread_flags);

  /// Create @a number_of_threads of dynamic threads.  Can be called
  /// multiple times.
  int create_dynamic_threads (CORBA::ULong number_of_threads);

  /// The Thread Pool to which this lane belongs.
  TAO_DTP_Thread_Pool &pool_;

  /// The id of this lane
  CORBA::ULong const id_;

  /// This boolean is set when we are shutting down, then we will not create
  /// any new dynamic threads
  bool shutdown_;

  /// Minimum number of threads
  CORBA::ULong const minimum_threads_number_;

  /// Initial number of threads to create. May be anywhere between minimum and maximum
  CORBA::ULong const initial_threads_number_;

  /// Maximum number of threads we are allowed to create
  CORBA::ULong const maximum_threads_number_;

  /// Array with all threads
  TAO_DTP_Thread_Pool_Threads threads_;

  TAO_DTP_New_Leader_Generator new_thread_generator_;

  TAO_Thread_Lane_Resources resources_;

  ACE_Time_Value const dynamic_thread_time_;

  /// Lock to guard all members of the lane
  mutable TAO_SYNCH_MUTEX lock_;
};

class TAO_DTP_Thread_Pool_Manager;

/**
 * @class TAO_DTP_Thread_Pool
 *
 * @brief Class representing the thread pool inside a thread pool
 * manager.
 *
 * \nosubgrouping
 *
 **/
class TAO_Dynamic_TP_Export TAO_DTP_Thread_Pool
{
public:
  TAO_DTP_Thread_Pool (TAO_DTP_Thread_Pool_Manager &manager,
                       CORBA::ULong id,
                       TAO_DTP_Definition &definition);

  /// Destructor.
  ~TAO_DTP_Thread_Pool (void);

  /// Open the pool.
  void open (void);

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

  /// Create the initial threads - only called once.
  int create_initial_threads (void);

  /// @name Accessors
  // @{

  TAO_DTP_Thread_Pool_Manager &manager (void) const;
  CORBA::ULong id (void) const;

  CORBA::ULong stack_size (void) const;
  // @}

private:

  TAO_DTP_Thread_Pool_Manager &manager_;
  CORBA::ULong id_;

  TAO_DTP_Definition definition_;

  TAO_DTP_Thread_Lane *lane_;
};

class TAO_ORB_Core;

/**
 * @class TAO_DTP_Thread_Pool_Manager
 *
 * @brief Class for managing thread pools.
 *
 * \nosubgrouping
 *
 **/
class TAO_Dynamic_TP_Export TAO_DTP_Thread_Pool_Manager
{
public:

  /// Constructor.
  TAO_DTP_Thread_Pool_Manager (TAO_ORB_Core &orb_core);

  /// Destructor.
  ~TAO_DTP_Thread_Pool_Manager (void);

  /// Finalize the resources.
  void finalize (void);

  /// Shutdown the reactor.
  void shutdown_reactor (void);

  /// Wait for threads to exit.
  void wait (void);

  /// Does @a mprofile belong to us?
  int is_collocated (const TAO_MProfile &mprofile);

  /// Create a threadpool without lanes.
  CORBA::ULong create_threadpool (TAO_DTP_Definition &def);

  /// Destroy a threadpool.
  void destroy_threadpool (CORBA::ULong threadpool);

  TAO_DTP_Thread_Pool *get_threadpool (CORBA::ULong thread_pool_id);

  /// Collection of thread pools.
  typedef ACE_Hash_Map_Manager<CORBA::ULong, TAO_DTP_Thread_Pool *, ACE_Null_Mutex> THREAD_POOLS;

  /// @name Accessors
  // @{
  TAO_ORB_Core &orb_core (void) const;
  // @}

private:

  /// @name Helpers
  // @{

  CORBA::ULong
  create_threadpool_i (TAO_DTP_Definition &def);

  CORBA::ULong
  create_threadpool_helper (TAO_DTP_Thread_Pool *thread_pool);
  // @}

private:

  TAO_ORB_Core &orb_core_;

  THREAD_POOLS thread_pools_;
  CORBA::ULong thread_pool_id_counter_;
  TAO_SYNCH_MUTEX lock_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/Dynamic_TP/DTP_Thread_Pool.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"

#endif /* TAO_THREAD_POOL_H */
