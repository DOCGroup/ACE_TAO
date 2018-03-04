#include "ServerInitializer.h"
#include "ServerInterceptor.h"

#if defined (ACE_HAS_STANDARD_CPP_LIBRARY) && (ACE_HAS_STANDARD_CPP_LIBRARY == 1)
#include <iostream>
#else
#include <iostream.h>
#endif

ServerInitializer::ServerInitializer ()
{
}

void
ServerInitializer::pre_init (
                             PortableInterceptor::ORBInitInfo_ptr)
{
}

void
ServerInitializer::post_init (
                              PortableInterceptor::ORBInitInfo_ptr info)
{

  // Create and register the request interceptors.
  PortableInterceptor::ServerRequestInterceptor_ptr si =
    PortableInterceptor::ServerRequestInterceptor::_nil ();

  try
    {
      si = new ServerInterceptor ();
    }
  catch(...)
    {
      std::cerr << "Exception occurred trying to create ServerInterceptor." << std::endl;
    }

  PortableInterceptor::ServerRequestInterceptor_var si_interceptor =
    si;

  info->add_server_request_interceptor (si_interceptor.in ());
}
