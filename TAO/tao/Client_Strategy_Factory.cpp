// -*- C++ -*-
// $Id$

#include "tao/Client_Strategy_Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Client_Strategy_Factory::~TAO_Client_Strategy_Factory (void)
{
}

TAO_Transport_Mux_Strategy *
TAO_Client_Strategy_Factory::create_transport_mux_strategy (TAO_Transport *)
{
  return 0;
}

ACE_Lock *
TAO_Client_Strategy_Factory::create_transport_mux_strategy_lock (void)
{
  return 0;
}

int
TAO_Client_Strategy_Factory::reply_dispatcher_table_size (void) const
{
  return 0;
}

TAO_Wait_Strategy *
TAO_Client_Strategy_Factory::create_wait_strategy (TAO_Transport *)
{
  return 0;
}

TAO_Client_Strategy_Factory::Connect_Strategy
TAO_Client_Strategy_Factory::connect_strategy (void) const
{
  return TAO_BLOCKED_CONNECT;
}

TAO_Connect_Strategy *
TAO_Client_Strategy_Factory::create_connect_strategy (TAO_ORB_Core *)
{
  return 0;
}

int
TAO_Client_Strategy_Factory::allow_callback (void)
{
  return 0;
}

bool
TAO_Client_Strategy_Factory::use_cleanup_options (void) const
{
  return false;
}

TAO_END_VERSIONED_NAMESPACE_DECL
