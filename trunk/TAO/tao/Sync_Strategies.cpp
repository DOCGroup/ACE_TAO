// -*- C++ -*-
// $Id$

#include "tao/Sync_Strategies.h"

#include "tao/Buffering_Constraint_Policy.h"
#include "tao/Stub.h"

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
                                   ACE_Time_Value *max_wait_time)
{
  // Immediate delegation to the transport.
  return transport.send (message_block,
                         max_wait_time);
}

#if defined (TAO_HAS_CORBA_MESSAGING)

ssize_t
TAO_None_Sync_Strategy::send (TAO_Transport &transport,
                              TAO_Stub &stub,
                              const ACE_Message_Block *message_block,
                              ACE_Time_Value *max_wait_time)
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
  if (this->buffering_constraints_reached (stub,
                                           buffering_queue))
    {
      // Copy the timeout value since we don't want to change it.  The
      // caller will change appropriately.
      ACE_Time_Value timeout_value;
      ACE_Time_Value *timeout = 0;
      if (max_wait_time)
        {
          timeout_value = *max_wait_time;
          timeout = &timeout_value;
        }

      ACE_Countdown_Time countdown (timeout);

      // Flush all queued messages.
      while (!buffering_queue.is_empty ())
        {
          // Get the first message from the queue.
          ACE_Message_Block *queued_message = 0;
          result = buffering_queue.peek_dequeue_head (queued_message);

          // @@ What to do here on failures?
          ACE_ASSERT (result != -1);

          // Actual network send.
          result = transport.send (queued_message,
                                   timeout);

          // Socket closed.
          if (result == 0)
            {
              this->dequeue_all (buffering_queue);
              return -1;
            }

          // Cannot send.
          if (result == -1)
            {
              // Timeout.
              if (errno == ETIME)
                {
                  // Since we queue up the message, this is not an
                  // error.  We can try next time around.
                  return 0;
                }
              // Non-timeout error.
              else
                {
                  this->dequeue_all (buffering_queue);
                  return -1;
                }
            }

          ssize_t total_length = queued_message->total_length ();

          // If successful in sending the complete queued message.
          if (result == total_length)
            {
              this->dequeue_head (buffering_queue);
              countdown.update ();
            }

          // Partial send (re-adjust pointers without dequeuing the
          // message). This is not an error.  We can try next time
          // around.
          else
            {
              this->reset_queued_message (buffering_queue,
                                          queued_message,
                                          result);
              return 0;
            }
        }
    }

  // I am hoping this return value is meaningful.
  return message_block->total_length ();
}

void
TAO_None_Sync_Strategy::dequeue_head (TAO_Transport_Buffering_Queue &buffering_queue)
{
  ACE_Message_Block *message_block = 0;

  // Remove from the head of the queue.
  int result = buffering_queue.dequeue_head (message_block);

  // @@ What to do here on failures?
  ACE_ASSERT (result != -1);
  ACE_UNUSED_ARG (result);

  // Release the memory.
  message_block->release ();
}

void
TAO_None_Sync_Strategy::dequeue_all (TAO_Transport_Buffering_Queue &buffering_queue)
{
  // Flush all queued messages.
  while (!buffering_queue.is_empty ())
    this->dequeue_head (buffering_queue);
}

void
TAO_None_Sync_Strategy::reset_queued_message (TAO_Transport_Buffering_Queue &buffering_queue,
                                              ACE_Message_Block *message_block,
                                              size_t bytes_delivered)
{
  for (ACE_Message_Block *i = message_block;
       i != 0 && bytes_delivered != 0;
       i = i->cont ())
    {
      if (i->length () > bytes_delivered)
        {
          i->rd_ptr (bytes_delivered);
          bytes_delivered = 0;
        }
      else
        {
          bytes_delivered -= i->length ();
          i->rd_ptr (i->length ());
        }
    }

  buffering_queue.message_length (buffering_queue.message_length () - bytes_delivered);
}

int
TAO_None_Sync_Strategy::buffering_constraints_reached (TAO_Stub &stub,
                                                       TAO_Transport_Buffering_Queue &buffering_queue)
{
  TAO_Buffering_Constraint_Policy *buffering_constraint_policy =
    stub.buffering_constraint ();

  if (buffering_constraint_policy == 0)
    return 1;

  TAO::BufferingConstraint buffering_constraint =
    buffering_constraint_policy->buffering_constraint ();

  if (buffering_constraint.mode == TAO::BUFFER_NONE)
    return 1;

  if (ACE_BIT_ENABLED (buffering_constraint.mode, TAO::BUFFER_MESSAGE_COUNT) &&
      buffering_queue.message_count () >= buffering_constraint.message_count)
    return 1;

  if (ACE_BIT_ENABLED (buffering_constraint.mode, TAO::BUFFER_MESSAGE_BYTES) &&
      buffering_queue.message_length () >= buffering_constraint.message_bytes)
    return 1;

  return 0;
}

int
TAO_Flush_Sync_Strategy::buffering_constraints_reached (TAO_Stub &,
                                                        TAO_Transport_Buffering_Queue &)
{
  // Always yes.
  return 1;
}

#endif /* TAO_HAS_CORBA_MESSAGING */
