// $Id$

#include "ETCL_FilterFactory.h"

#if ! defined (__ACE_INLINE__)
#include "ETCL_FilterFactory.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_ETCL_FilterFactory, "$id$")

#include "ETCL_Filter.h"

TAO_NS_ETCL_FilterFactory::TAO_NS_ETCL_FilterFactory (void)
{
}

TAO_NS_ETCL_FilterFactory::~TAO_NS_ETCL_FilterFactory ()
{
}

CosNotifyFilter::FilterFactory_ptr
TAO_NS_ETCL_FilterFactory::create (ACE_ENV_SINGLE_ARG_DECL)
{
  return _this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyFilter::Filter_ptr
TAO_NS_ETCL_FilterFactory::create_filter (const char *constraint_grammar ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::InvalidGrammar
                   ))
{
  // @@: change to "ExTCL" later.
  if (ACE_OS::strcmp (constraint_grammar, "TCL") != 0 &&
      ACE_OS::strcmp (constraint_grammar, "ETCL") != 0 &&
      ACE_OS::strcmp (constraint_grammar, "EXTENDED_TCL") != 0)
    ACE_THROW_RETURN (CosNotifyFilter::InvalidGrammar (), 0);


  // Create the RefCounted servant.
  TAO_NS_ETCL_Filter* filter = 0;

  ACE_NEW_THROW_EX (filter,
                    TAO_NS_ETCL_Filter (),
                    CORBA::NO_MEMORY ());

  PortableServer::POA_var my_POA = _default_POA ();

  PortableServer::ServantBase_var filter_var (filter);

  PortableServer::ObjectId_var oid =
    my_POA->activate_object (filter
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyFilter::Filter::_nil ());

  CORBA::Object_var obj =
    my_POA->id_to_reference (oid.in ()
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyFilter::Filter::_nil ());

  return CosNotifyFilter::Filter::_narrow (obj.in ()
                                           ACE_ENV_ARG_PARAMETER);
}

CosNotifyFilter::MappingFilter_ptr
TAO_NS_ETCL_FilterFactory::create_mapping_filter (const char * /*constraint_grammar*/,
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

ACE_FACTORY_DEFINE (TAO_Notify, TAO_NS_ETCL_FilterFactory)
