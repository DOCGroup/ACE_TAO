// $Id$

#include "StructuredEvent.h"
#include "../Consumer.h"
#include "tao/debug.h"

#if ! defined (__ACE_INLINE__)
#include "StructuredEvent.inl"
#endif /* __ACE_INLINE__ */


ACE_RCSID(RT_Notify, TAO_NS_StructuredEvent, "$Id$")

TAO_NS_StructuredEvent::TAO_NS_StructuredEvent (const CosNotification::StructuredEvent &notification)
  : notification_ (notification), type_ (notification.header.fixed_header.event_type)
{
}

TAO_NS_StructuredEvent::~TAO_NS_StructuredEvent ()
{
}

const TAO_NS_EventType&
TAO_NS_StructuredEvent::type (void) const
{
  return this->type_;
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
TAO_NS_StructuredEvent::push (Notify_Internal::Event_Forwarder_ptr forwarder ACE_ENV_ARG_DECL)
{
  forwarder->forward (notification_ ACE_ENV_ARG_PARAMETER);
}
