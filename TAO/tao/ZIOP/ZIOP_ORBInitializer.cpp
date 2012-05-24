// $Id$

#include "tao/ZIOP/ZIOP_ORBInitializer.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/ZIOP/ZIOP.h"
#include "tao/ZIOP/ZIOP_Stub_Factory.h"
#include "tao/ZIOP/ZIOP_PolicyFactory.h"
#include "tao/ZIOP/ZIOP_Service_Context_Handler.h"
#include "tao/ORB_Core.h"
#include "tao/PI/ORBInitInfo.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ZIOP_ORBInitializer::TAO_ZIOP_ORBInitializer (TAO_ZIOP_Loader* loader) : loader_ (loader)
{
}

void
TAO_ZIOP_ORBInitializer::pre_init (PortableInterceptor::ORBInitInfo_ptr info)
{
  // Narrow to a TAO_ORBInitInfo object to get access to the
  // orb_core() TAO extension.
  TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info);

  if (CORBA::is_nil (tao_info.in ()))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "ZIOP (%P|%t) TAO_ZIOP_ORBInitializer::pre_init:\n"
                    "ZIOP (%P|%t)    Unable to narrow "
                    "\"PortableInterceptor::ORBInitInfo_ptr\" to\n"
                    "ZIOP (%P|%t)   \"TAO_ORBInitInfo *.\"\n"));

      throw ::CORBA::INTERNAL ();
    }

  tao_info->orb_core ()->ziop_adapter (this->loader_);
  // Set the name of the stub factory to be ZIOP_Stub_Factory.
  tao_info->orb_core ()->orb_params ()->stub_factory_name ("ZIOP_Stub_Factory");
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_ZIOP_Stub_Factory);

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  // Bind the service context handler for ZIOP
  TAO_ZIOP_Service_Context_Handler* h = 0;
  ACE_NEW (h,
           TAO_ZIOP_Service_Context_Handler());
  tao_info->orb_core ()->service_context_registry ().bind (IOP::INVOCATION_POLICIES, h);
#endif
}

void
TAO_ZIOP_ORBInitializer::post_init (PortableInterceptor::ORBInitInfo_ptr info)
{
  this->register_policy_factories (info);
}

void
TAO_ZIOP_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info)
{
  // Register the ZIOP policy factories.
  PortableInterceptor::PolicyFactory_ptr policy_factory_ptr;
  ACE_NEW_THROW_EX (policy_factory_ptr,
                    TAO_ZIOP_PolicyFactory,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));


  PortableInterceptor::PolicyFactory_var policy_factory =
    policy_factory_ptr;

  try
    {
      info->register_policy_factory (ZIOP::COMPRESSION_ENABLING_POLICY_ID,
                                     policy_factory.in ());

      info->register_policy_factory (ZIOP::COMPRESSOR_ID_LEVEL_LIST_POLICY_ID,
                                     policy_factory.in ());

      info->register_policy_factory (ZIOP::COMPRESSION_LOW_VALUE_POLICY_ID,
                                     policy_factory.in ());

      info->register_policy_factory (ZIOP::COMPRESSION_MIN_RATIO_POLICY_ID,
                                     policy_factory.in ());
    }
  catch (const CORBA::BAD_INV_ORDER& ex)
    {
      if (ex.minor () == (CORBA::OMGVMCID | 16))
        {
          // The factory is already there, it happens because the
          // magic initializer in Messaging.cpp registers with the
          // ORB multiple times.  This is an indication that we
          // should do no more work in this ORBInitializer.
          return;
        }
      throw;
    }
  catch (...)
    {
      // Rethrow any other exceptions...
      throw;
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
