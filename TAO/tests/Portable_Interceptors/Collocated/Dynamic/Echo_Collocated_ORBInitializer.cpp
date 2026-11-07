// -*- C++ -*-
#include "Echo_Collocated_ORBInitializer.h"
#include "interceptors.h"

#include <cstring>

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

  if (std::strcmp (orb_id.in (), "client_orb") == 0)
    {
      // The var below takes ownership of this ACE_NEW allocation.
      PortableInterceptor::ClientRequestInterceptor_ptr interceptor {};

      // Install the Echo client request interceptor
      ACE_NEW_THROW_EX (interceptor,
                        Echo_Client_Request_Interceptor,
                        CORBA::NO_MEMORY ());

      PortableInterceptor::ClientRequestInterceptor_var
        client_interceptor = interceptor;

      info->add_client_request_interceptor (client_interceptor.in ());
    }
  else if (std::strcmp (orb_id.in (), "server_orb") == 0)
    {
      // The var below takes ownership of this ACE_NEW allocation.
      PortableInterceptor::ServerRequestInterceptor_ptr interceptor {};

      // Install the Echo server request interceptor
      ACE_NEW_THROW_EX (interceptor,
                        Echo_Server_Request_Interceptor,
                        CORBA::NO_MEMORY ());

      PortableInterceptor::ServerRequestInterceptor_var
        server_interceptor = interceptor;

      info->add_server_request_interceptor (server_interceptor.in ());
    }
}

