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
#include "Connection_Handler.h"
#include "Synch_Queued_Message.h"
#include "Asynch_Queued_Message.h"
#include "Flushing_Strategy.h"
#include "debug.h"

#include "ace/Message_Block.h"

#if !defined (__ACE_INLINE__)
# include "Transport.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Transport, "$Id$")

TAO_Synch_Refcountable::TAO_Synch_Refcountable (int refcount)
  : ACE_Refcountable (refcount)
{
}

TAO_Synch_Refcountable::~TAO_Synch_Refcountable (void)
{
}

int
TAO_Synch_Refcountable::increment (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
  return ACE_Refcountable::increment ();
}

int
TAO_Synch_Refcountable::decrement (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->mutex_, 0);
  return ACE_Refcountable::decrement ();
}

int
TAO_Synch_Refcountable::refcount (void) const
{
  return ACE_Refcountable::refcount ();
}

// Constructor.
TAO_Transport::TAO_Transport (CORBA::ULong tag,
                              TAO_ORB_Core *orb_core)
  : TAO_Synch_Refcountable (1)
  , tag_ (tag)
  , orb_core_ (orb_core)
  , cache_map_entry_ (0)
  , bidirectional_flag_ (-1)
  , head_ (0)
  , tail_ (0)
  , id_ ((long) this)
{
  TAO_Client_Strategy_Factory *cf =
    this->orb_core_->client_factory ();

  // Create WS now.
  this->ws_ = cf->create_wait_strategy (this);

  // Create TMS now.
  this->tms_ = cf->create_transport_mux_strategy (this);

  // Create a handler lock
  this->handler_lock_ =
    this->orb_core_->resource_factory ()->create_cached_connection_lock ();
}

TAO_Transport::~TAO_Transport (void)
{
  delete this->ws_;
  this->ws_ = 0;

  delete this->tms_;
  this->tms_ = 0;

  TAO_Queued_Message *i = this->head_;
  while (i != 0)
    {
      // @@ This is a good point to insert a flag to indicate that a
      // CloseConnection message was successfully received.
      i->connection_closed ();

      TAO_Queued_Message *tmp = i;
      i = i->next ();

      tmp->destroy ();
    }

  delete this->handler_lock_;
}

int
TAO_Transport::handle_output ()
{
  if (TAO_debug_level > 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::handle_output\n",
                  this->id ()));
    }

  // The flushing strategy (potentially via the Reactor) wants to send
  // more data, first check if there is a current message that needs
  // more sending...
  int retval = this->drain_queue ();

  if (TAO_debug_level > 4)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::handle_output, "
                  "drain_queue returns %d/%d\n",
                  this->id (),
                  retval, errno));
    }

  if (retval == 1)
    {
      // ... there is no current message or it was completely
      // sent, cancel output...
      TAO_Flushing_Strategy *flushing_strategy =
        this->orb_core ()->flushing_strategy ();

      flushing_strategy->cancel_output (this);
      return 0;
    }

  // Any errors are returned directly to the Reactor
  return retval;
}

void
TAO_Transport::provide_handle (ACE_Handle_Set &handle_set)
{
  ACE_MT (ACE_GUARD (ACE_Lock,
                     guard,
                     *this->handler_lock_));
  ACE_Event_Handler *eh = this->event_handler_i ();
  TAO_Connection_Handler *ch = ACE_reinterpret_cast (TAO_Connection_Handler *, eh);
  if (ch && ch->is_registered ())
    handle_set.set_bit (eh->get_handle ());
}

static void
dump_iov (iovec *iov, int iovcnt, int id,
          size_t current_transfer,
          const char *location)
{
  ACE_Log_Msg::instance ()->acquire ();

  ACE_DEBUG ((LM_DEBUG,
              "TAO (%P|%t) - Transport[%d]::%s"
              " sending %d buffers\n",
              id, location, iovcnt));
  for (int i = 0; i != iovcnt && 0 < current_transfer; ++i)
    {
      size_t iov_len = iov[i].iov_len;

      // Possibly a partially sent iovec entry.
      if (current_transfer < iov_len)
        iov_len = current_transfer;

      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::%s"
                  " buffer %d/%d has %d bytes\n",
                  id, location,
                  i, iovcnt,
                  iov_len));
      
      size_t len;
      for (size_t offset = 0; offset < iov_len; offset += len)
        {
          char header[1024];
          ACE_OS::sprintf (header,
                           "TAO - Transport[%d]::%s (%d/%d)\n",
                           id, location, offset, iov_len);

          len = iov_len - offset;
          if (len > 512)
            len = 512;
          ACE_HEX_DUMP ((LM_DEBUG,
                         ACE_static_cast(char*,iov[i].iov_base) + offset,
                         len,
                         header));
        }
      current_transfer -= iov_len;
    }
  ACE_DEBUG ((LM_DEBUG,
              "TAO (%P|%t) - Transport[%d]::%s"
              " end of data\n",
              id, location));

  ACE_Log_Msg::instance ()->release ();
}

