// $Id$

#include "tao/Strategies/Strategies_ORBInitializer.h"
#include "tao/Exception.h"
#include "tao/ORB_Core.h"
#include "tao/PI/ORBInitInfo.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_Strategies_ORBInitializer::pre_init (PortableInterceptor::ORBInitInfo_ptr info)
{
  // Narrow to a TAO_ORBInitInfo object to get access to the
  // orb_core() TAO extension.
  TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info);

  if (CORBA::is_nil (tao_info.in ()))
    {
      if (TAO_debug_level > 0)
        TAOLIB_ERROR ((LM_ERROR,
                    "(%P|%t) TAO_Strategies_ORBInitializer::pre_init:\n"
                    "(%P|%t)    Unable to narrow "
                    "\"PortableInterceptor::ORBInitInfo_ptr\" to\n"
                    "(%P|%t)   \"TAO_ORBInitInfo *.\"\n"));

      throw ::CORBA::INTERNAL ();
    }

  // Make sure we get the correct endpoint selector
  tao_info->orb_core ()->orb_params ()->endpoint_selector_factory_name ("OC_Endpoint_Selector_Factory");
}

void
TAO_Strategies_ORBInitializer::post_init (PortableInterceptor::ORBInitInfo_ptr)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL

