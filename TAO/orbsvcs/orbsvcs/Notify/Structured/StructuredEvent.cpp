// $Id$

#include "StructuredEvent.h"

#if ! defined (__ACE_INLINE__)
#include "StructuredEvent.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_Notify_StructuredEvent, "$Id$")

#include "../PropertySeq.h"
#include "../Consumer.h"
#include "tao/debug.h"

TAO_Notify_StructuredEvent_No_Copy::TAO_Notify_StructuredEvent_No_Copy (const CosNotification::StructuredEvent &notification)
  : notification_ (&notification), type_ (notification.header.fixed_header.event_type)
{
  const CosNotification::PropertySeq& prop_seq = notification.header.variable_header;

  for (CORBA::ULong i = 0; i < prop_seq.length (); ++i)
    {
      if (ACE_OS::strcmp (prop_seq[i].name.in (), CosNotification::Priority) == 0)
        this->priority_.set (prop_seq[i].value);
      else if (ACE_OS::strcmp (prop_seq[i].name.in (), CosNotification::Timeout) == 0)
        this->timeout_.set (prop_seq[i].value);
    }
}

const TAO_Notify_EventType&
TAO_Notify_StructuredEvent_No_Copy::type (void) const
{
  return this->type_;
}

TAO_Notify_StructuredEvent_No_Copy::~TAO_Notify_StructuredEvent_No_Copy ()
{
}

TAO_Notify_Event*
TAO_Notify_StructuredEvent_No_Copy::copy (ACE_ENV_SINGLE_ARG_DECL) const
{
  TAO_Notify_Event* copy;

  ACE_NEW_THROW_EX (copy,
                    TAO_Notify_StructuredEvent (*this->notification_),
                    CORBA::NO_MEMORY ());

  return copy;
}

CORBA::Boolean
TAO_Notify_StructuredEvent_No_Copy::do_match (CosNotifyFilter::Filter_ptr filter ACE_ENV_ARG_DECL) const
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                "TAO_Notify_StructuredEvent::do_match ()\n"));

  return filter->match_structured (*this->notification_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_StructuredEvent_No_Copy::convert (CosNotification::StructuredEvent& notification) const
{
  notification = *this->notification_;
}

void
TAO_Notify_StructuredEvent_No_Copy::push (TAO_Notify_Consumer* consumer ACE_ENV_ARG_DECL) const
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                          "TAO_Notify_StructuredEvent::do_push ("
                          "CosNotifyComm::StructuredPushConsumer_ptr)\n"));

  consumer->push (*this->notification_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_StructuredEvent_No_Copy::push (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) const
{
  forwarder->forward_structured (*this->notification_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_StructuredEvent_No_Copy::push_no_filtering (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) const
{
  forwarder->forward_structured_no_filtering (*this->notification_ ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_StructuredEvent_No_Copy::push (Event_Forwarder::ProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) const
{
  CORBA::Any any;

  TAO_Notify_Event::translate (*this->notification_, any);

  forwarder->forward_any (any ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_StructuredEvent_No_Copy::push_no_filtering (Event_Forwarder::ProxyPushSupplier_ptr forwarder ACE_ENV_ARG_DECL) const
{
  CORBA::Any any;

  TAO_Notify_Event::translate (*this->notification_, any);

  forwarder->forward_any_no_filtering (any ACE_ENV_ARG_PARAMETER);
}

/*****************************************************************************************************/

TAO_Notify_StructuredEvent::TAO_Notify_StructuredEvent (const CosNotification::StructuredEvent& notification)
  : TAO_Notify_StructuredEvent_No_Copy (notification)
    , notification_copy (notification)
{
  this->notification_ = &notification_copy;
}

TAO_Notify_StructuredEvent::~TAO_Notify_StructuredEvent ()
{
}
