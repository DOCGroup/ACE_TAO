// $Id$

#include "Event_Channel.h"

void
Proxy_Handler::id (ACE_INT32 id)
{
  this->id_ = id;
}

ACE_INT32
Proxy_Handler::id (void)
{
  return this->id_;
}

// The total number of bytes sent/received on this Proxy.

size_t 
Proxy_Handler::total_bytes (void)
{
  return this->total_bytes_;
}

void 
Proxy_Handler::total_bytes (size_t bytes)
{
  this->total_bytes_ += bytes;
}

Proxy_Handler::Proxy_Handler (ACE_Event_Channel &ec, 
			      const ACE_INET_Addr &remote_addr,
			      const ACE_INET_Addr &local_addr,
			      ACE_INT32 conn_id)
  : remote_addr_ (remote_addr),
    local_addr_ (local_addr),
    id_ (conn_id),
    total_bytes_ (0),
    state_ (Proxy_Handler::IDLE),
    timeout_ (1),
    max_timeout_ (Proxy_Handler::MAX_RETRY_TIMEOUT),
    event_channel_ (ec)
{
}

// Set the proxy_role.

void
Proxy_Handler::proxy_role (char d)
{
  this->proxy_role_ = d;
}

// Get the proxy_role.

char
Proxy_Handler::proxy_role (void)
{
  return this->proxy_role_;
}

// Sets the timeout delay.

void
Proxy_Handler::timeout (int to)
{
  if (to > this->max_timeout_)
    to = this->max_timeout_;

  this->timeout_ = to;
}

// Re-calculate the current retry timeout delay using exponential
// backoff.  Returns the original timeout (i.e., before the
// re-calculation).

int
Proxy_Handler::timeout (void)
{
  int old_timeout = this->timeout_;
  this->timeout_ *= 2;
  
  if (this->timeout_ > this->max_timeout_)
    this->timeout_ = this->max_timeout_;

  return old_timeout;
}

// Sets the max timeout delay.

void
Proxy_Handler::max_timeout (int mto)
{
  this->max_timeout_ = mto;
}

// Gets the max timeout delay.

int
Proxy_Handler::max_timeout (void)
{
  return this->max_timeout_;
}

// Restart connection asynchronously when timeout occurs.

int
Proxy_Handler::handle_timeout (const ACE_Time_Value &, 
			       const void *)
{
  ACE_DEBUG ((LM_DEBUG, 
	     "(%t) attempting to reconnect Proxy_Handler %d with timeout = %d\n", 
             this->id (), this->timeout_));

  // Delegate the re-connection attempt to the Event Channel.
  return this->event_channel_.initiate_proxy_connection 
    (this, ACE_Synch_Options::asynch);
}

// Handle shutdown of the Proxy_Handler object.

int
Proxy_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, 
	      "(%t) shutting down Proxy_Handler %d on handle %d\n", 
	     this->id (), this->get_handle ()));
  
  // Restart the connection, if possible.
  return this->event_channel_.reinitiate_proxy_connection (this);
}

// Set the state of the Proxy.

void
Proxy_Handler::state (Proxy_Handler::State s)
{
  this->state_ = s;
}

// Upcall from the <ACE_Acceptor> or <ACE_Connector> that delegates
// control to our Proxy_Handler.

int
Proxy_Handler::open (void *)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Proxy_Handler's handle = %d\n",
	      this->peer ().get_handle ()));

  // Turn on non-blocking I/O.
  if (this->peer ().enable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "enable"), -1);

  // Call back to the <Event_Channel> to complete our initialization.
  else if (this->event_channel_.complete_proxy_connection (this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "complete_proxy_connection"), -1);

  // Register ourselves to receive input events.
  else if (ACE_Service_Config::reactor ()->register_handler 
      (this, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "register_handler"), -1);
  else
    return 0;
}

// Return the current state of the Proxy.

Proxy_Handler::State
Proxy_Handler::state (void)
{
  return this->state_;
}

ACE_INET_Addr &
Proxy_Handler::remote_addr (void)
{
  return this->remote_addr_;
}

ACE_INET_Addr &
Proxy_Handler::local_addr (void)
{
  return this->local_addr_;
}

Consumer_Proxy::Consumer_Proxy (ACE_Event_Channel &ec, 
				const ACE_INET_Addr &remote_addr,
				const ACE_INET_Addr &local_addr,
				ACE_INT32 conn_id)
  : Proxy_Handler (ec, remote_addr, local_addr, conn_id)
{
  this->proxy_role_ = 'C';
  this->msg_queue ()->high_water_mark (Consumer_Proxy::MAX_QUEUE_SIZE);
}

