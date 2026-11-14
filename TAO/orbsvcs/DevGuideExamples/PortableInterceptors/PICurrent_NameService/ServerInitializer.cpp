#include "ServerInitializer.h"
#include "ServerInterceptor.h"

void
ServerInitializer::pre_init (PortableInterceptor::ORBInitInfo_ptr)
{
}

void
ServerInitializer::post_init (PortableInterceptor::ORBInitInfo_ptr info)
{
  PortableInterceptor::ServerRequestInterceptor_var const interceptor =
    new ServerInterceptor;
  info->add_server_request_interceptor (interceptor.in ());
}
