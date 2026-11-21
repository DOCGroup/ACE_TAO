// -*- C++ -*-
#include "Server_ORBInitializer.h"
#include "Interceptor_Type.h"
#include "server_interceptors.h"

Server_ORBInitializer::Server_ORBInitializer (Interceptor_Type interceptor_type)
  : interceptor_type_ (interceptor_type)
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
  PortableInterceptor::ServerRequestInterceptor_ptr tmp {};

  switch (this->interceptor_type_)
    {
    default:
    case Interceptor_Type::none:
      return;

    case Interceptor_Type::noop:
      {
        // Installing the Vault interceptor
        ACE_NEW_THROW_EX (tmp,
                          Vault_Server_Request_NOOP_Interceptor (),
                          CORBA::NO_MEMORY ());
        break;
      }
    case Interceptor_Type::context:
      {
        // Installing the Vault interceptor
        ACE_NEW_THROW_EX (tmp,
                          Vault_Server_Request_Context_Interceptor (),
                          CORBA::NO_MEMORY ());
        break;
      }
    case Interceptor_Type::dynamic:
      {
        // Installing the Vault interceptor
        ACE_NEW_THROW_EX (tmp,
                          Vault_Server_Request_Dynamic_Interceptor (),
                          CORBA::NO_MEMORY ());
        break;
      }
    }

  // The var takes ownership of the allocated interceptor.
  PortableInterceptor::ServerRequestInterceptor_var const interceptor (tmp);

  info->add_server_request_interceptor (interceptor.in ());
}
