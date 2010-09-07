// -*- C++ -*-
// $Id$

#include "tao/RTScheduling/RTScheduler_Initializer.h"
#include "tao/RTScheduling/Request_Interceptor.h"

#include "tao/RTScheduling/RTScheduler.h"
#include "tao/RTScheduling/Current.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"

#include "tao/PI_Server/PI_Server.h"

#include "tao/PI/ORBInitInfo.h"

#include "tao/Exception.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"
#include "ace/Service_Repository.h"
#include "ace/Svc_Conf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_RTScheduler_ORB_Initializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  //
  // Register all of the RT related services.
  //

  // Create the RT_Current.

  // Narrow to a TAO_ORBInitInfo object to get access to the
  // orb_core() TAO extension.

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "In pre_init\n"));

  TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info);

  if (CORBA::is_nil (tao_info.in ()))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) Security_ORBInitializer::pre_init:\n"
                    "(%P|%t)    Unable to narrow "
                    "\"PortableInterceptor::ORBInitInfo_ptr\" to\n"
                    "(%P|%t)   \"TAO_ORBInitInfo *.\"\n"));

      throw ::CORBA::INTERNAL ();
    }

  TAO_RTScheduler_Current *tmp_current = 0;
  ACE_NEW_THROW_EX (tmp_current,
                    TAO_RTScheduler_Current,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  this->current_ = tmp_current;

  this->current_->init (tao_info->orb_core ());

  CORBA::Object_var current_obj =
    RTScheduling::Current::_narrow (this->current_.in ());

  info->register_initial_reference ("RTScheduler_Current", current_obj.in ());

  Client_Interceptor *client_interceptor = 0;
  ACE_NEW_THROW_EX (client_interceptor,
                    Client_Interceptor,
                    CORBA::NO_MEMORY (
                        CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                        CORBA::COMPLETED_NO));

  PortableInterceptor::ClientRequestInterceptor_var safe_client =
    client_interceptor;

  info->add_client_request_interceptor (client_interceptor);

  Server_Interceptor *server_interceptor = 0;
  ACE_NEW_THROW_EX (server_interceptor,
                    Server_Interceptor (this->current_.in ()),
                    CORBA::NO_MEMORY (
                        CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                        CORBA::COMPLETED_NO));

  PortableInterceptor::ServerRequestInterceptor_var safe_server =
    server_interceptor;

  info->add_server_request_interceptor (server_interceptor);

  // Set the RTScheduler_Manager
  TAO_RTScheduler_Manager *manager = 0;

  ACE_NEW_THROW_EX (manager,
                    TAO_RTScheduler_Manager (tao_info->orb_core ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));


  TAO_RTScheduler_Manager_var safe_manager = manager;

  info->register_initial_reference ("RTSchedulerManager", manager);
}

void
TAO_RTScheduler_ORB_Initializer::post_init (PortableInterceptor::ORBInitInfo_ptr info)
{

  // @@ This is busted.  TAO_ORBInitInfo should do proper reference
  //    counting.
  // Narrow to a TAO_ORBInitInfo object to get access to the
  // orb_core() TAO extension.
  //TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info
  //                                                          );

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "In post_init\n"));

  CORBA::Object_var rt_current_obj =
    info->resolve_initial_references (TAO_OBJID_RTCURRENT);

  RTCORBA::Current_var rt_current =
    RTCORBA::Current::_narrow (rt_current_obj.in ());

  if (CORBA::is_nil (rt_current.in ()))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) ::post_init\n"
                  "(%P|%t) Unable to narrow to RTCORBA::Current\n"));
      throw ::CORBA::INTERNAL ();
    }

  this->current_->rt_current (rt_current.in ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
