// $Id$

#include "orbsvcs/LoadBalancing/LB_ClientORBInitializer.h"
#include "orbsvcs/LoadBalancing/LB_ClientRequestInterceptor.h"
#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_LB_ClientORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

void
TAO_LB_ClientORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
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

  info->add_client_request_interceptor (client_interceptor.in ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
