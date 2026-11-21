// -*- C++ -*-
#include "Client_ORBInitializer.h"
#include "client_interceptors.h"
#include "Interceptor_Type.h"

Client_ORBInitializer::Client_ORBInitializer (Interceptor_Type interceptor_type)
  : interceptor_type_ (interceptor_type)
{
}

void
Client_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

void
Client_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  PortableInterceptor::ClientRequestInterceptor_ptr tmp {};

  switch (this->interceptor_type_)
    {
    default:
    case Interceptor_Type::none:
      return;

    case Interceptor_Type::noop:
      {
        // Installing the Vault interceptor
        ACE_NEW_THROW_EX (tmp,
                          Vault_Client_Request_NOOP_Interceptor (),
                          CORBA::NO_MEMORY ());
        break;
      }
    case Interceptor_Type::context:
      {
        // Installing the Vault interceptor
        ACE_NEW_THROW_EX (tmp,
                          Vault_Client_Request_Context_Interceptor (),
                          CORBA::NO_MEMORY ());
        break;
      }
    case Interceptor_Type::dynamic:
      {
        // Installing the Vault interceptor
        ACE_NEW_THROW_EX (tmp,
                          Vault_Client_Request_Dynamic_Interceptor (),
                          CORBA::NO_MEMORY ());
        break;
      }
    }

  // The var takes ownership of the allocated interceptor.
  PortableInterceptor::ClientRequestInterceptor_var const interceptor (tmp);

  info->add_client_request_interceptor (interceptor.in ());
}
