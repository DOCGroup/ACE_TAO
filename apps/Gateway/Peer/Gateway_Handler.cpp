#include "ace/Get_Opt.h"
// $Id$


#include "Gateway_Handler.h"

Gateway_Handler::Gateway_Handler (ACE_Thread_Manager *)
  : routing_id_ (0),
    msg_frag_ (0),
    total_bytes_ (0)
{
  this->msg_queue ()->high_water_mark (Gateway_Handler::QUEUE_SIZE);
}

int
Gateway_Handler::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) %S\n", signum));

  // Shut down the main event loop.
  ACE_Service_Config::end_reactor_event_loop ();
  return 0;
}

// Cache a binding to the HANDLER_MAP.

void
Gateway_Handler::map (HANDLER_MAP *m)
{
  this->map_ = m;
}

// Upcall from the ACE_Acceptor::handle_input() that turns control
// over to our application-specific Gateway handler.

int
Gateway_Handler::open (void *a)
{
  ACE_DEBUG ((LM_DEBUG, "Gateway handler's fd = %d\n", 
	     this->peer ().get_handle ()));

  // Call down to the base class to activate and register this
  // handler.
  if (this->inherited::open (a) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);

  if (this->peer ().enable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "enable"), -1);    

  Gateway_Handler *this_ = this;

  // Add ourselves to the map so we can be removed later on.
  if (this->map_->bind (this->get_handle (), this_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "bind"), -1);    

  char *to = ACE_OS::getenv ("TIMEOUT");
  int timeout = to == 0 ? 100000 : ACE_OS::atoi (to);

  // Schedule the time between disconnects.  This should really be a
  // "tunable" parameter.
  if (ACE_Service_Config::reactor ()->schedule_timer (this, 0, timeout) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "schedule_timer"));

  // If there are messages left in the queue, make sure we 
  // enable the ACE_Reactor appropriately to get them sent out.
  if (this->msg_queue ()->is_empty () == 0
      && ACE_Service_Config::reactor ()->schedule_wakeup (this, 
							  ACE_Event_Handler::WRITE_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "schedule_wakeup"), -1);

  // First action is to wait to be notified of our routing id.
  this->do_action_ = &Gateway_Handler::await_route_id;
  return 0;
}

// Read messages from stdin and send them to the gatewayd.

int
Gateway_Handler::xmit_stdin (void)
{
  if (this->routing_id_ != -1)
    {
      ssize_t n;
      ACE_Message_Block *mb;

      ACE_NEW_RETURN (mb, 
		      ACE_Message_Block (sizeof (Peer_Message)),
		      -1);

      Peer_Message *peer_msg = (Peer_Message *) mb->rd_ptr ();
      peer_msg->header_.routing_id_ = this->routing_id_;

      n = ACE_OS::read (ACE_STDIN, peer_msg->buf_, sizeof peer_msg->buf_);

      switch (n)
	{
	case 0:
	  ACE_DEBUG ((LM_DEBUG, "stdin closing down\n"));

	  // Take stdin out of the ACE_Reactor so we stop trying to
	  // send messages.
	  if (ACE_Service_Config::reactor ()->remove_handler 
	      (0, ACE_Event_Handler::DONT_CALL | ACE_Event_Handler::READ_MASK) == -1)
	    ACE_ERROR ((LM_ERROR, "%p\n", "remove_handler"));
	  delete mb;
	  break;
	case -1:
	  delete mb;
	  ACE_ERROR ((LM_ERROR, "%p\n", "read"));
	  break;
	default:
	  peer_msg->header_.len_ = htonl (n);
	  mb->wr_ptr (sizeof (Peer_Header) + n);

	  if (this->put (mb) == -1)
	    {
	      if (errno == EWOULDBLOCK) // The queue has filled up!
		ACE_ERROR ((LM_ERROR, "%p\n", 
			   "gateway is flow controlled, so we're dropping messages"));
	      else
		ACE_ERROR ((LM_ERROR, "%p\n", "transmission failure in xmit_stdin"));
	      
	      // Caller is responsible for freeing a ACE_Message_Block
	      // if failures occur.
	      delete mb;
	    }
	}
    }
  return 0;
}

