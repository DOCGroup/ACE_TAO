//$Id$

#include "Notify_Command.h"
#include "Notify_Event.h"

ACE_INLINE
TAO_Notify_Command::TAO_Notify_Command (TAO_Notify_Event_Processor* event_processor, TAO_Notify_Event* event)
  :event_processor_ (event_processor),
   event_ (event)
{
  // Set the parameters that affect queuing in the message queue.
  // The ACE_Message_Block priorities go from 0 (lowest) to ULONG_MAX
  // (highest), while the Notification Events go from -32767 (lowest,
  // even though CORBA::Short goes to -32768) to 32767 (highest).
  if (this->event_)
    {
      // Convert to CORBA::Long to preserve the sign.  Conversion to
      // unsigned long will happen automatically and we do not have to worry
      // about losing the number in the addition since priority () returns a
      // CORBA::Short.
      this->msg_priority ((CORBA::Long)this->event_->priority () +
                          TAO_NOTIFY_COMMAND_PRIORITY_BASE);

      // The deadline time for the message block is absolute, while the
      // timeout for the event is relative to the time it was received.
      // So, we do a little conversion and set it on the message block (us)
      if (this->event_->timeout () != 0)
        {
          // I am multiplying timeout () by 1 because it returns a
          // CORBA::ULongLong, which on some platforms doesn't automatically
          // convert to the long that the ACE_Time_Value expects.  The /
          // operator returns a 32-bit integer.
          ACE_Time_Value current = ACE_OS::gettimeofday () +
# if defined (ACE_CONFIG_WIN32_H)
                          ACE_Time_Value (
                               ACE_static_cast (long,
                                                this->event_->timeout ()));
# else
                          ACE_Time_Value (this->event_->timeout () / 1);
# endif /* ACE_CONFIG_WIN32_H */
          this->msg_deadline_time (current);
        }
    }
}

ACE_INLINE
TAO_Notify_Command::~TAO_Notify_Command ()
{
}

ACE_INLINE CORBA::Short
TAO_Notify_Command::priority (void) const
{
  return (CORBA::Short)(this->msg_priority () -
                        TAO_NOTIFY_COMMAND_PRIORITY_BASE);
}
