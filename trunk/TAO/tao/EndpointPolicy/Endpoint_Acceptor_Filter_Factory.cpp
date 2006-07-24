// @(#) $Id$

#include "tao/EndpointPolicy/Endpoint_Acceptor_Filter_Factory.h"
#include "tao/EndpointPolicy/Endpoint_Acceptor_Filter.h"
#include "tao/PortableServer/Default_Acceptor_Filter.h"
#include "tao/PortableServer/POAManager.h"
#include "tao/EndpointPolicy/EndpointPolicyC.h"

ACE_RCSID(EndpointPolicy,
          Endpoint_Acceptor_Filter_Factory,
          "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL


TAO_Endpoint_Acceptor_Filter_Factory::~TAO_Endpoint_Acceptor_Filter_Factory(void)
{
}

 TAO_Acceptor_Filter*
 TAO_Endpoint_Acceptor_Filter_Factory::create_object (TAO_POA_Manager& poamanager)
{
  CORBA::PolicyList& policies = poamanager.get_policies ();
  EndpointPolicy::Policy_var policy;
  TAO_Acceptor_Filter* filter = 0;
  for (CORBA::ULong i = 0; i < policies.length(); i++)
    {
      if (policies[i]->policy_type() == EndpointPolicy::ENDPOINT_POLICY_TYPE)
        {
          policy = EndpointPolicy::Policy::_narrow (policies[i]);
          ACE_NEW_RETURN (filter,
                          TAO_Endpoint_Acceptor_Filter (policy.in()),
                          0);
          return filter;
        }
    }
  // no policy was in force, just return a default filter
  ACE_NEW_RETURN (filter,
                  TAO_Default_Acceptor_Filter (),
                  0);

  return filter;
}


TAO_END_VERSIONED_NAMESPACE_DECL

ACE_FACTORY_DEFINE (TAO_EndpointPolicy, TAO_Endpoint_Acceptor_Filter_Factory)
ACE_STATIC_SVC_DEFINE (TAO_Endpoint_Acceptor_Filter_Factory,
                       ACE_TEXT ("TAO_Acceptor_Filter_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Endpoint_Acceptor_Filter_Factory),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
