// $Id$

#include "StructuredEvent.h"

#if ! defined (__ACE_INLINE__)
#include "StructuredEvent.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_StructuredEvent, "$Id$")

#include "../PropertySeq.h"
#include "../Consumer.h"
#include "tao/debug.h"

TAO_NS_StructuredEvent::TAO_NS_StructuredEvent (const CosNotification::StructuredEvent &notification)
  : notification_ (notification), type_ (notification.header.fixed_header.event_type)
{
  TAO_NS_PropertySeq qos;

  if (qos.init (this->notification_.header.variable_header) != -1)
  {
    this->priority_.set (qos);
    this->timeout_.set (qos);
  }
}

const TAO_NS_EventType&
TAO_NS_StructuredEvent::type (void) const
{
  return this->type_;
}

TAO_NS_StructuredEvent::~TAO_NS_StructuredEvent ()
{
}

CORBA::Boolean
TAO_NS_StructuredEvent::do_match (CosNotifyFilter::Filter_ptr filter ACE_ENV_ARG_DECL)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                "TAO_Notify_StructuredEvent::do_match ()\n"));

  return filter->match_structured (this->notification_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_StructuredEvent::convert (CosNotification::StructuredEvent& notification)
{
  notification = this->notification_;
}

void
TAO_NS_StructuredEvent::push (TAO_NS_Consumer* consumer ACE_ENV_ARG_DECL) const
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                          "TAO_Notify_StructuredEvent::do_push ("
                          "CosNotifyComm::StructuredPushConsumer_ptr)\n"));

  consumer->push (notification_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_StructuredEvent::push (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL)
{
  forwarder->forward_structured (notification_ ACE_ENV_ARG_PARAMETER);
}
void
TAO_NS_StructuredEvent::push_no_filtering (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL)
{
  forwarder->forward_structured_no_filtering (notification_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_StructuredEvent::push (Event_Forwarder::ProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL)
{
  CORBA::Any any;

  TAO_NS_Event::translate (this->notification_, any);

  forwarder->forward_any (any ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_StructuredEvent::push_no_filtering (Event_Forwarder::ProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL)
{
  CORBA::Any any;

  TAO_NS_Event::translate (this->notification_, any);

  forwarder->forward_any_no_filtering (any ACE_ENV_ARG_PARAMETER);
}
