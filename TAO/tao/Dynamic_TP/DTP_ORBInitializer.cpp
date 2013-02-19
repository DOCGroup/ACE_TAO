// $Id$

#include "tao/Dynamic_TP/DTP_ORBInitializer.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/Dynamic_TP/DTP_Config.h"
#include "tao/Dynamic_TP/DTP_Thread_Lane_Resources_Manager.h"
#include "tao/Dynamic_TP/DTP_Thread_Pool.h"
#include "tao/Exception.h"
#include "tao/ORB_Core.h"
#include "tao/PI/ORBInitInfo.h"
#include "tao/debug.h"
#include "tao/Leader_Follower.h"

#include "ace/Service_Repository.h"
#include "ace/Svc_Conf.h"
#include "ace/Sched_Params.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_DTP_ORBInitializer::TAO_DTP_ORBInitializer (/*const ACE_CString &tplist*/)
{

}

void
TAO_DTP_ORBInitializer::pre_init (PortableInterceptor::ORBInitInfo_ptr info)
{
  TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info);

  if (CORBA::is_nil (tao_info.in ()))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) TAO_DTP_ORBInitializer::pre_init:\n")
                    ACE_TEXT ("(%P|%t)    Unable to narrow ")
                    ACE_TEXT ("\"PortableInterceptor::ORBInitInfo_ptr\" to\n")
                    ACE_TEXT ("(%P|%t)   \"TAO_ORBInitInfo *.\"\n")));

      throw ::CORBA::INTERNAL ();
    }


  ACE_Service_Gestalt *gestalt = tao_info->orb_core ()->configuration();

  ACE_Service_Object * const config_obj =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (
      gestalt,
      "DTP_Config",
      true);
  if (config_obj == 0)
    {
      // no config manager object loaded
      return;
    }

  TAO_DTP_Config *config_mgr = dynamic_cast<TAO_DTP_Config *>(config_obj);
  if (config_mgr == 0)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) TAO_DTP_ORBInitializer::pre_init:\n")
                    ACE_TEXT ("(%P|%t)   Unable to resolve DTP_Config ")
                    ACE_TEXT ("object\n")));

      throw ::CORBA::INTERNAL ();
    }

  // Set the name of the thread lane resources manager to be
  // DTP_Thread_Lane_Resources_Manager.
  tao_info->orb_core ()->orb_params ()
    ->thread_lane_resources_manager_factory_name (
        "DTP_Thread_Lane_Resources_Manager_Factory");
  ACE_Service_Config::process_directive (
    ace_svc_desc_TAO_DTP_Thread_Lane_Resources_Manager_Factory);

}

void
TAO_DTP_ORBInitializer::post_init (PortableInterceptor::ORBInitInfo_ptr info)
{
  TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info);

  TAO_Thread_Lane_Resources_Manager &tlrm =
    tao_info->orb_core ()->thread_lane_resources_manager();

  ACE_Service_Gestalt *gestalt = tao_info->orb_core ()->configuration();

  const char *dtp_name =
    tao_info->orb_core ()->orb_params ()->dynamic_thread_pool_config_name ();

  if (dtp_name != 0 && dtp_name[0] != 0)
    {
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) TAO_DTP_ORBInitializer::post_init ")
                      ACE_TEXT ("using thread pool name %s\n"), dtp_name));
        }

      TAO_DTP_Config_Registry *config_registry =
        dynamic_cast<TAO_DTP_Config_Registry *>
        (ACE_Dynamic_Service<ACE_Service_Object>::instance
         (gestalt, "DTP_Config_Registry", true));

      TAO_DTP_Definition def;

      if (config_registry == 0 || !config_registry->find(dtp_name, def))
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) TAO_DTP_ORBInitializer::")
                        ACE_TEXT ("post_init:\n")
                        ACE_TEXT ("(%P|%t)   Unable to resolve ")
                        ACE_TEXT ("DTP_Config object\n")));

          throw ::CORBA::INTERNAL ();
        }

      TAO_DTP_Thread_Lane_Resources_Manager &dtp_tlrm =
        dynamic_cast<TAO_DTP_Thread_Lane_Resources_Manager &>(tlrm);

      dtp_tlrm.tp_manager().create_threadpool (def);

      tao_info->orb_core ()->leader_follower ().set_avoid_client_leader ();

    }


}


TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
