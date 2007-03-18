// $Id$

#include "RT_Test_FilterFactory.h"

ACE_RCSID(Notify, TAO_RT_Test_FilterFactory, "$Id$")

#include "RT_Test_Filter.h"
#include "tao/debug.h"
#include "ace/OS_NS_string.h"

TAO_Notify_Tests_RT_Test_FilterFactory::TAO_Notify_Tests_RT_Test_FilterFactory (void)
{
}

TAO_Notify_Tests_RT_Test_FilterFactory::~TAO_Notify_Tests_RT_Test_FilterFactory ()
{
}

CosNotifyFilter::FilterFactory_ptr
TAO_Notify_Tests_RT_Test_FilterFactory::create (PortableServer::POA_var& filter_poa)
{
  this->filter_poa_ = filter_poa; // save the filter poa.

  PortableServer::ServantBase_var servant_var (this);

  return _this ();
}

CosNotifyFilter::Filter_ptr
TAO_Notify_Tests_RT_Test_FilterFactory::create_filter (const char *constraint_grammar)
{
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "(%P, %t)In TAO_Notify_Tests_RT_Test_FilterFactory::create_filter\n"));

  // @@: change to "ExTCL" later.
  if (ACE_OS::strcmp (constraint_grammar, "TCL") != 0 &&
      ACE_OS::strcmp (constraint_grammar, "ETCL") != 0 &&
      ACE_OS::strcmp (constraint_grammar, "EXTENDED_TCL") != 0)
    throw CosNotifyFilter::InvalidGrammar ();


  // Create the RefCounted servant.
  TAO_Notify_Tests_RT_Test_Filter* filter = 0;

  ACE_NEW_THROW_EX (filter,
                    TAO_Notify_Tests_RT_Test_Filter (),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var filter_var (filter);

  PortableServer::ObjectId_var oid =
    this->filter_poa_->activate_object (filter);

  CORBA::Object_var obj =
    this->filter_poa_->id_to_reference (oid.in ());

  return CosNotifyFilter::Filter::_narrow (obj.in ());
}

CosNotifyFilter::MappingFilter_ptr
TAO_Notify_Tests_RT_Test_FilterFactory::create_mapping_filter (const char * /*constraint_grammar*/,
                                                  const CORBA::Any & /*default_value*/
                                                  )
{
  throw CORBA::NO_IMPLEMENT ();
}

ACE_FACTORY_DEFINE (TAO_RT_Test_Filter, TAO_Notify_Tests_RT_Test_FilterFactory)
