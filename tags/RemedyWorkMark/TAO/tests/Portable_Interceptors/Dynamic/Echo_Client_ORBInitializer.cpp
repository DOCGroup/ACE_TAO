// -*- C++ -*-
// $Id$

#include "Echo_Client_ORBInitializer.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "client_interceptor.h"

void
Echo_Client_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

void
Echo_Client_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{

  PortableInterceptor::ClientRequestInterceptor_ptr interceptor =
    PortableInterceptor::ClientRequestInterceptor::_nil ();

  // Install the Echo client request interceptor
  ACE_NEW_THROW_EX (interceptor,
                    Echo_Client_Request_Interceptor,
                    CORBA::NO_MEMORY ());

  PortableInterceptor::ClientRequestInterceptor_var
    client_interceptor = interceptor;

  info->add_client_request_interceptor (client_interceptor.in ());

}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
