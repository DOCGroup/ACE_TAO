// -*- C++ -*-
//
// $Id$
//

#include "Server_ORBInitializer.h"
#include "Test_Protocols_Hooks.h"
#include "tao/ORB_Core.h"
#include "tao/PI/ORBInitInfo.h"

Server_ORBInitializer::Server_ORBInitializer (void)
{
}

void
Server_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  // Narrow to a TAO_ORBInitInfo object to get access to the
  // orb_core() TAO extension.
  TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info);

  if (CORBA::is_nil (tao_info.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }

  tao_info->orb_core ()->orb_params ()->protocols_hooks_name ("Test_Protocols_Hooks");
  ACE_Service_Config::process_directive (ace_svc_desc_Test_Protocols_Hooks);
}

void
Server_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
  // No-op.
}
