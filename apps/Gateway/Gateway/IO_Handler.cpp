// $Id$

#include "Consumer_Entry.h"
#include "IO_Handler_Connector.h"

// Convenient short-hands.
#define CO CONDITION
#define MU MAP_MUTEX

// The total number of bytes sent/received on this channel.

size_t 
IO_Handler::total_bytes (void)
{
  return this->total_bytes_;
}

void 
IO_Handler::total_bytes (size_t bytes)
{
  this->total_bytes_ += bytes;
}

IO_Handler::IO_Handler (Consumer_Map *consumer_map,
			IO_Handler_Connector *ioc,
			ACE_Thread_Manager *thr_mgr,
			int socket_queue_size)
  : ACE_Svc_Handler<ACE_SOCK_STREAM, SYNCH_STRATEGY> (thr_mgr),
    consumer_map_ (consumer_map),
    id_ (-1),
    total_bytes_ (0),
    state_ (IO_Handler::IDLE),
    connector_ (ioc),
    timeout_ (1),
    max_timeout_ (IO_Handler::MAX_RETRY_TIMEOUT), 
    socket_queue_size_ (socket_queue_size)
{
}

// Set the associated channel.

void
IO_Handler::active (int a)
{
  this->state (a == 0 ? IO_Handler::IDLE : IO_Handler::ESTABLISHED);
}

// Get the associated channel.

int
IO_Handler::active (void)
{
  return this->state () == IO_Handler::ESTABLISHED;
}

// Set the direction.

void
IO_Handler::direction (char d)
{
  this->direction_ = d;
}

// Get the direction.

char
IO_Handler::direction (void)
{
  return this->direction_;
}

// Sets the timeout delay.

void
IO_Handler::timeout (int to)
{
  if (to > this->max_timeout_)
    to = this->max_timeout_;

  this->timeout_ = to;
}

// Recalculate the current retry timeout delay using exponential
// backoff.  Returns the original timeout (i.e., before the
// recalculation).

int
IO_Handler::timeout (void)
{
  int old_timeout = this->timeout_;
  this->timeout_ *= 2;
  
  if (this->timeout_ > this->max_timeout_)
    this->timeout_ = this->max_timeout_;

  return old_timeout;
}

// Sets the max timeout delay.

void
IO_Handler::max_timeout (int mto)
{
  this->max_timeout_ = mto;
}

// Gets the max timeout delay.

int
IO_Handler::max_timeout (void)
{
  return this->max_timeout_;
}

// Restart connection asynchronously when timeout occurs.

int
IO_Handler::handle_timeout (const ACE_Time_Value &, const void *)
{
  ACE_DEBUG ((LM_DEBUG, 
	     "(%t) attempting to reconnect IO_Handler %d with timeout = %d\n", 
             this->id (), this->timeout_));
  return this->connector_->initiate_connection (this, ACE_Synch_Options::asynch);
}

// Restart connection (blocking_semantics dicates whether we
// restart synchronously or asynchronously).

int
IO_Handler::reinitiate_connection (void)
{
  // Skip over deactivated descriptors.
  if (this->get_handle () != -1)
    {
      // Make sure to close down peer to reclaim descriptor.
      this->peer ().close ();

#if 0
//      if (this->state () == FAILED)
//        {
      // Reinitiate timeout to improve reconnection time.
//          this->timeout (1);
#endif
      
      ACE_DEBUG ((LM_DEBUG, 
		 "(%t) scheduling reinitiation of IO_Handler %d\n",
		 this->id ()));

      // Reschedule ourselves to try and connect again.
      if (ACE_Service_Config::reactor ()->schedule_timer 
	  (this, 0, this->timeout ()) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", 
			  "schedule_timer"), -1);
    }
  return 0;
}

// Handle shutdown of the IO_Handler object.

int
IO_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, 
	      "(%t) shutting down IO_Handler %d on handle %d\n", 
	     this->id (), this->get_handle ()));
  
  return this->reinitiate_connection ();
}

// Set the state of the channel.

void
IO_Handler::state (IO_Handler::State s)
{
  this->state_ = s;
}

