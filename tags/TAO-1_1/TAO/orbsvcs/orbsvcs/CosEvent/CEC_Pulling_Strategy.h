/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Cos Event Channel
//
// = FILENAME
//   CEC_Pulling_Strategy
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//
// ============================================================================

#ifndef TAO_CEC_PULLING_STRATEGY_H
#define TAO_CEC_PULLING_STRATEGY_H

#include "orbsvcs/orbsvcs_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORBSVCS_Export TAO_CEC_Pulling_Strategy
{
  // = TITLE
  //   Define the interface for the pulling strategies.
  //
  // = DESCRIPTION
  //   The EC may be configured with different pulling strategies,
  //   for instance, it can use a pool of threads to dispatch the
  //   events, or a set of queues with threads at different priorities
  //   for each queue or can simply push the event to the consumer in
  //   FIFO order.
  //
public:
  virtual ~TAO_CEC_Pulling_Strategy (void);
  // destructor...

  virtual void activate (void) = 0;
  // Initialize all the data structures, activate any internal threads,
  // etc.

  virtual void shutdown (void) = 0;
  // Deactivate any internal threads and cleanup internal data
  // structures, it should only return once the threads have finished
  // their jobs.

#if 0
  virtual void connected (TAO_CEC_ProxyPullConsumer *,
                          CORBA::Environment &) = 0;
  virtual void reconnected (TAO_CEC_ProxyPullConsumer *,
                            CORBA::Environment &) = 0;
  virtual void diconnected (TAO_CEC_ProxyPullConsumer *,
                            CORBA::Environment &) = 0;
  // Some strategies may want to keep track of connected consumers.
#endif /* 0 */
};

#if defined (__ACE_INLINE__)
#include "CEC_Pulling_Strategy.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_CEC_PULLING_STRATEGY_H */
