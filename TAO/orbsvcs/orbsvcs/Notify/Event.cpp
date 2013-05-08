// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Notify/Event.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Event.inl"
#endif /* __ACE_INLINE__ */

#include "tao/debug.h"
#include "tao/CDR.h"
#include "orbsvcs/CosNotificationC.h"
// NOTE: unfortunately we must know about derived types to implement unmarshal
#include "orbsvcs/Notify/Any/AnyEvent.h"
#include "orbsvcs/Notify/Structured/StructuredEvent.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Event::TAO_Notify_Event (void)
: priority_ (CosNotification::Priority, CosNotification::DefaultPriority)
, timeout_ (CosNotification::Timeout)
, reliable_ (CosNotification::EventReliability, true)
, clone_ (0)
, is_on_heap_ (false)
, time_ (ACE_OS::gettimeofday ())
{
  //  if (TAO_debug_level > 0)
  //  ORBSVCS_DEBUG ((LM_DEBUG,"event:%x  created\n", this ));
}

TAO_Notify_Event::~TAO_Notify_Event ()
{
  // if (TAO_debug_level > 1)
  //  ORBSVCS_DEBUG ((LM_DEBUG,"event:%x  destroyed\n", this ));
}
void
TAO_Notify_Event::release (void)
{
  delete this;
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
/// Unmarshal an event from a CDR. (for persistence)
//static
TAO_Notify_Event *
TAO_Notify_Event::unmarshal (TAO_InputCDR & cdr)
{
  TAO_Notify_Event * result = 0;
  ACE_CDR::Octet code = 0;
  if (cdr.read_octet (code))
  {
    switch (code)
    {
    case MARSHAL_ANY:
      result = TAO_Notify_AnyEvent::unmarshal (cdr);
      break;
    case MARSHAL_STRUCTURED:
      result = TAO_Notify_StructuredEvent::unmarshal (cdr);
      break;
    default:
      ORBSVCS_ERROR ((LM_ERROR,
        ACE_TEXT ("(%P|%t) TAO_Notify_Event::unmarshal: unknown event code %d\n"),
        code));
      break;
    }
  }
  return result;
}

TAO_END_VERSIONED_NAMESPACE_DECL
