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

}

