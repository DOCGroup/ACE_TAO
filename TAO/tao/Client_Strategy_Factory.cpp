// $Id$

#include "tao/Client_Strategy_Factory.h"

ACE_RCSID(tao, Client_Strategy_Factory, "$Id$")

TAO_Client_Strategy_Factory::TAO_Client_Strategy_Factory (void)
{
}

TAO_Client_Strategy_Factory::~TAO_Client_Strategy_Factory (void)
{
}

ACE_Lock *
TAO_Client_Strategy_Factory::create_iiop_profile_lock (void)
{
  return 0;
}

ACE_Creation_Strategy<TAO_Client_Connection_Handler> *
TAO_Client_Strategy_Factory::create_client_creation_strategy (void)
{
  return 0;
}

// Create the correct client request muxing strategy.
TAO_Request_Mux_Strategy *
TAO_Client_Strategy_Factory::create_request_mux_strategy (void)
{
  return 0;
}


// Create the correct client wait_for_reply strategy.

TAO_Wait_Strategy *
TAO_Client_Strategy_Factory::create_wait_strategy (TAO_Transport *transport)
{
  return 0;
}
