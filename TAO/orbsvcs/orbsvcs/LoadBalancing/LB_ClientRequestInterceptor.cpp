#include "orbsvcs/LoadBalancing/LB_ClientRequestInterceptor.h"
#include "orbsvcs/CosLoadBalancingC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

char *
TAO_LB_ClientRequestInterceptor::name ()
{
  return CORBA::string_dup ("TAO_LB_ClientRequestInterceptor");
}

void
TAO_LB_ClientRequestInterceptor::destroy ()
{
}

void
TAO_LB_ClientRequestInterceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  // A ServiceContext with an empty payload (i.e. zero length
  // context_data) will be embedded into the request.
  IOP::ServiceContext service_context;
  service_context.context_id = CosLoadBalancing::LOAD_MANAGED;

  CORBA::Boolean const replace {};
  ri->add_request_service_context (service_context, replace);
}

void
TAO_LB_ClientRequestInterceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
TAO_LB_ClientRequestInterceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
TAO_LB_ClientRequestInterceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
TAO_LB_ClientRequestInterceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
