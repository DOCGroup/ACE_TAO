// $Id$

#include "orbsvcs/Notify/Method_Request.h"

#include "orbsvcs/Time_Utilities.h"
#include "ace/OS_NS_sys_time.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Method_Request::~TAO_Notify_Method_Request (void)
{
}

TAO_Notify_Method_Request_Queueable *
TAO_Notify_Method_Request_Queueable::copy (void)
{
  return this;
}

TAO_Notify_Method_Request_Queueable::TAO_Notify_Method_Request_Queueable()
{
}

TAO_Notify_Method_Request_Queueable::TAO_Notify_Method_Request_Queueable(const TAO_Notify_Event * event)
{
  this->init (event);
}

void
TAO_Notify_Method_Request_Queueable::init (const TAO_Notify_Event* event)
{
  ACE_ASSERT( event != 0 );
  // Set the parameters that affect queuing in the message queue.
  // The ACE_Message_Block priorities go from 0 (lowest) to ULONG_MAX
  // (highest), while the Notification Events go from -32767 (lowest,
  // even though CORBA::Short goes to -32768) to 32767 (highest).

  // Convert to CORBA::Long to preserve the sign.  Conversion to
  // unsigned long will happen automatically and we do not have to worry
  // about losing the number in the addition since priority () returns a
  // CORBA::Short.
  this->msg_priority ((CORBA::Long)event->priority ().value () + PRIORITY_BASE);

  // The deadline time for the message block is absolute, while the
  // timeout for the event is relative to the time it was received.
  // So, we do a little conversion and set it on the message block (us)

  const TAO_Notify_Property_Time& timeout = event->timeout ();

  if (timeout.is_valid () && timeout != 0)
  {
    ACE_Time_Value deadline;
    ORBSVCS_Time::TimeT_to_Time_Value(deadline, timeout.value());
    deadline += ACE_OS::gettimeofday ();
    this->msg_deadline_time (deadline);
  }

  this->time_ = event->creation_time ();
}

const ACE_Time_Value&
TAO_Notify_Method_Request_Queueable::creation_time (void) const
{
  return this->time_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
