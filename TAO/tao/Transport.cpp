// -*- C++ -*-
// $Id$

#include "Transport.h"

#include "Exception.h"
#include "ORB_Core.h"
#include "Client_Strategy_Factory.h"
#include "Wait_Strategy.h"
#include "Transport_Mux_Strategy.h"
#include "Stub.h"
#include "Sync_Strategies.h"
#include "Queued_Message.h"
#include "Flushing_Strategy.h"

#include "ace/Message_Block.h"

#if !defined (__ACE_INLINE__)
# include "Transport.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Transport, "$Id$")

// Constructor.
TAO_Transport::TAO_Transport (CORBA::ULong tag,
                              TAO_ORB_Core *orb_core)
  : tag_ (tag)
  , orb_core_ (orb_core)
  , bidirectional_flag_ (-1)
  , head_ (0)
  , tail_ (0)
  , current_message_ (0)
{
  TAO_Client_Strategy_Factory *cf =
    this->orb_core_->client_factory ();

  // Create WS now.
  this->ws_ = cf->create_wait_strategy (this);

  // Create TMS now.
  this->tms_ = cf->create_transport_mux_strategy (this);
}

TAO_Transport::~TAO_Transport (void)
{
  delete this->ws_;
  this->ws_ = 0;

  delete this->tms_;
  this->tms_ = 0;

  //  delete this->buffering_queue_;

  for (TAO_Queued_Message *i = this->head_; i != 0; i = i->next ())
    {
      // @@ This is a good point to insert a flag to indicate that a
      // CloseConnection message was successfully received.
      i->connection_closed ();

      i->destroy ();
    }
}

#if 0
ssize_t
TAO_Transport::send_or_buffer (TAO_Stub *stub,
                               int two_way,
                               const ACE_Message_Block *message_block,
                               const ACE_Time_Value *max_wait_time)
{
  if (stub == 0 || two_way)
    return this->send (message_block, max_wait_time);

  TAO_Sync_Strategy &sync_strategy = stub->sync_strategy ();

  return sync_strategy.send (*this,
                             *stub,
                             message_block,
                             max_wait_time);
}

ssize_t
TAO_Transport::send_buffered_messages (const ACE_Time_Value *max_wait_time)
{
  // Make sure we have a buffering queue and there are messages in it.
  if (this->buffering_queue_ == 0 ||
      this->buffering_queue_->is_empty ())
    return 1;

  // Get the first message from the queue.
  ACE_Message_Block *queued_message = 0;
  ssize_t result = this->buffering_queue_->peek_dequeue_head (queued_message);

  // @@ What to do here on failures?
  ACE_ASSERT (result != -1);

  // Actual network send.
  size_t bytes_transferred = 0;
  result = this->send (queued_message,
                       max_wait_time,
                       &bytes_transferred);

  // Cannot send completely: timed out.
  if (result == -1 &&
      errno == ETIME)
    {
      if (bytes_transferred > 0)
        {
          // If successful in sending some of the data, reset the
          // queue appropriately.
          this->reset_queued_message (queued_message,
                                      bytes_transferred);

          // Indicate some success.
          return bytes_transferred;
        }

      // Since we queue up the message, this is not an error.  We can
      // try next time around.
      return 1;
    }

  // EOF or other errors.
  if (result == -1 ||
      result == 0)
    {
      this->dequeue_all ();
      return -1;
    }

  // If successful in sending data, reset the queue appropriately.
  this->reset_queued_message (queued_message,
                              bytes_transferred);

  // Everything was successfully delivered.
  return result;
}

void
TAO_Transport::reset_sent_message (ACE_Message_Block *message_block,
                                   size_t bytes_delivered)
{
  this->reset_message (message_block,
                       bytes_delivered,
                       0);
}

void
TAO_Transport::reset_queued_message (ACE_Message_Block *message_block,
                                     size_t bytes_delivered)
{
  this->reset_message (message_block,
                       bytes_delivered,
                       1);
}

