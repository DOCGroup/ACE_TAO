// $Id$

#include "CSS_ORBInitializer.h"
#include "CSS_RequestInterceptor.h"


ACE_RCSID (CSIv2,
           CSS_ORBInitializer,
           "$Id$")


void
TAO::CSS_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr /* info */
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO::CSS_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableInterceptor::ClientRequestInterceptor_ptr i;
  ACE_NEW_THROW_EX (i,
                    TAO::CSS_RequestInterceptor,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::ClientRequestInterceptor_var interceptor = i;

  info->add_client_request_interceptor (interceptor.in ()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
