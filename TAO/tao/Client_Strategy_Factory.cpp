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
TAO_Client_Strategy_Factory::create_profile_lock (void)
{
  return 0;
}

TAO_Transport_Mux_Strategy *
TAO_Client_Strategy_Factory::create_transport_mux_strategy (TAO_Transport *)
{
  return 0;
}

TAO_Wait_Strategy *
TAO_Client_Strategy_Factory::create_wait_strategy (TAO_Transport *)
{
  return 0;
}

int
TAO_Client_Strategy_Factory::allow_callback (void)
{
  return 0;
}

ACE_Lock *
TAO_Client_Strategy_Factory::create_ft_service_retention_id_lock (void)
{
  return 0;
}
