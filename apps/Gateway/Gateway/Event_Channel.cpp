/* -*- C++ -*- */
// $Id$

#define ACE_BUILD_SVC_DLL
#include "Proxy_Handler_Connector.h"
#include "Event_Channel.h"

ACE_Event_Channel::~ACE_Event_Channel (void)
{
}

ACE_Event_Channel::ACE_Event_Channel (void)
  : supplier_acceptor_ (*this),
    consumer_acceptor_ (*this)
{
}

int
ACE_Event_Channel::compute_performance_statistics (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) doing the performance timeout here...\n"));
  PROXY_MAP_ITERATOR cmi (this->proxy_map_);

  // If we've got a ACE_Thread Manager then use it to suspend all the
  // threads.  This will enable us to get an accurate count.

  if (Options::instance ()->threading_strategy () 
      != Options::REACTIVE)
    {
      if (ACE_Thread_Manager::instance ()->suspend_all () == -1)
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
	      Options::instance ()->performance_window (),
	      total_bytes_in,
	      total_bytes_out));
  ACE_DEBUG ((LM_DEBUG,
              "(%t) %f Mbits/sec received.\n",
	      (float) (total_bytes_in * 8 / 
                       (float) (1024 * 1024 * Options::instance ()->performance_window ()))));
  ACE_DEBUG ((LM_DEBUG,
              "(%t) %f Mbits/sec sent.\n",
	      (float) (total_bytes_out * 8 /
                       (float) (1024 * 1024 * Options::instance ()->performance_window ()))));
#endif /* ACE_NLOGGING */

  // Resume all the threads again.

  if (Options::instance ()->threading_strategy () 
      != Options::REACTIVE)
    {
      if (ACE_Thread_Manager::instance ()->resume_all () == -1)
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
ACE_Event_Channel::initiate_proxy_connection (Proxy_Handler *proxy_handler)
{
  ACE_Synch_Options synch_options;

  if (Options::instance ()->blocking_semantics () == ACE_NONBLOCK)
    synch_options = ACE_Synch_Options::asynch;
  else
    synch_options = ACE_Synch_Options::synch;

  return this->connector_.initiate_connection (proxy_handler,
					       synch_options);
}

int
ACE_Event_Channel::complete_proxy_connection (Proxy_Handler *proxy_handler)
{
  int option = proxy_handler->proxy_role () == 'S' ? SO_RCVBUF : SO_SNDBUF;
  int socket_queue_size = Options::instance ()->socket_queue_size ();

  if (socket_queue_size > 0)
    if (proxy_handler->peer ().set_option (SOL_SOCKET,
					   option,
					   &socket_queue_size,
					   sizeof (int)) == -1)
      ACE_ERROR ((LM_ERROR, "(%t) %p\n", "set_option"));

  proxy_handler->thr_mgr (ACE_Thread_Manager::instance ());

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
    }

  if (proxy_handler->state () != Proxy_Handler::DISCONNECTING)
    {
      ACE_DEBUG ((LM_DEBUG,
		  "(%t) scheduling reinitiation of Proxy_Handler %d\n",
		  proxy_handler->id ()));

      // Reschedule ourselves to try and connect again.
      if (ACE_Reactor::instance ()->schedule_timer
	  (proxy_handler, 0, proxy_handler->timeout ()) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n",
			   "schedule_timer"), -1);
    }
  return 0;
}

// Initiate active connections with the Consumer and Supplier Peers.

void
ACE_Event_Channel::initiate_connector (void)
{
  if (Options::instance ()->enabled (Options::CONSUMER_CONNECTOR | Options::SUPPLIER_CONNECTOR))
    {
      PROXY_MAP_ITERATOR cmi (this->proxy_map_);

      // Iterate through the Consumer Map connecting all the
      // Proxy_Handlers.

      for (PROXY_MAP_ENTRY *me = 0;
           cmi.next (me) != 0;
           cmi.advance ())
        {
          Proxy_Handler *proxy_handler = me->int_id_;

          if (this->initiate_proxy_connection (proxy_handler) == -1)
            continue; // Failures are handled elsewhere...
        }
    }
}

