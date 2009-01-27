// $Id$

#include "orbsvcs/Notify/ETCL_FilterFactory.h"

ACE_RCSID(Notify, TAO_Notify_ETCL_FilterFactory, "$Id$")

#include "orbsvcs/Notify/ETCL_Filter.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_ETCL_FilterFactory::TAO_Notify_ETCL_FilterFactory (void) :
  filter_poa_ (PortableServer::POA::_nil ())
{
}

TAO_Notify_ETCL_FilterFactory::~TAO_Notify_ETCL_FilterFactory ()
{
}

CosNotifyFilter::FilterFactory_ptr
TAO_Notify_ETCL_FilterFactory::create (PortableServer::POA_ptr filter_poa)
{
  this->filter_poa_ = PortableServer::POA::_duplicate(filter_poa); // save the filter poa.

  PortableServer::ServantBase_var servant_var (this);

  PortableServer::ObjectId_var id = filter_poa->activate_object (this);

  CORBA::Object_var object = filter_poa->id_to_reference (id.in ());

  CosNotifyFilter::FilterFactory_var filter = CosNotifyFilter::FilterFactory::_narrow (object.in ());

  return filter._retn();
}

CosNotifyFilter::Filter_ptr
TAO_Notify_ETCL_FilterFactory::create_filter (const char *constraint_grammar)
{
  // @@: change to "ExTCL" later.
  if (ACE_OS::strcmp (constraint_grammar, "TCL") != 0 &&
      ACE_OS::strcmp (constraint_grammar, "ETCL") != 0 &&
      ACE_OS::strcmp (constraint_grammar, "EXTENDED_TCL") != 0)
    throw CosNotifyFilter::InvalidGrammar ();

  // Create the RefCounted servant.
  TAO_Notify_ETCL_Filter* filter = 0;

  ACE_NEW_THROW_EX (filter,
                    TAO_Notify_ETCL_Filter (this->filter_poa_.in ()),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var filter_var (filter);

  PortableServer::ObjectId_var oid =
    this->filter_poa_->activate_object (filter);

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

ACE_FACTORY_DEFINE (TAO_Notify_Serv, TAO_Notify_ETCL_FilterFactory)
