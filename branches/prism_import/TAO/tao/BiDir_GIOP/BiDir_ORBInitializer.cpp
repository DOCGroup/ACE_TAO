// $Id$

#include "tao/BiDir_GIOP/BiDir_ORBInitializer.h"
#include "tao/BiDir_GIOP/BiDir_PolicyFactory.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"

#include "tao/ORB_Constants.h"

#include "ace/CORBA_macros.h"

ACE_RCSID (BiDir_GIOP,
           BiDir_ORBInitializer,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_BiDir_ORBInitializer::pre_init (PortableInterceptor::ORBInitInfo_ptr)
{
  //
}

void
TAO_BiDir_ORBInitializer::post_init (PortableInterceptor::ORBInitInfo_ptr info)
{
  this->register_policy_factories (info);
}

void
TAO_BiDir_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info
  )
{
  /// Register the BiDir policy factories.
  PortableInterceptor::PolicyFactory_ptr temp_factory =
    PortableInterceptor::PolicyFactory_ptr ();
  /// This policy factory is used for all BiDir related policies.
  ACE_NEW_THROW_EX (temp_factory,
                    TAO_BiDir_PolicyFactory,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                         TAO::VMCID,
                         ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::PolicyFactory_var policy_factory = temp_factory;

  /// Bind the same policy factory to all BiDir related policy
  /// types since a single policy factory is used to create each of
  /// the different types of BiDir policies.

  info->register_policy_factory (BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE,
                                 policy_factory.in ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
