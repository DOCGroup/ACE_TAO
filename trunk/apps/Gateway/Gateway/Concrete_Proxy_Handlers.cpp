// $Id$

#define ACE_BUILD_SVC_DLL
#include "Event_Channel.h"
#include "Concrete_Proxy_Handlers.h"

Consumer_Proxy::Consumer_Proxy (const Proxy_Config_Info &pci)
  : Proxy_Handler (pci)
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
  ACE_DEBUG ((LM_DEBUG, "(%t) sending %d bytes to Consumer %d\n",
	      event->length (), this->id ()));

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

Supplier_Proxy::Supplier_Proxy (const Proxy_Config_Info &pci)
  : msg_frag_ (0), 
    Proxy_Handler (pci)
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
		    ACE_Message_Block (sizeof (Event),
				       ACE_Message_Block::MB_DATA,
				       0,
				       0,
				       0,
				       this->event_channel_->options ().locking_strategy_),
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
						this->msg_frag_,
						0,
						0,
						this->event_channel_->options ().locking_strategy_);
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
      ACE_DEBUG ((LM_DEBUG, "(%t) supplier id = %d, cur len = %d, total bytes read = %d\n",
		  event->header_.supplier_id_, event->header_.len_, data_received + header_received));
      if (this->event_channel_->options ().verbose_)
	ACE_DEBUG ((LM_DEBUG, "data_ = %*s\n", event->header_.len_ - 2, event->data_));

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
  return this->event_channel_->put (forward_addr);
}

#if defined (ACE_HAS_THREADS)
Thr_Consumer_Proxy::Thr_Consumer_Proxy (const Proxy_Config_Info &pci)
  : Consumer_Proxy (pci)
{
}

// This method should be called only when the Consumer shuts down
// unexpectedly.  This method marks the Proxy_Handler as having failed
// and deactivates the ACE_Message_Queue (to wake up the thread
// blocked on <dequeue_head> in svc()).
// Thr_Output_Handler::handle_close () will eventually try to
// reconnect...

int 
Thr_Consumer_Proxy::handle_input (ACE_HANDLE h)
{
  // Call down to the <Consumer_Proxy> to handle this first.
  this->Consumer_Proxy::handle_input (h);

  ACE_Service_Config::reactor ()->remove_handler 
    (h, ACE_Event_Handler::ALL_EVENTS_MASK | ACE_Event_Handler::DONT_CALL);

  // Deactivate the queue while we try to get reconnected.
  this->msg_queue ()->deactivate ();
  return 0;
}

// Initialize the threaded Consumer_Proxy object and spawn a new
// thread.

int 
Thr_Consumer_Proxy::open (void *)
{
  // Turn off non-blocking I/O.
  if (this->peer ().disable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "enable"), -1);

  // Call back to the <Event_Channel> to complete our initialization.
  else if (this->event_channel_->complete_proxy_connection (this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "complete_proxy_connection"), -1);

  // Register ourselves to receive input events (which indicate that
  // the Consumer has shut down unexpectedly).
  else if (ACE_Service_Config::reactor ()->register_handler 
      (this, ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "register_handler"), -1);

  // Reactivate message queue.  If it was active then this is the
  // first time in and we need to spawn a thread, otherwise the queue
  // was inactive due to some problem and we've already got a thread.
  else if (this->msg_queue ()->activate () == ACE_Message_Queue<ACE_SYNCH>::WAS_ACTIVE)
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) spawning new thread\n"));
      // Become an active object by spawning a new thread to transmit
      // events to Consumers.
      return this->activate (THR_NEW_LWP | THR_DETACHED);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) reusing existing thread\n"));
      return 0;
    }
}

// Queue up an event for transmission (must not block since
// Supplier_Proxys may be single-threaded).

int 
Thr_Consumer_Proxy::put (ACE_Message_Block *mb, ACE_Time_Value *)
{
  // Perform non-blocking enqueue.
  return this->msg_queue ()->enqueue_tail 
    (mb, (ACE_Time_Value *) &ACE_Time_Value::zero);
}

// Transmit events to the peer (note simplification resulting from
// threads...)

int 
Thr_Consumer_Proxy::svc (void)
{

  for (;;)
    {
      ACE_DEBUG ((LM_DEBUG, 
		  "(%t) Thr_Consumer_Proxy's handle = %d\n", 
		 this->peer ().get_handle ()));

      // Since this method runs in its own thread it is OK to block on
      // output.

      for (ACE_Message_Block *mb = 0; 
	   this->msg_queue ()->dequeue_head (mb) != -1; 
	   )
	{
	  if (this->send (mb) == -1)
	    ACE_ERROR ((LM_ERROR, "(%t) %p\n", "send failed"));
	}

      ACE_ASSERT (errno == ESHUTDOWN);

      ACE_DEBUG ((LM_DEBUG, 
		  "(%t) shutting down threaded Consumer_Proxy %d on handle %d\n", 
		  this->id (), this->get_handle ()));

      this->peer ().close ();

      for (this->timeout (1);
	   // Default is to reconnect synchronously.
	   this->event_channel_->initiate_proxy_connection (this) == -1; )
	{
	  ACE_Time_Value tv (this->timeout ());

	  ACE_ERROR ((LM_ERROR, 
		      "(%t) reattempting connection, sec = %d\n", 
		      tv.sec ()));

	  ACE_OS::sleep (tv);
	}
    }

  return 0;
}

Thr_Supplier_Proxy::Thr_Supplier_Proxy (const Proxy_Config_Info &pci)
  : Supplier_Proxy (pci)
{
}

int 
Thr_Supplier_Proxy::open (void *)
{
  // Turn off non-blocking I/O.
  if (this->peer ().disable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "enable"), -1);

  // Call back to the <Event_Channel> to complete our initialization.
  else if (this->event_channel_->complete_proxy_connection (this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "complete_proxy_connection"), -1);

  // Reactivate message queue.  If it was active then this is the
  // first time in and we need to spawn a thread, otherwise the queue
  // was inactive due to some problem and we've already got a thread.
  else if (this->msg_queue ()->activate () == ACE_Message_Queue<ACE_SYNCH>::WAS_ACTIVE)
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) spawning new thread\n"));
      // Become an active object by spawning a new thread to transmit
      // events to peers.
      return this->activate (THR_NEW_LWP | THR_DETACHED);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) reusing existing thread\n"));
      return 0;
    }
}

// Receive events from a Peer in a separate thread (note reuse of
// existing code!).

int 
Thr_Supplier_Proxy::svc (void)
{
  for (;;)
    {
      ACE_DEBUG ((LM_DEBUG, 
		  "(%t) Thr_Supplier_Proxy's handle = %d\n", 
		 this->peer ().get_handle ()));

      // Since this method runs in its own thread and processes events
      // for one connection it is OK to call down to the
      // <Supplier_Proxy::handle_input> method, which blocks on input.

      while (this->Supplier_Proxy::handle_input () != -1)
	continue;

      ACE_DEBUG ((LM_DEBUG, 
		  "(%t) shutting down threaded Supplier_Proxy %d on handle %d\n", 
		 this->id (), this->get_handle ()));

      this->peer ().close ();

      // Deactivate the queue while we try to get reconnected.
      this->msg_queue ()->deactivate ();

      for (this->timeout (1);
	   // Default is to reconnect synchronously.
	   this->event_channel_->initiate_proxy_connection (this) == -1; )
	{
	  ACE_Time_Value tv (this->timeout ());
	  ACE_ERROR ((LM_ERROR, 
		      "(%t) reattempting connection, sec = %d\n", 
		      tv.sec ()));
	  ACE_OS::sleep (tv);
	}
    }
  return 0;
}

#endif /* ACE_HAS_THREADS */