// Initiate passive acceptor to wait for Consumer and Supplier Peers
// to accept.

void
ACE_Event_Channel::initiate_acceptors (void)
{
  if (Options::instance ()->enabled (Options::CONSUMER_ACCEPTOR)
      && this->consumer_acceptor_.open 
      (Options::instance ()->consumer_acceptor_port (),
       ACE_Reactor::instance (),
       Options::instance ()->blocking_semantics ()) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n",
		"cannot register acceptor"));

  if (Options::instance ()->enabled (Options::SUPPLIER_CONNECTOR)
      && this->supplier_acceptor_.open 
      (Options::instance ()->supplier_acceptor_port (),
       ACE_Reactor::instance (),
       Options::instance ()->blocking_semantics ()) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n",
		"cannot register acceptor"));
}

// This method gracefully shuts down all the Handlers in the
// Proxy_Handler Connection Map.

int
ACE_Event_Channel::close (u_long)
{
  if (Options::instance ()->threading_strategy () 
      != Options::REACTIVE)
    {
      if (ACE_Thread_Manager::instance ()->suspend_all () == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", "suspend_all"), -1);
      ACE_DEBUG ((LM_DEBUG, "(%t) suspending all threads\n"));
    }

  // First tell everyone that the spaceship is here...
  {
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
      }
  }

  // Close down the connector
  this->connector_.close ();

  // Close down the supplier acceptor.
  this->supplier_acceptor_.close ();

  // Close down the consumer acceptor.
  this->consumer_acceptor_.close ();

  // Now tell everyone that it is now time to commit suicide.
  {
    PROXY_MAP_ITERATOR cmi (this->proxy_map_);

    for (PROXY_MAP_ENTRY *me;
	 cmi.next (me) != 0;
	 cmi.advance ())
      {
	Proxy_Handler *proxy_handler = me->int_id_;

	// Deallocate Proxy_Handler resources.
	proxy_handler->destroy (); // Will trigger a delete.
      }
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
      /* NOTREACHED */
    default:
      ACE_ERROR_RETURN ((LM_DEBUG,
			 "(%t) invalid result %d\n", result), -1);
      /* NOTREACHED */
    }

  return 0;
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

  return 0;
}

int
ACE_Event_Channel::open (void *)
{
  // Ignore SIPPIPE so each Consumer_Proxy can handle it.
  ACE_Sig_Action sig (ACE_SignalHandler (SIG_IGN), SIGPIPE);
  ACE_UNUSED_ARG (sig);

  // Actively initiate Peer connections.
  this->initiate_connector ();

  // Passively initiate Peer acceptor.
  this->initiate_acceptors ();

  // If we're not running reactively, then we need to make sure that
  // <ACE_Message_Block> reference counting operations are
  // thread-safe.  Therefore, we create an <ACE_Lock_Adapter> that is
  // parameterized by <ACE_SYNCH_MUTEX> to prevent race conditions.
  if (Options::instance ()->threading_strategy () 
      != Options::REACTIVE)
    {
      ACE_Lock_Adapter<ACE_SYNCH_MUTEX> *la;

      ACE_NEW_RETURN (la,
                      ACE_Lock_Adapter<ACE_SYNCH_MUTEX>,
                      -1);

      Options::instance ()->locking_strategy (la);
    }
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Lock_Adapter<ACE_SYNCH_MUTEX>;
template class ACE_Map_Entry<ACE_INT32, Proxy_Handler *>;
template class ACE_Map_Iterator<ACE_INT32, Proxy_Handler *, MAP_MUTEX>;
template class ACE_Map_Manager<ACE_INT32, Proxy_Handler *, MAP_MUTEX>;
template class ACE_Unbounded_Set_Iterator<Proxy_Handler *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Lock_Adapter<ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Map_Entry<ACE_INT32, Proxy_Handler *>
#pragma instantiate ACE_Map_Iterator<ACE_INT32, Proxy_Handler *, MAP_MUTEX>
#pragma instantiate ACE_Map_Manager<ACE_INT32, Proxy_Handler *, MAP_MUTEX>
#pragma instantiate ACE_Unbounded_Set_Iterator<Proxy_Handler *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