int
TAO_Transport::send_message_block_chain (const ACE_Message_Block *message_block,
                                         size_t &bytes_transferred,
                                         ACE_Time_Value *timeout)
{
  bytes_transferred = 0;

  iovec iov[IOV_MAX];
  int iovcnt = 0;

  while (message_block != 0)
    {
      size_t message_block_length =
        message_block->length ();

      // Check if this block has any data to be sent.
      if (message_block_length > 0)
        {
              // Collect the data in the iovec.
          iov[iovcnt].iov_base = message_block->rd_ptr ();
          iov[iovcnt].iov_len  = message_block_length;

          // Increment iovec counter.
          iovcnt++;

          // The buffer is full make a OS call.  @@ TODO find a way to
          // find IOV_MAX for platforms that do not define it rather
          // than simply setting IOV_MAX to some arbitrary value such
          // as 16.
          if (iovcnt == IOV_MAX)
            {
              size_t current_transfer = 0;

              ssize_t result =
                this->send (iov, iovcnt, current_transfer, timeout);

              if (TAO_debug_level == 2)
                {
                  dump_iov (iov, iovcnt, this->id (),
                            current_transfer, "send_message_block_chain");
                }

              // Add to total bytes transferred.
              bytes_transferred += current_transfer;

              // Errors.
              if (result == -1 || result == 0)
                return result;

              // Reset iovec counter.
              iovcnt = 0;
            }
        }

      // Select the next message block in the chain.
      message_block = message_block->cont ();
    }

  // Check for remaining buffers to be sent.  This will happen when
  // IOV_MAX is not a multiple of the number of message blocks.
  if (iovcnt != 0)
    {
      size_t current_transfer = 0;

      ssize_t result =
        this->send (iov, iovcnt, current_transfer, timeout);

      if (TAO_debug_level == 2)
        {
          dump_iov (iov, iovcnt, this->id (),
                    current_transfer, "send_message_block_chain");
        }

      // Add to total bytes transferred.
      bytes_transferred += current_transfer;

      // Errors.
      if (result == -1 || result == 0)
        return result;
    }

  // Return total bytes transferred.
  return bytes_transferred;
}

