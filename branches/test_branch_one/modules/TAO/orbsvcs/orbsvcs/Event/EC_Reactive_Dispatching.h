// -*- C++ -*-

/**
 *  @file   EC_Reactive_Dispatching.h
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

#ifndef TAO_EC_REACTIVE_DISPATCHING_H
#define TAO_EC_REACTIVE_DISPATCHING_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Event/EC_Dispatching.h"

#include /**/ "orbsvcs/Event/event_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_EC_Reactive_Dispatching
 *
 * @brief Dispatch using the caller thread.
 *
 * The events are dispatched in FIFO ordering, using the invoking
 * thread to push the event to the consumer.
 */
class TAO_RTEvent_Serv_Export TAO_EC_Reactive_Dispatching : public TAO_EC_Dispatching
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
                     TAO_EC_QOS_Info &qos_info);
  virtual void push_nocopy (TAO_EC_ProxyPushSupplier *proxy,
                            RtecEventComm::PushConsumer_ptr consumer,
                            RtecEventComm::EventSet &event,
                            TAO_EC_QOS_Info &qos_info);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_EC_REACTIVE_DISPATCHING_H */
