// $Id$

#include "AnyEvent.h"

#if ! defined (__ACE_INLINE__)
#include "AnyEvent.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_AnyEvent, "$id$")

#include "../Consumer.h"
#include "tao/debug.h"

TAO_NS_EventType TAO_NS_AnyEvent_No_Copy::event_type_;

TAO_NS_AnyEvent_No_Copy::TAO_NS_AnyEvent_No_Copy (const CORBA::Any &event)
  : event_ (&event)
{
}

TAO_NS_AnyEvent_No_Copy::~TAO_NS_AnyEvent_No_Copy ()
{
}

TAO_NS_Event*
TAO_NS_AnyEvent_No_Copy::copy (ACE_ENV_SINGLE_ARG_DECL) const
{
  TAO_NS_Event* copy;

  ACE_NEW_THROW_EX (copy,
                    TAO_NS_AnyEvent (*this->event_),
                    CORBA::NO_MEMORY ());

  return copy;
}

const TAO_NS_EventType&
TAO_NS_AnyEvent_No_Copy::type (void) const
{
  return this->event_type_;
}

void
TAO_NS_AnyEvent_No_Copy::convert (CosNotification::StructuredEvent& notification) const
{
  TAO_NS_Event::translate (*this->event_, notification);
}

CORBA::Boolean
TAO_NS_AnyEvent_No_Copy::do_match (CosNotifyFilter::Filter_ptr filter ACE_ENV_ARG_DECL) const
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                "TAO_Notify_AnyEvent::do_match ()\n"));

  return filter->match(*this->event_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_AnyEvent_No_Copy::push (TAO_NS_Consumer* consumer ACE_ENV_ARG_DECL) const
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                "TAO_Notify_AnyEvent::push \n"));

  consumer->push (*this->event_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_AnyEvent_No_Copy::push (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) const
{
  CosNotification::StructuredEvent notification;

  TAO_NS_Event::translate (*this->event_, notification);

  forwarder->forward_structured (notification ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_AnyEvent_No_Copy::push_no_filtering (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) const
{
  CosNotification::StructuredEvent notification;

  TAO_NS_Event::translate (*this->event_, notification);

  forwarder->forward_structured_no_filtering (notification ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_AnyEvent_No_Copy::push (Event_Forwarder::ProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) const
{
  forwarder->forward_any (*this->event_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_AnyEvent_No_Copy::push_no_filtering (Event_Forwarder::ProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) const
{
  forwarder->forward_any_no_filtering (*this->event_ ACE_ENV_ARG_PARAMETER);
}

/*****************************************************************************************************/

TAO_NS_AnyEvent::TAO_NS_AnyEvent (const CORBA::Any &event)
  : TAO_NS_AnyEvent_No_Copy (event)
    , any_copy_ (event)
{
  this->event_ = &any_copy_;
}

TAO_NS_AnyEvent::~TAO_NS_AnyEvent ()
{
}
