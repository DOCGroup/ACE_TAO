/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Dispatching
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

#ifndef TAO_EC_DISPATCHING_H
#include "ace/pre.h"
#define TAO_EC_DISPATCHING_H

#include "orbsvcs/RtecEventCommC.h"
#include "event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_QOS_Info;
class TAO_EC_ProxyPushSupplier;

class TAO_RTEvent_Export TAO_EC_Dispatching
{
  // = TITLE
  //   Abstract base class for the dispatching strategies.
  //
  // = DESCRIPTION
  //   The dispatching strategies.
  //   The EC may be configured with different dispatching strategies,
  //   for instance, it can use a pool of threads to dispatch the
  //   events, or a set of queues with threads at different priorities
  //   for each queue or can simply push the event to the consumer in
  //   FIFO order.
  //
public:
  virtual ~TAO_EC_Dispatching (void);
  // destructor...

  virtual void activate (void) = 0;
  // Initialize all the data structures, activate any internal threads,
  // etc.

  virtual void shutdown (void) = 0;
  // Deactivate any internal threads and cleanup internal data
  // structures, it should only return once the threads have finished
  // their jobs.

  virtual void push (TAO_EC_ProxyPushSupplier *proxy,
                     const RtecEventComm::EventSet &event,
                     TAO_EC_QOS_Info &qos_info,
                     CORBA::Environment &env = TAO_default_environment ()) = 0;
  virtual void push_nocopy (TAO_EC_ProxyPushSupplier *proxy,
                            RtecEventComm::EventSet &event,
                            TAO_EC_QOS_Info &qos_info,
                            CORBA::Environment &env = TAO_default_environment ()) = 0;
  // The consumer represented by <proxy> should receive <event>.
  // It can use the information in <qos_info> to determine the event
  // priority (among other things).
};

// ****************************************************************

class TAO_RTEvent_Export TAO_EC_Reactive_Dispatching : public TAO_EC_Dispatching
{
  // = TITLE
  //   Dispatch using the caller thread.
  //
  // = DESCRIPTION
  //   The events are dispatched in FIFO ordering, using the invoking
  //   thread to push the event to the consumer.
  //
public:
  TAO_EC_Reactive_Dispatching (void);
  // The scheduler is used to find the range of priorities and similar
  // info.

  // = The EC_Dispatching methods.
  virtual void activate (void);
  virtual void shutdown (void);
  virtual void push (TAO_EC_ProxyPushSupplier *proxy,
                     const RtecEventComm::EventSet &event,
                     TAO_EC_QOS_Info &qos_info,
                     CORBA::Environment &env = TAO_default_environment ());
  virtual void push_nocopy (TAO_EC_ProxyPushSupplier *proxy,
                            RtecEventComm::EventSet &event,
                            TAO_EC_QOS_Info &qos_info,
                            CORBA::Environment &env = TAO_default_environment ());
};

#if defined (__ACE_INLINE__)
#include "EC_Dispatching.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_DISPATCHING_H */
