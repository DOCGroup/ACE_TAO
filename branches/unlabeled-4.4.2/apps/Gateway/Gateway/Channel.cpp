
// $Id$

#include "Routing_Entry.h"
#include "Channel_Connector.h"

// Convenient short-hands.
#define CO CONDITION
#define MU MUTEX

// = The total number of bytes sent/received on this channel.
size_t 
Channel::total_bytes (void)
{
  return this->total_bytes_;
}

void 
Channel::total_bytes (size_t bytes)
{
  this->total_bytes_ += bytes;
}

Channel::Channel (ROUTING_TABLE *rt,
                  Channel_Connector *cc,
		  ACE_Thread_Manager *thr_mgr,
		  int socket_queue_size)
  : ACE_Svc_Handler<CHANNEL_PEER_STREAM, SYNCH> (thr_mgr),
    routing_table_ (rt),
    id_ (-1),
    total_bytes_ (0),
    state_ (Channel::IDLE),
    connector_ (cc),
    timeout_ (1),
    max_timeout_ (Channel::MAX_RETRY_TIMEOUT), 
    socket_queue_size_ (socket_queue_size)
{
}

// Set the associated channel.

void
Channel::active (int a)
{
  this->state (a == 0 ? Channel::IDLE : Channel::ESTABLISHED);
}

// Get the associated channel.

int
Channel::active (void)
{
  return this->state () == Channel::ESTABLISHED;
}

// Set the direction.

void
Channel::direction (char d)
{
  this->direction_ = d;
}

// Get the direction.

char
Channel::direction (void)
{
  return this->direction_;
}

// Sets the timeout delay.

void
Channel::timeout (int to)
{
  if (to > this->max_timeout_)
    to = this->max_timeout_;

  this->timeout_ = to;
}

// Recalculate the current retry timeout delay using exponential
// backoff.  Returns the original timeout (i.e., before the
// recalculation).

int
Channel::timeout (void)
{
  int old_timeout = this->timeout_;
  this->timeout_ *= 2;
  
  if (this->timeout_ > this->max_timeout_)
    this->timeout_ = this->max_timeout_;

  return old_timeout;
}

// Sets the max timeout delay.

void
Channel::max_timeout (int mto)
{
  this->max_timeout_ = mto;
}

// Gets the max timeout delay.

int
Channel::max_timeout (void)
{
  return this->max_timeout_;
}

// Restart connection asynchronously when timeout occurs.

int
Channel::handle_timeout (const ACE_Time_Value &, const void *)
{
  ACE_DEBUG ((LM_DEBUG, 
	     "(%t) attempting to reconnect Channel %d with timeout = %d\n", 
             this->id (), this->timeout_));
  return this->connector_->initiate_connection (this, ACE_Synch_Options::asynch);
}

// Restart connection (blocking_semantics dicates whether we
// restart synchronously or asynchronously).

int
Channel::reinitiate_connection (void)
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
		 "(%t) scheduling reinitiation of Channel %d\n",
		 this->id ()));

      // Reschedule ourselves to try and connect again.
      if (ACE_Service_Config::reactor ()->schedule_timer (this, 0, 
						   this->timeout ()) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", 
			  "schedule_timer"), -1);
    }
  return 0;
}

// Handle shutdown of the Channel object.

int
Channel::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) shutting down Channel %d on handle %d\n", 
	     this->id (), this->get_handle ()));
  
  return this->reinitiate_connection ();
}

// Set the state of the channel.

void
Channel::state (Channel::State s)
{
  this->state_ = s;
}

// Perform the first-time initiation of a connection to the peer.

int
Channel::initialize_connection (void)
{
  this->state_ = Channel::ESTABLISHED;

  // Restart the timeout to 1.
  this->timeout (1);

#if defined (ASSIGN_ROUTING_ID)
  // Action that sends the route id to the peerd.

  CONN_ID id = htons (this->id ());

  ssize_t n = this->peer ().send ((const void *) &id, sizeof id);

  if (n != sizeof id)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", 
		      n == 0 ? "gatewayd has closed down unexpectedly" : "send"), -1);
#endif /* ASSIGN_ROUTING_ID */
  return 0;
}

// Set the size of the socket queue.

void
Channel::socket_queue_size (void)
{
  if (this->socket_queue_size_ > 0)
    {
      int option = this->direction_ == 'I' ? SO_RCVBUF : SO_SNDBUF;

      if (this->peer ().set_option (SOL_SOCKET, option,
				  &this->socket_queue_size_, sizeof (int)) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "set_option"));
    }
}

// Upcall from the ACE_Acceptor::handle_input() that
// delegates control to our application-specific Channel.

