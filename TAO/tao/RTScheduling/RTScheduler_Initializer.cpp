// -*- C++ -*-
//
// $Id$

#include "RTScheduler_Initializer.h"
#include "Request_Interceptor.h"

ACE_RCSID (TAO, RTScheduler_Initializer, "$Id$")

#include "tao/RTScheduling/RTScheduler.h"
#include "tao/RTScheduling/Current.h"
#include "tao/RTScheduling/RTScheduler_Manager.h"

#include "tao/Exception.h"
#include "tao/ORB_Core.h"
#include "tao/ORBInitInfo.h"

#include "ace/Service_Repository.h"
#include "ace/Svc_Conf.h"

void
 TAO_RTScheduler_ORB_Initializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
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

  TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info
                                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  if (CORBA::is_nil (tao_info.in ()))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) Security_ORBInitializer::pre_init:\n"
                    "(%P|%t)    Unable to narrow "
                    "\"PortableInterceptor::ORBInitInfo_ptr\" to\n"
                    "(%P|%t)   \"TAO_ORBInitInfo *.\"\n"));

      ACE_THROW (CORBA::INTERNAL ());
    }

  
  ACE_NEW_THROW_EX (this->current_,
                    TAO_RTScheduler_Current,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  this->current_->init (tao_info->orb_core ()
			ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Object_ptr current_obj = RTScheduling::Current::_narrow (this->current_
								  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  CORBA::Object_var safe_current = current_obj;
  
    info->register_initial_reference ("RTScheduler_Current",
				      current_obj
				      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

  Client_Interceptor *client_interceptor;
  ACE_NEW_THROW_EX (client_interceptor,
                    Client_Interceptor,
                    CORBA::NO_MEMORY (
			CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
			CORBA::COMPLETED_NO));
  ACE_CHECK;
  
 info->add_client_request_interceptor (client_interceptor
					ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  Server_Interceptor *server_interceptor;
  ACE_NEW_THROW_EX (server_interceptor,
                    Server_Interceptor (this->current_),
                    CORBA::NO_MEMORY (
			CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
			CORBA::COMPLETED_NO));
  ACE_CHECK;
  
 info->add_server_request_interceptor (server_interceptor
					ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Set the RTScheduler_Manager
  TAO_RTScheduler_Manager *manager = 0;

  ACE_NEW_THROW_EX (manager,
                    TAO_RTScheduler_Manager (tao_info->orb_core ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;


  TAO_RTScheduler_Manager_var safe_manager = manager;

  info->register_initial_reference ("RTSchedulerManager",
                                    manager
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  
}

void
TAO_RTScheduler_ORB_Initializer::post_init (PortableInterceptor::ORBInitInfo_ptr info
					    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  
  // @@ This is busted.  TAO_ORBInitInfo should do proper reference
  //    counting.
  // Narrow to a TAO_ORBInitInfo object to get access to the
  // orb_core() TAO extension.
  //TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info
  //                                                           ACE_ENV_ARG_PARAMETER);
  //ACE_CHECK;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
		"In post_init\n"));
  
  CORBA::Object_ptr rt_current_obj = info->resolve_initial_references ("RTCurrent"
								       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  
  RTCORBA::Current_var rt_current = RTCORBA::Current::_narrow (rt_current_obj
							       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  if (CORBA::is_nil (rt_current.in ()))
    {
      ACE_DEBUG ((LM_DEBUG,
		  "(%P|%t) ::post_init \n"
		  "(%P|%t) Unable to narrow to RTCORBA::Current\n"));
      ACE_THROW (CORBA::INTERNAL ());
    }
  
  this->current_->rt_current (rt_current.in ());
}

