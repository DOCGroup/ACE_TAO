// $Id$

#include "ORT_test_IORInterceptor_ORBInitializer.h"

#include "ORT_test_IORInterceptor.h"

ACE_RCSID (ORT,
           ORT_test_IORInterceptor_ORBInitializer,
           "$Id: ")

void
ORT_test_IORInterceptor_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr /* info */
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
ORT_test_IORInterceptor_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableInterceptor::IORInterceptor_ptr ort_test_interceptor;
  ACE_NEW_THROW_EX (ort_test_interceptor,
                    ORT_test_IORInterceptor (),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::IORInterceptor_var ior_interceptor =
    ort_test_interceptor;

  info->add_ior_interceptor (ior_interceptor.in ()
                             TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

}
