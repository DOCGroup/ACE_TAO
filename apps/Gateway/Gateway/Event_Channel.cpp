/* -*- C++ -*- */
// $Id$

#define ACE_BUILD_SVC_DLL
#include "Proxy_Handler_Connector.h"
#include "Event_Channel.h"

ACE_Event_Channel_Options::ACE_Event_Channel_Options (void)
  : performance_window_ (0),
    blocking_semantics_ (ACE_NONBLOCK),
    socket_queue_size_ (0)
{
}

ACE_Event_Channel::~ACE_Event_Channel (void)
{
}

ACE_Event_Channel::ACE_Event_Channel (void)
{
}

ACE_Event_Channel_Options &
ACE_Event_Channel::options (void)
{
  return this->options_;
}

ACE_Event_Channel::handle_timeout (const ACE_Time_Value &, 
				   const void *)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) doing the performance timeout here...\n"));
  CONNECTION_MAP_ITERATOR cmi (this->connection_map_);

  // If we've got a ACE_Thread Manager then use it to suspend all the
  // threads.  This will enable us to get an accurate count.

#if defined (USE_OUTPUT_MT) || defined (USE_INPUT_MT)
  if (ACE_Service_Config::thr_mgr ()->suspend_all () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "suspend_all"), -1);
  ACE_DEBUG ((LM_DEBUG, "(%t) suspending all threads..."));
#endif /* USE_INPUT_MT || USE_OUTPUT_MT */

  size_t total_bytes_in = 0;
  size_t total_bytes_out = 0;
  
  // Iterate through the connection map summing up the number of bytes
  // sent/received.

  for (CONNECTION_MAP_ENTRY *me = 0;
       cmi.next (me) != 0; 
       cmi.advance ())
    {
      Proxy_Handler *proxy_handler = me->int_id_;

      if (proxy_handler->proxy_role () == 'C')
	total_bytes_out += proxy_handler->total_bytes ();
      else // proxy_handler->proxy_role () == 'S'
	total_bytes_in += proxy_handler->total_bytes ();
    }

#if defined (ACE_NLOGGING)
  ACE_OS::fprintf (stderr, "After %d seconds, \ntotal_bytes_in = %d\ntotal_bytes_out = %d\n",
		   performance_window_,
		   total_bytes_in, 
		   total_bytes_out);

  ACE_OS::fprintf (stderr, "%f Mbits/sec received.\n", 
		   (float) (total_bytes_in * 8 / (float) (1024*1024*this->performance_window_)));

  ACE_OS::fprintf (stderr, "%f Mbits/sec sent.\n", 
		   (float) (total_bytes_out * 8 / (float) (1024*1024*this->performance_window_)));
#else
  ACE_DEBUG ((LM_DEBUG, "(%t) after %d seconds, \ntotal_bytes_in = %d\ntotal_bytes_out = %d\n",
	      this->options ().performance_window_,
	      total_bytes_in,
	      total_bytes_out));
  ACE_DEBUG ((LM_DEBUG, "(%t) %f Mbits/sec received.\n", 
	      (float) (total_bytes_in * 8 / (float) (1024 * 1024 * this->options ().performance_window_))));
  ACE_DEBUG ((LM_DEBUG, "(%t) %f Mbits/sec sent.\n", 
	      (float) (total_bytes_out * 8 / (float) (1024 * 1024 * this->options ().performance_window_))));
#endif /* ACE_NLOGGING */

#if defined (USE_INPUT_MT) || defined (USE_OUTPUT_MT)
  ACE_DEBUG ((LM_DEBUG, "(%t) resuming all threads..."));

  // Resume all the threads again.

  if (ACE_Service_Config::thr_mgr ()->resume_all () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "resume_all"), -1);
#endif /* USE_INPUT_MT || USE_OUTPUT_MT */

  return 0;
}

