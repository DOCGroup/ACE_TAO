// -*- C++ -*-

#include "LB_RPMS_Monitor_ORBInitializer.h"

ACE_RCSID (LoadBalancing,
           LB_RPMS_Monitor_ORBInitializer,
           "$Id$")


TAO_LB_RPMS_Monitor_ORBInitializer::TAO_LB_RPMS_Monitor_ORBInitializer (
  PortableInterceptor::ServerRequestInterceptor_ptr interceptor)
  : interceptor_ (interceptor)
{
}

void
TAO_LB_RPMS_Monitor_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_LB_RPMS_Monitor_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ENV_ARG_DEFN;

  info->add_server_request_interceptor (this->interceptor_,
                                        ACE_TRY_ENV);
  ACE_CHECK;
}
