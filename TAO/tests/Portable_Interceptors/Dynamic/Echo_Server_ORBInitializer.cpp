// -*- C++ -*-
// $Id$

#include "Echo_Server_ORBInitializer.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "server_interceptor.h"

void
Echo_Server_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

void
Echo_Server_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{

  PortableInterceptor::ServerRequestInterceptor_ptr interceptor =
    PortableInterceptor::ServerRequestInterceptor::_nil ();

  // Install the Echo server request interceptor
  ACE_NEW_THROW_EX (interceptor,
                    Echo_Server_Request_Interceptor,
                    CORBA::NO_MEMORY ());

  PortableInterceptor::ServerRequestInterceptor_var
    server_interceptor = interceptor;

  info->add_server_request_interceptor (server_interceptor.in ());

}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
