// $Id$

#include "Connection_Handler_Connector.h"

Connection_Handler_Connector::Connection_Handler_Connector (void)
{
}

// Initiate (or reinitiate) a connection to the Connection_Handler.

int
Connection_Handler_Connector::initiate_connection (Connection_Handler *connection_handler,
					      ACE_Synch_Options &synch_options)
{
  char addr_buf[MAXHOSTNAMELEN];

  // Mark ourselves as idle so that the various iterators
  // will ignore us until we are reconnected.
  connection_handler->state (Connection_Handler::IDLE);

  // We check the remote addr second so that it remains in the addr_buf.
  if (connection_handler->local_addr ().addr_to_string (addr_buf, sizeof addr_buf) == -1
      || connection_handler->remote_addr ().addr_to_string (addr_buf, sizeof addr_buf) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) %p\n",
		      "can't obtain peer's address"), -1);

  // Try to connect to the Peer.

  if (this->connect (connection_handler,
                     connection_handler->remote_addr (),
		     synch_options,
                     connection_handler->local_addr ()) == -1)
    {
      if (errno != EWOULDBLOCK)
	{
	  connection_handler->state (Connection_Handler::FAILED);
	  ACE_DEBUG ((LM_DEBUG, "(%t) %p on address %s\n",
		      "connect", addr_buf));

	  return -1;
	}
      else
	{
	  connection_handler->state (Connection_Handler::CONNECTING);
	  ACE_DEBUG ((LM_DEBUG,
		      "(%t) in the process of connecting to %s\n",
		      addr_buf));
	}
    }
  else
    {
      connection_handler->state (Connection_Handler::ESTABLISHED);
      ACE_DEBUG ((LM_DEBUG, "(%t) connected to %s on %d\n",
		  addr_buf, connection_handler->get_handle ()));
    }
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Connector<Connection_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Svc_Tuple<Connection_Handler>;
template class ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<Connection_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator_Base<ACE_HANDLE, ACE_Svc_Tuple<Connection_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Connection_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Connection_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<Connection_Handler> *>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Connector<Connection_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Svc_Tuple<Connection_Handler>
#pragma instantiate ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<Connection_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<ACE_HANDLE, ACE_Svc_Tuple<Connection_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Connection_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Connection_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<Connection_Handler> *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
