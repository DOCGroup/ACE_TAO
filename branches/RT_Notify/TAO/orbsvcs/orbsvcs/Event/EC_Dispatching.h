/* -*- C++ -*- */
/**
 *  @file   EC_Dispatching.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_DISPATCHING_H
#define TAO_EC_DISPATCHING_H
#include "ace/pre.h"

#include "orbsvcs/RtecEventCommC.h"
#include "event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_QOS_Info;
class TAO_EC_ProxyPushSupplier;

/**
 * @class TAO_EC_Dispatching
 *
 * @brief Abstract base class for the dispatching strategies.
 *
 * The dispatching strategies.
 * The EC may be configured with different dispatching strategies,
 * for instance, it can use a pool of threads to dispatch the
 * events, or a set of queues with threads at different priorities
 * for each queue or can simply push the event to the consumer in
 * FIFO order.
 */
class TAO_RTEvent_Export TAO_EC_Dispatching
{
public:
  /// destructor...
  virtual ~TAO_EC_Dispatching (void);

  /// Initialize all the data structures, activate any internal threads,
  /// etc.
  virtual void activate (void) = 0;

  /**
   * Deactivate any internal threads and cleanup internal data
   * structures, it should only return once the threads have finished
   * their jobs.
   */
  virtual void shutdown (void) = 0;

  /**
   * The consumer represented by <proxy> should receive <event>.
   * It can use the information in <qos_info> to determine the event
   * priority (among other things).
   */
  virtual void push (TAO_EC_ProxyPushSupplier *proxy,
                     RtecEventComm::PushConsumer_ptr consumer,
                     const RtecEventComm::EventSet &event,
                     TAO_EC_QOS_Info &qos_info
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS) = 0;
  virtual void push_nocopy (TAO_EC_ProxyPushSupplier *proxy,
                            RtecEventComm::PushConsumer_ptr consumer,
                            RtecEventComm::EventSet &event,
                            TAO_EC_QOS_Info &qos_info
                            ACE_ENV_ARG_DECL_WITH_DEFAULTS) = 0;
};

// ****************************************************************

/**
 * @class TAO_EC_Reactive_Dispatching
 *
 * @brief Dispatch using the caller thread.
 *
 * The events are dispatched in FIFO ordering, using the invoking
 * thread to push the event to the consumer.
 */
class TAO_RTEvent_Export TAO_EC_Reactive_Dispatching : public TAO_EC_Dispatching
{
public:
  /// The scheduler is used to find the range of priorities and similar
  /// info.
  TAO_EC_Reactive_Dispatching (void);

  // = The EC_Dispatching methods.
  virtual void activate (void);
  virtual void shutdown (void);
  virtual void push (TAO_EC_ProxyPushSupplier *proxy,
                     RtecEventComm::PushConsumer_ptr consumer,
                     const RtecEventComm::EventSet &event,
                     TAO_EC_QOS_Info &qos_info
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  virtual void push_nocopy (TAO_EC_ProxyPushSupplier *proxy,
                            RtecEventComm::PushConsumer_ptr consumer,
                            RtecEventComm::EventSet &event,
                            TAO_EC_QOS_Info &qos_info
                            ACE_ENV_ARG_DECL_WITH_DEFAULTS);
};

#if defined (__ACE_INLINE__)
#include "EC_Dispatching.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_DISPATCHING_H */