// Perform a non-blocking put() of message MB.  If we are unable to
// send the entire message  the remainder is re-Taskd at the *front* of
// the Message_List.

int
Gateway_Handler::nonblk_put (ACE_Message_Block *mb)
{
  // Try to send the message.  If we don't send it all (e.g., due to
  // flow control), then re-ACE_Task the remainder at the head of the
  // Message_List and ask the ACE_Reactor to inform us (via
  // handle_output()) when it is possible to try again.

  ssize_t n;

  if ((n = this->send_peer (mb)) == -1)
    return -1;
  else if (errno == EWOULDBLOCK) // Didn't manage to send everything.
    {
      ACE_DEBUG ((LM_DEBUG, 
		  "queueing activated on handle %d to routing id %d\n",
		 this->get_handle (), this->routing_id_));

      // ACE_Queue in *front* of the list to preserve order.
      if (this->msg_queue ()->enqueue_head 
	  (mb, (ACE_Time_Value *) &ACE_Time_Value::zero) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "enqueue_head"), -1);
      
      // Tell ACE_Reactor to call us back when we can send again.
      if (ACE_Service_Config::reactor ()->schedule_wakeup
	  (this, ACE_Event_Handler::WRITE_MASK) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "schedule_wakeup"), -1);
      return 0;
    }
  else
    return n;
}

// Finish sending a message when flow control conditions abate.  This
// method is automatically called by the ACE_Reactor.

int 
Gateway_Handler::handle_output (ACE_HANDLE)
{
  ACE_Message_Block *mb = 0;
  
  ACE_DEBUG ((LM_DEBUG, "in handle_output\n"));
  // The list had better not be empty, otherwise there's a bug!

  if (this->msg_queue ()->dequeue_head 
      (mb, (ACE_Time_Value *) &ACE_Time_Value::zero) != -1)
    {
      switch (this->nonblk_put (mb))
	{
	case 0: 	  // Partial send.
	  ACE_ASSERT (errno == EWOULDBLOCK);
	  // Didn't write everything this time, come back later...
	  break;

	case -1:
	  // Caller is responsible for freeing a ACE_Message_Block if
	  // failures occur.
	  delete mb;
	  ACE_ERROR ((LM_ERROR, "%p\n", 
		      "transmission failure in handle_output"));

	  /* FALLTHROUGH */
	default: // Sent the whole thing.
	  
	  // If we succeed in writing the entire message (or we did
	  // not fail due to EWOULDBLOCK) then check if there are more
	  // messages on the Message_List.  If there aren't, tell the
	  // ACE_Reactor not to notify us anymore (at least until
	  // there are new messages queued up).

	  if (this->msg_queue ()->is_empty ())
	    {
	      ACE_DEBUG ((LM_DEBUG, 
			  "queue now empty on handle %d to routing id %d\n",
			  this->get_handle (), 
			  this->routing_id_));

	      if (ACE_Service_Config::reactor ()->cancel_wakeup
		  (this, ACE_Event_Handler::WRITE_MASK) == -1)
		ACE_ERROR ((LM_ERROR, "%p\n", "cancel_wakeup"));
	    }
	}
    }
  else 
    ACE_ERROR ((LM_ERROR, "%p\n", "dequeue_head"));
  return 0;      
}

// Send a message to a peer (may ACE_Task if necessary).

int 
Gateway_Handler::put (ACE_Message_Block *mb, ACE_Time_Value *)
{
  if (this->msg_queue ()->is_empty ())
    // Try to send the message *without* blocking!
    return this->nonblk_put (mb); 
  else
    // If we have queued up messages due to flow control then just
    // enqueue and return.
    return this->msg_queue ()->enqueue_tail 
      (mb, (ACE_Time_Value *) &ACE_Time_Value::zero); 
}

// Send an Peer message to gatewayd.

