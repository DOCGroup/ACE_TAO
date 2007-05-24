// $Id$

#include "tao/Server_Strategy_Factory.h"
#include "tao/orbconf.h"

ACE_RCSID (tao, 
           Server_Strategy_Factory, 
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

int
TAO_Server_Strategy_Factory::enable_poa_locking (void)
{
  return -1;
}

int
TAO_Server_Strategy_Factory::activate_server_connections (void)
{
  return 0;
}

int
TAO_Server_Strategy_Factory::thread_per_connection_timeout (ACE_Time_Value &)
{
  return -1;
}

int
TAO_Server_Strategy_Factory::server_connection_thread_flags (void)
{
  return 0;
}

int
TAO_Server_Strategy_Factory::server_connection_thread_count (void)
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

TAO_END_VERSIONED_NAMESPACE_DECL