int
TAO_Transport::send_message_i (TAO_Stub *stub,
                               int is_synchronous,
                               const ACE_Message_Block *message_block,
                               ACE_Time_Value *max_wait_time)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->queue_mutex_, -1);

  if (is_synchronous)
    {
      return this->send_synchronous_message_i (stub,
                                               message_block,
                                               max_wait_time);
    }

  // Let's figure out if the message should be queued without trying
  // to send first:
  int try_sending_first = 1;

  int queue_empty = (this->head_ == 0);

  if (!queue_empty)
    try_sending_first = 0;
  else if (stub->sync_strategy ().must_queue (queue_empty))
    try_sending_first = 0;

  size_t byte_count = 0;
  ssize_t n;
  if (try_sending_first)
    {
      // ... in this case we must try to send the message first ...

      if (TAO_debug_level > 6)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - Transport[%d]::send_message_i, "
                      "trying to send the message\n",
                      this->id ()));
        }

      // @@ I don't think we want to hold the mutex here, however if
      // we release it we need to recheck the status of the transport
      // after we return... once I understand the final form for this
      // code I will re-visit this decision
      n = this->send_message_block_chain (message_block,
                                          byte_count,
                                          max_wait_time);
      if (n == 0)
        return -1; // EOF
      else if (n == -1)
        {
          // ... if this is just an EWOULDBLOCK we must schedule the
          // message for later, if it is ETIME we still have to send
          // the complete message, because cutting off the message at
          // this point will destroy the synchronization with the
          // server ...
          if (errno != EWOULDBLOCK && errno != ETIME)
            {
              return -1;
            }
        }

      // ... let's figure out if the complete message was sent ...
      if (message_block->total_length () == byte_count)
        {
          // Done, just return.  Notice that there are no allocations
          // or copies up to this point (though some fancy calling
          // back and forth).
          // This is the common case for the critical path, it should
          // be fast.
          return 0;
        }
    }

  // ... either the message must be queued or we need to queue it
  // because it was not completely sent out ...

  if (TAO_debug_level > 6)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::send_message_i, "
                  "message is queued\n",
                  this->id ()));
    }

  TAO_Queued_Message *queued_message = 0;
  ACE_NEW_RETURN (queued_message,
                  TAO_Asynch_Queued_Message (message_block),
                  -1);
  queued_message->bytes_transferred (byte_count);
  queued_message->push_back (this->head_, this->tail_);

  // ... if the queue is full we need to activate the output on the
  // queue ...
  if (this->must_flush_queue_i (stub))
    {
      this->orb_core ()->flushing_strategy ()->schedule_output (this);
    }

  // ... in any case, check for timeouts and report them to the
  // application ...
  if (max_wait_time != 0 && n == -1 && errno == ETIME)
    return -1;

  return 0;
}

int
TAO_Transport::send_synchronous_message_i (TAO_Stub *stub,
                                           const ACE_Message_Block *message_block,
                                           ACE_Time_Value *max_wait_time)
{
  // We are going to block, so there is no need to clone
  // the message block.
  TAO_Synch_Queued_Message synch_message (message_block);

  synch_message.push_back (this->head_, this->tail_);

  int n = this->drain_queue_i ();
  if (n == -1)
    return -1; // Error while sending...
  else if (n == 1)
    {
      ACE_ASSERT (synch_message.all_data_sent ());
      ACE_ASSERT (synch_message.next () == 0);
      ACE_ASSERT (synch_message.prev () == 0);
      return 1;  // Empty queue, message was sent..
    }

  ACE_ASSERT (n == 0); // Some data sent, but data remains.

  if (synch_message.all_data_sent ())
    return 1;

  // @todo: Check for timeouts!
  // if (max_wait_time != 0 && errno == ETIME) return -1;

  TAO_Flushing_Strategy *flushing_strategy =
    this->orb_core ()->flushing_strategy ();
  (void) flushing_strategy->schedule_output (this);

  // Release the mutex, other threads may modify the queue as we
  // block for a long time writing out data.
  int result;
  {
    typedef ACE_Reverse_Lock<TAO_SYNCH_MUTEX> TAO_REVERSE_SYNCH_MUTEX;
    TAO_REVERSE_SYNCH_MUTEX reverse (this->queue_mutex_);

    ACE_GUARD_RETURN (TAO_REVERSE_SYNCH_MUTEX, ace_mon, reverse, -1);
    result = flushing_strategy->flush_message (this,
                                               &synch_message);

  }
  ACE_ASSERT (synch_message.all_data_sent () != 0);
  ACE_ASSERT (synch_message.next () == 0);
  ACE_ASSERT (synch_message.prev () == 0);
  return result;
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
  ACE_NOTSUP_RETURN (-1);
}

int
TAO_Transport::reactor_signalling (void)
{
  return 0;
}

void
TAO_Transport::connection_handler_closing (void)
{
  {
    ACE_MT (ACE_GUARD (ACE_Lock,
                     guard,
                     *this->handler_lock_));

    this->transition_handler_state_i ();

    this->orb_core_->transport_cache ().purge_entry (
      this->cache_map_entry_);
  }
  // Can't hold the lock while we release, b/c the release could
  // invoke the destructor!

  // This should be the last thing we do here
  TAO_Transport::release(this);
}

TAO_Transport*
TAO_Transport::_duplicate (TAO_Transport* transport)
{
  if (transport != 0)
    {
      transport->increment ();
    }
  return transport;
}

void
TAO_Transport::release (TAO_Transport* transport)
{
  if (transport != 0)
    {
      int count = transport->decrement ();

      if (count == 0)
        {
          delete transport;
        }
      else if (count < 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) TAO_Transport::release, ")
                      ACE_TEXT ("reference countis less than zero: %d\n"),
                      count));
          ACE_OS::abort ();
        }
    }
}

