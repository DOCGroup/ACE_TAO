// $Id$

#include "Proxy_Handler_Connector.h"

Proxy_Handler_Connector::Proxy_Handler_Connector (void)
{
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

	  return -1;
	}
      else
	{
	  proxy_handler->state (Proxy_Handler::CONNECTING);
	  ACE_DEBUG ((LM_DEBUG, 
		      "(%t) in the process of connecting to %s\n",
		      addr_buf));
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
template class ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<Proxy_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Proxy_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<Proxy_Handler> *>;

#if defined (ACE_HAS_THREADS)
  template class ACE_Guard<ACE_SYNCH_RW_MUTEX>;
  template class ACE_Read_Guard<ACE_SYNCH_RW_MUTEX>;
  template class ACE_Write_Guard<ACE_SYNCH_RW_MUTEX>;
#else
  // These are specialized in libACE if ACE doesn't have threads.
#endif /* ACE_HAS_THREADS */
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */
