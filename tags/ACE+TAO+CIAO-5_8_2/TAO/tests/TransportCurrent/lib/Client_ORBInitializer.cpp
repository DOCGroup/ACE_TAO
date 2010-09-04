// -*- C++ -*-
// $Id$

#include "ace/Log_Msg.h"

#include "Client_Request_Interceptor.h"
#include "Client_ORBInitializer.h"

namespace Test
{

  Client_ORBInitializer::Client_ORBInitializer
    (PortableInterceptor::ClientRequestInterceptor_ptr interceptor)
      : interceptor_
          (PortableInterceptor::ClientRequestInterceptor::_duplicate (interceptor))
  {
    /* no-op */
  }

  Client_ORBInitializer::~Client_ORBInitializer (void)
  {
    /* no-op */
  }

  void
  Client_ORBInitializer::pre_init(PortableInterceptor::ORBInitInfo*)
  {
    /* no-op */
  }

  // A specialization to handle client-side interceptors
  void Client_ORBInitializer::post_init (PortableInterceptor::ORBInitInfo* oii)
  {
    oii->add_client_request_interceptor (this->interceptor_.in ());
  }

}