int
Channel::open (void *a)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) Channel's fd = %d\n", this->peer ().get_handle ()));

  // Set the size of the socket queue.
  this->socket_queue_size ();

  // Turn on non-blocking I/O.
  if (this->peer ().enable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "enable"), -1);

  // Call down to the base class to activate and register this handler.
  if (this->ACE_Svc_Handler<CHANNEL_PEER_STREAM, SYNCH>::open (a) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "activate"), -1);

  return this->initialize_connection ();
}

// Return the current state of the channel.

Channel::State
Channel::state (void)
{
  return this->state_;
}

void
Channel::id (CONN_ID id)
{
  this->id_ = id;
}

CONN_ID
Channel::id (void)
{
  return this->id_;
}

// Set the peer's address information.
int 
Channel::bind (const ACE_INET_Addr &remote_addr, 
	       const ACE_INET_Addr &local_addr,
	       CONN_ID id)
{
  this->remote_addr_ = remote_addr;
  this->local_addr_ = local_addr;
  this->id_ = id;
  return 0;
}

ACE_INET_Addr &
Channel::remote_addr (void)
{
  return this->remote_addr_;
}

ACE_INET_Addr &
Channel::local_addr (void)
{
  return this->local_addr_;
}

// Constructor sets the routing table pointer.

Output_Channel::Output_Channel (ROUTING_TABLE *rt,
                                Channel_Connector *cc,
				ACE_Thread_Manager *thr_mgr,
				int socket_queue_size)
  : Channel (rt, cc, thr_mgr, socket_queue_size)
{
  this->direction_ = 'O';
  this->msg_queue ()->high_water_mark (Output_Channel::QUEUE_SIZE);
}

// This method should be called only when the peer shuts down
// unexpectedly.  This method simply marks the Channel as 
// having failed so that handle_close () can reconnect.

int 
Output_Channel::handle_input (ACE_HANDLE)
{
  char buf[1];

  this->state (Channel::FAILED);

  switch (this->peer ().recv (buf, sizeof buf))
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR,
			"(%t) Peer has failed unexpectedly for Output Channel %d\n",
			this->id ()), -1);
      /* NOTREACHED */
    case 0:
      ACE_ERROR_RETURN ((LM_ERROR,
			"(%t) Peer has shutdown unexpectedly for Output Channel %d\n",
			this->id ()), -1);
      /* NOTREACHED */
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
			"(%t) Peer is sending input on Output Channel %d\n",
			this->id ()), -1);
      /* NOTREACHED */
    }
}

int 
Output_Channel::svc (void)
{
  ACE_ERROR_RETURN ((LM_ERROR, "(%t) svc should not be called on Output_Channel!\n"), -1);
}

// Perform a non-blocking put() of message MB.  If we are unable to
// send the entire message  the remainder is re-queued at the *front* of
// the Message_List.

int
Output_Channel::nonblk_put (ACE_Message_Block *mb)
{
  // Try to send the message.  If we don't send it all (e.g., due to
  // flow control), then re-queue the remainder at the head of the
  // Message_List and ask the ACE_Reactor to inform us (via
  // handle_output()) when it is possible to try again. 

  ssize_t n;

  if ((n = this->send_peer (mb)) == -1)
    {
      // Things have gone wrong, let's try to close down and set up a new reconnection.
      this->state (Channel::FAILED);
      this->handle_close ();
      return -1;
    }
  else if (errno == EWOULDBLOCK) // Didn't manage to send everything.
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) queueing activated on handle %d to routing id %d\n",
                 this->get_handle (), this->id ()));

      // ACE_Queue in *front* of the list to preserve order.
      if (this->msg_queue ()->enqueue_head (mb, (ACE_Time_Value *) &ACE_Time_Value::zero) == -1)
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
Output_Channel::send_peer (ACE_Message_Block *mb)
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
      // The whole message is sent, we can now safely deallocate the buffer.
      // Note that this should decrement a reference count...
      delete mb;
      errno = 0;
    }
  this->total_bytes (n);
  return n;
}

// Finish sending a message when flow control conditions abate.
// This method is automatically called by the ACE_Reactor.