ACE_Event_Channel::put (ACE_Message_Block *forward_addr, 
			ACE_Time_Value *)
{
  // We got a valid event, so determine its virtual forwarding
  // address, which is stored in the first of the two event blocks,
  // which are chained together by this->recv().
	
  Event_Key *forwarding_addr = (Event_Key *) forward_addr->rd_ptr ();
	
  // Skip over the address portion and get the data.
  ACE_Message_Block *data = forward_addr->cont ();
	
  // <dispatch_set> points to the set of Consumers associated with
  // this forwarding address.
  Consumer_Dispatch_Set *dispatch_set = 0;
	
  if (this->efd_.find (*forwarding_addr, dispatch_set) == -1)
    // Failure.
    ACE_ERROR ((LM_DEBUG, 
		"(%t) find failed on conn id = %d, logical id = %d, type = %d\n",
		forwarding_addr->conn_id_, 
		forwarding_addr->supplier_id_, 
		forwarding_addr->type_));
  else
    {
      // Check to see if there are any consumers.
      if (dispatch_set->size () == 0)
	ACE_DEBUG ((LM_WARNING, 
		   "there are no active consumers for this event currently\n"));
	    
      else // There are consumers, so forward the event.
	{
	  Consumer_Dispatch_Set_Iterator dsi (*dispatch_set);
		
	  // At this point, we should assign a thread-safe locking
	  // strategy to the Message_Block is we're running in a
	  // multi-threaded configuration.
	  // data->locking_strategy (MB_Locking_Strategy::instance ());

	  for (Proxy_Handler **proxy_handler = 0; 
	       dsi.next (proxy_handler) != 0; 
	       dsi.advance ())
	    {
	      // Only process active proxy_handlers.
	      if ((*proxy_handler)->state () == Proxy_Handler::ESTABLISHED)
		{
		  // Duplicate the event portion via reference
		  // counting.
		  ACE_Message_Block *dup_msg = data->duplicate ();
		    
		  ACE_DEBUG ((LM_DEBUG, "(%t) sending to peer %d\n", 
			      (*proxy_handler)->id ()));
		    
		  if ((*proxy_handler)->put (dup_msg) == -1)
		    {
		      if (errno == EWOULDBLOCK) // The queue has filled up!
			ACE_ERROR ((LM_ERROR, "(%t) %p\n", 
				   "gateway is flow controlled, so we're dropping events"));
		      else
			ACE_ERROR ((LM_ERROR, "(%t) %p transmission error to peer %d\n",
				   "put", (*proxy_handler)->id ()));
			
		      // We are responsible for releasing an
		      // ACE_Message_Block if failures occur.
		      dup_msg->release ();
		    }
		}
	    }
	}
    }

  // Release the memory in the message block.
  forward_addr->release ();
  return 0;
}

ACE_Event_Channel::svc (void)
{
  return 0;
}

int
ACE_Event_Channel::initiate_proxy_connection (Proxy_Handler *proxy_handler,
					      ACE_Synch_Options &synch_options)
{
  return this->connector_.initiate_connection (proxy_handler, 
					       synch_options);
}

int
ACE_Event_Channel::complete_proxy_connection (Proxy_Handler *proxy_handler)
{
  int option = proxy_handler->proxy_role () == 'S' ? SO_RCVBUF : SO_SNDBUF;
  int socket_queue_size = this->options ().socket_queue_size_;

  if (proxy_handler->peer ().set_option (SOL_SOCKET, 
					 option,
					 &socket_queue_size,
					 sizeof (int)) == -1)
    ACE_ERROR ((LM_ERROR, "(%t) %p\n", "set_option"));

  proxy_handler->thr_mgr (ACE_Service_Config::thr_mgr ());

  // Our state is now "established."
  proxy_handler->state (Proxy_Handler::ESTABLISHED);

  // Restart the timeout to 1.
  proxy_handler->timeout (1);

  ACE_INT32 id = htonl (proxy_handler->id ());

  // Send the connection id to the peerd.

  ssize_t n = proxy_handler->peer ().send ((const void *) &id, sizeof id);

  if (n != sizeof id)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", 
		      n == 0 ? "peer has closed down unexpectedly" : "send"), 
		      -1);
}

// Restart connection (blocking_semantics dicates whether we restart
// synchronously or asynchronously).

int
ACE_Event_Channel::reinitiate_proxy_connection (Proxy_Handler *proxy_handler)
{
  // Skip over deactivated descriptors.
  if (proxy_handler->get_handle () != ACE_INVALID_HANDLE)
    {
      // Make sure to close down peer to reclaim descriptor.
      proxy_handler->peer ().close ();

      ACE_DEBUG ((LM_DEBUG, 
		 "(%t) scheduling reinitiation of Proxy_Handler %d\n",
		 proxy_handler->id ()));

      // Reschedule ourselves to try and connect again.
      if (ACE_Service_Config::reactor ()->schedule_timer 
	  (proxy_handler, 0, proxy_handler->timeout ()) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", 
			  "schedule_timer"), -1);
    }
  return 0;
}

