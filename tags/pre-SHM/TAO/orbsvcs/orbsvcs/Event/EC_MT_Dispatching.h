/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_MT_Dispatching
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
//
// ============================================================================

#ifndef TAO_EC_MT_DISPATCHING_H
#define TAO_EC_MT_DISPATCHING_H

#include "EC_Dispatching.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "EC_Dispatching_Task.h"

class TAO_EC_Event_Channel;

class TAO_ORBSVCS_Export TAO_EC_MT_Dispatching : public TAO_EC_Dispatching
{
  // = TITLE
  //   Dispatching strategy that minimizes mt inversion.
  //
  // = DESCRIPTION
  //   This strategy uses a single queue, serviced by one or more
  //   threads.  It's main purpose is to decouple the suppliers from
  //   the client execution time, specially in the collocated case.
  //
public:
  TAO_EC_MT_Dispatching (int nthreads,
                         int thread_creation_flags,
                         int thread_priority,
                         int force_activate);
  // Constructor
  // It will create <nthreads> servicing threads...

  // = The EC_Dispatching methods.
  virtual void activate (void);
  virtual void shutdown (void);
  virtual void push (TAO_EC_ProxyPushSupplier* proxy,
                     const RtecEventComm::EventSet& event,
                     TAO_EC_QOS_Info& qos_info,
                     CORBA::Environment& env);
  virtual void push_nocopy (TAO_EC_ProxyPushSupplier* proxy,
                            RtecEventComm::EventSet& event,
                            TAO_EC_QOS_Info& qos_info,
                            CORBA::Environment& env);

private:
  ACE_Thread_Manager thread_manager_;
  // Use our own thread manager.

  int nthreads_;
  // The number of active tasks

  int thread_creation_flags_;
  // The flags (THR_BOUND, THR_NEW_LWP, etc.) used to create the
  // dispatching threads.

  int thread_priority_;
  // The priority of the dispatching threads.

  int force_activate_;
  // If activation at the requested priority fails then we fallback on
  // the defaults for thread activation.

  TAO_EC_Dispatching_Task task_;
  // The dispatching task

  ACE_SYNCH_MUTEX lock_;
  // Synchronize access to internal data

  int active_;
  // Are the threads running?
};

#if defined (__ACE_INLINE__)
#include "EC_MT_Dispatching.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_MT_DISPATCHING_H */
