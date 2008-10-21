#include "ServerInitializer.h"
#include "ServerInterceptor.h"

ServerInitializer::ServerInitializer ()
{
}

void
ServerInitializer::pre_init (
                             PortableInterceptor::ORBInitInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ServerInitializer::post_init (
                              PortableInterceptor::ORBInitInfo_ptr info)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Create and register the request interceptors.
  PortableInterceptor::ServerRequestInterceptor_var si =
    new ServerInterceptor();
  info->add_server_request_interceptor (si.in());
}