// This method should be called only when the Consumer shuts down
// unexpectedly.  This method simply marks the Proxy_Handler as having
// failed so that handle_close () can reconnect.

int 
Consumer_Proxy::handle_input (ACE_HANDLE)
{
  char buf[1];

  this->state (Proxy_Handler::FAILED);

  switch (this->peer ().recv (buf, sizeof buf))
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR,
			"(%t) Peer has failed unexpectedly for Consumer_Proxy %d\n",
			this->id ()), -1);
      /* NOTREACHED */
    case 0:
      ACE_ERROR_RETURN ((LM_ERROR,
			"(%t) Peer has shutdown unexpectedly for Consumer_Proxy %d\n",
			this->id ()), -1);
      /* NOTREACHED */
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
			"(%t) Consumer is erroneously sending input to Consumer_Proxy %d\n",
			this->id ()), -1);
      /* NOTREACHED */
    }
}

// Perform a non-blocking put() of event.  If we are unable to send
// the entire event the remainder is re-queued at the *front* of the
// Event_List.

int
Consumer_Proxy::nonblk_put (ACE_Message_Block *event)
{
  // Try to send the event.  If we don't send it all (e.g., due to
  // flow control), then re-queue the remainder at the head of the
  // Event_List and ask the ACE_Reactor to inform us (via
  // handle_output()) when it is possible to try again.

  ssize_t n = this->send (event);

  if (n == -1)
    {
      // Things have gone wrong, let's try to close down and set up a
      // new reconnection by calling handle_close().
      this->state (Proxy_Handler::FAILED);
      this->handle_close ();
      return -1;
    }
  else if (errno == EWOULDBLOCK) // Didn't manage to send everything.
    {
      ACE_DEBUG ((LM_DEBUG, 
		  "(%t) queueing activated on handle %d to routing id %d\n",
                 this->get_handle (), this->id ()));

      // ACE_Queue in *front* of the list to preserve order.
      if (this->msg_queue ()->enqueue_head 
	  (event, (ACE_Time_Value *) &ACE_Time_Value::zero) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "enqueue_head"), -1);
      
      // Tell ACE_Reactor to call us back when we can send again.
      else if (ACE_Service_Config::reactor ()->schedule_wakeup 
	       (this, ACE_Event_Handler::WRITE_MASK) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "schedule_wakeup"), -1);
      return 0;
    }
  else
    return n;
}

ssize_t
Consumer_Proxy::send (ACE_Message_Block *event)
{
  ssize_t len = event->length ();
  ssize_t n = this->peer ().send (event->rd_ptr (), len);

  if (n <= 0)
    return errno == EWOULDBLOCK ? 0 : n;
  else if (n < len)
    // Re-adjust pointer to skip over the part we did send.
    event->rd_ptr (n);
  else // if (n == length)
    {
      // The whole event is sent, we now decrement the reference count
      // (which deletes itself with it reaches 0.
      event->release ();
      errno = 0;
    }
  this->total_bytes (n);
  return n;
}

// Finish sending an event when flow control conditions abate.
// This method is automatically called by the ACE_Reactor.

int 
Consumer_Proxy::handle_output (ACE_HANDLE)
{
  ACE_Message_Block *event = 0;

  ACE_DEBUG ((LM_DEBUG, 
	      "(%t) in handle_output on handle %d\n", 
	      this->get_handle ()));
  // The list had better not be empty, otherwise there's a bug!

  if (this->msg_queue ()->dequeue_head 
      (event, (ACE_Time_Value *) &ACE_Time_Value::zero) != -1)
    {
      switch (this->nonblk_put (event))
        {
        case 0:           // Partial send.
          ACE_ASSERT (errno == EWOULDBLOCK);
          // Didn't write everything this time, come back later...
          break;

        case -1:
          // We are responsible for releasing an ACE_Message_Block if
          // failures occur.
          event->release ();
          ACE_ERROR ((LM_ERROR, "(%t) %p\n", "transmission failure"));

          /* FALLTHROUGH */
        default: // Sent the whole thing.
          
          // If we succeed in writing the entire event (or we did not
          // fail due to EWOULDBLOCK) then check if there are more
          // events on the Message_Queue.  If there aren't, tell the
          // ACE_Reactor not to notify us anymore (at least until
          // there are new events queued up).

          if (this->msg_queue ()->is_empty ())
            {
              ACE_DEBUG ((LM_DEBUG, 
			  "(%t) queueing deactivated on handle %d to routing id %d\n",
                         this->get_handle (), this->id ()));


              if (ACE_Service_Config::reactor ()->cancel_wakeup 
		  (this, ACE_Event_Handler::WRITE_MASK) == -1)
                ACE_ERROR ((LM_ERROR, "(%t) %p\n", "cancel_wakeup"));
            }
        }
    }
  else 
    ACE_ERROR ((LM_ERROR, "(%t) %p\n", "dequeue_head"));
  return 0;      
}

