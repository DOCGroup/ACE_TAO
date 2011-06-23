// $Id$

#include "orbsvcs/FaultTolerance/FT_ClientORBInitializer.h"
#include "orbsvcs/FaultTolerance/FT_ClientPolicyFactory.h"
#include "orbsvcs/FaultTolerance/FT_ClientRequest_Interceptor.h"
#include "orbsvcs/FaultTolerance/FT_Endpoint_Selector_Factory.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "tao/PI/ORBInitInfo.h"
#include "tao/ORB_Core.h"
#include "tao/Exception.h"
#include "tao/ORB_Constants.h"
#include "tao/ORB_Core.h"
#include "tao/PI/ORBInitInfo.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_FT_ClientORBInitializer::pre_init (PortableInterceptor::ORBInitInfo_ptr info)
{
  TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info);

  // Set the name of the endpoint selector factory
  tao_info->orb_core ()->orb_params ()->endpoint_selector_factory_name ("FT_Endpoint_Selector_Factory");
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_FT_Endpoint_Selector_Factory);
}

void
TAO_FT_ClientORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  this->register_policy_factories (info);

  this->register_client_request_interceptors (info);
}

void
TAO_FT_ClientORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info)
{
  // Register the FTCORBA policy factories.

  PortableInterceptor::PolicyFactory_ptr temp_factory =
    PortableInterceptor::PolicyFactory::_nil ();
  PortableInterceptor::PolicyFactory_var policy_factory;

  // This policy factory is used for all FTCORBA related policies.

  ACE_NEW_THROW_EX (temp_factory,
                    TAO_FT_ClientPolicyFactory,
                    CORBA::NO_MEMORY (
                                      CORBA::SystemException::_tao_minor_code (
                         TAO::VMCID,
                         ENOMEM),
                      CORBA::COMPLETED_NO));

  policy_factory = temp_factory;

  // Bind the same policy factory to all RTCORBA related policy
  // types since a single policy factory is used to create each of
  // the different types of RTCORBA policies.

  CORBA::PolicyType type = FT::REQUEST_DURATION_POLICY;
  info->register_policy_factory (type, policy_factory.in ());

  type = FT::HEARTBEAT_POLICY;
  info->register_policy_factory (type, policy_factory.in ());

  /*
  type = FT::HEARTBEAT_ENABLED_POLICY;
  info->register_policy_factory (type,
                                 policy_factory.in ());
  */
}


void
TAO_FT_ClientORBInitializer::register_client_request_interceptors (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  TAO::FT_ClientRequest_Interceptor* ftcri = 0;

  ACE_NEW_THROW_EX (ftcri,
                    TAO::FT_ClientRequest_Interceptor,
                    CORBA::NO_MEMORY ());

  PortableInterceptor::ClientRequestInterceptor_var
    client_interceptor = ftcri;

  TAO_ORBInitInfo* real_info = dynamic_cast<TAO_ORBInitInfo*> (info);

  if (real_info)
    {
      ftcri->ft_send_extended_sc (real_info->orb_core ()->ft_send_extended_sc ());
    }

  info->add_client_request_interceptor (client_interceptor.in ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
