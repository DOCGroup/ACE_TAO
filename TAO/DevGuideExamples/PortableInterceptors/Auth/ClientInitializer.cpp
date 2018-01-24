#include "ClientInitializer.h"
#include "ClientInterceptor.h"
#include <iostream>

ClientInitializer::ClientInitializer (void)
{
}

void
ClientInitializer::pre_init (PortableInterceptor::ORBInitInfo_ptr)
{
}

void
ClientInitializer::post_init (PortableInterceptor::ORBInitInfo_ptr info)
{

  // Create and register the request interceptors.
  PortableInterceptor::ClientRequestInterceptor_ptr ci =
    PortableInterceptor::ClientRequestInterceptor::_nil ();

  try
    {
      ci = new ClientInterceptor();
    }
  catch(...)
    {
      std::cerr << "Exception occurred creating ClientInterceptor" << std::endl;
    }

  PortableInterceptor::ClientRequestInterceptor_var ci_interceptor =
    ci;

  info->add_client_request_interceptor (ci_interceptor.in ());
}

