// $Id$

#include "tao/Resource_Factory.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(tao, Resource_Factory, "$Id$")

TAO_Protocol_Item::TAO_Protocol_Item (const ACE_CString &name)
  :   name_ (name),
      factory_ (0)
{
}

const ACE_CString &
TAO_Protocol_Item::protocol_name (void)
{
  return this->name_;
}

TAO_Protocol_Factory *
TAO_Protocol_Item::factory (void)
{
  return this->factory_;
}

void
TAO_Protocol_Item::factory (TAO_Protocol_Factory *factory)
{
  this->factory_ = factory;
}


TAO_Resource_Factory::TAO_Resource_Factory (void)
{
}

TAO_Resource_Factory::~TAO_Resource_Factory (void)
{
}

ACE_Reactor *
TAO_Resource_Factory::get_reactor (void)
{
  return 0;
}

ACE_Thread_Manager *
TAO_Resource_Factory::get_thr_mgr (void)
{
  return 0;
}

TAO_Connector_Registry *
TAO_Resource_Factory::get_connector_registry (void)
{
  return 0;
}

TAO_CACHED_CONNECT_STRATEGY *
TAO_Resource_Factory::get_cached_connect_strategy (void)
{
  return 0;
}

TAO_NULL_CREATION_STRATEGY *
TAO_Resource_Factory::get_null_creation_strategy (void)
{
  return 0;
}

TAO_NULL_ACTIVATION_STRATEGY *
TAO_Resource_Factory::get_null_activation_strategy (void)
{
  return 0;
}

TAO_Acceptor_Registry *
TAO_Resource_Factory::get_acceptor_registry (void)
{
  return 0;
}

TAO_POA *
TAO_Resource_Factory::get_root_poa (void)
{
  return 0;
}

TAO_Object_Adapter *
TAO_Resource_Factory::object_adapter (void)
{
  return 0;
}

ACE_Allocator *
TAO_Resource_Factory::input_cdr_dblock_allocator (void)
{
  return 0;
}

ACE_Allocator *
TAO_Resource_Factory::input_cdr_buffer_allocator (void)
{
  return 0;
}

ACE_Allocator *
TAO_Resource_Factory::output_cdr_dblock_allocator (void)
{
  return 0;
}

ACE_Allocator *
TAO_Resource_Factory::output_cdr_buffer_allocator (void)
{
  return 0;
}

ACE_Data_Block *
TAO_Resource_Factory::create_input_cdr_data_block (size_t size)
{
  return 0;
}

TAO_ProtocolFactorySet *
TAO_Resource_Factory::get_protocol_factories (void)
{
  return 0;
}

int
TAO_Resource_Factory::init_protocol_factories (void)
{
  return -1;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_Resource_Factory>;

template class ACE_Node<TAO_Protocol_Item*>;

template class ACE_Unbounded_Set<TAO_Protocol_Item*>;

template class ACE_Unbounded_Set_Iterator<TAO_Protocol_Item*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_Resource_Factory>

#pragma instantiate ACE_Node<TAO_Protocol_Item*>;

#pragma instantiate ACE_Unbounded_Set<TAO_Protocol_Item*>;

#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_Protocol_Item*>;

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
