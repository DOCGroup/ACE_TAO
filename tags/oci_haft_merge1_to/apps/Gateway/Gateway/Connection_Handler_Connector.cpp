// $Id$

#include "Connection_Handler_Connector.h"

ACE_RCSID(Gateway, Connection_Handler_Connector, "$Id$")

Connection_Handler_Connector::Connection_Handler_Connector (void)
{
}

// Initiate (or reinitiate) a connection to the Connection_Handler.

int
Connection_Handler_Connector::initiate_connection (Connection_Handler *connection_handler,
                                                   ACE_Synch_Options &synch_options)
{
  char addr_buf[MAXHOSTNAMELEN];

  // Mark ourselves as idle so that the various iterators will ignore
  // us until we are reconnected.
  connection_handler->state (Connection_Handler::IDLE);

  // We check the remote addr second so that it remains in the
  // addr_buf.
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
template class ACE_NonBlocking_Connect_Handler<Connection_Handler>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Connector<Connection_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_NonBlocking_Connect_Handler<Connection_Handler>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