// Perform the first-time initiation of a connection to the peer.

int
IO_Handler::initialize_connection (void)
{
  this->state_ = IO_Handler::ESTABLISHED;

  // Restart the timeout to 1.
  this->timeout (1);

#if defined (ASSIGN_SUPPLIER_ID)
  // Action that sends the route id to the peerd.

  CONN_ID id = htons (this->id ());

  ssize_t n = this->peer ().send ((const void *) &id, sizeof id);

  if (n != sizeof id)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", 
		      n == 0 ? "gatewayd has closed down unexpectedly" : "send"), 
		      -1);
#endif /* ASSIGN_SUPPLIER_ID */
  return 0;
}

// Set the size of the socket queue.

void
IO_Handler::socket_queue_size (void)
{
  if (this->socket_queue_size_ > 0)
    {
      int option = this->direction_ == 'S' ? SO_RCVBUF : SO_SNDBUF;

      if (this->peer ().set_option (SOL_SOCKET, option,
				  &this->socket_queue_size_, sizeof (int)) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "set_option"));
    }
}

// Upcall from the ACE_Acceptor::handle_input() that
// delegates control to our application-specific IO_Handler.

int
IO_Handler::open (void *a)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) IO_Handler's fd = %d\n", 
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

// Return the current state of the channel.

IO_Handler::State
IO_Handler::state (void)
{
  return this->state_;
}

void
IO_Handler::id (CONN_ID id)
{
  this->id_ = id;
}

CONN_ID
IO_Handler::id (void)
{
  return this->id_;
}

// Set the peer's address information.
int 
IO_Handler::bind (const ACE_INET_Addr &remote_addr, 
	       const ACE_INET_Addr &local_addr,
	       CONN_ID id)
{
  this->remote_addr_ = remote_addr;
  this->local_addr_ = local_addr;
  this->id_ = id;
  return 0;
}

ACE_INET_Addr &
IO_Handler::remote_addr (void)
{
  return this->remote_addr_;
}

ACE_INET_Addr &
IO_Handler::local_addr (void)
{
  return this->local_addr_;
}

// Constructor sets the consumer map pointer.

Consumer_Handler::Consumer_Handler (Consumer_Map *consumer_map,
				    IO_Handler_Connector *ioc,
				    ACE_Thread_Manager *thr_mgr,
				    int socket_queue_size)
  : IO_Handler (consumer_map, ioc, thr_mgr, socket_queue_size)
{
  this->direction_ = 'C';
  this->msg_queue ()->high_water_mark (Consumer_Handler::QUEUE_SIZE);
}

// This method should be called only when the peer shuts down
// unexpectedly.  This method simply marks the IO_Handler as 
// having failed so that handle_close () can reconnect.

int 
Consumer_Handler::handle_input (ACE_HANDLE)
{
  char buf[1];

  this->state (IO_Handler::FAILED);

  switch (this->peer ().recv (buf, sizeof buf))
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR,
			"(%t) Peer has failed unexpectedly for Output IO_Handler %d\n",
			this->id ()), -1);
      /* NOTREACHED */
    case 0:
      ACE_ERROR_RETURN ((LM_ERROR,
			"(%t) Peer has shutdown unexpectedly for Output IO_Handler %d\n",
			this->id ()), -1);
      /* NOTREACHED */
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
			"(%t) Peer is sending input on Output IO_Handler %d\n",
			this->id ()), -1);
      /* NOTREACHED */
    }
}

// Perform a non-blocking put() of event MB.  If we are unable to
// send the entire event  the remainder is re-queued at the *front* of
// the Event_List.

