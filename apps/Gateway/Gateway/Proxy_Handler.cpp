// $Id$

#include "Dispatch_Set.h"
#include "Proxy_Handler_Connector.h"

// Convenient short-hands.
#define CO CONDITION
#define MU MAP_MUTEX

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

Proxy_Handler::Proxy_Handler (Event_Forwarding_Discriminator *efd,
			      Proxy_Handler_Connector *ioc,
			      ACE_Thread_Manager *thr_mgr,
			      int socket_queue_size)
  : ACE_Svc_Handler<ACE_SOCK_STREAM, SYNCH_STRATEGY> (thr_mgr),
    efd_ (efd),
    id_ (-1),
    total_bytes_ (0),
    state_ (Proxy_Handler::IDLE),
    connector_ (ioc),
    timeout_ (1),
    max_timeout_ (Proxy_Handler::MAX_RETRY_TIMEOUT), 
    socket_queue_size_ (socket_queue_size)
{
}

// Set the direction.

void
Proxy_Handler::direction (char d)
{
  this->direction_ = d;
}

// Get the direction.

char
Proxy_Handler::direction (void)
{
  return this->direction_;
}

// Sets the timeout delay.

void
Proxy_Handler::timeout (int to)
{
  if (to > this->max_timeout_)
    to = this->max_timeout_;

  this->timeout_ = to;
}

// Recalculate the current retry timeout delay using exponential
// backoff.  Returns the original timeout (i.e., before the
// recalculation).

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
Proxy_Handler::handle_timeout (const ACE_Time_Value &, const void *)
{
  ACE_DEBUG ((LM_DEBUG, 
	     "(%t) attempting to reconnect Proxy_Handler %d with timeout = %d\n", 
             this->id (), this->timeout_));
  return this->connector_->initiate_connection (this, ACE_Synch_Options::asynch);
}

// Restart connection (blocking_semantics dicates whether we
// restart synchronously or asynchronously).

int
Proxy_Handler::reinitiate_connection (void)
{
  // Skip over deactivated descriptors.
  if (this->get_handle () != ACE_INVALID_HANDLE)
    {
      // Make sure to close down peer to reclaim descriptor.
      this->peer ().close ();

      ACE_DEBUG ((LM_DEBUG, 
		 "(%t) scheduling reinitiation of Proxy_Handler %d\n",
		 this->id ()));

      // Reschedule ourselves to try and connect again.
      if (ACE_Service_Config::reactor ()->schedule_timer 
	  (this, 0, this->timeout ()) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", 
			  "schedule_timer"), -1);
    }
  return 0;
}

// Handle shutdown of the Proxy_Handler object.

int
Proxy_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, 
	      "(%t) shutting down Proxy_Handler %d on handle %d\n", 
	     this->id (), this->get_handle ()));
  
  return this->reinitiate_connection ();
}

// Set the state of the Proxy.

void
Proxy_Handler::state (Proxy_Handler::State s)
{
  this->state_ = s;
}

// Perform the first-time initiation of a connection to the peer.

int
Proxy_Handler::initialize_connection (void)
{
  this->state_ = Proxy_Handler::ESTABLISHED;

  // Restart the timeout to 1.
  this->timeout (1);

  // Action that sends the connection id to the peerd.

  ACE_INT32 id = htonl (this->id ());

  ssize_t n = this->peer ().send ((const void *) &id, sizeof id);

  if (n != sizeof id)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", 
		      n == 0 ? "peer has closed down unexpectedly" : "send"), 
		      -1);
  return 0;
}

// Set the size of the socket queue.

void
Proxy_Handler::socket_queue_size (void)
{
  if (this->socket_queue_size_ > 0)
    {
      int option = this->direction_ == 'S' ? SO_RCVBUF : SO_SNDBUF;

      if (this->peer ().set_option (SOL_SOCKET, option,
				    &this->socket_queue_size_, 
				    sizeof (int)) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "set_option"));
    }
}

// Upcall from the ACE_Acceptor::handle_input() that
// delegates control to our application-specific Proxy_Handler.

int
Proxy_Handler::open (void *a)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Proxy_Handler's fd = %d\n", 
	      this->peer ().get_handle ()));

  // Set the size of the socket queue.
  this->socket_queue_size ();

  // Turn on non-blocking I/O.
  if (this->peer ().enable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "enable"), -1);

  // Call down to the base class to activate and register this handler.
  if (this->ACE_Svc_Handler<ACE_SOCK_STREAM, SYNCH_STRATEGY>::open (a) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "activate"), -1);

  return this->initialize_connection ();
}

// Return the current state of the Proxy.

Proxy_Handler::State
Proxy_Handler::state (void)
{
  return this->state_;
}

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