// Send an event to a Consumer (may queue if necessary).

int 
Consumer_Proxy::put (ACE_Message_Block *event, ACE_Time_Value *)
{
  if (this->msg_queue ()->is_empty ())
    // Try to send the event *without* blocking!
    return this->nonblk_put (event); 
  else
    // If we have queued up events due to flow control then just
    // enqueue and return.
    return this->msg_queue ()->enqueue_tail 
      (event, (ACE_Time_Value *) &ACE_Time_Value::zero);
}

Supplier_Proxy::Supplier_Proxy (ACE_Event_Channel &ec, 
				const ACE_INET_Addr &remote_addr,
				const ACE_INET_Addr &local_addr,
				ACE_INT32 conn_id)
  : msg_frag_ (0), 
    Proxy_Handler (ec, remote_addr, local_addr, conn_id)
{
  this->proxy_role_ = 'S';
  this->msg_queue ()->high_water_mark (0);
}

// Receive an Event from a Supplier.  Handles fragmentation.
//
// The event returned from recv consists of two parts:
//
// 1. The Address part, contains the "virtual" routing id.
//
// 2. The Data part, which contains the actual data to be forwarded.
// 
// The reason for having two parts is to shield the higher layers
// of software from knowledge of the event structure.

int
Supplier_Proxy::recv (ACE_Message_Block *&forward_addr)
{ 
  if (this->msg_frag_ == 0)
    // No existing fragment...
    ACE_NEW_RETURN (this->msg_frag_, 
		    ACE_Message_Block (sizeof (Event)), 
		    -1);

  Event *event = (Event *) this->msg_frag_->rd_ptr ();
  ssize_t header_received = 0;

  const ssize_t HEADER_SIZE = sizeof (Event_Header);
  ssize_t header_bytes_left_to_read = 
    HEADER_SIZE - this->msg_frag_->length ();

  if (header_bytes_left_to_read > 0)
    {
      header_received = this->peer ().recv 
	(this->msg_frag_->wr_ptr (), header_bytes_left_to_read);

      if (header_received == -1 /* error */
	  || header_received == 0  /* EOF */)
	{
	  ACE_ERROR ((LM_ERROR, "%p\n", 
		      "Recv error during header read "));
	  ACE_DEBUG ((LM_DEBUG, 
		      "attempted to read %d\n", 
		      header_bytes_left_to_read));
	  this->msg_frag_ = this->msg_frag_->release ();
	  return header_received;
	}

      // Bump the write pointer by the amount read.
      this->msg_frag_->wr_ptr (header_received);

      // At this point we may or may not have the ENTIRE header.
      if (this->msg_frag_->length () < HEADER_SIZE)
	{
	  ACE_DEBUG ((LM_DEBUG, 
		      "Partial header received: only %d bytes\n",
		     this->msg_frag_->length ()));
	  // Notify the caller that we didn't get an entire event.
	  errno = EWOULDBLOCK;
	  return -1;
	}

      // Convert the header into host byte order so that we can access
      // it directly without having to repeatedly muck with it...
      event->header_.decode ();

      if (event->header_.len_ > sizeof event->data_)
	{
	  // This data_ payload is too big!
	  errno = EINVAL;
	  ACE_DEBUG ((LM_DEBUG, 
		      "Data payload is too big (%d bytes)\n",
		      event->header_.len_));
	  return -1;
	}
      
    }

  // At this point there is a complete, valid header in Event.  Now we
  // need to get the event payload.  Due to incomplete reads this may
  // not be the first time we've read in a fragment for this message.
  // We account for this here.  Note that the first time in here
  // msg_frag_->wr_ptr() will point to event->data_.  Every time we do
  // a successful fragment read, we advance wr_ptr().  Therefore, by
  // subtracting how much we've already read from the
  // event->header_.len_ we complete the data_bytes_left_to_read...

  ssize_t data_bytes_left_to_read = 
    ssize_t (event->header_.len_ - (msg_frag_->wr_ptr () - event->data_));

  ssize_t data_received = 
    this->peer ().recv (this->msg_frag_->wr_ptr (), data_bytes_left_to_read);

  // Try to receive the remainder of the event.

  switch (data_received)
    {
    case -1:
      if (errno == EWOULDBLOCK) 
	// This might happen if only the header came through.
	return -1;
      else
        /* FALLTHROUGH */;

    case 0: // Premature EOF.
      this->msg_frag_ = this->msg_frag_->release ();
      return 0;

    default:
      // Set the write pointer at 1 past the end of the event.
      this->msg_frag_->wr_ptr (data_received);

      if (data_received != data_bytes_left_to_read)
        {
          errno = EWOULDBLOCK;
	  // Inform caller that we didn't get the whole event.
          return -1; 
        }
      else
        {
          // Set the read pointer to the beginning of the event.
          this->msg_frag_->rd_ptr (this->msg_frag_->base ());

          // Allocate an event forwarding header and chain the data
          // portion onto its continuation field.
          forward_addr = new ACE_Message_Block (sizeof (Event_Key), 
						ACE_Message_Block::MB_PROTO, 
						this->msg_frag_);
	  if (forward_addr == 0)
	    {
	      this->msg_frag_ = this->msg_frag_->release ();
	      errno = ENOMEM;
	      return -1;
	    }

          Event_Key event_addr (this->id (), 
				 event->header_.supplier_id_,
				 event->header_.type_);
          // Copy the forwarding address from the Event_Key into
          // forward_addr.
          forward_addr->copy ((char *) &event_addr, sizeof (Event_Key));

          // Reset the pointer to indicate we've got an entire event.
          this->msg_frag_ = 0; 
        }

      this->total_bytes (data_received + header_received);
#if defined (VERBOSE)
      ACE_DEBUG ((LM_DEBUG, "(%t) connection id = %d, supplier id = %d, len = %d, payload = %*s",
		 event_addr.conn_id_, event->header_.supplier_id_, event->header_.len_,
		 event->header_.len_, event->data_));
#else
      ACE_DEBUG ((LM_DEBUG, "(%t) supplier id = %d, cur len = %d, total bytes read = %d\n",
		 event->header_.supplier_id_, event->header_.len_, data_received + header_received));
#endif /* VERBOSE */

      // Encode before returning so that we can set things out in
      // network byte order.
      event->header_.encode ();
      return data_received + header_received;
    }
}