int 
Output_Channel::handle_output (ACE_HANDLE)
{
  ACE_Message_Block *mb = 0;
  int           status = 0;
  
  ACE_DEBUG ((LM_DEBUG, "(%t) in handle_output on handle %d\n", this->get_handle ()));
  // The list had better not be empty, otherwise there's a bug!

  if (this->msg_queue ()->dequeue_head (mb, (ACE_Time_Value *) &ACE_Time_Value::zero) != -1)
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
          
          // If we succeed in writing the entire message (or we did not fail
          // due to EWOULDBLOCK) then check if there are more messages on the Message_List.
          // If there aren't, tell the ACE_Reactor not to notify us anymore (at least
          // until there are new messages queued up).

          if (this->msg_queue ()->is_empty ())
            {
              ACE_DEBUG ((LM_DEBUG, "(%t) queueing deactivated on handle %d to routing id %d\n",
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

// Send a message to a peer (may queue if necessary).

int 
Output_Channel::put (ACE_Message_Block *mb, ACE_Time_Value *)
{
  if (this->msg_queue ()->is_empty ())
    // Try to send the message *without* blocking!
    return this->nonblk_put (mb); 
  else
    // If we have queued up messages due to flow control 
    // then just enqueue and return.
    return this->msg_queue ()->enqueue_tail (mb, (ACE_Time_Value *) &ACE_Time_Value::zero);
}

// Constructor sets the routing table pointer and the connector pointer.

Input_Channel::Input_Channel (ROUTING_TABLE *rt,
                              Channel_Connector *cc,
			      ACE_Thread_Manager *thr_mgr,
			      int socket_queue_size)
  : msg_frag_ (0), 
    Channel (rt, cc, thr_mgr, socket_queue_size)
{
  this->direction_ = 'I';
  this->msg_queue ()->high_water_mark (0);
}

int 
Input_Channel::put (ACE_Message_Block *, ACE_Time_Value *)
{
  ACE_ERROR_RETURN ((LM_ERROR, "(%t) put should not be called on Input_Channel!\n"), -1);
}

int 
Input_Channel::svc (void)
{
  ACE_ERROR_RETURN ((LM_ERROR, "(%t) svc should not be called on Input_Channel!\n"), -1);
}

// Receive a Peer message from peerd.  Handles fragmentation.
//
// The routing message returned from recv_peer consists of two parts:
// 1. The Address part, contains the virtual routing id.
// 2. The Data part, which contains the actual data to be routed.
// 
// The reason for having two parts is to shield the higher layers
// of software from knowledge of the message structure.

int
Input_Channel::recv_peer (ACE_Message_Block *&route_addr)
{ 
  Peer_Message *peer_msg;
  size_t len;
  ssize_t n = 0;
  ssize_t m = 0;
  size_t offset = 0;

  if (this->msg_frag_ == 0)
    // No existing fragment...
    ACE_NEW_RETURN (this->msg_frag_, 
		    ACE_Message_Block (sizeof (Peer_Message)), 
		    -1);

  peer_msg = (Peer_Message *) this->msg_frag_->rd_ptr ();

  const ssize_t HEADER_SIZE = sizeof (Peer_Header);
  ssize_t header_bytes_left_to_read = HEADER_SIZE - this->msg_frag_->length ();

  if (header_bytes_left_to_read > 0)
    {
      n = this->peer ().recv (this->msg_frag_->wr_ptr (), header_bytes_left_to_read);

      if (n == -1 /* error */
	  || n == 0  /* EOF */)
	{
	  ACE_ERROR ((LM_ERROR, "%p\n", "Recv error during header read "));
	  ACE_DEBUG ((LM_DEBUG, "attempted to read %d\n", header_bytes_left_to_read));
	  delete this->msg_frag_;
	  this->msg_frag_ = 0;
	  return n;
	}

      // Bump the write pointer by the amount read.
      this->msg_frag_->wr_ptr (n);

      // At this point we may or may not have the ENTIRE header.
      if (this->msg_frag_->length () < HEADER_SIZE)
	{
	  ACE_DEBUG ((LM_DEBUG, "Partial header received: only %d bytes\n",
		     this->msg_frag_->length ()));
	  // Notify the caller that we didn't get an entire message.
	  errno = EWOULDBLOCK;
	  return -1;
	}
    }

  // At this point there is a complete, valid header in msg_frag_
  len = sizeof peer_msg->buf_ + HEADER_SIZE - this->msg_frag_->length ();

  // Try to receive the remainder of the message

  switch (m = this->peer ().recv (peer_msg->buf_ + offset, len))
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
          return -1; // Inform caller that we didn't get the whole message.
        }
      else
        {
          // Set the write pointer at 1 past the end of the message.
          this->msg_frag_->wr_ptr (m);

          // Set the read pointer to the beginning of the message.
          this->msg_frag_->rd_ptr (this->msg_frag_->base ());

          // Allocate a routing message header and chain the data portion 
          // onto its continuation field.
          ACE_NEW_RETURN (route_addr,
			  ACE_Message_Block (sizeof (Peer_Addr), 
					     ACE_Message_Block::MB_PROTO, 
					     this->msg_frag_),
			  -1);

          Peer_Addr peer_addr (this->id (), peer_msg->header_.routing_id_, 0);
          // Copy the routing address from the Peer_Message into routing_addr.
          route_addr->copy ((char *) &peer_addr, sizeof (Peer_Addr));

          // Reset the pointer to indicate we've got an entire message.
          this->msg_frag_ = 0; 
        }
      this->total_bytes (m + n);
#if defined (VERBOSE)
      ACE_DEBUG ((LM_DEBUG, "(%t) channel id = %d, route id = %d, len = %d, payload = %*s",
		 peer_addr.conn_id_, peer_msg->header_.routing_id_, peer_msg->header_.len_,
		 peer_msg->header_.len_, peer_msg->buf_));
#else
      ACE_DEBUG ((LM_DEBUG, "(%t) route id = %d, cur len = %d, total bytes read = %d\n",
		 peer_msg->header_.routing_id_, peer_msg->header_.len_, this->total_bytes ()));
#endif
      return m + n;
    }
}

// Receive various types of input (e.g., Peer message from the 
// gatewayd, as well as stdio).

int 
Input_Channel::handle_input (ACE_HANDLE)
{
  ACE_Message_Block *route_addr = 0;

  switch (this->recv_peer (route_addr))
    {
    case 0:
      // Note that a peer should never initiate a shutdown.
      this->state (Channel::FAILED);
      ACE_ERROR_RETURN ((LM_ERROR, 
			"(%t) Peer has closed down unexpectedly for Input Channel %d\n", 
                        this->id ()), -1);
      /* NOTREACHED */
    case -1:
      if (errno == EWOULDBLOCK)
        // A short-read, we'll come back and finish it up later on!
        return 0;
      else // A weird problem occurred, shut down and start again.
        {
          this->state (Channel::FAILED);
          ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p for Input Channel %d\n", 
			    "Peer has failed unexpectedly",
                           this->id ()), -1);
        }
      /* NOTREACHED */
    default:
      return this->route_message (route_addr);
    }
}

