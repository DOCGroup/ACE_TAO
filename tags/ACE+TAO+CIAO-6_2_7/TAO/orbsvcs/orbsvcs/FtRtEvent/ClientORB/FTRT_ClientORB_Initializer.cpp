// $Id$

#include "orbsvcs/FtRtEvent/ClientORB/FTRT_ClientORB_Initializer.h"
#include "orbsvcs/FtRtEvent/ClientORB/FTRT_ClientORB_Interceptor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

FTRT_ClientORB_Initializer::FTRT_ClientORB_Initializer (int transaction_depth)
  : client_interceptor_ ( new FTRT_ClientORB_Interceptor(transaction_depth) )
{
}

void
FTRT_ClientORB_Initializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

void
FTRT_ClientORB_Initializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  info->add_client_request_interceptor (client_interceptor_.in());
}

TAO_END_VERSIONED_NAMESPACE_DECL
