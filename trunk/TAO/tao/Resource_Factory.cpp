// $Id$

#include "Resource_Factory.h"
#include "Protocol_Factory.h"
#include "orbconf.h"

#include "ace/Dynamic_Service.h"
#include "ace/ACE.h"


ACE_RCSID (tao,
           Resource_Factory,
           "$Id$")


// ****************************************************************

TAO_Protocol_Item::TAO_Protocol_Item (const ACE_CString &name)
  : name_ (name),
    factory_ (0),
    factory_owner_ (0)
{
}

TAO_Protocol_Item::~TAO_Protocol_Item (void)
{
  if (this->factory_owner_ == 1)
    delete this->factory_;
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
TAO_Protocol_Item::factory (TAO_Protocol_Factory *factory,
                            int owner)
{
  this->factory_ = factory;
  this->factory_owner_ = owner;
}

// **********************************************************************

TAO_Codeset_Descriptor::TAO_Codeset_Descriptor ()
  :ncs_ (0),
   max_bytes_ (1),
   ncs_set_ (0),
   trans_base_(0)
{
}

TAO_Codeset_Descriptor::~TAO_Codeset_Descriptor ()
{
  Translator_Node *temp = trans_base_;
  while (temp)
    {
      temp = trans_base_->next_;
      delete [] trans_base_->name_;
      delete trans_base_;
      trans_base_ = temp;
    }
}

void
TAO_Codeset_Descriptor::ncs (ACE_CDR::ULong ncs, int mb)
{
  this->ncs_ = ncs;
  this->max_bytes_ = mb;
  this->ncs_set_ = 1;
}

ACE_CDR::ULong
TAO_Codeset_Descriptor::ncs (void) const
{
  return this->ncs_;
}

int
TAO_Codeset_Descriptor::max_bytes (void) const
{
  return this->max_bytes_;
}

int
TAO_Codeset_Descriptor::ncs_set (void) const
{
  return this->ncs_set_;
}

void
TAO_Codeset_Descriptor::add_translator (const char *name)
{
  Translator_Node *temp = trans_base_;
  if (this->trans_base_ == 0)
    {
      this->trans_base_ = new Translator_Node;
      temp = trans_base_;
    }
  else
    {
      while (temp->next_ != 0)
        temp = temp->next_;
      temp->next_ = new Translator_Node;
      temp = temp->next_;
    }
  temp->name_ = new char[ACE_OS::strlen (name) + 1];
  ACE_OS::strcpy (temp->name_,name);
  temp->next_ = 0;
}

const TAO_Codeset_Descriptor::Translator_Node *
TAO_Codeset_Descriptor::translators (void) const
{
  return this->trans_base_;
}

// ********************************************************************

TAO_Resource_Factory::TAO_Resource_Factory (void)
{
}

TAO_Resource_Factory::~TAO_Resource_Factory (void)
{
}

#if 0
// @@todo: Need to go at a later date!
int
TAO_Resource_Factory::use_tss_resources (void) const
{
  return 0;
}
#endif /*if 0*/

int
TAO_Resource_Factory::use_locked_data_blocks (void) const
{
  return 0;
}

ACE_Reactor *
TAO_Resource_Factory::get_reactor (void)
{
  return 0;
}

void
TAO_Resource_Factory::reclaim_reactor (ACE_Reactor *)
{
  return;
}

TAO_Connector_Registry *
TAO_Resource_Factory::get_connector_registry (void)
{
  return 0;
}

TAO_Acceptor_Registry *
TAO_Resource_Factory::get_acceptor_registry (void)
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
TAO_Resource_Factory::input_cdr_msgblock_allocator (void)
{
  return 0;
}

int
TAO_Resource_Factory::input_cdr_allocator_type_locked (void)
{
  return -1;
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

ACE_Allocator *
TAO_Resource_Factory::output_cdr_msgblock_allocator (void)
{
  return 0;
}

ACE_Allocator *
TAO_Resource_Factory::amh_response_handler_allocator (void)
{
  return 0;
}

ACE_Allocator *
TAO_Resource_Factory::ami_response_handler_allocator (void)
{
  return 0;
}

TAO_ProtocolFactorySet *
TAO_Resource_Factory::get_protocol_factories (void)
{
  return 0;
}

const TAO_Codeset_Descriptor *
TAO_Resource_Factory::get_codeset_descriptor (int) const
{
  return 0;
}

int
TAO_Resource_Factory::init_protocol_factories (void)
{
  return -1;
}

int
TAO_Resource_Factory::cache_maximum (void) const
{
  return TAO_CONNECTION_CACHE_MAXIMUM;
}

int
TAO_Resource_Factory::purge_percentage (void) const
{
  return 0;
}

int
TAO_Resource_Factory::max_muxed_connections (void) const
{
  return 0;
}


int
TAO_Resource_Factory::get_parser_names (char **&,
                                        int &)
{
  return 0;
}

ACE_Lock *
TAO_Resource_Factory::create_cached_connection_lock (void)
{
  return 0;
}

int
TAO_Resource_Factory::locked_transport_cache (void)
{
  return 0;
}

ACE_Lock *
TAO_Resource_Factory::create_object_key_table_lock (void)
{
  return 0;
}

ACE_Lock *
TAO_Resource_Factory::create_corba_object_lock (void)
{
  return 0;
}

int
TAO_Resource_Factory::load_default_protocols (void)
{
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_Resource_Factory>;
template class ACE_Node<TAO_Protocol_Item*>;
///
template class ACE_Unbounded_Set<TAO_Protocol_Item*>;
template class ACE_Unbounded_Set_Iterator<TAO_Protocol_Item*>;
///
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_Resource_Factory>
#pragma instantiate ACE_Node<TAO_Protocol_Item*>
///
#pragma instantiate ACE_Unbounded_Set<TAO_Protocol_Item*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_Protocol_Item*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
