/* -*- C++ -*- */
/**
 *  @file   EC_Scheduling_Strategy.h
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

#ifndef TAO_EC_SCHEDULING_STRATEGY_H
#define TAO_EC_SCHEDULING_STRATEGY_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecBaseC.h"
#include "orbsvcs/RtecEventCommC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_ProxyPushConsumer;
class TAO_EC_ProxyPushSupplier;
class TAO_EC_Supplier_Filter;
class TAO_EC_QOS_Info;

/**
 * @class TAO_EC_Scheduling_Strategy
 *
 * @brief Define the interface for the scheduling strategy
 *
 * The scheduling strategy controls the actions that the event
 * channel must take to update the dependency information in the
 * scheduler and to query the scheduler for the priority of each
 * event pushed by a supplier.
 * The base
 */
class TAO_RTEvent_Export TAO_EC_Scheduling_Strategy
{
public:
  /// Destructor
  virtual ~TAO_EC_Scheduling_Strategy (void);

  /// Add all the dependencies between @a supplier and @a consumer
  virtual void add_proxy_supplier_dependencies (
      TAO_EC_ProxyPushSupplier *supplier,
      TAO_EC_ProxyPushConsumer *consumer
      ACE_ENV_ARG_DECL) = 0;

  /// Schedule an event set and deliver them to the filter in the
  /// desired order and grouping.
  virtual void schedule_event (const RtecEventComm::EventSet &event,
                               TAO_EC_ProxyPushConsumer *consumer,
                               TAO_EC_Supplier_Filter *filter
                               ACE_ENV_ARG_DECL) = 0;

};

#if defined (__ACE_INLINE__)
#include "EC_Scheduling_Strategy.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_EC_SCHEDULING_STRATEGY_H */
