// -*- C++ -*-

#include "Server_ORBInitializer.h"

ACE_RCSID (ForwardRequest,
           Server_ORBInitializer,
           "$Id$")

#if TAO_HAS_INTERCEPTORS == 1

#include "Server_Request_Interceptor.h"

Server_ORBInitializer::Server_ORBInitializer (void)
  : server_interceptor_ (0)
{
}

void
Server_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Server_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ENV_ARG_DEFN;

  // Install the server request interceptor.
  ACE_NEW_THROW_EX (this->server_interceptor_,
                    Server_Request_Interceptor,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::ServerRequestInterceptor_var
    server_interceptor = this->server_interceptor_;

  info->add_server_request_interceptor (server_interceptor.in ()
                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

Server_Request_Interceptor *
Server_ORBInitializer::server_interceptor (void)
{
  return this->server_interceptor_;
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
