/* -*- C++ -*- */
/**
 *  @file   EC_Supplier_Filter.h
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

#ifndef TAO_EC_SUPPLIER_FILTER_H
#define TAO_EC_SUPPLIER_FILTER_H
#include "ace/pre.h"

#include "orbsvcs/RtecEventCommC.h"
#include "orbsvcs/ESF/ESF_Worker.h"
#include "event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_ProxyPushSupplier;
class TAO_EC_ProxyPushConsumer;
class TAO_EC_QOS_Info;

/**
 * @class TAO_EC_Supplier_Filter
 *
 * @brief The strategy to filter close to the supplier.
 *
 * After an event is received by the a ProxyPushConsumer it must
 * be dispatched to the right set of ProxyPushSuppliers;
 * determining this set is the task of this class.
 * Notice that this is in fact a filter, and enforces (in part)
 * the subscriptions and publications of the Event Service
 * clients.
 * Several implementations are possible:
 * - Each ProxyPushConsumer keeps a list of ProxyPushSuppliers,
 * using the subscriptions and publications to find potential
 * matches.
 * - Each ProxyPushConsumer keeps several such lists, indexed by
 * event type and/or source, this has the advantage of further
 * minimizing the set of ProxyPushSuppliers invoked.
 * - A single list of consumers is kept (global for the event
 * channel), such a list results is faster updates and requires
 * an smaller memory footprint.
 * - Multiple global lists are kept, indexed by type and/or
 * source, this is a tradeoff between the solutions above.
 * - No list is kept, the events are sent to the consumers which
 * must filter out what they want, this is good when no
 * filtering is wanted or when the amount of filtering is coarse
 * i.e. each event goes to a large subset of the
 * ProxyPushSuppliers.
 * Different applications will require to use different
 * implementations of this class; as usual the EC_Factory will
 * create the right instance.
 */
class TAO_RTEvent_Export TAO_EC_Supplier_Filter
{
public:
  /// Destructor
  virtual ~TAO_EC_Supplier_Filter (void);

  /**
   * Whenever a ProxyPushConsumer is initialized it calls this method
   * to inform the Supplier_Filter of its identity.
   * Strategies that do not keep ProxyPushConsumer specific
   * information, or that are shared between multiple
   * ProxyPushConsumers can ignore this message.
   */
  virtual void bind (TAO_EC_ProxyPushConsumer* consumer) = 0;

  /**
   * Wheneve a ProxyPushConsumer is about to be destroyed it calls
   * this method to inform the Supplier_Filter that it should
   * release any resources related to it.
   * Supplier_Filter strategies that are bound to a particular
   * ProxyConsumer can use this opportunity to destroy themselves;
   * filter strategies that do not keep ProxyPushConsumer specific
   * information can simply ignore the message.
   */
  virtual void unbind (TAO_EC_ProxyPushConsumer* consumer) = 0;

  /// Concrete implementations can use this methods to keep track of
  /// the consumers interested in this events.
  virtual void connected (TAO_EC_ProxyPushSupplier *supplier
                          ACE_ENV_ARG_DECL) = 0;
  virtual void reconnected (TAO_EC_ProxyPushSupplier *supplier
                          ACE_ENV_ARG_DECL) = 0;
  virtual void disconnected (TAO_EC_ProxyPushSupplier *supplier
                             ACE_ENV_ARG_DECL) = 0;

  /// The event channel is shutting down.
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL) = 0;

  /// The ProxyPushConsumer delegates on this class to actually send
  /// the event.
  virtual void push (const RtecEventComm::EventSet &event,
                     TAO_EC_ProxyPushConsumer *consumer
                     ACE_ENV_ARG_DECL) = 0;

  /// Events are first scheduled by the TAO_EC_Scheduling_Strategy,
  /// and then pushed through this class again.
  virtual void push_scheduled_event (RtecEventComm::EventSet &event,
                                     const TAO_EC_QOS_Info &event_info
                                     ACE_ENV_ARG_DECL) = 0;

  /// Increment and decrement the reference count, locking must be
  /// provided by the user.
  virtual CORBA::ULong _incr_refcnt (void) = 0;
  virtual CORBA::ULong _decr_refcnt (void) = 0;
};

// ****************************************************************

class TAO_EC_Filter_Worker : public TAO_ESF_Worker<TAO_EC_ProxyPushSupplier>
{
public:
  TAO_EC_Filter_Worker (RtecEventComm::EventSet &event,
                        const TAO_EC_QOS_Info &event_info);

  virtual void work (TAO_EC_ProxyPushSupplier *supplier
                     ACE_ENV_ARG_DECL);

private:
  /// The event we push on each case, use a reference to avoid copies.
  RtecEventComm::EventSet &event_;

  /// The QoS info propagated on each event.
  const TAO_EC_QOS_Info &event_info_;
};

#if defined (__ACE_INLINE__)
#include "EC_Supplier_Filter.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_SUPPLIER_FILTER_H */
