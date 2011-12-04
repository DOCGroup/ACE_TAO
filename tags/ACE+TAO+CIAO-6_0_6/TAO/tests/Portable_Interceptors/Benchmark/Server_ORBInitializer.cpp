// -*- C++ -*-
// $Id$

#include "Server_ORBInitializer.h"
#include "Interceptor_Type.h"
#include "server_interceptors.h"

Server_ORBInitializer::Server_ORBInitializer (int interceptor_type)
  :  interceptor_type_ (interceptor_type)
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

  PortableInterceptor::ServerRequestInterceptor_ptr tmp =
    PortableInterceptor::ServerRequestInterceptor::_nil ();

  switch (this->interceptor_type_)
    {
    default:
    case IT_NONE:
      return;

    case IT_NOOP:
      {
        // Installing the Vault interceptor
        ACE_NEW_THROW_EX (tmp,
                          Vault_Server_Request_NOOP_Interceptor (),
                          CORBA::NO_MEMORY ());
        break;
      }
    case IT_CONTEXT:
      {
        // Installing the Vault interceptor
        ACE_NEW_THROW_EX (tmp,
                          Vault_Server_Request_Context_Interceptor (),
                          CORBA::NO_MEMORY ());
        break;
      }
    case IT_DYNAMIC:
      {
        // Installing the Vault interceptor
        ACE_NEW_THROW_EX (tmp,
                          Vault_Server_Request_Dynamic_Interceptor (),
                          CORBA::NO_MEMORY ());
        break;
      }
    }

  PortableInterceptor::ServerRequestInterceptor_var interceptor = tmp;

  info->add_server_request_interceptor (interceptor.in ());
}
