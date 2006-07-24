// -*- C++ -*-

#include "Server_ORBInitializer.h"

ACE_RCSID (ForwardRequest,
           Server_ORBInitializer,
           "$Id$")

#if TAO_HAS_INTERCEPTORS == 1

#include "Server_Request_Interceptor.h"

#include "tao/ORB_Constants.h"

Server_ORBInitializer::Server_ORBInitializer (bool & destroy_called)
  : destroy_called_ (destroy_called)
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
  PortableInterceptor::ServerRequestInterceptor_ptr interceptor;
  ACE_NEW_THROW_EX (interceptor,
                    Server_Request_Interceptor (this->destroy_called_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::ServerRequestInterceptor_var server_interceptor =
    interceptor;

  info->add_server_request_interceptor (interceptor
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
