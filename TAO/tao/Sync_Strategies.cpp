// -*- C++ -*-
// $Id$

#include "tao/Sync_Strategies.h"
#include "tao/Buffering_Constraint_Policy.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"

#if !defined (__ACE_INLINE__)
# include "tao/Sync_Strategies.i"
#endif /* ! __ACE_INLINE__ */

TAO_Sync_Strategy::~TAO_Sync_Strategy (void)
{
}

ssize_t
TAO_Transport_Sync_Strategy::send (TAO_Transport &transport,
                                   TAO_Stub &,
                                   const ACE_Message_Block *message_block,
                                   const ACE_Time_Value *max_wait_time)
{
  // Immediate delegation to the transport.
  return transport.send (message_block,
                         max_wait_time);
}

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

ssize_t
TAO_Delayed_Buffering_Sync_Strategy::send (TAO_Transport &transport,
                                           TAO_Stub &stub,
                                           const ACE_Message_Block *mb,
                                           const ACE_Time_Value *max_wait_time)
{
  ACE_Message_Block *message_block =
    ACE_const_cast (ACE_Message_Block *, mb);

  ssize_t result = 0;

  // Get the message queue from the transport.
  TAO_Transport_Buffering_Queue &buffering_queue =
    transport.buffering_queue ();

  // Check if there are messages already in the queue.
  if (!buffering_queue.is_empty ())
    return TAO_Eager_Buffering_Sync_Strategy::send (transport,
                                                    stub,
                                                    message_block,
                                                    max_wait_time);

  //
  // Otherwise there were no queued messages.  We first try to send
  // the message right away.
  //

  // Actual network send.
  size_t bytes_transferred = 0;
  result = transport.send (message_block,
                           max_wait_time,
                           &bytes_transferred);

  // Cannot send completely: timed out.
  if (result == -1 &&
      errno == ETIME)
    {
      if (bytes_transferred > 0)
        {
          // If successful in sending some of the data, reset the
          // message block appropriately.
          transport.reset_sent_message (message_block,
                                        bytes_transferred);
        }

      // Queue the rest.
      return bytes_transferred +
        TAO_Eager_Buffering_Sync_Strategy::send (transport,
                                                 stub,
                                                 message_block,
                                                 max_wait_time);
    }

  // EOF or other errors.
  if (result == -1 ||
      result == 0)
    return -1;

  // Everything was successfully delivered.
  return result;
}

ssize_t
TAO_Eager_Buffering_Sync_Strategy::send (TAO_Transport &transport,
                                         TAO_Stub &stub,
                                         const ACE_Message_Block *message_block,
                                         const ACE_Time_Value *max_wait_time)
{
  ssize_t result = 0;

  // Get the message queue from the transport.
  TAO_Transport_Buffering_Queue &buffering_queue =
    transport.buffering_queue ();

  // Copy the message.
  ACE_Message_Block *copy = message_block->clone ();

  // Enqueue current message.
  result = buffering_queue.enqueue_tail (copy);

  // Enqueuing error.
  if (result == -1)
    {
      // Eliminate the copy.
      copy->release ();

      // Return error.
      return -1;
    }

  // Check if upper bound has been reached.
  if (this->buffering_constraints_reached (transport,
                                           stub,
                                           buffering_queue))
    {
      return transport.send_buffered_messages (max_wait_time);
    }

  // Hoping that this return value is meaningful or at least
  // acceptable.
  return message_block->total_length ();
}

int
TAO_Eager_Buffering_Sync_Strategy::buffering_constraints_reached (TAO_Transport &transport,
                                                                  TAO_Stub &stub,
                                                                  TAO_Transport_Buffering_Queue &buffering_queue)
{
  TAO_Buffering_Constraint_Policy *buffering_constraint_policy =
    stub.buffering_constraint ();

  if (buffering_constraint_policy == 0)
    return 1;

  // Automatically release the policy
  CORBA::Object_var auto_release = buffering_constraint_policy;

  TAO::BufferingConstraint buffering_constraint;
  buffering_constraint_policy->get_buffering_constraint (buffering_constraint);

  this->timer_check (transport,
                     buffering_constraint);

  if (buffering_constraint.mode == TAO::BUFFER_FLUSH)
    return 1;

  if (ACE_BIT_ENABLED (buffering_constraint.mode,
                       TAO::BUFFER_MESSAGE_COUNT) &&
      buffering_queue.message_count () >= buffering_constraint.message_count)
    return 1;

  if (ACE_BIT_ENABLED (buffering_constraint.mode,
                       TAO::BUFFER_MESSAGE_BYTES) &&
      buffering_queue.message_length () >= buffering_constraint.message_bytes)
    return 1;

  return 0;
}

void
TAO_Eager_Buffering_Sync_Strategy::timer_check (TAO_Transport &transport,
                                                const TAO::BufferingConstraint &buffering_constraint)
{
  if (transport.buffering_timer_id () != 0)
    {
      //
      // There is a timeout set by us, though we are not sure if we
      // still need the timeout or if the timeout value is correct or
      // not.
      //

      // Get our reactor.
      ACE_Reactor *reactor = transport.orb_core ()->reactor ();

      if (!ACE_BIT_ENABLED (buffering_constraint.mode,
                            TAO::BUFFER_TIMEOUT))
        {
          // Timeouts are no longer needed.  Cancel existing one.
          reactor->cancel_timer (transport.buffering_timer_id ());
          transport.buffering_timer_id (0);
        }
      else
        {
          ACE_Time_Value timeout =
            this->time_conversion (buffering_constraint.timeout);

          if (transport.buffering_timeout_value () == timeout)
            {
              // Timeout value is the same, nothing to be done.
            }
          else
            {
              // Timeout value has changed, reset the old timer.
              reactor->reset_timer_interval (transport.buffering_timer_id (),
                                             timeout);
            }
        }
    }
  else if (ACE_BIT_ENABLED (buffering_constraint.mode,
                            TAO::BUFFER_TIMEOUT))
    {
      // We didn't have timeouts before, but we want them now.
      ACE_Time_Value timeout =
        this->time_conversion (buffering_constraint.timeout);

      long timer_id = transport.register_for_timer_event (0, timeout, timeout);

      transport.buffering_timer_id (timer_id);
      transport.buffering_timeout_value (timeout);
    }
}

ACE_Time_Value
TAO_Eager_Buffering_Sync_Strategy::time_conversion (const TimeBase::TimeT &time)
{
  TimeBase::TimeT seconds = time / 10000000u;
  TimeBase::TimeT microseconds = (time % 10000000u) / 10;
  return ACE_Time_Value (ACE_U64_TO_U32 (seconds),
                         ACE_U64_TO_U32 (microseconds));
}

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */
