#include "Thr_Proxy_Handler.h"
// $Id$

#include "Proxy_Handler_Connector.h"

#if defined (ACE_HAS_THREADS)
Thr_Consumer_Proxy::Thr_Consumer_Proxy (Event_Forwarding_Discriminator *efd,
					    Proxy_Handler_Connector *ioc,
					    ACE_Thread_Manager *thr_mgr,
					    int socket_queue_size)
  : Consumer_Proxy (efd, ioc, thr_mgr, socket_queue_size)
{
}

// This method should be called only when the peer shuts down
// unexpectedly.  This method marks the Proxy_Handler as having failed and
// deactivates the ACE_Message_Queue (to wake up the thread blocked on
// <dequeue_head> in svc()).  Thr_Output_Handler::handle_close () will
// eventually try to reconnect...

int 
Thr_Consumer_Proxy::handle_input (ACE_HANDLE h)
{
  this->Consumer_Proxy::handle_input (h);
  ACE_Service_Config::reactor ()->remove_handler (h,
					   ACE_Event_Handler::RWE_MASK
					   | ACE_Event_Handler::DONT_CALL);
  // Deactivate the queue while we try to get reconnected.
  this->msg_queue ()->deactivate ();
  return 0;
}

// Initialize the threaded Consumer_Proxy object and spawn a new
// thread.

int 
Thr_Consumer_Proxy::open (void *)
{
  // Set the size of the socket queue.
  this->socket_queue_size ();

  // Turn off non-blocking I/O.
  if (this->peer ().disable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "enable"), -1);

  // Register ourselves to receive input events (which indicate that
  // the Peer has shut down unexpectedly).
  if (ACE_Service_Config::reactor ()->register_handler (this,
						 ACE_Event_Handler::READ_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "register_handler"), -1);

  if (this->initialize_connection ())
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", 
		      "initialize_connection"), -1);

  // Reactivate message queue.  If it was active then this is the
  // first time in and we need to spawn a thread, otherwise the queue
  // was inactive due to some problem and we've already got a thread.
  if (this->msg_queue ()->activate () == ACE_Message_Queue<SYNCH_STRATEGY>::WAS_ACTIVE)
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) spawning new thread\n"));
      // Become an active object by spawning a new thread to transmit
      // messages to peers.
      return this->activate (THR_NEW_LWP | THR_DETACHED);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) reusing existing thread\n"));
      return 0;
    }
}

// ACE_Queue up a message for transmission (must not block since all
// Supplier_Proxys are single-threaded).

int 
Thr_Consumer_Proxy::put (ACE_Message_Block *mb, ACE_Time_Value *)
{
  // Perform non-blocking enqueue.
  return this->msg_queue ()->enqueue_tail (mb, (ACE_Time_Value *) &ACE_Time_Value::zero);
}

// Transmit messages to the peer (note simplification resulting from
// threads...)

int 
Thr_Consumer_Proxy::svc (void)
{
  for (;;)
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) connected! Thr_Consumer_Proxy's fd = %d\n", 
		 this->peer ().get_handle ()));

      // Since this method runs in its own thread it is OK to block on
      // output.

      for (ACE_Message_Block *mb = 0; 
	   this->msg_queue ()->dequeue_head (mb) != -1; )
	if (this->send (mb) == -1)
	  ACE_ERROR ((LM_ERROR, "(%t) %p\n", "send failed"));

	ACE_ASSERT (errno == ESHUTDOWN);

	ACE_DEBUG ((LM_DEBUG, "(%t) shutting down threaded Consumer_Proxy %d on handle %d\n", 
		   this->id (), this->get_handle ()));

	this->peer ().close ();

	for (this->timeout (1);
	     // Default is to reconnect synchronously.
	     this->connector_->initiate_connection (this) == -1; )
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

Thr_Supplier_Proxy::Thr_Supplier_Proxy (Event_Forwarding_Discriminator *efd,
					    Proxy_Handler_Connector *ioc,
					    ACE_Thread_Manager *thr_mgr,
					    int socket_queue_size)
  : Supplier_Proxy (efd, ioc, thr_mgr, socket_queue_size)
{
}

int 
Thr_Supplier_Proxy::open (void *)
{
  // Set the size of the socket queue.
  this->socket_queue_size ();

  // Turn off non-blocking I/O.
  if (this->peer ().disable (ACE_NONBLOCK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "enable"), -1);

  if (this->initialize_connection ())
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", 
		      "initialize_connection"), -1);

  // Reactivate message queue.  If it was active then this is the
  // first time in and we need to spawn a thread, otherwise the queue
  // was inactive due to some problem and we've already got a thread.
  if (this->msg_queue ()->activate () == ACE_Message_Queue<SYNCH_STRATEGY>::WAS_ACTIVE)
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) spawning new thread\n"));
      // Become an active object by spawning a new thread to transmit
      // messages to peers.
      return this->activate (THR_NEW_LWP | THR_DETACHED);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) reusing existing thread\n"));
      return 0;
    }
}

// Receive messages from a Peer in a separate thread (note reuse of
// existing code!).

int 
Thr_Supplier_Proxy::svc (void)
{
  for (;;)
    {
      ACE_DEBUG ((LM_DEBUG, "(%t) connected! Thr_Supplier_Proxy's fd = %d\n", 
		 this->peer ().get_handle ()));

      // Since this method runs in its own thread and processes
      // messages for one connection it is OK to block on input and
      // output.

      while (this->handle_input () != -1)
	continue;

      ACE_DEBUG ((LM_DEBUG, 
		  "(%t) shutting down threaded Supplier_Proxy %d on handle %d\n", 
		 this->id (), 
		 this->get_handle ()));

      this->peer ().close ();

      // Deactivate the queue while we try to get reconnected.
      this->msg_queue ()->deactivate ();

      for (this->timeout (1);
	   // Default is to reconnect synchronously.
	   this->connector_->initiate_connection (this) == -1; )
	{
	  ACE_Time_Value tv (this->timeout ());
	  ACE_ERROR ((LM_ERROR, 
		      "(%t) reattempting connection, sec = %d\n", tv.sec ()));
	  ACE_OS::sleep (tv);
	}
    }
  return 0;
}

#endif /* ACE_HAS_THREADS */