int
Consumer_Handler::nonblk_put (ACE_Message_Block *mb)
{
  // Try to send the event.  If we don't send it all (e.g., due to
  // flow control), then re-queue the remainder at the head of the
  // Event_List and ask the ACE_Reactor to inform us (via
  // handle_output()) when it is possible to try again. 

  ssize_t n;

  if ((n = this->send (mb)) == -1)
    {
      // Things have gone wrong, let's try to close down and set up a new reconnection.
      this->state (IO_Handler::FAILED);
      this->handle_close ();
      return -1;
    }
  else if (errno == EWOULDBLOCK) // Didn't manage to send everything.
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) queueing activated on handle %d to routing id %d\n",
                 this->get_handle (), this->id ()));

      // ACE_Queue in *front* of the list to preserve order.
      if (this->msg_queue ()->enqueue_head 
	  (mb, (ACE_Time_Value *) &ACE_Time_Value::zero) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "enqueue_head"), -1);
      
      // Tell ACE_Reactor to call us back when we can send again.
      else if (ACE_Service_Config::reactor ()->
	       schedule_wakeup (this, ACE_Event_Handler::WRITE_MASK) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "schedule_wakeup"), -1);
      return 0;
    }
  else
    return n;
}

int
Consumer_Handler::send (ACE_Message_Block *mb)
{
  ssize_t n;    
  size_t len = mb->length ();

  if ((n = this->peer ().send (mb->rd_ptr (), len)) <= 0)
    return errno == EWOULDBLOCK ? 0 : n;
  else if (n < len)
    // Re-adjust pointer to skip over the part we did send.
    mb->rd_ptr (n);
  else /* if (n == length) */
    {
      // The whole event is sent, we can now safely deallocate the
      // buffer.  Note that this should decrement a reference count...
      delete mb;
      errno = 0;
    }
  this->total_bytes (n);
  return n;
}

// Finish sending an event when flow control conditions abate.
// This method is automatically called by the ACE_Reactor.

int 
Consumer_Handler::handle_output (ACE_HANDLE)
{
  ACE_Message_Block *mb = 0;
  int           status = 0;
  
  ACE_DEBUG ((LM_DEBUG, 
	      "(%t) in handle_output on handle %d\n", 
	      this->get_handle ()));
  // The list had better not be empty, otherwise there's a bug!

  if (this->msg_queue ()->dequeue_head 
      (mb, (ACE_Time_Value *) &ACE_Time_Value::zero) != -1)
    {
      switch (this->nonblk_put (mb))
        {
        case 0:           // Partial send.
          ACE_ASSERT (errno == EWOULDBLOCK);
          // Didn't write everything this time, come back later...
          break;

        case -1:
          // Caller is responsible for freeing a ACE_Message_Block if failures occur.
          delete mb;
          ACE_ERROR ((LM_ERROR, "(%t) %p\n", "transmission failure"));

          /* FALLTHROUGH */
        default: // Sent the whole thing.
          
          // If we succeed in writing the entire event (or we did not
          // fail due to EWOULDBLOCK) then check if there are more
          // events on the Event_List.  If there aren't, tell the
          // ACE_Reactor not to notify us anymore (at least until
          // there are new events queued up).

          if (this->msg_queue ()->is_empty ())
            {
              ACE_DEBUG ((LM_DEBUG, 
			  "(%t) queueing deactivated on handle %d to routing id %d\n",
                         this->get_handle (), this->id ()));


              if (ACE_Service_Config::reactor ()->
		  cancel_wakeup (this, ACE_Event_Handler::WRITE_MASK) == -1)
                ACE_ERROR ((LM_ERROR, "(%t) %p\n", "cancel_wakeup"));
            }
        }
    }
  else 
    ACE_ERROR ((LM_ERROR, "(%t) %p\n", "dequeue_head"));
  return 0;      
}

// Send an event to a peer (may queue if necessary).

int 
Consumer_Handler::put (ACE_Message_Block *mb, ACE_Time_Value *)
{
  if (this->msg_queue ()->is_empty ())
    // Try to send the event *without* blocking!
    return this->nonblk_put (mb); 
  else
    // If we have queued up events due to flow control then just
    // enqueue and return.
    return this->msg_queue ()->enqueue_tail 
      (mb, (ACE_Time_Value *) &ACE_Time_Value::zero);
}

// Constructor sets the consumer map pointer and the connector
// pointer.

