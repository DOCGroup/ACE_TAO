// $Id$

#include "Event.h"

#if ! defined (__ACE_INLINE__)
#include "Event.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_Event, "$Id$")

#include "tao/debug.h"
#include "orbsvcs/CosNotificationC.h"

TAO_Notify_Event::TAO_Notify_Event (void)
  :priority_ (CosNotification::Priority, CosNotification::DefaultPriority),
   timeout_ (CosNotification::Timeout)
{
  //  if (TAO_debug_level > 0)
  //  ACE_DEBUG ((LM_DEBUG,"event:%x  created\n", this ));
}

TAO_Notify_Event::~TAO_Notify_Event ()
{
  // if (TAO_debug_level > 1)
  //  ACE_DEBUG ((LM_DEBUG,"event:%x  destroyed\n", this ));
}

void
TAO_Notify_Event::translate (const CORBA::Any& any, CosNotification::StructuredEvent& notification)
{
  notification.remainder_of_body <<= any;
  notification.header.fixed_header.event_type.type_name = CORBA::string_dup ("%ANY");
  notification.header.fixed_header.event_type.domain_name = CORBA::string_dup ("");
}

void
TAO_Notify_Event::translate (const CosNotification::StructuredEvent& notification, CORBA::Any& any)
{
  any <<= notification;   // is the typecode set by this operation or do we need to set it explicity.
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Auto_Basic_Ptr<TAO_Notify_Event>;
template class ACE_Refcounted_Auto_Ptr<const TAO_Notify_Event, TAO_SYNCH_MUTEX>;
template class ACE_Unbounded_Queue<TAO_Notify_Event_var>;
template class ACE_Node<TAO_Notify_Event_var>;
template class ACE_Unbounded_Queue_Iterator<TAO_Notify_Event_var>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Notify_Event>
#pragma instantiate ACE_Refcounted_Auto_Ptr<const TAO_Notify_Event, TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Unbounded_Queue<TAO_Notify_Event_var>
#pragma instantiate ACE_Node<TAO_Notify_Event_var>
#pragma instantiate ACE_Unbounded_Queue_Iterator<TAO_Notify_Event_var>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