void
TAO_Transport::reset_message (ACE_Message_Block *message_block,
                              size_t bytes_delivered,
                              int queued_message)
{
  while (bytes_delivered != 0)
    {
      // Our current message block chain.
      ACE_Message_Block *current_message_block = message_block;

      int completely_delivered_current_message_block_chain = 0;

      while (current_message_block != 0 &&
             bytes_delivered != 0)
        {
          size_t current_message_block_length =
            current_message_block->length ();

          int completely_delivered_current_message_block =
            bytes_delivered >= current_message_block_length;

          size_t adjustment_size =
            ACE_MIN (current_message_block_length, bytes_delivered);

          // Reset according to send size.
          current_message_block->rd_ptr (adjustment_size);

          // If queued message, adjust the queue.
          if (queued_message)
            // Hand adjust <message_length>.
            this->buffering_queue_->message_length (
              this->buffering_queue_->message_length () - adjustment_size);

          // Adjust <bytes_delivered>.
          bytes_delivered -= adjustment_size;

          if (completely_delivered_current_message_block)
            {
              // Next message block in the continuation chain.
              current_message_block = current_message_block->cont ();

              if (current_message_block == 0)
                completely_delivered_current_message_block_chain = 1;
            }
        }

      if (completely_delivered_current_message_block_chain)
        {
          // Go to the next message block chain.
          message_block = message_block->next ();

          // If queued message, adjust the queue.
          if (queued_message)
            // Release this <current_message_block>.
            this->dequeue_head ();
        }
    }
}
#endif /* 0 */

int
TAO_Transport::handle_output ()
{
  // The reactor is asking us to send more data, first check if
  // there is a current message that needs more sending:
  int result = this->send_current_message ();
  if (result == 0)
    return 0;

  if (result > 0)
    {
      // ... there is no current message or it was completely
      // sent, time to check the queue....
      result = this->dequeue_next_message ();
      if (result == 0)
        return 0;
    }
  // else { there was an error.... }

  if (result > 0)
    {
      // ... no more data to send ... remove ourselves from the
      // reactor ...
      result = this->cancel_output ();
    }

  if (result == -1)
    return -1; // There was an error, return -1 so the Reactor
  else if (result == 0)
    return 0;  // There is no more data or socket blocked, just return 0

  // else if (result > 0)
  // There is more data to be sent, don't try right now, let the
  // reactor handle it, because it can handle starvation better
  return 0;
}

int
TAO_Transport::send_current_message (void)
{
  if (this->current_message_ == 0)
    return 1;

  size_t byte_count;
  ssize_t n = this->send (this->current_message_->mb (),
                          0, /* it is non-blocking */
                          &byte_count);
  if (n == 0)
    {
      // The connection was closed, return -1 to have the Reactor
      // close this transport and event handler
      return -1;
    }

  // Because there can be a partial transfer we need to adjust the
  // number of bytes sent.
  this->current_message_->bytes_transferred (byte_count);
  if (this->current_message_->done ())
    {
      // Remove the current message....
      // @@ We should be using a pool for these guys!
      this->current_message_->destroy ();

      this->current_message_ = 0;

      if (n == -1)
        return -1;

      return 1;
    }

  if (n == -1)
    {
      // ... timeouts and flow control are not real errors, the
      // connection is still valid and we must continue sending the
      // current message ...
      if (errno == EWOULDBLOCK
          || errno == ETIME)
        return 0;

      return -1;
    }

  return 0;
}

int
TAO_Transport::dequeue_next_message (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->queue_mutex_, -1);
  if (this->head_ == 0)
    return 1;

  this->current_message_ = this->head_;
  this->head_->remove_from_list (this->head_, this->tail_);

  return 0;
}

int
TAO_Transport::cancel_output (void)
{
  return 0;
}

int
TAO_Transport::schedule_output (void)
{
  return 0;
}

