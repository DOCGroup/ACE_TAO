// -*- C++ -*-

#include "LB_ORBInitializer.h"
#include "LB_LoadManager.h"

#include "tao/ORBInitInfo.h"
#include "tao/ORB_Core.h"
#include "tao/debug.h"


ACE_RCSID (LoadBalancing,
           LB_ORBInitializer,
           "$Id$")


void
TAO_LB_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_LB_LoadManager * lm_servant = 0;
  ACE_NEW_THROW_EX (lm_servant,
                    TAO_LB_LoadManager,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableServer::ServantBase_var safe_lm_servant = lm_servant;

  CORBA::Object_var obj;

  // The RootPOA better be available at this point in time!
  ACE_TRY
    {
      obj = info->resolve_initial_references ("RootPOA"
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::ORB::InvalidName, ex)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "TAO_LB_ORBInitializer::pre_init() - "
                    "Unable to resolve RootPOA reference.\n"));

      ACE_THROW (CORBA::INTERNAL ());
    }
  ACE_ENDTRY;
  ACE_CHECK;

  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (obj.in ()
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_ORBInitInfo_var tao_info =
    TAO_ORBInitInfo::_narrow (info
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  lm_servant->init (tao_info->orb_core ()->reactor (),
                    root_poa.in ()
                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CosLoadBalancing::LoadManager_var load_manager =
    lm_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  info->register_initial_reference ("LoadManager",
                                    load_manager.in ()
                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_LB_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}
