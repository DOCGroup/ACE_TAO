// -*- C++ -*-

#include "Client_ORBInitializer.h"
#include "interceptors.h"

void
Client_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
  // No Op
}

void
Client_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  CORBA::String_var const orb_id =
    info->orb_id ();

  PortableInterceptor::ClientRequestInterceptor_ptr interceptor {};
    PortableInterceptor::ClientRequestInterceptor::_nil ();

  // Install the Echo client request interceptor
  ACE_NEW_THROW_EX (interceptor,
                    Echo_Client_Request_Interceptor (orb_id.in ()),
                    CORBA::NO_MEMORY ());

  // The var owns the newly allocated interceptor reference.
  PortableInterceptor::ClientRequestInterceptor_var const client_interceptor = interceptor;

  info->add_client_request_interceptor (client_interceptor.in ());
}

