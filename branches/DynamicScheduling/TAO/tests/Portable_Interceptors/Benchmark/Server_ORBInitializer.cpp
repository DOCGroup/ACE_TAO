// -*- C++ -*-
//
// $Id$
//

#include "Server_ORBInitializer.h"
#include "Interceptor_Type.h"
#include "interceptors.h"

ACE_RCSID (Benchmark,
           Server_ORBInitializer,
           "$Id$")

Server_ORBInitializer::Server_ORBInitializer (int interceptor_type)
  :  interceptor_type_ (interceptor_type)
{
}

void
Server_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Server_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
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
  ACE_CHECK;

  PortableInterceptor::ServerRequestInterceptor_var interceptor = tmp;

  info->add_server_request_interceptor (interceptor.in ()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