int
Gateway_Handler::send_peer (ACE_Message_Block *mb)
{
  ssize_t n;	
  size_t len = mb->length ();

  if ((n = this->peer ().send (mb->rd_ptr (), len)) <= 0)
    return errno == EWOULDBLOCK ? 0 : n;
  else if (n < (ssize_t) len)
    {
      // Re-adjust pointer to skip over the part we did send.
      mb->rd_ptr (n);
      this->total_bytes_ += n;
    }
  else /* if (n == length) */
    {
      // The whole message is sent, we can now safely deallocate the
      // buffer.  Note that this should decrement a reference count...
      this->total_bytes_ += n;
      delete mb;
      errno = 0;
    }
  ACE_DEBUG ((LM_DEBUG, "sent %d bytes, total bytes sent = %d\n",
	     n, this->total_bytes_));
  return n;
}

// Receive an Peer message from gatewayd.  Handles fragmentation.

int
Gateway_Handler::recv_peer (ACE_Message_Block *&mb)
{ 
  Peer_Message *peer_msg;
  size_t       len;
  ssize_t      n;
  size_t       offset = 0;

  if (this->msg_frag_ == 0)
    {
      ACE_NEW_RETURN (this->msg_frag_,
		      ACE_Message_Block (sizeof (Peer_Message)),
		      -1);

      // No existing fragment...
      if (this->msg_frag_ == 0)
	ACE_ERROR_RETURN ((LM_ERROR, "out of memory\n"), -1);

      peer_msg = (Peer_Message *) this->msg_frag_->rd_ptr ();

      switch (n = this->peer ().recv (peer_msg, sizeof (Peer_Header)))
	{
	case sizeof (Peer_Header):
	  len = ntohl (peer_msg->header_.len_);
	  if (len <= sizeof peer_msg->buf_)
	    {
	      this->msg_frag_->wr_ptr (sizeof (Peer_Header));
	      break; // The message is within the maximum size range.
	    }
	  else
	    ACE_ERROR ((LM_ERROR, "message too long = %d\n", len));
	  /* FALLTHROUGH */
	default:
	  ACE_ERROR ((LM_ERROR, "invalid length = %d\n", n));
	  n = -1;
	  /* FALLTHROUGH */
	case -1:
	  /* FALLTHROUGH */
	case 0:
	  // Make sure to free up memory on error returns.
	  delete this->msg_frag_;
	  this->msg_frag_ = 0;
	  return n;
	}
    }
  else
    {
      offset = this->msg_frag_->length () - sizeof (Peer_Header);
      len    = peer_msg->header_.len_ - offset;
    }

  switch (n = this->peer ().recv (peer_msg->buf_ + offset, len))
    {
    case -1:
      if (errno == EWOULDBLOCK) 
	{
          // This shouldn't happen since the ACE_Reactor
	  // just triggered us to handle pending I/O!
	  ACE_DEBUG ((LM_DEBUG, "unexpected recv failure\n"));
	  // Since ACE_DEBUG might change errno, we need to reset it
	  // here.
	  errno = EWOULDBLOCK;
	  return -1;
        }
      else
	/* FALLTHROUGH */;

    case 0: // EOF.
      delete this->msg_frag_;
      this->msg_frag_ = 0;
      return n;

    default:
      if (n != (ssize_t) len)
	// Re-adjust pointer to skip over the part we've read.
	{
	  this->msg_frag_->wr_ptr (n);
	  errno = EWOULDBLOCK;
	  // Inform caller that we didn't get the whole message.
	  return -1; 
	}
      else
	{
	  // Set the write pointer at 1 past the end of the message.
	  this->msg_frag_->wr_ptr (n);

	  // Set the read pointer to the beginning of the message.
	  this->msg_frag_->rd_ptr (this->msg_frag_->base ());

	  mb = this->msg_frag_;

	  // Reset the pointer to indicate we've got an entire
	  // message.
	  this->msg_frag_ = 0; 
	}
      return n;
    }
}

// Receive various types of input (e.g., Peer message from the
// gatewayd, as well as stdio).

int 
Gateway_Handler::handle_input (ACE_HANDLE sd)
{
  ACE_DEBUG ((LM_DEBUG, "in handle_input, sd = %d\n", sd));
  if (sd == ACE_STDIN) // Handle message from stdin.
    return this->xmit_stdin ();
  else
    // Perform the appropriate action depending on the state we are
    // in.
    return (this->*do_action_) ();
}

