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
#include "tao/Dynamic_TP/DTP_Config.h"
#include "tao/New_Leader_Generator.h"
#include "ace/Task.h"
#include "ace/Null_Mutex.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;
class TAO_DTP_Thread_Pool;
class TAO_DTP_Thread_Pool_Manager;

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
  TAO_DTP_New_Leader_Generator (TAO_DTP_Thread_Pool &lane);

  /// Leader/Follower class uses this method to notify the system that
  /// we are out of leaders.
  void no_leaders_available (void);

private:

  /// Pool associated with this leader generator.
  TAO_DTP_Thread_Pool &pool_;
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
  TAO_DTP_Thread_Pool_Threads (TAO_DTP_Thread_Pool &pool);

  /// Method executed when a thread is spawned.
  int svc (void);

  /// Accessor to the pool to which this thread belongs to.
  TAO_DTP_Thread_Pool &pool (void) const;

protected:
  /// Do the real work
  virtual int run (TAO_ORB_Core &orb_core);

  /// Pool to which this thread belongs to.
  TAO_DTP_Thread_Pool &pool_;
};


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

  /// Wait for threads to exit.
  void wait (void);

  /// Mark this thread pool that we are shutting down.
  void shutting_down (void);

  /// Create the initial threads - only called once.
  int create_initial_threads (void);

  /// Called by the TAO_DTP_New_Leader_Generator to request a new dynamic
  /// thread.
  /**
   * It can be that no thread can be created because the number of
   * threads is equal to the maximum we can have or the Thread Lane
   * is shutting down.
   * @retval true A new thread is created
   * @retval false No thread could be created
   */
  bool new_dynamic_thread (void);

  /// Called by the run loop to determine if to expire a thread or not
  /// when the dynamic timeout is reached.
  bool above_minimum (void);

  /// @name Accessors
  // @{

  bool use_timeouts (void) const;
  const ACE_Time_Value& dynamic_thread_time (void) const;

  TAO_DTP_Thread_Pool_Manager &manager (void) const;
  CORBA::ULong id (void) const;
  CORBA::ULong current_threads (void) const;

  // @}

private:

  int create_threads_i (size_t count, long thread_flags);

  /// Create @a number_of_threads of dynamic threads.  Can be called
  /// multiple times.
  int create_dynamic_threads (size_t count);

  TAO_DTP_Thread_Pool_Manager &manager_;

  CORBA::ULong id_;

  /// This boolean is set when we are shutting down, then we will not create
  /// any new dynamic threads
  bool shutdown_;

  TAO_DTP_Definition definition_;

  /// Array with all threads
  TAO_DTP_Thread_Pool_Threads threads_;

  TAO_DTP_New_Leader_Generator new_thread_generator_;

  /// Lock to guard all members of the pool
  mutable TAO_SYNCH_MUTEX lock_;
};

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

  /// Wait for threads to exit.
  void wait (void);

  /// Create a threadpool without lanes.
  CORBA::ULong create_threadpool (TAO_DTP_Definition &def);

  /// Destroy a threadpool.
  void destroy_threadpool (CORBA::ULong threadpool);

  /// Collection of thread pools.
  typedef ACE_Hash_Map_Manager<CORBA::ULong, TAO_DTP_Thread_Pool *,
                               ACE_Null_Mutex> THREAD_POOLS;

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
