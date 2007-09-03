// -*- C++ -*-
//
// $Id$

#include "Echo_Client_ORBInitializer.h"
#include "client_interceptor.h"
#include "tao/PI/ORBInitInfo.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_string.h"

Echo_Client_ORBInitializer::Echo_Client_ORBInitializer (
    PortableInterceptor::ProcessingMode proc_mode)
  : processing_mode_(proc_mode)
{
}

void
Echo_Client_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

void
Echo_Client_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  // TAO-Specific way to get to the ORB Core (and thus, the ORB).
  TAO_ORBInitInfo_var tao_info =
    TAO_ORBInitInfo::_narrow (info);

  CORBA::ORB_var orb = CORBA::ORB::_duplicate(tao_info->orb_core()->orb());

  if (CORBA::is_nil(orb.in()))
    {
      throw CORBA::INTERNAL ();
    }

  PortableInterceptor::ORBInitInfo_3_1_var info_3_1 =
    PortableInterceptor::ORBInitInfo_3_1::_narrow(info);

  if (CORBA::is_nil(info_3_1.in()))
    {
      throw CORBA::INTERNAL ();
    }

  PortableInterceptor::ClientRequestInterceptor_ptr interceptor =
    PortableInterceptor::ClientRequestInterceptor::_nil ();

  // Install the Echo client request interceptor
  ACE_NEW_THROW_EX (interceptor,
                    Echo_Client_Request_Interceptor,
                    CORBA::NO_MEMORY ());

  PortableInterceptor::ClientRequestInterceptor_var
    client_interceptor = interceptor;

  CORBA::Any client_proc_mode_as_any;
  client_proc_mode_as_any <<= this->processing_mode_;

  CORBA::PolicyList policy_list (1);

  policy_list.length (1);
  policy_list[0] =
    orb->create_policy (PortableInterceptor::PROCESSING_MODE_POLICY_TYPE,
                        client_proc_mode_as_any);

  info_3_1->add_client_request_interceptor_with_policy (
                                    client_interceptor.in (),
                                    policy_list);

  policy_list[0]->destroy ();
  policy_list[0] = CORBA::Policy::_nil ();
}

