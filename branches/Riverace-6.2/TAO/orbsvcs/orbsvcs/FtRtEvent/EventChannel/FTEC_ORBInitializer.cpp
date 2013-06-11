// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/FTEC_ORBInitializer.h"
#include "orbsvcs/FtRtEvent/EventChannel/Set_Update_Interceptor.h"
#include "orbsvcs/FtRtEvent/EventChannel/ForwardCtrlServerInterceptor.h"
#include "orbsvcs/FtRtEvent/EventChannel/FtEventServiceInterceptor.h"
#include "orbsvcs/FtRtEvent/EventChannel/Request_Context_Repository.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
FTEC_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

void
FTEC_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{

  Request_Context_Repository().allocate_slots(info);

  PortableInterceptor::ClientRequestInterceptor_var client_interceptor;
  PortableInterceptor::ClientRequestInterceptor_ptr ctmp;

  ACE_NEW_THROW_EX(ctmp,
                   TAO_Set_Update_Interceptor,
                   CORBA::NO_MEMORY());

  client_interceptor = ctmp;

  info->add_client_request_interceptor (client_interceptor.in());

  PortableInterceptor::ServerRequestInterceptor_var  server_interceptor;
  PortableInterceptor::ServerRequestInterceptor_ptr  stmp;

  ACE_NEW_THROW_EX(stmp,
                ForwardCtrlServerInterceptor,
                CORBA::NO_MEMORY());
  server_interceptor = stmp;

  info->add_server_request_interceptor (server_interceptor.in());
  ACE_NEW_THROW_EX(stmp,
                FtEventServiceInterceptor,
                CORBA::NO_MEMORY());
  server_interceptor = stmp;

  info->add_server_request_interceptor (server_interceptor.in());
}

TAO_END_VERSIONED_NAMESPACE_DECL
