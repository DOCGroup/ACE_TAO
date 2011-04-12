// -*- C++ -*-
// $Id$

#include "Client_ORBInitializer.h"
#include "client_interceptors.h"
#include "Interceptor_Type.h"

Client_ORBInitializer::Client_ORBInitializer (int interceptor_type)
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

  PortableInterceptor::ClientRequestInterceptor_ptr tmp =
    PortableInterceptor::ClientRequestInterceptor::_nil ();

  switch (this->interceptor_type_)
    {
    default:
    case IT_NONE:
      return;

    case IT_NOOP:
      {
        // Installing the Vault interceptor
        ACE_NEW_THROW_EX (tmp,
                          Vault_Client_Request_NOOP_Interceptor (),
                          CORBA::NO_MEMORY ());
        break;
      }
    case IT_CONTEXT:
      {
        // Installing the Vault interceptor
        ACE_NEW_THROW_EX (tmp,
                          Vault_Client_Request_Context_Interceptor (),
                          CORBA::NO_MEMORY ());
        break;
      }
    case IT_DYNAMIC:
      {
        // Installing the Vault interceptor
        ACE_NEW_THROW_EX (tmp,
                          Vault_Client_Request_Dynamic_Interceptor (),
                          CORBA::NO_MEMORY ());
        break;
      }
    }

  PortableInterceptor::ClientRequestInterceptor_var interceptor = tmp;

  info->add_client_request_interceptor (interceptor.in ());
}
