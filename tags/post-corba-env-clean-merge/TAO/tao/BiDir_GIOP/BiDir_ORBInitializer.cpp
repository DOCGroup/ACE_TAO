// -*- C++ -*-
//
// $Id$

#include "BiDir_ORBInitializer.h"
#include "BiDir_PolicyFactory.h"
#include "BiDirGIOP.h"

ACE_RCSID (BiDir_GIOP, BiDir_ORBInitializer, "$Id$")




void
TAO_BiDir_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //
}

void
TAO_BiDir_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->register_policy_factories (info
                                   TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_BiDir_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info
  TAO_ENV_ARG_DECL)
{
  /// Register the BiDir policy factories.
  PortableInterceptor::PolicyFactory_ptr temp_factory =
    PortableInterceptor::PolicyFactory::_nil ();
  PortableInterceptor::PolicyFactory_var policy_factory;

  /// This policy factory is used for all BiDir related policies.
  ACE_NEW_THROW_EX (temp_factory,
                    TAO_BiDir_PolicyFactory,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                         TAO_DEFAULT_MINOR_CODE,
                         ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  policy_factory = temp_factory;

  /// Bind the same policy factory to all BiDir related policy
  /// types since a single policy factory is used to create each of
  /// the different types of BiDir policies.

  CORBA::PolicyType type;

  type = BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

}
