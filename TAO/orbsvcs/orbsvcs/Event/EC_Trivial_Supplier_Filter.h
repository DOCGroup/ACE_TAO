/* -*- C++ -*- */
/**
 *  @file   EC_Trivial_Supplier_Filter.h
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

#ifndef TAO_EC_TRIVIAL_SUPPLIER_FILTER_H
#define TAO_EC_TRIVIAL_SUPPLIER_FILTER_H
#include "ace/pre.h"

#include "EC_Supplier_Filter.h"
#include "EC_Supplier_Filter_Builder.h"
#include "event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_Event_Channel_Base;
class TAO_EC_ProxyPushSupplier_Set;

// ****************************************************************

/**
 * @class TAO_EC_Trivial_Supplier_Filter
 *
 * @brief A null filtering strategy.
 *
 * This strategy does no filtering, it is useful for backends of
 * the CosEventChannel, testing and broadcasters; it uses the
 * ConsumerAdmin to find all the consumers and pushes the event to
 * all of them.
 */
class TAO_RTEvent_Export TAO_EC_Trivial_Supplier_Filter : public TAO_EC_Supplier_Filter
{
public:
  /// Constructor
  TAO_EC_Trivial_Supplier_Filter (TAO_EC_Event_Channel_Base* ec);

  // = The TAO_EC_Supplier_Filter methods.
  virtual void bind (TAO_EC_ProxyPushConsumer* consumer);
  virtual void unbind (TAO_EC_ProxyPushConsumer* consumer);
  virtual void connected (TAO_EC_ProxyPushSupplier* supplier
                          ACE_ENV_ARG_DECL);
  virtual void reconnected (TAO_EC_ProxyPushSupplier* supplier
                            ACE_ENV_ARG_DECL);
  virtual void disconnected (TAO_EC_ProxyPushSupplier* supplier
                             ACE_ENV_ARG_DECL);
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL);
  virtual void push (const RtecEventComm::EventSet& event,
                     TAO_EC_ProxyPushConsumer *consumer
                     ACE_ENV_ARG_DECL_NOT_USED);
  virtual void push_scheduled_event (RtecEventComm::EventSet &event,
                                     const TAO_EC_QOS_Info &event_info
                                     ACE_ENV_ARG_DECL);
  virtual CORBA::ULong _decr_refcnt (void);
  virtual CORBA::ULong _incr_refcnt (void);

private:
  /// The event channel, used to locate the set of consumers.
  TAO_EC_Event_Channel_Base *event_channel_;
};

// ****************************************************************

/**
 * @class TAO_EC_Trivial_Supplier_Filter_Builder
 *
 * @brief Create a single Trivial_Supplier_Filter.
 *
 * This Factory creates a single Trivial_Supplier_Filter that is
 * used by all the suppliers (i.e. ProxyConsumers) of an event
 * channel.
 */
class TAO_RTEvent_Export TAO_EC_Trivial_Supplier_Filter_Builder : public TAO_EC_Supplier_Filter_Builder
{
public:
  /// constructor....
  TAO_EC_Trivial_Supplier_Filter_Builder (TAO_EC_Event_Channel_Base* ec);

  // = The TAO_EC_Supplier_Filter_Builder methods...
  virtual TAO_EC_Supplier_Filter*
      create (RtecEventChannelAdmin::SupplierQOS& qos);
  virtual void
      destroy (TAO_EC_Supplier_Filter *filter);

private:
  /// The filter....
  TAO_EC_Trivial_Supplier_Filter filter_;
};

#if defined (__ACE_INLINE__)
#include "EC_Trivial_Supplier_Filter.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_TRIVIAL_SUPPLIER_FILTER_H */