Supplier_Handler::Supplier_Handler (Consumer_Map *consumer_map,
				    IO_Handler_Connector *ioc,
				    ACE_Thread_Manager *thr_mgr,
				    int socket_queue_size)
  : msg_frag_ (0), 
    IO_Handler (consumer_map, ioc, thr_mgr, socket_queue_size)
{
  this->direction_ = 'S';
  this->msg_queue ()->high_water_mark (0);
}

// Receive a Peer event from peerd.  Handles fragmentation.
//
// The routing event returned from recv consists of two parts:
// 1. The Address part, contains the virtual routing id.
// 2. The Data part, which contains the actual data to be routed.
// 
// The reason for having two parts is to shield the higher layers
// of software from knowledge of the event structure.

int
Supplier_Handler::recv (ACE_Message_Block *&forward_addr)
{ 
  Event *event;
  size_t len;
  ssize_t n = 0;
  ssize_t m = 0;
  size_t offset = 0;

  if (this->msg_frag_ == 0)
    // No existing fragment...
    ACE_NEW_RETURN (this->msg_frag_, 
		    ACE_Message_Block (sizeof (Event)), 
		    -1);

  event = (Event *) this->msg_frag_->rd_ptr ();

  const ssize_t HEADER_SIZE = sizeof (Event_Header);
  ssize_t header_bytes_left_to_read = HEADER_SIZE - this->msg_frag_->length ();

  if (header_bytes_left_to_read > 0)
    {
      n = this->peer ().recv (this->msg_frag_->wr_ptr (), 
			      header_bytes_left_to_read);

      if (n == -1 /* error */
	  || n == 0  /* EOF */)
	{
	  ACE_ERROR ((LM_ERROR, "%p\n", 
		      "Recv error during header read "));
	  ACE_DEBUG ((LM_DEBUG, 
		      "attempted to read %d\n", 
		      header_bytes_left_to_read));
	  delete this->msg_frag_;
	  this->msg_frag_ = 0;
	  return n;
	}

      // Bump the write pointer by the amount read.
      this->msg_frag_->wr_ptr (n);

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
  len = sizeof event->buf_ + HEADER_SIZE - this->msg_frag_->length ();

  // Try to receive the remainder of the event

  switch (m = this->peer ().recv (event->buf_ + offset, len))
    {
    case -1:
      if (errno == EWOULDBLOCK) 
        {
          // This shouldn't happen since the ACE_Reactor
          // just triggered us to handle pending I/O!
          ACE_DEBUG ((LM_DEBUG, "(%t) unexpected recv failure\n"));
          errno = EWOULDBLOCK;
          return -1;
        }
      else
        /* FALLTHROUGH */;

    case 0: // Premature EOF.
      delete this->msg_frag_;
      this->msg_frag_ = 0;
      return 0;

    default:
      if (m != len)
        // Re-adjust pointer to skip over the part we've read.
        {
          this->msg_frag_->wr_ptr (m);
          errno = EWOULDBLOCK;
          return -1; // Inform caller that we didn't get the whole event.
        }
      else
        {
          // Set the write pointer at 1 past the end of the event.
          this->msg_frag_->wr_ptr (m);

          // Set the read pointer to the beginning of the event.
          this->msg_frag_->rd_ptr (this->msg_frag_->base ());

          // Allocate an event forwarding header and chain the data
          // portion onto its continuation field.
          ACE_NEW_RETURN (forward_addr,
			  ACE_Message_Block (sizeof (Event_Addr), 
					     ACE_Message_Block::MB_PROTO, 
					     this->msg_frag_),
			  -1);

          Event_Addr event_addr (this->id (), event->header_.routing_id_, 0);
          // Copy the forwarding address from the Event_Addr into
          // forward_addr.
          forward_addr->copy ((char *) &event_addr, sizeof (Event));

          // Reset the pointer to indicate we've got an entire event.
          this->msg_frag_ = 0; 
        }
      this->total_bytes (m + n);
#if defined (VERBOSE)
      ACE_DEBUG ((LM_DEBUG, "(%t) channel id = %d, route id = %d, len = %d, payload = %*s",
		 event_addr.conn_id_, event->header_.routing_id_, event->header_.len_,
		 event->header_.len_, event->buf_));
#else
      ACE_DEBUG ((LM_DEBUG, "(%t) route id = %d, cur len = %d, total bytes read = %d\n",
		 event->header_.routing_id_, event->header_.len_, this->total_bytes ()));
#endif
      return m + n;
    }
}

// Receive various types of input (e.g., Peer event from the 
// gatewayd, as well as stdio).

int 
Supplier_Handler::handle_input (ACE_HANDLE)
{
  ACE_Message_Block *forward_addr = 0;

  switch (this->recv (forward_addr))
    {
    case 0:
      // Note that a peer should never initiate a shutdown.
      this->state (IO_Handler::FAILED);
      ACE_ERROR_RETURN ((LM_ERROR, 
			"(%t) Peer has closed down unexpectedly for Input IO_Handler %d\n", 
                        this->id ()), -1);
      /* NOTREACHED */
    case -1:
      if (errno == EWOULDBLOCK)
        // A short-read, we'll come back and finish it up later on!
        return 0;
      else // A weird problem occurred, shut down and start again.
        {
          this->state (IO_Handler::FAILED);
          ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p for Input IO_Handler %d\n", 
			    "Peer has failed unexpectedly",
                           this->id ()), -1);
        }
      /* NOTREACHED */
    default:
      return this->forward (forward_addr);
    }
}

