// $Id$

#include "Proxy_Handler_Connector.h"

Proxy_Handler_Connector::Proxy_Handler_Connector (void)
{
}

// Override the connection-failure method to add timer support.
// Note that these timers perform "expoential backoff" to 
// avoid rapidly trying to reestablish connections when a link
// goes down.

int
Proxy_Handler_Connector::handle_close (ACE_HANDLE sd, ACE_Reactor_Mask)
{
  ACE_Connector<Proxy_Handler, ACE_SOCK_CONNECTOR>::AST *stp = 0;

  // Locate the ACE_Svc_Handler corresponding to the socket descriptor.
  if (this->handler_map_.find (sd, stp) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) can't locate proxy %d in connector map, %p\n",
		      sd, "find"), -1);

  Proxy_Handler *proxy_handler = stp->svc_handler ();

  // Schedule a reconnection request at some point in the future
  // (note that proxy_handler uses an exponential backoff scheme).
  if (ACE_Service_Config::reactor ()->schedule_timer 
      (proxy_handler, 0, proxy_handler->timeout ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", 
		      "schedule_timer"), -1);
  return 0;
}

// Initiate (or reinitiate) a connection to the Proxy_Handler.

int
Proxy_Handler_Connector::initiate_connection (Proxy_Handler *proxy_handler,
					      ACE_Synch_Options &synch_options)
{
  char addr_buf[MAXHOSTNAMELEN];

  // Mark ourselves as idle so that the various iterators 
  // will ignore us until we are reconnected.
  proxy_handler->state (Proxy_Handler::IDLE);

  // We check the remote addr second so that it remains in the addr_buf.
  if (proxy_handler->local_addr ().addr_to_string (addr_buf, sizeof addr_buf) == -1
      || proxy_handler->remote_addr ().addr_to_string (addr_buf, sizeof addr_buf) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", 
		      "can't obtain peer's address"), -1);

  // Try to connect to the Peer.

  if (this->connect (proxy_handler, proxy_handler->remote_addr (), 
		     synch_options, proxy_handler->local_addr ()) == -1)
    {
      if (errno != EWOULDBLOCK)
	{
	  proxy_handler->state (Proxy_Handler::FAILED);
	  ACE_DEBUG ((LM_DEBUG, "(%t) %p on address %s\n", 
		     "connect", addr_buf));

	  // Reschedule ourselves to try and connect again.
	  if (synch_options[ACE_Synch_Options::USE_REACTOR])
	    {
	      if (ACE_Service_Config::reactor ()->schedule_timer 
		  (proxy_handler, 0, proxy_handler->timeout ()) == 0)
		ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n", 
				  "schedule_timer"), -1);
	    }
	  else 
	    // Failures on synchronous connects are reported as errors
	    // so that the caller can decide how to proceed.
	    return -1;
	}
      else
	{
	  proxy_handler->state (Proxy_Handler::CONNECTING);
	  ACE_DEBUG ((LM_DEBUG, 
		     "(%t) in the process of connecting %s to %s\n",
		     synch_options[ACE_Synch_Options::USE_REACTOR] 
		     ? "asynchronously" : "synchronously", addr_buf));
	}
    }
  else 
    {
      proxy_handler->state (Proxy_Handler::ESTABLISHED);
      ACE_DEBUG ((LM_DEBUG, "(%t) connected to %s on %d\n", 
		 addr_buf, proxy_handler->get_handle ()));
    }
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Connector<Proxy_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Svc_Tuple<Proxy_Handler>;
#if defined (ACE_MT_SAFE)
  template class ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<Proxy_Handler> *, ACE_RW_Mutex>;
  template class ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Proxy_Handler> *, ACE_RW_Mutex>;
  template class ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<Proxy_Handler> *>;
  template class ACE_Guard<ACE_RW_Mutex>;
  template class ACE_Read_Guard<ACE_RW_Mutex>;
  template class ACE_Write_Guard<ACE_RW_Mutex>;
#else
  template class ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<Proxy_Handler> *, ACE_Null_Mutex>;
  template class ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Proxy_Handler> *, ACE_Null_Mutex>;
  template class ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<Proxy_Handler> *>;
#endif /* ACE_MT_SAFE */
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
