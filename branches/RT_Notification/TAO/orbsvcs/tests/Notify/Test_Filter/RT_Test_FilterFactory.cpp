// $Id$

#include "RT_Test_FilterFactory.h"

ACE_RCSID(Notify, TAO_RT_Test_FilterFactory, "$id$")

#include "RT_Test_Filter.h"
#include "tao/debug.h"

TAO_NS_RT_Test_FilterFactory::TAO_NS_RT_Test_FilterFactory (void)
{
}

TAO_NS_RT_Test_FilterFactory::~TAO_NS_RT_Test_FilterFactory ()
{
}

CosNotifyFilter::FilterFactory_ptr
TAO_NS_RT_Test_FilterFactory::create (PortableServer::POA_var& filter_poa ACE_ENV_ARG_DECL)
{
  this->filter_poa_ = filter_poa; // save the filter poa.

  PortableServer::ServantBase_var servant_var (this);

  return _this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyFilter::Filter_ptr
TAO_NS_RT_Test_FilterFactory::create_filter (const char *constraint_grammar ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::InvalidGrammar
                   ))
{
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "(%P, %t)In TAO_NS_RT_Test_FilterFactory::create_filter\n"));

  // @@: change to "ExTCL" later.
  if (ACE_OS::strcmp (constraint_grammar, "TCL") != 0 &&
      ACE_OS::strcmp (constraint_grammar, "ETCL") != 0 &&
      ACE_OS::strcmp (constraint_grammar, "EXTENDED_TCL") != 0)
    ACE_THROW_RETURN (CosNotifyFilter::InvalidGrammar (), 0);


  // Create the RefCounted servant.
  TAO_NS_RT_Test_Filter* filter = 0;

  ACE_NEW_THROW_EX (filter,
                    TAO_NS_RT_Test_Filter (),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var filter_var (filter);

  PortableServer::ObjectId_var oid =
    this->filter_poa_->activate_object (filter
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyFilter::Filter::_nil ());

  CORBA::Object_var obj =
    this->filter_poa_->id_to_reference (oid.in ()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyFilter::Filter::_nil ());

  return CosNotifyFilter::Filter::_narrow (obj.in ()
                                           ACE_ENV_ARG_PARAMETER);
}

CosNotifyFilter::MappingFilter_ptr
TAO_NS_RT_Test_FilterFactory::create_mapping_filter (const char * /*constraint_grammar*/,
                                                  const CORBA::Any & /*default_value*/
                                                  ACE_ENV_ARG_DECL
                                                  )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::InvalidGrammar
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), CosNotifyFilter::MappingFilter::_nil ());
}

ACE_FACTORY_DEFINE (TAO_RT_Test_Filter, TAO_NS_RT_Test_FilterFactory)