// Route an event to its appropriate destination.

int
Supplier_Handler::forward (ACE_Message_Block *forward_addr)
{
  // We got a valid event, so determine its virtual routing id,
  // which is stored in the first of the two event blocks chained
  // together.
	
  Event_Addr *forwarding_key = (Event_Addr *) forward_addr->rd_ptr ();
	
  // Skip over the address portion.
  const ACE_Message_Block *const data = forward_addr->cont ();
	
  // RE points to the routing entry located for this routing id.
  Consumer_Entry *re = 0;
	
  if (this->consumer_map_->find (*forwarding_key, re) != -1)
    {
      // Check to see if there are any destinations.
      if (re->destinations ()->size () == 0)
	ACE_DEBUG ((LM_WARNING, 
		   "there are no active destinations for this event currently\n"));
	    
      else		// There are destinations, so forward the event.
	{
	  Consumer_Entry::ENTRY_SET *esp = re->destinations ();
	  Consumer_Entry::ENTRY_ITERATOR si (*esp);
		
	  for (IO_Handler **channel = 0; si.next (channel) != 0; si.advance ())
	    {
	      // Only process active channels.
	      if ((*channel)->active ())
		{
		  // Clone the event portion (should be doing reference counting here...)
		  ACE_Message_Block *newmsg = data->clone (); 
		    
		  ACE_DEBUG ((LM_DEBUG, "(%t) sending to peer %d\n", (*channel)->id ()));
		    
		  if ((*channel)->put (newmsg) == -1)
		    {
		      if (errno == EWOULDBLOCK) // The queue has filled up!
			ACE_ERROR ((LM_ERROR, "(%t) %p\n", 
				   "gateway is flow controlled, so we're dropping events"));
		      else
			ACE_ERROR ((LM_ERROR, "(%t) %p transmission error to route %d\n",
				   "put", (*channel)->id ()));
			
		      // Caller is responsible for freeing a ACE_Message_Block if failures occur.
		      delete newmsg;
		    }
		}
	    }
	  // Will become superfluous once we have reference counting...
	  delete forward_addr;
	  return 0;
	}
    }
  delete forward_addr;
  // Failure return.
  ACE_ERROR ((LM_DEBUG, "(%t) find failed on conn id = %d, logical id = %d, payload = %d\n", 
	     forwarding_key->conn_id_, forwarding_key->logical_id_, forwarding_key->payload_));
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Map_Manager<Event_Addr, Consumer_Entry *, MAP_MUTEX>;
template class ACE_Map_Iterator<Event_Addr, Consumer_Entry *, MAP_MUTEX>;
template class ACE_Map_Entry<Event_Addr, Consumer_Entry *>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