int
TAO_Transport::send_message_i (TAO_Stub *stub,
                               int twoway_flag,
                               const ACE_Message_Block *message_block,
                               ACE_Time_Value *max_wait_time)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->queue_mutex_, -1);

  int queue_empty = (this->head_ == 0);

  // Let's figure out if the message should be queued without trying
  // to send first:
  int non_queued_message =
    (this->current_message_ == 0) // There is an outgoing message already
    && (twoway_flag
        || !stub->sync_strategy ().must_queue (stub,
                                               queue_empty));

  TAO_Queued_Message *queued_message = 0;
  if (non_queued_message)
    {
      // ... in this case we must try to send the message first ...
      
      size_t byte_count;

      // @@ I don't think we want to hold the mutex here, however if
      // we release it we need to recheck the status of the transport
      // after we return... once I understand the final form for this
      // code I will re-visit this stuff.
      ssize_t n = this->send (message_block,
                              0, // non-blocking
                              &byte_count);
      if (n == 0)
        return -1;
      else if (n == -1 && errno != EWOULDBLOCK)
        return -1;

      // ... let's figure out if the complete message was sent ...
      if (message_block->total_length () == byte_count)
        {
          // Done, just return.  Notice that there are no allocations
          // or copies up to this point (though some fancy calling
          // back and forth).
          // This should be the normal path in the ORB, it should be
          // fast.
          return 0;
        }

      // ... the message was only partially sent, set it as the
      // current message ...
      queued_message =
        new TAO_Queued_Message (message_block->clone ());
      // @@ Revisit message queue allocations

      queued_message->bytes_transferred (byte_count);
      this->current_message_ = queued_message;
    }
  else
    {
      // ... otherwise simply queue the message ...
      queued_message =
        new TAO_Queued_Message (message_block->clone ());
      queued_message->push_back (this->head_, this->tail_);
    }

  // ... two choices, this is a twoway request or not, if it is
  // then we must only return once the complete message has been
  // sent:

  TAO_Flushing_Strategy *flushing_strategy =
    this->orb_core ()->flushing_strategy ();
  if (twoway_flag)
    {
      // Release the mutex, other threads may modify the queue as we
      // block for a long time writing out data.
      ace_mon.release ();
      int result = flushing_strategy->flush_message (this, queued_message);
      queued_message->destroy ();
      return result;
    }

  // ... this is not a twoway.  We must check if the buffering
  // constraints have been reached, if so, then we should start
  // flushing out data....

  // First let's compute the size of the queue:
  size_t msg_count = 0;
  size_t total_bytes = 0;
  for (TAO_Queued_Message *i = this->head_; i != 0; i = i->next ())
    {
      msg_count++;
      total_bytes += i->mb ()->total_length ();
    }
  if (this->current_message_ != 0)
    {
      msg_count++;
      total_bytes += this->current_message_->mb ()->total_length ();
    }

  int set_timer;
  ACE_Time_Value interval;

  if (stub->sync_strategy ().buffering_constraints_reached (stub,
                                                            msg_count,
                                                            total_bytes,
                                                            set_timer,
                                                            interval))
    {
      ace_mon.release ();
      // @@ memory management of the queued messages is getting tricky.
      int result = flushing_strategy->flush_message (this,
                                                     this->tail_);
      return result;
    }
  else
    {
      // ... it is not time to flush yet, but maybe we need to set a
      // timer ...
      if (set_timer)
        {
          // @@ We need to schedule the timer. We should also be
          // careful not to schedule one if there is one scheduled
          // already.
        }
    }
  return 0;
}

int
TAO_Transport::idle_after_send (void)
{
  return this->tms ()->idle_after_send ();
}

int
TAO_Transport::idle_after_reply (void)
{
  return this->tms ()->idle_after_reply ();
}

TAO_SYNCH_CONDITION *
TAO_Transport::leader_follower_condition_variable (void)
{
  return this->wait_strategy ()->leader_follower_condition_variable ();
}

int
TAO_Transport::tear_listen_point_list (TAO_InputCDR &)
{
  ACE_NOTSUP_RETURN(-1);
}
