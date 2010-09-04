// $Id$

#include "orbsvcs/Notify/ETCL_FilterFactory.h"
#include "orbsvcs/Notify/ETCL_Filter.h"
#include "orbsvcs/Notify/Properties.h"
#include "tao/debug.h"

#ifndef DEBUG_LEVEL
# define DEBUG_LEVEL TAO_debug_level
#endif //DEBUG_LEVEL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_ETCL_FilterFactory::TAO_Notify_ETCL_FilterFactory (void) :
  filter_poa_ (PortableServer::POA::_nil ())
{
}

TAO_Notify_ETCL_FilterFactory::~TAO_Notify_ETCL_FilterFactory ()
{
  FILTERMAP::ITERATOR iterator (this->filters_);
  
  for (FILTERMAP::ENTRY *entry = 0;
    iterator.next (entry) != 0;
    iterator.advance ())
  {
    delete entry->int_id_;
    entry->int_id_ = 0;
  }

  this->filters_.unbind_all ();

}

CosNotifyFilter::FilterFactory_ptr
TAO_Notify_ETCL_FilterFactory::create (PortableServer::POA_ptr filter_poa)
{
  this->filter_poa_ = PortableServer::POA::_duplicate(filter_poa);

  CORBA::Object_var object = CORBA::Object::_nil();
  try
    {
      PortableServer::ObjectId_var id = filter_poa->activate_object (this);
      object = filter_poa->id_to_reference (id.in());
    }
  catch (PortableServer::POA::ServantAlreadyActive&)
    {
      try
        {
          object = filter_poa->servant_to_reference (this);
        }
      catch (CORBA::Exception& )
        {
         return CosNotifyFilter::FilterFactory::_nil();
        }
    }

  return  CosNotifyFilter::FilterFactory::_narrow (object.in ());
}

void
TAO_Notify_ETCL_FilterFactory::destroy (void)
{
  if (CORBA::is_nil(this->filter_poa_.in ()))
    return;
  PortableServer::ServantBase_var guard(this);
  try
    {
      PortableServer::ObjectId_var id =
        this->filter_poa_->servant_to_id (this);
      this->filter_poa_->deactivate_object (id.in());
    }
  catch (CORBA::Exception&)
    {
    }

}

CosNotifyFilter::Filter_ptr
TAO_Notify_ETCL_FilterFactory::create_filter (const char *constraint_grammar)
{
  // @@: change to "ExTCL" later.
  if (ACE_OS::strcmp (constraint_grammar, "TCL") != 0 &&
      ACE_OS::strcmp (constraint_grammar, "ETCL") != 0 &&
      ACE_OS::strcmp (constraint_grammar, "EXTENDED_TCL") != 0)
    throw CosNotifyFilter::InvalidGrammar ();

  TAO_Notify_Object::ID id = filter_ids_.id ();
  TAO_Notify_ETCL_Filter* filter = 0;
  return this->create_filter (constraint_grammar, id, filter);
}


CosNotifyFilter::Filter_ptr
TAO_Notify_ETCL_FilterFactory::create_filter (
  const char *constraint_grammar,
  const TAO_Notify_Object::ID& id,
  TAO_Notify_ETCL_Filter*& filter)
{
  // Create the RefCounted servant.
  filter = 0;

  ACE_NEW_THROW_EX (filter,
                    TAO_Notify_ETCL_Filter (this->filter_poa_.in (), 
                                            constraint_grammar, 
                                            id),
                    CORBA::NO_MEMORY ());

  if (filters_.bind (id, filter) == -1)
  {
    throw CORBA::INTERNAL ();
    return 0;
  }  
  
  PortableServer::ObjectId_var oid;
  try
    {
      oid = this->filter_poa_->activate_object (filter);
    }
  catch (PortableServer::POA::ServantAlreadyActive&)
    {
      try
        {
          oid = this->filter_poa_->servant_to_id (filter);
        }
      catch (CORBA::Exception& )
        {
          throw CORBA::INTERNAL ();
          return 0;
        }
    }
  CORBA::Object_var obj =
    this->filter_poa_->id_to_reference (oid.in ());

  return CosNotifyFilter::Filter::_narrow (obj.in ());
}

CosNotifyFilter::MappingFilter_ptr
TAO_Notify_ETCL_FilterFactory::create_mapping_filter (const char * /*constraint_grammar*/,
                                                      const CORBA::Any & /*default_value*/)
{
  throw CORBA::NO_IMPLEMENT ();
}

TAO_END_VERSIONED_NAMESPACE_DECL


void 
TAO_Notify_ETCL_FilterFactory::save_persistent (TAO_Notify::Topology_Saver& saver)
{
  bool changed = true;
  TAO_Notify::NVPList attrs; // ECF has no attributes
  saver.begin_object(0, "filter_factory", attrs, changed);

  if (this->filters_.current_size () > 0)
  {
    int index = 0;
    FILTERMAP::ITERATOR iterator (this->filters_);

    for (FILTERMAP::ENTRY *entry = 0;
      iterator.next (entry) != 0;
      iterator.advance (), ++index)
    {
      entry->int_id_->save_persistent (saver);
    }
  }

  saver.end_object(0, "filter_factory");
}


TAO_Notify::Topology_Object*
TAO_Notify_ETCL_FilterFactory::load_child (const ACE_CString &type,
  CORBA::Long id, const TAO_Notify::NVPList& attrs)
{
  ACE_UNUSED_ARG (id);

  TAO_Notify::Topology_Object* result = this;
  if (type == "filter")
  {
    const char* value = 0;
    if (attrs.find ("FilterId", value))
    {
      TAO_Notify_Object::ID const id = ACE_OS::atoi (value);
      if (DEBUG_LEVEL) 
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) reload filter %d\n"),
                    static_cast<int> (id)
                    ));

      filter_ids_.set_last_used (id);

      TAO_Notify_ETCL_Filter* filter = 0;
      this->create_filter (0, id, filter);
      filter->load_attrs (attrs);
 
      return  filter;
    }
  }
  return result;
}


void
TAO_Notify_ETCL_FilterFactory::release (void)
{
  delete this;
  //@@ inform factory
}


TAO_Notify_Object::ID 
TAO_Notify_ETCL_FilterFactory::get_filter_id (CosNotifyFilter::Filter_ptr filter)
{
  ::PortableServer::Servant svt 
    = this->filter_poa_->reference_to_servant (filter);

  FILTERMAP::ITERATOR iterator (this->filters_);

  for (FILTERMAP::ENTRY *entry = 0;
    iterator.next (entry) != 0;
    iterator.advance ())
  {
   if (svt == entry->int_id_)
     return entry->ext_id_;
  }

  throw CORBA::INTERNAL ();
  return 0;
}


CosNotifyFilter::Filter_ptr 
TAO_Notify_ETCL_FilterFactory::get_filter (const TAO_Notify_Object::ID& id)
{
  TAO_Notify_ETCL_Filter* filter = 0;
  if (filters_.find (id, filter) == -1)
    return CosNotifyFilter::Filter::_nil ();
  else
  {
    CORBA::Object_var obj =
      this->filter_poa_->servant_to_reference (filter);

    CosNotifyFilter::Filter_var filter 
      = CosNotifyFilter::Filter::_narrow (obj.in ());

    return filter._retn ();
  }
}


ACE_FACTORY_DEFINE (TAO_Notify_Serv, TAO_Notify_ETCL_FilterFactory)
