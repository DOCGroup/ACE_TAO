/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   CEC_Dispatching.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_CEC_DISPATCHING_H
#define TAO_CEC_DISPATCHING_H
#include /**/ "ace/pre.h"

#include "orbsvcs/CosEventCommC.h"
#include "event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_CEC_ProxyPushSupplier;

/**
 * @class TAO_CEC_Dispatching
 *
 * @brief Define the interface for the dispatching strategies.
 *
 * The EC may be configured with different dispatching strategies,
 * for instance, it can use a pool of threads to dispatch the
 * events, or a set of queues with threads at different priorities
 * for each queue or can simply push the event to the consumer in
 * FIFO order.
 */
class TAO_Event_Export TAO_CEC_Dispatching
{
public:
  /// destructor...
  virtual ~TAO_CEC_Dispatching (void);

  /// Initialize all the data structures, activate any internal threads,
  /// etc.
  virtual void activate (void) = 0;

  /**
   * Deactivate any internal threads and cleanup internal data
   * structures, it should only return once the threads have finished
   * their jobs.
   */
  virtual void shutdown (void) = 0;

  /// The consumer represented by <proxy> should receive <event>.
  virtual void push (TAO_CEC_ProxyPushSupplier *proxy,
                     const CORBA::Any &event
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS) = 0;
  virtual void push_nocopy (TAO_CEC_ProxyPushSupplier *proxy,
                            CORBA::Any &event
                            ACE_ENV_ARG_DECL_WITH_DEFAULTS) = 0;
};

// ****************************************************************

/**
 * @class TAO_CEC_Reactive_Dispatching
 *
 * @brief Dispatch using the caller thread.
 *
 * The events are dispatched in FIFO ordering, using the invoking
 * thread to push the event to the consumer.
 */
class TAO_Event_Export TAO_CEC_Reactive_Dispatching : public TAO_CEC_Dispatching
{
public:
  /// The scheduler is used to find the range of priorities and similar
  /// info.
  TAO_CEC_Reactive_Dispatching (void);

  // = The CEC_Dispatching methods.
  virtual void activate (void);
  virtual void shutdown (void);
  virtual void push (TAO_CEC_ProxyPushSupplier *proxy,
                     const CORBA::Any &event
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  virtual void push_nocopy (TAO_CEC_ProxyPushSupplier *proxy,
                            CORBA::Any &event
                            ACE_ENV_ARG_DECL_WITH_DEFAULTS);
};

#if defined (__ACE_INLINE__)
#include "CEC_Dispatching.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_CEC_DISPATCHING_H */
