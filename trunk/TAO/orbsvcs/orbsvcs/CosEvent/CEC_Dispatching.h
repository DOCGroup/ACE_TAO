/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Cos Event Channel
//
// = FILENAME
//   CEC_Dispatching
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//
// ============================================================================

#ifndef TAO_CEC_DISPATCHING_H
#include "ace/pre.h"
#define TAO_CEC_DISPATCHING_H

#include "orbsvcs/CosEventCommC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_CEC_ProxyPushSupplier;

class TAO_ORBSVCS_Export TAO_CEC_Dispatching
{
  // = TITLE
  //   Define the interface for the dispatching strategies.
  //
  // = DESCRIPTION
  //   The EC may be configured with different dispatching strategies,
  //   for instance, it can use a pool of threads to dispatch the
  //   events, or a set of queues with threads at different priorities
  //   for each queue or can simply push the event to the consumer in
  //   FIFO order.
  //
public:
  virtual ~TAO_CEC_Dispatching (void);
  // destructor...

  virtual void activate (void) = 0;
  // Initialize all the data structures, activate any internal threads,
  // etc.

  virtual void shutdown (void) = 0;
  // Deactivate any internal threads and cleanup internal data
  // structures, it should only return once the threads have finished
  // their jobs.

  virtual void push (TAO_CEC_ProxyPushSupplier *proxy,
                     const CORBA::Any &event,
                     CORBA::Environment &env = TAO_default_environment ()) = 0;
  virtual void push_nocopy (TAO_CEC_ProxyPushSupplier *proxy,
                            CORBA::Any &event,
                            CORBA::Environment &env = TAO_default_environment ()) = 0;
  // The consumer represented by <proxy> should receive <event>.
};

// ****************************************************************

class TAO_ORBSVCS_Export TAO_CEC_Reactive_Dispatching : public TAO_CEC_Dispatching
{
  // = TITLE
  //   Dispatch using the caller thread.
  //
  // = DESCRIPTION
  //   The events are dispatched in FIFO ordering, using the invoking
  //   thread to push the event to the consumer.
  //
public:
  TAO_CEC_Reactive_Dispatching (void);
  // The scheduler is used to find the range of priorities and similar
  // info.

  // = The CEC_Dispatching methods.
  virtual void activate (void);
  virtual void shutdown (void);
  virtual void push (TAO_CEC_ProxyPushSupplier *proxy,
                     const CORBA::Any &event,
                     CORBA::Environment &env = TAO_default_environment ());
  virtual void push_nocopy (TAO_CEC_ProxyPushSupplier *proxy,
                            CORBA::Any &event,
                            CORBA::Environment &env = TAO_default_environment ());
};

#if defined (__ACE_INLINE__)
#include "CEC_Dispatching.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_CEC_DISPATCHING_H */
