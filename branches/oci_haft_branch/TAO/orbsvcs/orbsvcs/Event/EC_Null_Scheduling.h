/* -*- C++ -*- */
/**
 *  @file   EC_Null_Scheduling.h
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

#ifndef TAO_EC_NULL_SCHEDULING_H
#define TAO_EC_NULL_SCHEDULING_H
#include /**/ "ace/pre.h"

#include "EC_Scheduling_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_EC_Null_Scheduling
 *
 * @brief A scheduling strategy that separates event groups and
 * delivers one event at a time.
 */
class TAO_RTEvent_Export TAO_EC_Null_Scheduling : public TAO_EC_Scheduling_Strategy
{
public:
  /// Constructor.
  TAO_EC_Null_Scheduling (void);

  // Read EC_Scheduling_Strategy.h for more details
  virtual void add_proxy_supplier_dependencies (
      TAO_EC_ProxyPushSupplier *supplier,
      TAO_EC_ProxyPushConsumer *consumer
      ACE_ENV_ARG_DECL);
  virtual void schedule_event (const RtecEventComm::EventSet &event,
                               TAO_EC_ProxyPushConsumer *consumer,
                               TAO_EC_Supplier_Filter *filter
                               ACE_ENV_ARG_DECL);

private:
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Null_Scheduling
                              (const TAO_EC_Null_Scheduling&))
  ACE_UNIMPLEMENTED_FUNC (TAO_EC_Null_Scheduling& operator=
                              (const TAO_EC_Null_Scheduling&))
};

#if defined (__ACE_INLINE__)
#include "EC_Null_Scheduling.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_EC_NULL_SCHEDULING_H */
