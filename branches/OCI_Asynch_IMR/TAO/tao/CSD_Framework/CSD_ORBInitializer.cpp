// -*- C++ -*-
// $Id$

#include "tao/CSD_Framework/CSD_ORBInitializer.h"
#include "tao/Exception.h"
#include "tao/ORB_Core.h"
#include "tao/PI/ORBInitInfo.h"
#include "tao/debug.h"

static const ACE_TCHAR csd_poa_factory_directive[] =
  ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE(
    "TAO_CSD_Object_Adapter_Factory",
    "TAO_CSD_Framework",
    TAO_VERSION,
    "_make_TAO_CSD_Object_Adapter_Factory",
    "");

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_CSD_ORBInitializer::pre_init (PortableInterceptor::ORBInitInfo_ptr info)
{
  // Narrow to a TAO_ORBInitInfo object to get access to the
  // orb_core() TAO extension.
  TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info);

  if (CORBA::is_nil (tao_info.in ()))
    {
      if (TAO_debug_level > 0)
        TAOLIB_ERROR ((LM_ERROR,
                    "(%P|%t) TAO_CSD_ORBInitializer::pre_init:\n"
                    "(%P|%t)    Unable to narrow "
                    "\"PortableInterceptor::ORBInitInfo_ptr\" to\n"
                    "(%P|%t)   \"TAO_ORBInitInfo *.\"\n"));

      throw ::CORBA::INTERNAL ();
    }

  // If the application resolves the root POA, make sure we load the CSD POA.
  tao_info->orb_core ()->orb_params ()->poa_factory_name ("TAO_CSD_Object_Adapter_Factory");

  tao_info->orb_core ()->orb_params ()->poa_factory_directive (csd_poa_factory_directive);
}

void
TAO_CSD_ORBInitializer::post_init (PortableInterceptor::ORBInitInfo_ptr)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL

