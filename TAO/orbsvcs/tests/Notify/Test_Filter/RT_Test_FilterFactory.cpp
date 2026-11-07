#include "RT_Test_FilterFactory.h"
#include "RT_Test_Filter.h"
#include "tao/debug.h"

#include <cstring>

CosNotifyFilter::FilterFactory_ptr
TAO_Notify_Tests_RT_Test_FilterFactory::create (PortableServer::POA_ptr filter_poa)
{
  this->filter_poa_ = PortableServer::POA::_duplicate (filter_poa);

  PortableServer::ObjectId_var id = filter_poa->activate_object (this);

  CORBA::Object_var const object = filter_poa->id_to_reference (id.in ());

  CosNotifyFilter::FilterFactory_var filter =
    CosNotifyFilter::FilterFactory::_narrow (object.in ());

  return filter._retn();
}

void
TAO_Notify_Tests_RT_Test_FilterFactory::destroy ()
{
  if (CORBA::is_nil (this->filter_poa_.in ()))
    return;
  PortableServer::ServantBase_var guard (this);
  try
    {
      PortableServer::ObjectId_var const id =
        this->filter_poa_->servant_to_id (this);
      this->filter_poa_->deactivate_object (id.in ());
    }
  catch (CORBA::Exception const&)
    {
    }
}

CosNotifyFilter::Filter_ptr
TAO_Notify_Tests_RT_Test_FilterFactory::create_filter (char const* constraint_grammar)
{
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "(%P, %t)In TAO_Notify_Tests_RT_Test_FilterFactory::create_filter\n"));

  // @@: change to "ExTCL" later.
  if (std::strcmp (constraint_grammar, "TCL") != 0 &&
      std::strcmp (constraint_grammar, "ETCL") != 0 &&
      std::strcmp (constraint_grammar, "EXTENDED_TCL") != 0)
    throw CosNotifyFilter::InvalidGrammar ();

  // Create the RefCounted servant.
  TAO_Notify_Tests_RT_Test_Filter* filter {};

  ACE_NEW_THROW_EX (filter,
                    TAO_Notify_Tests_RT_Test_Filter (),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var filter_var (filter);

  PortableServer::ObjectId_var const oid =
    this->filter_poa_->activate_object (filter);

  CORBA::Object_var const obj =
    this->filter_poa_->id_to_reference (oid.in ());

  return CosNotifyFilter::Filter::_narrow (obj.in ());
}

CosNotifyFilter::MappingFilter_ptr
TAO_Notify_Tests_RT_Test_FilterFactory::create_mapping_filter (
  char const* /*constraint_grammar*/, CORBA::Any const& /*default_value*/)
{
  throw CORBA::NO_IMPLEMENT ();
}

TAO_Notify_Object::ID
TAO_Notify_Tests_RT_Test_FilterFactory::get_filter_id (CosNotifyFilter::Filter_ptr /*filter*/)
{
  throw CORBA::NO_IMPLEMENT ();
}

CosNotifyFilter::Filter_ptr
TAO_Notify_Tests_RT_Test_FilterFactory::get_filter (TAO_Notify_Object::ID const& /*id*/)
{
  throw CORBA::NO_IMPLEMENT ();
}

void
TAO_Notify_Tests_RT_Test_FilterFactory::release ()
{
  delete this;
  //@@ inform factory
}

void
TAO_Notify_Tests_RT_Test_FilterFactory::save_persistent (TAO_Notify::Topology_Saver& /*saver*/)
{
  throw CORBA::NO_IMPLEMENT ();
}

ACE_FACTORY_DEFINE (TAO_RT_Test_Filter, TAO_Notify_Tests_RT_Test_FilterFactory)
