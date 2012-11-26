// $Id$

#include "tao/Dynamic_TP/Dynamic_TP_ORBInitializer.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#if 0
#define TAO_RTCORBA_SAFE_INCLUDE
#include "tao/RTCORBA/RTCORBAC.h"
#undef TAO_RTCORBA_SAFE_INCLUDE

#include "tao/RTCORBA/RT_Policy_i.h"
#include "tao/RTCORBA/RT_PolicyFactory.h"
#include "tao/RTCORBA/RT_Protocols_Hooks.h"
#include "tao/RTCORBA/Priority_Mapping_Manager.h"
#include "tao/RTCORBA/Network_Priority_Mapping_Manager.h"
#include "tao/RTCORBA/RT_ORB_Loader.h"
#include "tao/RTCORBA/RT_Stub_Factory.h"
#include "tao/RTCORBA/RT_Endpoint_Selector_Factory.h"
#include "tao/RTCORBA/Continuous_Priority_Mapping.h"
#include "tao/RTCORBA/Linear_Priority_Mapping.h"
#include "tao/RTCORBA/Direct_Priority_Mapping.h"
#include "tao/RTCORBA/Linear_Network_Priority_Mapping.h"
#include "tao/RTCORBA/RT_ORB.h"
#include "tao/RTCORBA/RT_Current.h"
#include "tao/RTCORBA/RT_Thread_Lane_Resources_Manager.h"
#include "tao/RTCORBA/RT_Service_Context_Handler.h"
#endif

#include "tao/Dynamic_TP/Dynamic_TP_Config.h"
#include "tao/Exception.h"
#include "tao/ORB_Core.h"
#include "tao/PI/ORBInitInfo.h"
#include "tao/debug.h"

#include "ace/Service_Repository.h"
#include "ace/Svc_Conf.h"
#include "ace/Sched_Params.h"

#if 0
static const char rt_poa_factory_name[] = "TAO_Dynamic_TP_Object_Adapter_Factory";
static const ACE_TCHAR rt_poa_factory_directive[] =
  ACE_DYNAMIC_SERVICE_DIRECTIVE(
    "TAO_Dynamic_TP_Object_Adapter_Factory",
    "TAO_RTPortableServer",
    "_make_TAO_Dynamic_TP_Object_Adapter_Factory",
    "");
#endif

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Dynamic_TP_ORBInitializer::TAO_Dynamic_TP_ORBInitializer (/*const ACE_CString &tplist*/)
{

}

void
TAO_Dynamic_TP_ORBInitializer::pre_init (PortableInterceptor::ORBInitInfo_ptr info)
{
  // Narrow to a TAO_ORBInitInfo object to get access to the
  // orb_core() TAO extension.
  TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info);

  if (CORBA::is_nil (tao_info.in ()))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) TAO_Dynamic_TP_ORBInitializer::pre_init:\n"
                    "(%P|%t)    Unable to narrow "
                    "\"PortableInterceptor::ORBInitInfo_ptr\" to\n"
                    "(%P|%t)   \"TAO_ORBInitInfo *.\"\n"));

      throw ::CORBA::INTERNAL ();
    }


  ACE_Service_Gestalt *gestalt = ACE_Service_Config::current ();

  ACE_Service_Object * const config_obj =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (
      gestalt,
      "Dynamic_TP_Config",
      true);
  if (config_obj == 0)
    {
      // no config manager object loaded
      return;
    }

  TAO_Dynamic_TP_Config *config_mgr = dynamic_cast<TAO_Dynamic_TP_Config *>(config_obj);
  if (config_mgr == 0)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) TAO_Dynamic_TP_ORBInitializer::pre_init:\n")
                    ACE_TEXT ("(%P|%t)   Unable to resolve Dynamic_TP_Config object\n")));

      throw ::CORBA::INTERNAL ();
    }
}

void
TAO_Dynamic_TP_ORBInitializer::post_init (PortableInterceptor::ORBInitInfo_ptr )
{
  //  this->register_policy_factories (info);
}


TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