// Action that receives the route id.  

int
Gateway_Handler::await_route_id (void)
{
  ssize_t n = this->peer ().recv (&this->routing_id_, 
				  sizeof this->routing_id_);

  if (n != sizeof this->routing_id_)
    {
      if (n == 0)
	ACE_ERROR_RETURN ((LM_ERROR, 
			   "gatewayd has closed down unexpectedly\n"), -1);
      else
	ACE_ERROR_RETURN ((LM_ERROR, 
			   "%p, bytes received on handle %d = %d\n", 
			  "recv", this->get_handle (), n), -1);
    }
  else
    ACE_DEBUG ((LM_DEBUG, "assigned routing id %d\n", 
		this->routing_id_));

  // Transition to the action that waits for Peer messages.
  this->do_action_ = &Gateway_Handler::await_messages;

  // Reset standard input.
  ACE_OS::rewind (stdin);

  // Register this handler to receive test messages on stdin.
  if (ACE_Service_Config::reactor ()->register_handler 
      (ACE_STDIN, this, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "register_handler"), -1);
  return 0;
}

// Action that receives messages.

int
Gateway_Handler::await_messages (void)
{
  ACE_Message_Block *mb = 0;
  ssize_t n = this->recv_peer (mb);

  switch (n)
    {
    case 0:
      ACE_ERROR_RETURN ((LM_ERROR, "gatewayd has closed down\n"), -1);
      /* NOTREACHED */
    case -1:
      if (errno == EWOULDBLOCK)
	// A short-read, we'll come back and finish it up later on!
	return 0;
      else
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "recv_peer"), -1);
      /* NOTREACHED */
    default:
      {
	// We got a valid message, so let's process it now!  At the
	// moment, we just print out the message contents...

	Peer_Message  *peer_msg = (Peer_Message *) mb->rd_ptr ();
	this->total_bytes_ += mb->length ();

#if defined (VERBOSE)
	ACE_DEBUG ((LM_DEBUG, 
		    "route id = %d, len = %d, payload = %*s",
		   peer_msg->header_.routing_id_, peer_msg->header_.len_,
		   peer_msg->header_.len_, peer_msg->buf_));
#else
	ACE_DEBUG ((LM_DEBUG, 
		    "route id = %d, cur len = %d, total len = %d\n",
		    peer_msg->header_.routing_id_, 
		    peer_msg->header_.len_, 
		    this->total_bytes_));
#endif
	delete mb;
	return 0;
      }
    }
}

// Periodically send messages via ACE_Reactor timer mechanism.

int
Gateway_Handler::handle_timeout (const ACE_Time_Value &, const void *)
{
  // Skip over deactivated descriptors.
  if (this->get_handle () != -1)
    {
      // Unbind ourselves from the map.
      if (this->map_->unbind (this->get_handle ()) == -1)
	ACE_ERROR ((LM_ERROR, "%p\n", "unbind"));

      // Shut down the handler.
      this->handle_close ();
    }
  return 0;
}

// Handle shutdown of the Gateway_Handler object.

int
Gateway_Handler::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  if (this->get_handle () != ACE_INVALID_HANDLE)
    {
      ACE_DEBUG ((LM_DEBUG, "shutting down Gateway_Handler on handle %d\n", 
		 this->get_handle ()));

      // Explicitly remove ourselves for handle 0 (the ACE_Reactor
      // removes this->handle (), note that
      // ACE_Event_Handler::DONT_CALL instructs the ACE_Reactor *not*
      // to call this->handle_close(), which would otherwise lead to
      // recursion!).
      if (ACE_Service_Config::reactor ()->remove_handler 
	  (0, ACE_Event_Handler::DONT_CALL | ACE_Event_Handler::READ_MASK) == -1)
	ACE_ERROR ((LM_ERROR, "handle = %d: %p\n", 
		   0, "remove_handler"));

      // Deregister this handler with the ACE_Reactor.
      if (ACE_Service_Config::reactor ()->remove_handler 
	  (this, ACE_Event_Handler::DONT_CALL | ACE_Event_Handler::RWE_MASK) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "handle = %d: %p\n", 
			  this->get_handle (), "remove_handler"), -1);

      // Close down the peer.
      this->peer ().close ();
    }
  return 0;
}

