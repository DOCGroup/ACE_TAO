// -*- C++ -*-

#include "FOO_IORInterceptor_ORBInitializer.h"
#include "FOO_IORInterceptor.h"

ACE_RCSID (IORInterceptor,
           FOO_IORInterceptor_ORBInitializer,
           "$Id$")

void
FOO_IORInterceptor_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr /* info */
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
FOO_IORInterceptor_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  PortableInterceptor::IORInterceptor_ptr foo;
  ACE_NEW_THROW_EX (foo,
                    FOO_IORInterceptor,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::IORInterceptor_var ior_interceptor =
    foo;

  info->add_ior_interceptor (ior_interceptor.in ()
                             TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
