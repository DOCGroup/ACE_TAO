// $Id$

#include "tao/Server_Strategy_Factory.h"

ACE_RCSID(tao, Server_Strategy_Factory, "$Id$")

TAO_Server_Strategy_Factory::TAO_Server_Strategy_Factory (void)
{
}

TAO_Server_Strategy_Factory::~TAO_Server_Strategy_Factory(void)
{
}

int
TAO_Server_Strategy_Factory::open (void)
{
  return 0;
}

TAO_Server_Strategy_Factory::CREATION_STRATEGY *
TAO_Server_Strategy_Factory::creation_strategy (void)
{
  return 0;
}

TAO_Server_Strategy_Factory::ACCEPT_STRATEGY *
TAO_Server_Strategy_Factory::accept_strategy (void)
{
  return 0;
}

TAO_Server_Strategy_Factory::CONCURRENCY_STRATEGY *
TAO_Server_Strategy_Factory::concurrency_strategy (void)
{
  return 0;
}

TAO_Server_Strategy_Factory::SCHEDULING_STRATEGY *
TAO_Server_Strategy_Factory::scheduling_strategy (void)
{
  return 0;
}

TAO_Active_Object_Map_Impl *
TAO_Server_Strategy_Factory::create_active_object_map (int user_id_policy)
{
  ACE_UNUSED_ARG (user_id_policy);
  return 0;
}

TAO_Reverse_Active_Object_Map_Impl *
TAO_Server_Strategy_Factory::create_reverse_active_object_map (int unique_id_policy)
{
  ACE_UNUSED_ARG (unique_id_policy);
  return 0;
}

ACE_Lock *
TAO_Server_Strategy_Factory::create_poa_lock (void)
{
  return 0;
}

ACE_Lock *
TAO_Server_Strategy_Factory::create_poa_mgr_lock (void)
{
  return 0;
}

ACE_Lock *
TAO_Server_Strategy_Factory::create_servant_lock (void)
{
  return 0;
}

ACE_Lock *
TAO_Server_Strategy_Factory::create_event_loop_lock (void)
{
  return 0;
}

ACE_Lock *
TAO_Server_Strategy_Factory::create_collocation_table_lock (void)
{
  return 0;
}

ACE_Lock *
TAO_Server_Strategy_Factory::create_cached_connector_lock (void)
{
  return 0;
}

u_long 
TAO_Server_Strategy_Factory::active_object_map_size (void) const
{
  return TAO_DEFAULT_SERVER_ACTIVE_OBJECT_MAP_SIZE;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Creation_Strategy<TAO_Server_Connection_Handler>;
template class ACE_Acceptor<TAO_Server_Connection_Handler, TAO_SOCK_ACCEPTOR>;
template class ACE_Accept_Strategy<TAO_Server_Connection_Handler, TAO_SOCK_ACCEPTOR>;
template class ACE_Concurrency_Strategy<TAO_Server_Connection_Handler>;
template class ACE_Scheduling_Strategy<TAO_Server_Connection_Handler>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Creation_Strategy<TAO_Server_Connection_Handler>
#pragma instantiate ACE_Acceptor<TAO_Server_Connection_Handler, TAO_SOCK_ACCEPTOR>
#pragma instantiate ACE_Accept_Strategy<TAO_Server_Connection_Handler, TAO_SOCK_ACCEPTOR>
#pragma instantiate ACE_Concurrency_Strategy<TAO_Server_Connection_Handler>
#pragma instantiate ACE_Scheduling_Strategy<TAO_Server_Connection_Handler>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