Gateway_Acceptor::Gateway_Acceptor (Gateway_Handler *handler)
  : gateway_handler_ (handler)
{
  this->gateway_handler_->map (&this->map_);
}

// Note how this method just passes back the pre-allocated
// Gateway_Handler instead of having the ACE_Acceptor allocate a new
// one each time!

Gateway_Handler *
Gateway_Acceptor::make_svc_handler (void)
{
  return this->gateway_handler_;
}

int 
Gateway_Acceptor::handle_signal (int signum, siginfo_t *, ucontext_t *)
{
  ACE_DEBUG ((LM_DEBUG, "signal %S occurred\n", signum));
  return 0;
}

/* Returns information on the currently active service. */

int
Gateway_Acceptor::info (char **strp, size_t length) const
{
  char buf[BUFSIZ];
  char addr_str[BUFSIZ];

  ACE_INET_Addr addr;

  if (this->acceptor ().get_local_addr (addr) == -1)
    return -1;
  else if (addr.addr_to_string (addr_str, sizeof addr) == -1)
    return -1;

  ACE_OS::sprintf (buf, "%s\t %s/%s %s",
	     "Gateway peer daemon", addr_str, "tcp",
	     "# IRIDIUM SRP traffic generator and data sink\n");

  if (*strp == 0 && (*strp = ACE_OS::strdup (buf)) == 0)
    return -1;
  else
    ACE_OS::strncpy (*strp, buf, length);
  return ACE_OS::strlen (buf);
}

// Hook called by the explicit dynamic linking facility to terminate
// the peer.

int
Gateway_Acceptor::fini (void)
{
  HANDLER_ITERATOR mi (this->map_);
  
  for (MAP_ENTRY *me = 0;
       mi.next (me) != 0; 
       mi.advance ())
    {
      if (me->int_id_->get_handle () != -1)
	{
	  ACE_DEBUG ((LM_DEBUG, "closing down handle %d\n", 
		      me->int_id_->get_handle ()));
	  me->int_id_->handle_close ();
	}
      else
	ACE_DEBUG ((LM_DEBUG, "already closed %d\n"));
      me->int_id_->destroy (); // Will trigger a delete.
    }

  this->gateway_handler_->destroy (); // Will trigger a delete.
  return inherited::fini ();
}

// Hook called by the explicit dynamic linking facility to initialize
// the peer.

int
Gateway_Acceptor::init (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "dp:", 0);
  ACE_INET_Addr addr;

  for (int c; (c = get_opt ()) != -1; )
    {
      switch (c)
	{
	case 'p': 
	  addr.set (ACE_OS::atoi (get_opt.optarg));
	  break;
	case 'd':
	  break;
	default:
	  break;
	}
    }

  if (ACE_Service_Config::reactor ()->register_handler (SIGPIPE, this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "register_handler"), -1);
    
  if (this->open (addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "open"), -1);
  else if (ACE_Service_Config::reactor ()->register_handler 
	   (this, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, 
		       "registering service with ACE_Reactor\n"), -1);

  ACE_Sig_Set sig_set;
  sig_set.sig_add (SIGINT);
  sig_set.sig_add (SIGQUIT);

  // Register ourselves to receive SIGINT and SIGQUIT so we can shut
  // down gracefully via signals.
  
  if (ACE_Service_Config::reactor ()->register_handler (sig_set, 
						 this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "register_handler"), -1);
  return 0;
}

// Dynamically linked factory function that dynamically allocates a
// new Gateway_Acceptor object.

ACE_Service_Object *
_alloc_peerd (void)
{
  // This function illustrates how we can associate a ACE_Svc_Handler
  // with the ACE_Acceptor at initialization time.
  Gateway_Handler *handler;
  
  ACE_NEW_RETURN (handler, Gateway_Handler, 0);
  ACE_Service_Object *temp;
  
  ACE_NEW_RETURN (temp, Gateway_Acceptor (handler), 0);
  return temp;
}
