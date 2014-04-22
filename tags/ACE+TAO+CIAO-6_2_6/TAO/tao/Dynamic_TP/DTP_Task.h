// -*- C++ -*-

//=============================================================================
/**
 *  @file    DTP_Task.h
 *
 *  $Id$
 */
//=============================================================================

#ifndef TAO_DYNAMIC_TP_TASK_H
#define TAO_DYNAMIC_TP_TASK_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/Dynamic_TP/dynamic_tp_export.h"
#include "tao/Dynamic_TP/DTP_Config.h"
#include "tao/CSD_ThreadPool/CSD_TP_Queue.h"
#include "tao/CSD_ThreadPool/CSD_TP_Request.h"
#include "tao/CSD_ThreadPool/CSD_TP_Dispatchable_Visitor.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/Condition.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Synch.h"
#include "ace/Containers_T.h"
#include "ace/Vector_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

    /**
     * @class TP_Task
     *
     * @brief Active Object managing a queue of request objects.
     *
     * There are two types of "users" of a TP_Task object:
     *
     *    1) The TP_Strategy object that "owns" this task object.
     *    2) The worker threads that "run" this task object as an
     *       "active object".
     *
     * The TP_Strategy object that "owns" this task object dictates
     * when the worker threads are activated and when they are shutdown.  It
     * also injects requests into this task's queue via calls to the
     * add_request() method.  It is also the TP_Strategy object that
     * dictates the number of worker threads to be activated via a call to
     * the set_num_threads() method.
     *
     * The active object pattern is implemented via the use of the
     * the ACE_Task_Base base class, and each worker thread will
     * invoke this task's svc() method, and when the svc() returns, the
     * worker thread will invoke this task's close() method (with the
     * flag argument equal to 0).
     */
    class TAO_Dynamic_TP_Export TAO_DTP_Task : public ACE_Task_Base
    {
    public:

      /// Default Constructor.
      TAO_DTP_Task();

      /// Virtual Destructor.
      virtual ~TAO_DTP_Task();

      struct Open_Args {
        TAO_DTP_Definition task_thread_config;
      };


      /// Put a request object on to the request queue.
      /// Returns true if successful, false otherwise (it has been "rejected").
      bool add_request(TAO::CSD::TP_Request* request);

      /// Activate the worker threads
      virtual int open(void* args = 0);

      /// The "mainline" executed by each worker thread.
      virtual int svc();

      virtual int close (u_long flag = 0);

      /// Set the thread and queue config.

      void set_init_pool_threads(size_t thr_count);

      void set_min_pool_threads(size_t thr_count);

      void set_max_pool_threads(size_t thr_count);

      void set_thread_stack_size(size_t stack_sz);

      void set_thread_idle_time(ACE_Time_Value thr_timeout);

      void set_max_request_queue_depth(size_t queue_depth);

      /// Get the thread and queue config.

      size_t get_init_pool_threads();

      size_t get_min_pool_threads();

      size_t get_max_pool_threads();

      size_t get_max_request_queue_depth();

      size_t get_thread_stack_size();

      time_t get_thread_idle_time();

      /// Cancel all requests that are targeted for the provided servant.
      void cancel_servant (PortableServer::Servant servant);

    private:
      /// get the next available request. Return true if one available, nonblocking
      bool request_ready (TAO::CSD::TP_Dispatchable_Visitor &v,
                          TAO::CSD::TP_Request_Handle &r);

      /// release the request, reset the accepting flag if necessary
      void clear_request (TAO::CSD::TP_Request_Handle &r);



      typedef TAO_SYNCH_MUTEX         LockType;
      typedef TAO_Condition<LockType> ConditionType;

      /// Lock used to synchronize the "active_workers_" condition
      LockType aw_lock_;
      /// Lock used to synchronize manipulation of the queue
      LockType queue_lock_;
      /// Lock used to synchronize the "work_available_" condition
      LockType work_lock_;

      /// Condition used to signal worker threads that they may be able to
      /// find a request in the queue_ that needs to be dispatched to a
      /// servant that is currently "not busy".
      /// This condition will be signal()'ed each time a new request is
      /// added to the queue_, and also when a servant has become "not busy".
      ConditionType work_available_;

      /// This condition will be signal()'ed each time the num_threads_
      /// data member has its value changed.  This is used to keep the
      /// close(1) invocation (ie, a shutdown request) blocked until all
      /// of the worker threads have stopped running.
      ConditionType active_workers_;

      /// Flag used to indicate when this task will (or will not) accept
      /// requests via the the add_request() method.
      bool accepting_requests_;

      /// Flag used to initiate a shutdown request to all worker threads.
      bool shutdown_;

      /// Flag used to avoid multiple open() calls.
      bool opened_;

      /// The number of requests in the local queue.
      size_t num_queue_requests_;

      /// The number of currently active worker threads.
      ACE_Atomic_Op <TAO_SYNCH_MUTEX, unsigned long> busy_threads_;

      /// The queue of pending servant requests (a.k.a. the "request queue").
      TAO::CSD::TP_Queue queue_;

      /// The low water mark for dynamic threads to settle to.
      size_t init_pool_threads_;

      /// The low water mark for dynamic threads to settle to.
      size_t min_pool_threads_;

      /// The high water mark for dynamic threads to be limited to.
      size_t max_pool_threads_;

      /// If the max_pool_threads_ value has been met, then ORB requests coming in can be queued.
      /// This is the maximum number that will be allowed.
      size_t max_request_queue_depth_;

      /// This is the memory stack size allowable for each thread.
      size_t thread_stack_size_;

      /// This is the maximum amount of time in seconds that an idle thread can
      /// stay alive before being taken out of the pool.
      ACE_Time_Value thread_idle_time_;
    };


TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Dynamic_TP/DTP_Task.inl"
#endif /* __ACE_INLINE__ */

#endif /* (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"

#endif /* TAO_DYNAMIC_TP_TASK_H */