// Receive various types of input (e.g., Peer event from the 
// gatewayd, as well as stdio).

int 
Supplier_Proxy::handle_input (ACE_HANDLE)
{
  ACE_Message_Block *forward_addr = 0;

  switch (this->recv (forward_addr))
    {
    case 0:
      // Note that a peer should never initiate a shutdown by closing
      // the connection.  Instead, it should reconnect.
      this->state (Proxy_Handler::FAILED);
      ACE_ERROR_RETURN ((LM_ERROR, 
			"(%t) Peer has closed down unexpectedly for Input Proxy_Handler %d\n", 
                        this->id ()), -1);
      /* NOTREACHED */
    case -1:
      if (errno == EWOULDBLOCK)
        // A short-read, we'll come back and finish it up later on!
        return 0;
      else // A weird problem occurred, shut down and start again.
        {
          this->state (Proxy_Handler::FAILED);
          ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p for Input Proxy_Handler %d\n", 
			    "Peer has failed unexpectedly",
                           this->id ()), -1);
        }
      /* NOTREACHED */
    default:
      return this->forward (forward_addr);
    }
}

// Forward an event to its appropriate Consumer(s).  This delegates to
// the <ACE_Event_Channel> to do the actual forwarding.

int
Supplier_Proxy::forward (ACE_Message_Block *forward_addr)
{
  return this->event_channel_.put (forward_addr);
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Map_Manager<Event_Key, Consumer_Dispatch_Set *, MAP_MUTEX>;
template class ACE_Map_Iterator<Event_Key, Consumer_Dispatch_Set *, MAP_MUTEX>;
template class ACE_Map_Entry<Event_Key, Consumer_Dispatch_Set *>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
