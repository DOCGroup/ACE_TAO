/* -*- C++ -*- */
// $Id$

#define ACE_BUILD_SVC_DLL
#include "Proxy_Handler_Connector.h"
#include "Event_Channel.h"

ACE_Event_Channel_Options::ACE_Event_Channel_Options (void)
  : locking_strategy_ (0),
    performance_window_ (0),
    blocking_semantics_ (ACE_NONBLOCK),
    socket_queue_size_ (0),
    threading_strategy_ (REACTIVE),
    acceptor_port_ (ACE_DEFAULT_GATEWAY_SERVER_PORT),
    connector_role_ (0),
    acceptor_role_ (0),
    verbose_ (0)
{
}

ACE_Event_Channel_Options::~ACE_Event_Channel_Options (void)
{
  delete this->locking_strategy_;
}

ACE_Event_Channel::~ACE_Event_Channel (void)
{
}

ACE_Event_Channel::ACE_Event_Channel (void)
  : acceptor_ (*this)
{
}

ACE_Event_Channel_Options &
ACE_Event_Channel::options (void)
{
  return this->options_;
}

int
ACE_Event_Channel::compute_performance_statistics (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) doing the performance timeout here...\n"));
  PROXY_MAP_ITERATOR cmi (this->proxy_map_);

  // If we've got a ACE_Thread Manager then use it to suspend all the
  // threads.  This will enable us to get an accurate count.

  if (this->options ().threading_strategy_ != ACE_Event_Channel_Options::REACTIVE)
    {
      if (ACE_Service_Config::thr_mgr ()->suspend_all () == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "suspend_all"), -1);
      ACE_DEBUG ((LM_DEBUG, "(%t) suspending all threads..."));
    }

  size_t total_bytes_in = 0;
  size_t total_bytes_out = 0;
  
  // Iterate through the connection map summing up the number of bytes
  // sent/received.

  for (PROXY_MAP_ENTRY *me = 0;
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
  ACE_OS::fprintf (stderr, 
		   "After %d seconds, \ntotal_bytes_in = %d\ntotal_bytes_out = %d\n",
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

  // Resume all the threads again.

  if (this->options ().threading_strategy_ != ACE_Event_Channel_Options::REACTIVE)
    {
      if (ACE_Service_Config::thr_mgr ()->resume_all () == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "resume_all"), -1);
      ACE_DEBUG ((LM_DEBUG, "(%t) resuming all threads..."));
    }

  return 0;
}

int
ACE_Event_Channel::handle_timeout (const ACE_Time_Value &, 
				   const void *)
{
  return this->compute_performance_statistics ();
}

// This method forwards the <event> to Consumer that have registered
// to receive it.

int
ACE_Event_Channel::put (ACE_Message_Block *event, 
			ACE_Time_Value *)
{
  // We got a valid event, so determine its virtual forwarding
  // address, which is stored in the first of the two event blocks,
  // which are chained together by this->recv().
	
  Event_Key *forwarding_addr = (Event_Key *) event->rd_ptr ();
	
  // Skip over the address portion and get the data.
  ACE_Message_Block *data = event->cont ();
	
  // <dispatch_set> points to the set of Consumers associated with
  // this forwarding address.
  Consumer_Dispatch_Set *dispatch_set = 0;
	
  if (this->efd_.find (*forwarding_addr, dispatch_set) == -1)
    // Failure.
    ACE_ERROR ((LM_DEBUG, 
		"(%t) find failed on conn id = %d, supplier id = %d, type = %d\n",
		forwarding_addr->proxy_id_, 
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
		    
		  ACE_DEBUG ((LM_DEBUG, "(%t) forwarding to Consumer %d\n", 
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
  event->release ();
  return 0;
}

int
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
  return 0;
}

// Restart connection (blocking_semantics dicates whether we restart
// synchronously or asynchronously).

int
ACE_Event_Channel::reinitiate_proxy_connection (Proxy_Handler *proxy_handler)
{
  // Skip over proxies with deactivated handles.
  if (proxy_handler->get_handle () != ACE_INVALID_HANDLE)
    {
      // Make sure to close down peer to reclaim descriptor.
      proxy_handler->peer ().close ();

      if (proxy_handler->state () != Proxy_Handler::DISCONNECTING)
	{
	  ACE_DEBUG ((LM_DEBUG, 
		      "(%t) scheduling reinitiation of Proxy_Handler %d\n",
		      proxy_handler->id ()));

	  // Reschedule ourselves to try and connect again.
	  if (ACE_Service_Config::reactor ()->schedule_timer 
	      (proxy_handler, 0, proxy_handler->timeout ()) == -1)
	    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", 
			       "schedule_timer"), -1);
	}
    }
  return 0;
}

// Initiate active connections with the Consumer and Supplier Peers.

void
ACE_Event_Channel::initiate_connector (void)
{
  PROXY_MAP_ITERATOR cmi (this->proxy_map_);
				
  ACE_Synch_Options synch_options;
  
  if (this->options ().blocking_semantics_ == ACE_NONBLOCK)
    synch_options = ACE_Synch_Options::asynch;
  else 
    synch_options = ACE_Synch_Options::synch;

  // Iterate through the Consumer Map connecting all the
  // Proxy_Handlers.

  for (PROXY_MAP_ENTRY *me = 0;
       cmi.next (me) != 0;
       cmi.advance ())
    {
      Proxy_Handler *proxy_handler = me->int_id_;

      if (this->initiate_proxy_connection 
	  (proxy_handler, synch_options) == -1)
	continue; // Failures are handled elsewhere...
    }
}

// Initiate passive acceptor to wait for Consumer and Supplier Peers
// to accept.

void
ACE_Event_Channel::initiate_acceptor (void)
{
  if (ACE_Service_Config::reactor ()->register_handler 
      (&this->acceptor_, ACE_Event_Handler::ACCEPT_MASK) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", 
		"cannot register acceptor"));
}

