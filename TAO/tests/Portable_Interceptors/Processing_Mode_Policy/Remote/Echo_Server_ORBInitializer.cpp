// -*- C++ -*-
//
// $Id$

#include "Echo_Server_ORBInitializer.h"
#include "server_interceptor.h"
#include "tao/PI/ORBInitInfo.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_string.h"

Echo_Server_ORBInitializer::Echo_Server_ORBInitializer (
    PortableInterceptor::ProcessingMode proc_mode)
  : processing_mode_(proc_mode)
{
}

void
Echo_Server_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Echo_Server_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::String_var orb_id =
    info->orb_id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // TAO-Specific way to get to the ORB Core (and thus, the ORB).
  TAO_ORBInitInfo_var tao_info =
    TAO_ORBInitInfo::_narrow (info
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::ORB_var orb = CORBA::ORB::_duplicate(tao_info->orb_core()->orb());

  if (CORBA::is_nil(orb.in()))
    {
      ACE_THROW (CORBA::INTERNAL ());
      ACE_CHECK;
    }

  PortableInterceptor::ORBInitInfo_3_1_var info_3_1 =
    PortableInterceptor::ORBInitInfo_3_1::_narrow(info
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil(info_3_1.in()))
    {
      ACE_THROW (CORBA::INTERNAL ());
      ACE_CHECK;
    }

  PortableInterceptor::ServerRequestInterceptor_ptr interceptor =
    PortableInterceptor::ServerRequestInterceptor::_nil ();

  // Install the Echo server request interceptor
  ACE_NEW_THROW_EX (interceptor,
                    Echo_Server_Request_Interceptor,
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  PortableInterceptor::ServerRequestInterceptor_var
    server_interceptor = interceptor;

  CORBA::Any proc_mode_as_any;
  proc_mode_as_any <<= this->processing_mode_;

  CORBA::PolicyList policy_list (1);

  policy_list.length (1);
  policy_list[0] =
    orb->create_policy (PortableInterceptor::PROCESSING_MODE_POLICY_TYPE,
                        proc_mode_as_any
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  info_3_1->add_server_request_interceptor_with_policy (
                                        server_interceptor.in (),
                                        policy_list
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  policy_list[0]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  policy_list[0] = CORBA::Policy::_nil ();
}

