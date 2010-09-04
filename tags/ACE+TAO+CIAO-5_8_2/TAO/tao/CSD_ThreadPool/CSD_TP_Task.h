// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_TP_Task.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_TP_TASK_H
#define TAO_CSD_TP_TASK_H

#include /**/ "ace/pre.h"

#include "tao/CSD_ThreadPool/CSD_TP_Export.h"

#include "tao/CSD_ThreadPool/CSD_TP_Queue.h"
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

namespace TAO
{
  namespace CSD
  {
    /// Typedef for the number of threads.
    typedef unsigned long Thread_Counter;

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
     *
     * @note I just wanted to document an idea...  When the pool consists
     *       of only one worker thread, we could care less about checking
     *       if target servant objects are busy or not.  The simple fact
     *       that only one thread will be dispatching all requests means
     *       that servant objects will never be busy when the thread
     *       tests to see if a request is "ready_for_dispatch()".  I'm
     *       just wondering if this knowledge can be applied to the
     *       implementation such that the "pool with one worker thread" case
     *       performs more efficiently.  This is STP vs SSTP.
     *
     */
    class TAO_CSD_TP_Export TP_Task : public ACE_Task_Base
    {
    public:

      /// Default Constructor.
      TP_Task();

      /// Virtual Destructor.
      virtual ~TP_Task();

      /// Put a request object on to the request queue.
      /// Returns true if successful, false otherwise (it has been "rejected").
      bool add_request(TP_Request* request);

      /// Activate the worker threads
      virtual int open(void* num_threads_ptr = 0);

      /// The "mainline" executed by each worker thread.
      virtual int svc();

      /// Multi-purpose: argument value is used to differentiate purpose.
      ///
      /// 0) Invoked by each worker thread after its invocation of the
      ///    svc() method has completed (ie, returned).
      /// 1) Invoked by the strategy object to shutdown all worker threads.
      virtual int close(u_long flag = 0);

      /// Cancel all requests that are targeted for the provided servant.
      void cancel_servant (PortableServer::Servant servant);

    private:

      typedef TAO_SYNCH_MUTEX         LockType;
      typedef ACE_Guard<LockType>     GuardType;
      typedef TAO_Condition<LockType> ConditionType;

      /// Lock to protect the "state" (all of the data members) of this object.
      LockType lock_;

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
      bool shutdown_initiated_;

      /// Complete shutdown needed to be deferred because the thread calling
      /// close(1) was also one of the ThreadPool threads
      bool deferred_shutdown_initiated_;

      /// Flag used to avoid multiple open() calls.
      bool opened_;

      /// The number of currently active worker threads.
      Thread_Counter num_threads_;

      /// The queue of pending servant requests (a.k.a. the "request queue").
      TP_Queue queue_;

      typedef ACE_Vector <ACE_thread_t> Thread_Ids;

      /// The list of ids for the threads launched by this task.
      Thread_Ids activated_threads_;

      enum { MAX_THREADPOOL_TASK_WORKER_THREADS = 50 };
    };

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Task.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CSD_TP_TASK_H */