// This method gracefully shuts down all the Handlers in the
// Proxy_Handler Connection Map.

int
ACE_Event_Channel::close (u_long)
{
  if (this->options ().threading_strategy_ != ACE_Event_Channel_Options::REACTIVE)
    {
      if (ACE_Service_Config::thr_mgr ()->suspend_all () == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "suspend_all"), -1);
      ACE_DEBUG ((LM_DEBUG, "(%t) suspending all threads\n"));
    }

  PROXY_MAP_ITERATOR cmi (this->proxy_map_);

  // Iterate over all the handlers and shut them down.

  for (PROXY_MAP_ENTRY *me;
       cmi.next (me) != 0; 
       cmi.advance ())
    {
      Proxy_Handler *proxy_handler = me->int_id_;

      ACE_DEBUG ((LM_DEBUG, "(%t) closing down connection %d\n",
		 proxy_handler->id ()));

      // Mark Proxy_Handler as DISCONNECTING so we don't try to
      // reconnect...
      proxy_handler->state (Proxy_Handler::DISCONNECTING);

      // Deallocate Proxy_Handler resources.
      proxy_handler->destroy (); // Will trigger a delete.
    }

  return 0;
}

int
ACE_Event_Channel::find_proxy (ACE_INT32 proxy_id, 
			       Proxy_Handler *&proxy_handler)
{
  return this->proxy_map_.find (proxy_id, proxy_handler);
}

int
ACE_Event_Channel::bind_proxy (Proxy_Handler *proxy_handler)
{
  int result = this->proxy_map_.bind (proxy_handler->id (), proxy_handler);

  switch (result)
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
    default:
      ACE_ERROR_RETURN ((LM_DEBUG, 
			 "(%t) invalid result %d\n", result), -1);
      /* NOTREACHED */
    }
}

int
ACE_Event_Channel::subscribe (const Event_Key &event_addr, 
			      Consumer_Dispatch_Set *cds)
{
  int result = this->efd_.bind (event_addr, cds);

  // Bind with consumer map, keyed by peer address.
  switch (result)
    {
    case -1:
      ACE_ERROR_RETURN ((LM_ERROR, 
			 "(%t) bind failed for connection %d\n", 
			 event_addr.proxy_id_), -1);
      /* NOTREACHED */
    case 1: // Oops, found a duplicate!
      ACE_ERROR_RETURN ((LM_DEBUG, 
			 "(%t) duplicate consumer map entry %d, "
			 "already bound\n", event_addr.proxy_id_), -1);
      /* NOTREACHED */
    case 0:
      // Success.
      return 0;
    default:
      ACE_ERROR_RETURN ((LM_DEBUG, 
			 "(%t) invalid result %d\n", result), -1);
      /* NOTREACHED */
    }
}

int
ACE_Event_Channel::open (void *)
{
  // Ignore SIPPIPE so each Consumer_Proxy can handle it.
  ACE_Sig_Action sig (ACE_SignalHandler (SIG_IGN), SIGPIPE);
  ACE_UNUSED_ARG (sig);

  if (this->options ().connector_role_)
    // Actively initiate Peer connections.
    this->initiate_connector ();

  if (this->options ().acceptor_role_)
    // Passively initiate Peer acceptor.
    this->initiate_acceptor ();

  // If we're not running reactively, then we need to make sure that
  // <ACE_Message_Block> reference counting operations are
  // thread-safe.  Therefore, we create an <ACE_Lock_Adapter> that is
  // parameterized by <REF_COUNT_MUTEX> to prevent race conditions.
  if (this->options ().threading_strategy_ != ACE_Event_Channel_Options::REACTIVE)
    ACE_NEW_RETURN (this->options ().locking_strategy_, 
		    ACE_Lock_Adapter<REF_COUNT_MUTEX>, 
		    -1);
  return 0;
}
