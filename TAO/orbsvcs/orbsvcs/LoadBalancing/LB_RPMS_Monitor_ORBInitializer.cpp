// -*- C++ -*-

// -*- C++ -*-

#include "LB_RPMS_Monitor_ORBInitializer.h"

ACE_RCSID (LoadBalancing,
           LB_RPMS_Monitor_ORBInitializer,
           "$Id$")


void
LB_RPMS_Monitor_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
LB_RPMS_Monitor_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ENV_ARG_DEFN;

  PortableInterceptor::ServerRequestInterceptor_ptr interceptor;
  // Install the server request interceptor.
  ACE_NEW_THROW_EX (interceptor,
                    Server_Request_Interceptor,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  this->server_interceptor_ = interceptor;

  info->add_server_request_interceptor (interceptor,
                                        ACE_TRY_ENV);
  ACE_CHECK;
}