// Set the peer's address information.
int 
Proxy_Handler::bind (const ACE_INET_Addr &remote_addr, 
		     const ACE_INET_Addr &local_addr,
		     ACE_INT32 id)
{
  this->remote_addr_ = remote_addr;
  this->local_addr_ = local_addr;
  this->id_ = id;
  return 0;
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

// Constructor sets the consumer map pointer.

Consumer_Proxy::Consumer_Proxy (Event_Forwarding_Discriminator *efd,
				    Proxy_Handler_Connector *ioc,
				    ACE_Thread_Manager *thr_mgr,
				    int socket_queue_size)
  : Proxy_Handler (efd, ioc, thr_mgr, socket_queue_size)
{
  this->direction_ = 'C';
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
      ACE_DEBUG ((LM_DEBUG, "(%t) queueing activated on handle %d to routing id %d\n",
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
  else /* if (n == length) */
    {
      // The whole event is sent, we can now safely deallocate the
      // buffer.  Note that this should decrement a reference count...
      delete event;
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
          // We are responsible for freeing an ACE_Message_Block if
          // failures occur.
          delete event;
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

// Constructor sets the consumer map pointer and the connector
// pointer.

Supplier_Proxy::Supplier_Proxy (Event_Forwarding_Discriminator *efd,
				Proxy_Handler_Connector *ioc,
				ACE_Thread_Manager *thr_mgr,
				int socket_queue_size)
  : msg_frag_ (0), 
    Proxy_Handler (efd, ioc, thr_mgr, socket_queue_size)
{
  this->direction_ = 'S';
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
	  delete this->msg_frag_;
	  this->msg_frag_ = 0;
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
    }

  // At this point there is a complete, valid header in msg_frag_
  ssize_t data_bytes_left_to_read = 
    sizeof (Event) - this->msg_frag_->length ();

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
      delete this->msg_frag_;
      this->msg_frag_ = 0;
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
          forward_addr = new ACE_Message_Block (sizeof (Event_Addr), 
						ACE_Message_Block::MB_PROTO, 
						this->msg_frag_);
	  if (forward_addr == 0)
	    {
	      delete this->msg_frag_;
	      this->msg_frag_ = 0;
	      errno = ENOMEM;
	      return -1;
	    }

          Event_Addr event_addr (this->id (), 
				 event->header_.supplier_id_,
				 event->header_.type_);
          // Copy the forwarding address from the Event_Addr into
          // forward_addr.
          forward_addr->copy ((char *) &event_addr, sizeof (Event_Addr));

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
		 event->header_.supplier_id_, event->header_.len_, this->total_bytes ()));
#endif
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

// Forward an event to its appropriate Consumer(s).

int
Supplier_Proxy::forward (ACE_Message_Block *forward_addr)
{
  // We got a valid event, so determine its virtual forwarding
  // address, which is stored in the first of the two event blocks,
  // which are chained together by this->recv().
	
  Event_Addr *forwarding_addr = (Event_Addr *) forward_addr->rd_ptr ();
	
  // Skip over the address portion and get the data.
  const ACE_Message_Block *const data = forward_addr->cont ();
	
  // <dispatch_set> points to the set of Consumers associated with
  // this forwarding address.
  Dispatch_Set *dispatch_set = 0;
	
  if (this->efd_->find (*forwarding_addr, dispatch_set) != -1)
    {
      // Check to see if there are any destinations.
      if (dispatch_set->size () == 0)
	ACE_DEBUG ((LM_WARNING, 
		   "there are no active destinations for this event currently\n"));
	    
      else		// There are destinations, so forward the event.
	{
	  Dispatch_Set_Iterator dsi (*dispatch_set);
		
	  for (Proxy_Handler **proxy_handler = 0; 
	       dsi.next (proxy_handler) != 0; 
	       dsi.advance ())
	    {
	      // Only process active proxy_handlers.
	      if ((*proxy_handler)->state () == Proxy_Handler::ESTABLISHED)
		{
		  // Clone the event portion (should be doing
		  // reference counting here...)
		  ACE_Message_Block *newmsg = data->clone (); 
		    
		  ACE_DEBUG ((LM_DEBUG, "(%t) sending to peer %d\n", 
			      (*proxy_handler)->id ()));
		    
		  if ((*proxy_handler)->put (newmsg) == -1)
		    {
		      if (errno == EWOULDBLOCK) // The queue has filled up!
			ACE_ERROR ((LM_ERROR, "(%t) %p\n", 
				   "gateway is flow controlled, so we're dropping events"));
		      else
			ACE_ERROR ((LM_ERROR, "(%t) %p transmission error to peer %d\n",
				   "put", (*proxy_handler)->id ()));
			
		      // We are responsible for freeing a
		      // ACE_Message_Block if failures occur.
		      delete newmsg;
		    }
		}
	    }
	  // Will become superfluous once we have reference
	  // counting...
	  delete forward_addr;
	  return 0;
	}
    }
  delete forward_addr;
  // Failure return.
  ACE_ERROR ((LM_DEBUG, "(%t) find failed on conn id = %d, logical id = %d, payload = %d\n", 
	     forwarding_addr->conn_id_, forwarding_addr->supplier_id_, forwarding_addr->type_));
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Map_Manager<Event_Addr, Dispatch_Set *, MAP_MUTEX>;
template class ACE_Map_Iterator<Event_Addr, Dispatch_Set *, MAP_MUTEX>;
template class ACE_Map_Entry<Event_Addr, Dispatch_Set *>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