int
TAO_Transport::recache_transport (TAO_Transport_Descriptor_Interface *desc)
{
  // First purge our entry
  this->orb_core_->transport_cache ().purge_entry (this->cache_map_entry_);

  // Then add ourselves to the cache
  return this->orb_core_->transport_cache ().cache_transport (desc,
                                                              this);
}

void
TAO_Transport::mark_invalid (void)
{
  // @@ Do we need this method at all??
  this->orb_core_->transport_cache ().mark_invalid (
    this->cache_map_entry_);
}

int
TAO_Transport::make_idle (void)
{
  return this->orb_core_->transport_cache ().make_idle (this->cache_map_entry_);
}

void
TAO_Transport::close_connection (void)
{
  ACE_Event_Handler *eh = 0;
  {
    ACE_MT (ACE_GUARD (ACE_Lock, guard, *this->handler_lock_));

    eh = this->event_handler_i ();

    this->transition_handler_state_i ();

    if (eh == 0)
      return;
  }

  // Close the underlying connection, it is enough to get an
  // Event_Handler pointer to do this, so we can factor out the code
  // in the base TAO_Transport class.
  (void) eh->handle_close (ACE_INVALID_HANDLE,
                           ACE_Event_Handler::ALL_EVENTS_MASK);

  // Purge the entry
  // @todo This is redundant, handle_close() eventually calls
  //       this->connection_handler_closing(), that performs the same
  //       work, for some reason they hold the mutex while they do
  //       that work though.
  this->orb_core_->transport_cache ().purge_entry (this->cache_map_entry_);

  for (TAO_Queued_Message *i = this->head_; i != 0; i = i->next ())
    {
      i->connection_closed ();
    }
}

ssize_t
TAO_Transport::send (iovec *iov, int iovcnt,
                     size_t &bytes_transferred,
                     const ACE_Time_Value *timeout)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->handler_lock_,
                            -1));

  // if there's no associated event handler, then we act like a null transport
  if (this->event_handler_i () == 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) transport %d (tag=%d) send() ")
                      ACE_TEXT ("no longer associated with handler, returning -1 with errno = ENOENT\n"),
                      this->id (),
                      this->tag_));
        }
      errno = ENOENT;
      return -1;
    }

  // now call the template method
  return this->send_i (iov, iovcnt, bytes_transferred, timeout);
}

ssize_t
TAO_Transport::recv (char *buffer,
                     size_t len,
                     const ACE_Time_Value *timeout)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->handler_lock_,
                            -1));

  // if there's no associated event handler, then we act like a null transport
  if (this->event_handler_i () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) transport %d (tag=%d) recv() ")
                  ACE_TEXT ("no longer associated with handler, returning -1 with errno = ENOENT\n"),
                  this->id (),
                  this->tag_));
      // @@CJC Should we return -1, like an error, or should we return 0, like an EOF?
      errno = ENOENT;
      return -1;
    }

  // now call the template method
  return this->recv_i (buffer, len, timeout);
}

long
TAO_Transport::register_for_timer_event (const void* arg,
                                         const ACE_Time_Value &delay,
                                         const ACE_Time_Value &interval)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->handler_lock_,
                            -1));

  ACE_Event_Handler *eh = this->event_handler_i ();
  if (eh == 0)
    return -1;

  return this->orb_core_->reactor ()->schedule_timer (eh, arg, delay, interval);
}

int
TAO_Transport::register_handler (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->handler_lock_,
                            -1));
  return this->register_handler_i ();
}

int
TAO_Transport::id (void) const
{
  return this->id_;
}

void
TAO_Transport::id (int id)
{
  this->id_ = id;
}

int
TAO_Transport::schedule_output (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->handler_lock_,
                            -1));

  ACE_Event_Handler *eh = this->event_handler_i ();
  if (eh == 0)
    return -1;

  ACE_Reactor *reactor = eh->reactor ();
  if (reactor == 0)
    return -1;

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::schedule_output\n",
                  this->id ()));
    }

  return reactor->schedule_wakeup (eh, ACE_Event_Handler::WRITE_MASK);
}

