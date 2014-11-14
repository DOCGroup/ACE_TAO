// -*- C++ -*-
//
// $Id$
//

#include "Server_ORBInitializer.h"
#include "server_interceptor.h"

Server_ORBInitializer::Server_ORBInitializer (int *counter)
  : counter_ (counter),
    intr_ (0)
{
}

void
Server_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

void
Server_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  if (this->intr_ != 0)
    {
      return;
    }

  PortableInterceptor::ServerRequestInterceptor_ptr interceptor =
    PortableInterceptor::ServerRequestInterceptor::_nil ();

  // Install the Echo server request interceptor
  ACE_NEW_THROW_EX (this->intr_,
                    Ping_Death_Request_Interceptor (this->counter_),
                    CORBA::NO_MEMORY ());

  PortableInterceptor::ServerRequestInterceptor_var
    server_interceptor = interceptor;

  info->add_server_request_interceptor (this->intr_); //server_interceptor.in ());
}

void
Server_ORBInitializer::set_poa (PortableServer::POA_ptr poa)
{
  if (this->intr_ == 0)
    {
      return;
    }

  this->intr_->set_poa (poa);
}
