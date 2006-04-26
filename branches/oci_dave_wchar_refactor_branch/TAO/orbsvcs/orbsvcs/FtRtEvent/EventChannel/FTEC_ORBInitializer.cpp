// $Id$


#include "orbsvcs/FtRtEvent/EventChannel/FTEC_ORBInitializer.h"
#include "orbsvcs/FtRtEvent/EventChannel/Set_Update_Interceptor.h"
#include "orbsvcs/FtRtEvent/EventChannel/ForwardCtrlServerInterceptor.h"
#include "orbsvcs/FtRtEvent/EventChannel/FtEventServiceInterceptor.h"
#include "orbsvcs/FtRtEvent/EventChannel/Request_Context_Repository.h"

ACE_RCSID (EventChannel,
           FTEC_ORBInitializer,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
FTEC_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
FTEC_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  Request_Context_Repository().allocate_slots(info ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableInterceptor::ClientRequestInterceptor_var client_interceptor;
  PortableInterceptor::ClientRequestInterceptor_ptr ctmp;

  ACE_NEW_THROW_EX(ctmp,
                   TAO_Set_Update_Interceptor,
                   CORBA::NO_MEMORY());
  ACE_CHECK;

  client_interceptor = ctmp;

  info->add_client_request_interceptor (client_interceptor.in()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableInterceptor::ServerRequestInterceptor_var  server_interceptor;
  PortableInterceptor::ServerRequestInterceptor_ptr  stmp;

  ACE_NEW_THROW_EX(stmp,
                ForwardCtrlServerInterceptor,
                CORBA::NO_MEMORY());
  server_interceptor = stmp;

  info->add_server_request_interceptor (server_interceptor.in()
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  ACE_NEW_THROW_EX(stmp,
                FtEventServiceInterceptor,
                CORBA::NO_MEMORY());
  server_interceptor = stmp;

  info->add_server_request_interceptor (server_interceptor.in()
                                        ACE_ENV_ARG_PARAMETER);
}

TAO_END_VERSIONED_NAMESPACE_DECL
