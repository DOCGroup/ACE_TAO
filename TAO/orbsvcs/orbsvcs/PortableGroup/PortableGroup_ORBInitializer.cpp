// -*- C++ -*-
//
// $Id$

#include "PortableGroup_ORBInitializer.h"

ACE_RCSID (PortableGroup,
           PortableGroup_ORBInitializer,
           "$Id$")

#include "ace/Service_Repository.h"
#include "ace/Svc_Conf.h"

#include "tao/Exception.h"
#include "tao/ORB_Core.h"
#include "tao/ORBInitInfo.h"

#include "PortableGroup_Request_Dispatcher.h"
#include "POA_Hooks.h"

TAO_PortableGroup_ORBInitializer::TAO_PortableGroup_ORBInitializer ()
{
}

void
TAO_PortableGroup_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Narrow to a TAO_ORBInitInfo object to get access to the
  // orb_core() TAO extension.
  TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info
                                                           TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (tao_info.in ()))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) PortableGroup_ORBInitializer::pre_init:\n"
                    "(%P|%t)    Unable to narrow "
                    "\"PortableInterceptor::ORBInitInfo_ptr\" to\n"
                    "(%P|%t)   \"TAO_ORBInitInfo_ptr.\"\n"));

      ACE_THROW (CORBA::INTERNAL ());
    }

  // Set a new request dispatcher in the ORB.
  PortableGroup_Request_Dispatcher *rd;
  ACE_NEW_THROW_EX (rd,
                    PortableGroup_Request_Dispatcher (),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  tao_info->orb_core ()->request_dispatcher (rd);

  // Create and save the hooks for the POA.
  TAO_POA_PortableGroup_Hooks *poa_hooks;
  ACE_NEW_THROW_EX (poa_hooks,
                    TAO_POA_Hooks (*rd),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;
  tao_info->orb_core ()->portable_group_poa_hooks (poa_hooks);
}

void
TAO_PortableGroup_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

