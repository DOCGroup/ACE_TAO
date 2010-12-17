// -*- C++ -*-
//
// $Id$

#include "ClientORBInitializer.h"

void
ClientORBInitializer::pre_init (PortableInterceptor::ORBInitInfo_ptr)
{
    // No-op
}

void
ClientORBInitializer::post_init (PortableInterceptor::ORBInitInfo_ptr info)
{
  PortableInterceptor::ClientRequestInterceptor_ptr cri =
    PortableInterceptor::ClientRequestInterceptor::_nil ();

  ACE_NEW_THROW_EX (cri,
                    ClientRequest_Interceptor,
                    CORBA::NO_MEMORY ());

  PortableInterceptor::ClientRequestInterceptor_var
    client_interceptor = cri;

  info->add_client_request_interceptor (client_interceptor.in ());

}
