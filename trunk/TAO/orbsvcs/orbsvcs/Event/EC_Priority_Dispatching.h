/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Priority_Dispatching
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

#ifndef TAO_EC_PRIORITY_DISPATCHING_H
#define TAO_EC_PRIORITY_DISPATCHING_H

#include "EC_Dispatching.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Message_Block.h"

class TAO_EC_Dispatching_Task;

class TAO_ORBSVCS_Export TAO_EC_Priority_Dispatching : public TAO_EC_Dispatching
{
  // = TITLE
  //   Dispatching strategy that minimizes priority inversion.
  //
  // = DESCRIPTION
  //   This strategy uses multiple queues, each serviced by a thread
  //   at different priority.  This minimizes priority inversion
  //   because the consumers at higher priority are serviced before
  //   consumers at lower priority.
  //   It is more flexible than using the supplier thread to dispatch
  //   because it allows high-priority suppliers to push events to
  //   low-priority consumers (and vice-versa).
  //   It also isolates the supplier threads from the time spent on
  //   upcalls to the consumer objects, making the system easier to
  //   analyze and schedule.
  //
public:
  TAO_EC_Priority_Dispatching (void);
  // The scheduler is used to find the range of priorities and similar
  // info.

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

  int ntasks_;
  // The number of active tasks

  TAO_EC_Dispatching_Task** tasks_;
  // The tasks..

  ACE_Locked_Data_Block<ACE_Lock_Adapter<ACE_SYNCH_MUTEX> > data_block_;
  // Helper data structure to minimize memory allocations...
  // @@ Should be a per-queue object!
};

#if defined (__ACE_INLINE__)
#include "EC_Priority_Dispatching.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_PRIORITY_DISPATCHING_H */
