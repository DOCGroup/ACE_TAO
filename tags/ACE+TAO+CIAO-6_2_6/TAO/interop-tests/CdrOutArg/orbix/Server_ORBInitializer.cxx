// -*- C++ -*-
//
// $Id$
//

#include "Server_ORBInitializer.h"
#include "server_interceptor.h"

Server_ORBInitializer::Server_ORBInitializer (void)
{
}

void
Server_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  /* Orbix 6 requires interceptor to be registered during pre_init,
   * otherwise interceptor's interception points do not get called.
   * mcknerney, 11/07/2013
   */
  PortableInterceptor::ServerRequestInterceptor_var
    server_interceptor = new Echo_Server_Request_Interceptor();

  info->add_server_request_interceptor (server_interceptor.in ());
}

void
Server_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
}

