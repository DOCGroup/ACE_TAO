#include "orbsvcs/LoadBalancing/LB_ClientORBInitializer.h"
#include "orbsvcs/LoadBalancing/LB_ClientRequestInterceptor.h"

#include "tao/ORB_Constants.h"

ACE_RCSID (LoadBalancing,
           LB_ClientORBInitializer,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_LB_ClientORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_LB_ClientORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableInterceptor::ClientRequestInterceptor_ptr tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_LB_ClientRequestInterceptor,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::ClientRequestInterceptor_var client_interceptor = tmp;

  info->add_client_request_interceptor (client_interceptor.in ()
                                        ACE_ENV_ARG_PARAMETER);
}

TAO_END_VERSIONED_NAMESPACE_DECL
