// -*- C++ -*-
//
// $Id$

#include "Echo_Collocated_ORBInitializer.h"
#include "interceptors.h"
#include "tao/PI/ORBInitInfo.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_string.h"

Echo_Collocated_ORBInitializer::Echo_Collocated_ORBInitializer (
    PortableInterceptor::ProcessingMode server_proc_mode,
    PortableInterceptor::ProcessingMode client_proc_mode)
  : server_processing_mode_(server_proc_mode),
    client_processing_mode_(client_proc_mode)
{
}

void
Echo_Collocated_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

void
Echo_Collocated_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  CORBA::String_var orb_id =
    info->orb_id ();

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

  if (!ACE_OS::strcmp (orb_id.in (), "client_orb"))
    {
      PortableInterceptor::ClientRequestInterceptor_ptr interceptor =
        PortableInterceptor::ClientRequestInterceptor::_nil ();

      // Install the Echo client request interceptor
      ACE_NEW_THROW_EX (interceptor,
                        Echo_Client_Request_Interceptor,
                        CORBA::NO_MEMORY ());

      PortableInterceptor::ClientRequestInterceptor_var
        client_interceptor = interceptor;

      CORBA::Any client_proc_mode_as_any;
      client_proc_mode_as_any <<= this->client_processing_mode_;

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
  else if (!ACE_OS::strcmp (orb_id.in (), "server_orb"))
    {
      PortableInterceptor::ServerRequestInterceptor_ptr interceptor =
        PortableInterceptor::ServerRequestInterceptor::_nil ();

      // Install the Echo server request interceptor
      ACE_NEW_THROW_EX (interceptor,
                        Echo_Server_Request_Interceptor,
                        CORBA::NO_MEMORY ());

      PortableInterceptor::ServerRequestInterceptor_var
        server_interceptor = interceptor;

      CORBA::Any server_proc_mode_as_any;
      server_proc_mode_as_any <<= this->server_processing_mode_;

      CORBA::PolicyList policy_list (1);

      policy_list.length (1);
      policy_list[0] =
        orb->create_policy (PortableInterceptor::PROCESSING_MODE_POLICY_TYPE,
                            server_proc_mode_as_any);

      info_3_1->add_server_request_interceptor_with_policy (
                                            server_interceptor.in (),
                                            policy_list);

      policy_list[0]->destroy ();
      policy_list[0] = CORBA::Policy::_nil ();
    }
  else
  {
  }
}

