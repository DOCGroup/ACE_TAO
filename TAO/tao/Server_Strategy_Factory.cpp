// $Id$

#include "tao/Server_Strategy_Factory.h"

ACE_RCSID(tao, Server_Strategy_Factory, "$Id$")

TAO_Server_Strategy_Factory::TAO_Server_Strategy_Factory (void)
  : active_object_map_creation_parameters_ ()
{
}

TAO_Server_Strategy_Factory::~TAO_Server_Strategy_Factory(void)
{
}

int
TAO_Server_Strategy_Factory::open (TAO_ORB_Core*)
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

int
TAO_Server_Strategy_Factory::enable_poa_locking (void)
{
  return -1;
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
TAO_Server_Strategy_Factory::create_cached_connector_lock (void)
{
  return 0;
}

const TAO_Server_Strategy_Factory::Active_Object_Map_Creation_Parameters &
TAO_Server_Strategy_Factory::active_object_map_creation_parameters (void) const
{
  return this->active_object_map_creation_parameters_;
}

TAO_Server_Strategy_Factory::Active_Object_Map_Creation_Parameters::Active_Object_Map_Creation_Parameters (void)
  : active_object_map_size_ (TAO_DEFAULT_SERVER_ACTIVE_OBJECT_MAP_SIZE),
    object_lookup_strategy_for_user_id_policy_ (TAO_DYNAMIC_HASH),
    object_lookup_strategy_for_system_id_policy_ (TAO_ACTIVE_DEMUX),
    reverse_object_lookup_strategy_for_unique_id_policy_ (TAO_DYNAMIC_HASH),
    use_active_hint_in_ids_ (1),
    allow_reactivation_of_system_ids_ (1),
    poa_map_size_ (TAO_DEFAULT_SERVER_POA_MAP_SIZE),
    poa_lookup_strategy_for_transient_id_policy_ (TAO_ACTIVE_DEMUX),
    poa_lookup_strategy_for_persistent_id_policy_ (TAO_DYNAMIC_HASH),
    use_active_hint_in_poa_names_ (1)
{
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
