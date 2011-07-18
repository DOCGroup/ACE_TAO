// $Id$

#include "tao/BiDir_GIOP/BiDir_ORBInitializer.h"
#include "tao/BiDir_GIOP/BiDir_PolicyFactory.h"
#include "tao/BiDir_GIOP/BiDirGIOP.h"
#include "tao/BiDir_GIOP/BiDir_Service_Context_Handler.h"
#include "tao/PI/ORBInitInfo.h"

#include "tao/ORB_Constants.h"
#include "tao/ORB_Core.h"

#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_BiDir_ORBInitializer::pre_init (PortableInterceptor::ORBInitInfo_ptr info)
{
  // Narrow to a TAO_ORBInitInfo object to get access to the
  // orb_core() TAO extension.
  TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info);

  if (CORBA::is_nil (tao_info.in ()))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) TAO_Bidir_ORBInitializer::pre_init:\n"
                    "(%P|%t)    Unable to narrow "
                    "\"PortableInterceptor::ORBInitInfo_ptr\" to\n"
                    "(%P|%t)   \"TAO_ORBInitInfo *.\"\n"));

      throw ::CORBA::INTERNAL ();
    }

  // Bind the service context handler for BiDIR GIOP
  TAO_BiDIR_Service_Context_Handler* h = 0;
  ACE_NEW (h,
           TAO_BiDIR_Service_Context_Handler());
  tao_info->orb_core ()->service_context_registry ().bind (IOP::BI_DIR_IIOP, h);
}

void
TAO_BiDir_ORBInitializer::post_init (PortableInterceptor::ORBInitInfo_ptr info)
{
  this->register_policy_factories (info);
}

void
TAO_BiDir_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info)
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
