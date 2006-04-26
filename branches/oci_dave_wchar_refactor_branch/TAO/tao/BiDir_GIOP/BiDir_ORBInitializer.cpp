// $Id$

#include "tao/BiDir_GIOP/BiDir_ORBInitializer.h"
#include "tao/BiDir_GIOP/BiDir_PolicyFactory.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"

#include "tao/ORB_Constants.h"

ACE_RCSID (BiDir_GIOP,
           BiDir_ORBInitializer,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_BiDir_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //
}

void
TAO_BiDir_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->register_policy_factories (info
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_BiDir_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info
  ACE_ENV_ARG_DECL)
{
  /// Register the BiDir policy factories.
  PortableInterceptor::PolicyFactory_ptr temp_factory =
    PortableInterceptor::PolicyFactory::_nil ();
  /// This policy factory is used for all BiDir related policies.
  ACE_NEW_THROW_EX (temp_factory,
                    TAO_BiDir_PolicyFactory,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                         TAO::VMCID,
                         ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::PolicyFactory_var policy_factory = temp_factory;

  /// Bind the same policy factory to all BiDir related policy
  /// types since a single policy factory is used to create each of
  /// the different types of BiDir policies.

  CORBA::PolicyType type;

  type = BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE;
  info->register_policy_factory (type,
                                 policy_factory.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

TAO_END_VERSIONED_NAMESPACE_DECL
