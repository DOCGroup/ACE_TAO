// $Id$

#include "Delivery_Method.h"

#if ! defined (__ACE_INLINE__)
#include "Delivery_Method.inl"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/Time_Utilities.h"
#include "tao/debug.h"
#include "ace/OS_NS_sys_time.h"

//#define DEBUG_LEVEL 9
#define DEBUG_LEVEL TAO_debug_level

ACE_RCSID(RT_Notify, TAO_Notify_Method_Request, "$Id$")

namespace TAO_NOTIFY
{

//////////////////
// Delivery Method
Delivery_Method::Delivery_Method (const TAO_NOTIFY::Delivery_Request_Ptr & delivery_request)
  : TAO_Notify_Method_Request ()
  , delivery_request_ (delivery_request)
{
  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Delivery Method: constructor\n")
      ));
  // Set the parameters that affect queuing in the message queue.
  // The ACE_Message_Block priorities go from 0 (lowest) to ULONG_MAX
  // (highest), while the Notification Events go from -32767 (lowest,
  // even though CORBA::Short goes to -32768) to 32767 (highest).

  // Convert to CORBA::Long to preserve the sign.  Conversion to
  // unsigned long will happen automatically and we do not have to worry
  // about losing the number in the addition since priority () returns a
  // CORBA::Short.
  const TAO_Notify_Event_Ptr pevent = this->delivery_request_->event ();
  this->msg_priority ((CORBA::Long)pevent->priority ().value () +
                      PRIORITY_BASE);

  // The deadline time for the message block is absolute, while the
  // timeout for the event is relative to the time it was received.
  // So, we do a little conversion and set it on the message block (us)

  TAO_Notify_Property_Time& timeout = pevent->timeout ();

  if (timeout.is_valid () && timeout.value() != 0)
  {
    ACE_Time_Value tv = ACE_OS::gettimeofday();
    tv += ORBSVCS_Time::to_Time_Value(timeout.value());
    this->msg_deadline_time (tv);
  }
}

Delivery_Method::Delivery_Method (const Delivery_Method & rhs)
  : TAO_Notify_Method_Request ()
  , delivery_request_ (rhs.delivery_request_)
{
}

Delivery_Method::~Delivery_Method ()
{
  if (DEBUG_LEVEL > 8) ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t) Delivery Method: destructor\n")
      ));
}

const TAO_Notify_Event&
Delivery_Method::event (void) const
{
  return * this->delivery_request_->event ();
}

} //namespace TAO_NOTIFY

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
