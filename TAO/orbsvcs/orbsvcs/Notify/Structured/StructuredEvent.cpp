// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/Structured/StructuredEvent.h"
#include "orbsvcs/Notify/PropertySeq.h"
#include "orbsvcs/Notify/Consumer.h"
#include "tao/debug.h"
#include "tao/corba.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

void
TAO_Notify_StructuredEvent_No_Copy::marshal (TAO_OutputCDR & cdr) const
{
  static const ACE_CDR::Octet STRUCTURED_CODE = MARSHAL_STRUCTURED;
  cdr.write_octet (STRUCTURED_CODE);
  cdr << (*this->notification_);
}

//static
TAO_Notify_StructuredEvent *
TAO_Notify_StructuredEvent_No_Copy::unmarshal (TAO_InputCDR & cdr)
{
  TAO_Notify_StructuredEvent * event = 0;
  CosNotification::StructuredEvent body;
  if (cdr >> body)
  {
    event = new TAO_Notify_StructuredEvent (body);
  }
  return event;
}

TAO_Notify_Event *
TAO_Notify_StructuredEvent_No_Copy::copy (void) const
{
  TAO_Notify_Event * new_event;
  ACE_NEW_THROW_EX (new_event,
                    TAO_Notify_StructuredEvent (*this->notification_),
                    CORBA::NO_MEMORY ());
  return new_event;
}

CORBA::Boolean
TAO_Notify_StructuredEvent_No_Copy::do_match (CosNotifyFilter::Filter_ptr filter) const
{
  if (TAO_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                "TAO_Notify_StructuredEvent::do_match ()\n"));

  return filter->match_structured (*this->notification_);
}

void
TAO_Notify_StructuredEvent_No_Copy::convert (CosNotification::StructuredEvent& notification) const
{
  notification = *this->notification_;
}

void
TAO_Notify_StructuredEvent_No_Copy::push (TAO_Notify_Consumer* consumer) const
{
  if (TAO_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                          "TAO_Notify_StructuredEvent::do_push ("
                          "CosNotifyComm::StructuredPushConsumer_ptr)\n"));

  consumer->push (*this->notification_);
}

void
TAO_Notify_StructuredEvent_No_Copy::push (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder) const
{
  forwarder->forward_structured (*this->notification_);
}

void
TAO_Notify_StructuredEvent_No_Copy::push_no_filtering (Event_Forwarder::StructuredProxyPushSupplier_ptr forwarder) const
{
  forwarder->forward_structured_no_filtering (*this->notification_);
}

void
TAO_Notify_StructuredEvent_No_Copy::push (Event_Forwarder::ProxyPushSupplier_ptr forwarder) const
{
  CORBA::Any any;

  TAO_Notify_Event::translate (*this->notification_, any);

  forwarder->forward_any (any);
}

void
TAO_Notify_StructuredEvent_No_Copy::push_no_filtering (Event_Forwarder::ProxyPushSupplier_ptr forwarder) const
{
  CORBA::Any any;

  TAO_Notify_Event::translate (*this->notification_, any);

  forwarder->forward_any_no_filtering (any);
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

TAO_END_VERSIONED_NAMESPACE_DECL
