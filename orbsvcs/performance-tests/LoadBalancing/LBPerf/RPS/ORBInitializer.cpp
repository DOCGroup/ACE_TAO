// $Id$

#include "ORBInitializer.h"
#include "ServerRequestInterceptor.h"

#include "orbsvcs/LoadBalancing/LB_ServerRequestInterceptor.h"
#include "orbsvcs/CosLoadBalancingC.h"

#include "tao/ORB_Constants.h"

ORBInitializer::ORBInitializer (void)
  : load_alert_ (),
    interceptor_ (0)
{
}

void
ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

void
ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  ACE_NEW_THROW_EX (this->interceptor_,
                    ServerRequestInterceptor,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::ServerRequestInterceptor_var sr_interceptor =
    this->interceptor_;

  info->add_server_request_interceptor (sr_interceptor.in ());

  PortableInterceptor::ServerRequestInterceptor_ptr reject_interceptor;
  ACE_NEW_THROW_EX (reject_interceptor,
                    TAO_LB_ServerRequestInterceptor (this->load_alert_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::ServerRequestInterceptor_var safe_reject_interceptor =
    reject_interceptor;

  info->add_server_request_interceptor (safe_reject_interceptor.in ());
}


TAO_LB_LoadAlert &
ORBInitializer::load_alert (void)
{
  return this->load_alert_;
}

ServerRequestInterceptor *
ORBInitializer::interceptor (void) const
{
  return this->interceptor_;
}