int
TAO_Transport::cancel_output (void)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->handler_lock_,
                            -1));

  ACE_Event_Handler *eh = this->event_handler_i ();
  if (eh == 0)
    return -1;

  ACE_Reactor *reactor = eh->reactor ();
  if (reactor == 0)
    return -1;

  if (TAO_debug_level > 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TAO (%P|%t) - Transport[%d]::cancel output\n",
                  this->id ()));
    }

  return reactor->cancel_wakeup (eh, ACE_Event_Handler::WRITE_MASK);
}

int
TAO_Transport::drain_queue (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->queue_mutex_, -1);

  return this->drain_queue_i ();
}

int
TAO_Transport::drain_queue_i (void)
{
  if (this->head_ == 0)
    return 1;

  // This is the vector used to send data, it must be declared outside
  // the loop because after the loop there may still be data to be
  // sent
  int iovcnt = 0;
  iovec iov[IOV_MAX];

  // We loop over all the elements in the queue ...
  TAO_Queued_Message *i = this->head_;
  while (i != 0)
    {
      // ... each element fills the iovector ...
      i->fill_iov (IOV_MAX, iovcnt, iov);

      // ... the vector is full, no choice but to send some data out.
      // We need to loop because a single message can span multiple
      // IOV_MAX elements ...
      if (iovcnt == IOV_MAX)
        {
          size_t byte_count = 0;

          // ... send the message ...
          ssize_t retval =
            this->send (iov, iovcnt, byte_count);

          if (TAO_debug_level == 2)
            {
              dump_iov (iov, iovcnt, this->id (),
                        byte_count, "drain_queue_i");
            }

          // ... now we need to update the queue, removing elements
          // that have been sent, and updating the last element if it
          // was only partially sent ...
          this->cleanup_queue (byte_count);
          iovcnt = 0;

          if (retval == 0)
            {
              return -1;
            }
          else if (retval == -1)
            {
              if (errno == EWOULDBLOCK || errno == ETIME)
                return 0;
              return -1;
            }

          // ... start over, how do we guarantee progress?  Because if
          // no bytes are sent send() can only return 0 or -1
          ACE_ASSERT (byte_count != 0);
          i = this->head_;
          continue;
        }
      // ... notice that this line is only reached if there is still
      // room in the iovector ...
      i = i->next ();
    }

  size_t byte_count = 0;
  ssize_t retval =
    this->send (iov, iovcnt, byte_count);

  if (TAO_debug_level == 2)
    {
      dump_iov (iov, iovcnt, this->id (),
                byte_count, "drain_queue_i");
    }

  this->cleanup_queue (byte_count);
  iovcnt = 0;

  if (retval == 0)
    {
      return -1;
    }
  else if (retval == -1)
    {
      if (errno == EWOULDBLOCK || errno == ETIME)
        return 0;
      return -1;
    }
  ACE_ASSERT (byte_count != 0);

  if (this->head_ == 0)
    return 1;

  return 0;
}

void
TAO_Transport::cleanup_queue (size_t byte_count)
{
  while (this->head_ != 0 && byte_count > 0)
    {
      TAO_Queued_Message *i = this->head_;

      // Update the state of the first message
      i->bytes_transferred (byte_count);

      // ... if all the data was sent the message must be removed from
      // the queue...
      if (i->all_data_sent ())
        {
          i->remove_from_list (this->head_, this->tail_);
          i->destroy ();
        }
    }
}

int
TAO_Transport::must_flush_queue_i (TAO_Stub *stub)
{
  // First let's compute the size of the queue:
  size_t msg_count = 0;
  size_t total_bytes = 0;
  for (TAO_Queued_Message *i = this->head_; i != 0; i = i->next ())
    {
      msg_count++;
      total_bytes += i->message_length ();
    }

  int set_timer;
  ACE_Time_Value interval;

  if (stub->sync_strategy ().buffering_constraints_reached (stub,
                                                            msg_count,
                                                            total_bytes,
                                                            set_timer,
                                                            interval))
    {
      return 1;
    }

  // ... it is not time to flush yet, but maybe we need to set a
  // timer ...
  if (set_timer)
    {
      // @@ We need to schedule the timer. We should also be
      // careful not to schedule one if there is one scheduled
      // already.
    }

  return 0;
}
