// -*- C++ -*-
//
// $Id$

#include "RTScheduler_Initializer.h"

ACE_RCSID (TAO, RTScheduler_Initializer, "$Id$")

#define TAO_RTSCHEDULER_SAFE_INCLUDE
#include "tao/RTScheduling/RTSchedulerC.h"
#include "tao/RTScheduling/Current.h"
#undef TAO_RTSCHEDULER_SAFE_INCLUDE

#include "tao/Exception.h"
#include "tao/ORB_Core.h"
#include "tao/ORBInitInfo.h"

#include "ace/Service_Repository.h"
#include "ace/Svc_Conf.h"

TAO_RTScheduler_Initializer::TAO_RTScheduler_Initializer (void)
{
}

void
TAO_RTScheduler_Initializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  //
  // Register all of the RT related services.
  //

  // Create the RT_Current.
  CORBA::Object_ptr current;
  ACE_NEW_THROW_EX (current,
                    TAO_RTScheduler_Current,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  TAO_RTScheduler_Current_var safe_current = current;

  info->register_initial_reference ("RTScheduler_Current",
                                    current
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

void
TAO_RTScheduler_Initializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Object_ptr rt_current_obj = info->resolve_initial_reference ("RT_Current"
								      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  
  RTCORBA::Current_var rt_current = RTCORBA::Current::_narrow (rt_current_obj.in ()
							       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  
  if (CORBA::is_nil (rt_current.in ()))
    {
      ACE_DEBUG ((LM_DEBUG,
		  "(%P|%t) TAO_RTScheduler_Initializer::post_init \n"
		  "(%P|%t) Unable to narrow to RTCORBA::Current\n"));
      ACE_THROW (CORBA::INTERNAL ());
    }
  
  CORBA::Object_ptr rtscheduler_current_obj = info->resolve_initial_reference ("RTScheduler_Current");
  TAO_RTScheduler_Current_var rtscheduler_current = TAO_RTScheduler_Current::_narrow (rtscheduler_current_obj);
  
  if (CORBA::is_nil (rtscheduler_current.in ()))
    {
      ACE_DEBUG ((LM_DEBUG,
		  "(%P|%t) TAO_RTScheduler_Initializer::post_init \n"
		  "(%P|%t) Unable to narrow to TAO_RTScheduler::Current\n"));
      ACE_THROW (CORBA::INTERNAL ());
    }
  
  rtscheduler_current->rt_current (rt_current.in ());
}