// Initiate connections with the Consumer and Supplier Peers.

ACE_Event_Channel::initiate_connections (void)
{
  CONNECTION_MAP_ITERATOR cmi (this->connection_map_);
				
  ACE_Synch_Options synch_options;
  
  if (this->options ().blocking_semantics_ == ACE_NONBLOCK)
    synch_options = ACE_Synch_Options::asynch;
  else 
    synch_options = ACE_Synch_Options::synch;

  // Iterate through the Consumer Map connecting all the
  // Proxy_Handlers.

  for (CONNECTION_MAP_ENTRY *me = 0;
       cmi.next (me) != 0;
       cmi.advance ())
    {
      Proxy_Handler *proxy_handler = me->int_id_;

      if (this->initiate_proxy_connection 
	  (proxy_handler, synch_options) == -1)
	continue; // Failures are handled elsewhere...
    }

  return 0;
}

// This method gracefully shuts down all the Handlers in the
// Proxy_Handler Connection Map.

ACE_Event_Channel::close (u_long)
{
#if defined (USE_INPUT_MT) || defined (USE_OUTPUT_MT)
  ACE_DEBUG ((LM_DEBUG, "(%t) suspending all threads\n"));
  if (ACE_Service_Config::thr_mgr ()->suspend_all () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "suspend_all"), -1);
#endif /* USE_INPUT_MT || USE_OUTPUT_MT */

  CONNECTION_MAP_ITERATOR cmi (this->connection_map_);

  // Iterate over all the handlers and shut them down.

  for (CONNECTION_MAP_ENTRY *me;
       cmi.next (me) != 0; 
       cmi.advance ())
    {
      Proxy_Handler *proxy_handler = me->int_id_;

      ACE_DEBUG ((LM_DEBUG, "(%t) closing down connection %d\n",
		 proxy_handler->id ()));

      if (proxy_handler->state () != Proxy_Handler::IDLE)
	// Mark Proxy_Handler as DISCONNECTING so we don't try to
	// reconnect...
	proxy_handler->state (Proxy_Handler::DISCONNECTING);

      // Deallocate Proxy_Handler resources.
      proxy_handler->destroy (); // Will trigger a delete.
    }

  return 0;
}

int
ACE_Event_Channel::find_proxy (ACE_INT32 conn_id, 
			       Proxy_Handler *&proxy_handler)
{
  return this->connection_map_.find (conn_id, proxy_handler);
}

int
ACE_Event_Channel::bind_proxy (Proxy_Handler *proxy_handler)
{
  switch (this->connection_map_.bind (proxy_handler->id (), proxy_handler))
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR, 
			 "(%t) bind failed for connection %d\n", 
			 proxy_handler->id ()), -1);
      /* NOTREACHED */
    case 1: // Oops, found a duplicate!
      ACE_ERROR_RETURN ((LM_ERROR, 
			 "(%t) duplicate connection %d, already bound\n", 
			 proxy_handler->id ()), -1);
      /* NOTREACHED */
    case 0:
      // Success.
      return 0;
    }
}

int
ACE_Event_Channel::subscribe (const Event_Key &event_addr, 
			      Consumer_Dispatch_Set *cds)
{
  // Bind with consumer map, keyed by peer address.
  switch (this->efd_.bind (event_addr, cds))
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR, "(%t) bind failed for connection %d\n", 
			 event_addr.conn_id_), -1);
      /* NOTREACHED */
    case 1: // Oops, found a duplicate!
      ACE_ERROR_RETURN ((LM_DEBUG, "(%t) duplicate consumer map entry %d, "
			 "already bound\n", event_addr.conn_id_), -1);
      /* NOTREACHED */
    case 0:
      // Success.
      return 0;
    }
}

ACE_Event_Channel::open (void *)
{
  // Ignore SIPPIPE so each Consumer_Proxy can handle it.
  ACE_Sig_Action sig (ACE_SignalHandler (SIG_IGN), SIGPIPE);

#if 0
  // If this->performance_window_ > 0 start a timer.

  if (this->options ().performance_window_ > 0)
    {
      if (ACE_Service_Config::reactor ()->schedule_timer 
	  (this, 0, this->options ().performance_window_) == -1)
	ACE_ERROR ((LM_ERROR, "(%t) %p\n", "schedule_timer"));
      else
	ACE_DEBUG ((LM_DEBUG, "starting timer for %d seconds...\n", 
		   this->options ().performance_window_)));
    }
#endif

  return 0;
}
