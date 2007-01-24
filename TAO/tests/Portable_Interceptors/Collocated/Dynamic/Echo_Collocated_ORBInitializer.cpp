// -*- C++ -*-
//
// $Id$

#include "Echo_Collocated_ORBInitializer.h"
#include "interceptors.h"

#include "ace/OS_NS_string.h"

void
Echo_Collocated_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Echo_Collocated_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::String_var orb_id =
    info->orb_id ();

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

      info->add_client_request_interceptor (client_interceptor.in ()
                                        ACE_ENV_ARG_PARAMETER);
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

      info->add_server_request_interceptor (server_interceptor.in ()
                                            ACE_ENV_ARG_PARAMETER);
    }
  else
  {
  }

}

