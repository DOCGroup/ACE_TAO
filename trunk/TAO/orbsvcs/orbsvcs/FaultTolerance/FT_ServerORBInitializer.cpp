// $Id$

#include "orbsvcs/FaultTolerance/FT_ServerORBInitializer.h"
#include "orbsvcs/FaultTolerance/FT_ServerPolicyFactory.h"
#include "orbsvcs/FaultTolerance/FT_ServerRequest_Interceptor.h"
#include "tao/ORB_Constants.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/Exception.h"


ACE_RCSID (FaultTolerance,
           FT_ServerORBInitializer,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_FT_ServerORBInitializer::pre_init (PortableInterceptor::ORBInitInfo_ptr)
{
}

void
TAO_FT_ServerORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  this->register_policy_factories (info);

  this->register_server_request_interceptors (info);
}

void
TAO_FT_ServerORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info)
{
  // Register the FTCORBA policy factories.

  PortableInterceptor::PolicyFactory_ptr temp_factory =
    PortableInterceptor::PolicyFactory::_nil ();
  PortableInterceptor::PolicyFactory_var policy_factory;

  // This policy factory is used for all FTCORBA related policies.

  ACE_NEW_THROW_EX (temp_factory,
                    TAO_FT_ServerPolicyFactory,
                    CORBA::NO_MEMORY (
                                      CORBA::SystemException::_tao_minor_code (
                         TAO::VMCID,
                         ENOMEM),
                      CORBA::COMPLETED_NO));

  policy_factory = temp_factory;

  // Bind the same policy factory to all FTCORBA related policy
  // types since a single policy factory is used to create each of
  // the different types of FTCORBA policies.


  CORBA::PolicyType type = FT::HEARTBEAT_ENABLED_POLICY;
  info->register_policy_factory (type,
                                 policy_factory.in ());
}


void
TAO_FT_ServerORBInitializer::register_server_request_interceptors (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  PortableInterceptor::ServerRequestInterceptor_ptr sri =
    PortableInterceptor::ServerRequestInterceptor::_nil ();

  ACE_NEW_THROW_EX (sri,
                    TAO::FT_ServerRequest_Interceptor,
                    CORBA::NO_MEMORY ());

  PortableInterceptor::ServerRequestInterceptor_var
    server_interceptor = sri;

  info->add_server_request_interceptor (server_interceptor.in ());
}


TAO_END_VERSIONED_NAMESPACE_DECL
