#include "tao/Resource_Factory.h"
#include "tao/Protocol_Factory.h"
#include "tao/orbconf.h"
#include "ace/ACE.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Protocol_Item::TAO_Protocol_Item (const ACE_CString &name)
  : name_ (name),
    factory_ (nullptr),
    factory_owner_ (0)
{
}

TAO_Protocol_Item::~TAO_Protocol_Item ()
{
  if (this->factory_owner_ == 1)
    delete this->factory_;
}

const ACE_CString &
TAO_Protocol_Item::protocol_name ()
{
  return this->name_;
}

TAO_Protocol_Factory *
TAO_Protocol_Item::factory ()
{
  return this->factory_;
}

void
TAO_Protocol_Item::factory (TAO_Protocol_Factory *factory,
                            int owner)
{
  this->factory_ = factory;
  this->factory_owner_ = owner;
}

// **********************************************************************

// ********************************************************************

TAO_Resource_Factory::TAO_Resource_Factory ()
{
}

TAO_Resource_Factory::~TAO_Resource_Factory ()
{
}

int
TAO_Resource_Factory::use_locked_data_blocks () const
{
  return 0;
}

ACE_Reactor *
TAO_Resource_Factory::get_reactor ()
{
  return nullptr;
}

void
TAO_Resource_Factory::reclaim_reactor (ACE_Reactor *)
{
  return;
}

TAO_Connector_Registry *
TAO_Resource_Factory::get_connector_registry ()
{
  return nullptr;
}

TAO_Acceptor_Registry *
TAO_Resource_Factory::get_acceptor_registry ()
{
  return nullptr;
}

void
TAO_Resource_Factory::use_local_memory_pool (bool)
{
}

ACE_Allocator *
TAO_Resource_Factory::input_cdr_dblock_allocator ()
{
  return nullptr;
}

ACE_Allocator *
TAO_Resource_Factory::input_cdr_buffer_allocator ()
{
  return nullptr;
}

ACE_Allocator *
TAO_Resource_Factory::input_cdr_msgblock_allocator ()
{
  return nullptr;
}

int
TAO_Resource_Factory::input_cdr_allocator_type_locked ()
{
  return -1;
}

ACE_Allocator *
TAO_Resource_Factory::output_cdr_dblock_allocator ()
{
  return nullptr;
}

ACE_Allocator *
TAO_Resource_Factory::output_cdr_buffer_allocator ()
{
  return nullptr;
}

ACE_Allocator *
TAO_Resource_Factory::output_cdr_msgblock_allocator ()
{
  return nullptr;
}

ACE_Allocator *
TAO_Resource_Factory::amh_response_handler_allocator ()
{
  return nullptr;
}

ACE_Allocator *
TAO_Resource_Factory::ami_response_handler_allocator ()
{
  return nullptr;
}

TAO_ProtocolFactorySet *
TAO_Resource_Factory::get_protocol_factories ()
{
  return nullptr;
}

int
TAO_Resource_Factory::init_protocol_factories ()
{
  return -1;
}

TAO_Codeset_Manager *
TAO_Resource_Factory::codeset_manager ()
{
  return nullptr;
}

int
TAO_Resource_Factory::cache_maximum () const
{
  return TAO_CONNECTION_CACHE_MAXIMUM;
}

int
TAO_Resource_Factory::purge_percentage () const
{
  return 0;
}

int
TAO_Resource_Factory::max_muxed_connections () const
{
  return 0;
}


int
TAO_Resource_Factory::get_parser_names (char **&, int &)
{
  return 0;
}

ACE_Lock *
TAO_Resource_Factory::create_cached_connection_lock ()
{
  return nullptr;
}

int
TAO_Resource_Factory::locked_transport_cache ()
{
  return 0;
}

int
TAO_Resource_Factory::load_default_protocols ()
{
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
