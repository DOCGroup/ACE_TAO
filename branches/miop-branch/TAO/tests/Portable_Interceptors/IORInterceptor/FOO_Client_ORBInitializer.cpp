// -*- C++ -*-

#include "FOO_Client_ORBInitializer.h"
#include "FOO_ClientRequestInterceptor.h"

ACE_RCSID (IORInterceptor,
           FOO_Client_ORBInitializer,
           "$Id$")

void
FOO_Client_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr /* info */
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
FOO_Client_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableInterceptor::ClientRequestInterceptor_ptr foo;
  ACE_NEW_THROW_EX (foo,
                    FOO_ClientRequestInterceptor,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::ClientRequestInterceptor_var interceptor =
    foo;

  info->add_client_request_interceptor (interceptor.in ()
                                        TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
