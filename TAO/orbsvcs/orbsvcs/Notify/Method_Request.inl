// $Id$

ACE_INLINE void
TAO_NS_Method_Request::init (const TAO_NS_Event_var& event)
{
  // Set the parameters that affect queuing in the message queue.
  // The ACE_Message_Block priorities go from 0 (lowest) to ULONG_MAX
  // (highest), while the Notification Events go from -32767 (lowest,
  // even though CORBA::Short goes to -32768) to 32767 (highest).

  // Convert to CORBA::Long to preserve the sign.  Conversion to
  // unsigned long will happen automatically and we do not have to worry
  // about losing the number in the addition since priority () returns a
  // CORBA::Short.
  this->msg_priority ((CORBA::Long)event->priority ().value () +
                      PRIORITY_BASE);

  // The deadline time for the message block is absolute, while the
  // timeout for the event is relative to the time it was received.
  // So, we do a little conversion and set it on the message block (us)

  const TAO_NS_Property_Time& timeout = event->timeout ();

  if (timeout.is_valid () && timeout != 0)
    {
      // I am multiplying timeout () by 1 because it returns a
      // CORBA::ULongLong, which on some platforms doesn't automatically
      // convert to the long that the ACE_Time_Value expects.  The /
      // operator returns a 32-bit integer.
      ACE_Time_Value current = ACE_OS::gettimeofday () +
# if defined (ACE_CONFIG_WIN32_H)
        ACE_Time_Value (
                        ACE_static_cast (long,
                                         timeout.value ()));
# else
      ACE_Time_Value (timeout.value () / 1);
# endif /* ACE_CONFIG_WIN32_H */
      this->msg_deadline_time (current);
    }
}