// Route a message to its appropriate destination.

int
Input_Channel::route_message (ACE_Message_Block *route_addr)
{
  // We got a valid message, so determine its virtual routing id,
  // which is stored in the first of the two message blocks chained together.
	
  Peer_Addr *routing_key = (Peer_Addr *) route_addr->rd_ptr ();
	
  // Skip over the address portion.
  const ACE_Message_Block *const data = route_addr->cont ();
	
  // RE points to the routing entry located for this routing id.
  Routing_Entry *re = 0;
	
  if (this->routing_table_->find (*routing_key, re) != -1)
    {
      // Check to see if there are any destinations.
      if (re->destinations ()->size () == 0)
	ACE_DEBUG ((LM_WARNING, 
		   "there are no active destinations for this message currently\n"));
	    
      else		// There are destinations, so forward the message.
	{
	  Routing_Entry::ENTRY_SET *esp = re->destinations ();
	  Routing_Entry::ENTRY_ITERATOR si (*esp);
		
	  for (Channel **channel = 0; si.next (channel) != 0; si.advance ())
	    {
	      // Only process active channels.
	      if ((*channel)->active ())
		{
		  // Clone the message portion (should be doing reference counting here...)
		  ACE_Message_Block *newmsg = data->clone (); 
		    
		  ACE_DEBUG ((LM_DEBUG, "(%t) sending to peer %d\n", (*channel)->id ()));
		    
		  if ((*channel)->put (newmsg) == -1)
		    {
		      if (errno == EWOULDBLOCK) // The queue has filled up!
			ACE_ERROR ((LM_ERROR, "(%t) %p\n", 
				   "gateway is flow controlled, so we're dropping messages"));
		      else
			ACE_ERROR ((LM_ERROR, "(%t) %p transmission error to route %d\n",
				   "put", (*channel)->id ()));
			
		      // Caller is responsible for freeing a ACE_Message_Block if failures occur.
		      delete newmsg;
		    }
		}
	    }
	  // Will become superfluous once we have reference counting...
	  delete route_addr;
	  return 0;
	}
    }
  delete route_addr;
  // Failure return.
  ACE_ERROR ((LM_DEBUG, "(%t) find failed on conn id = %d, logical id = %d, payload = %d\n", 
	     routing_key->conn_id_, routing_key->logical_id_, routing_key->payload_));
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Map_Manager<Peer_Addr, Routing_Entry *, MUTEX>;
template class ACE_Map_Iterator<Peer_Addr, Routing_Entry *, MUTEX>;
template class ACE_Map_Entry<Peer_Addr, Routing_Entry *>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
