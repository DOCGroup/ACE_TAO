// -*- C++ -*-
// $Id$

#include "tao/EndpointPolicy/Endpoint_Acceptor_Filter_Factory.h"
#include "tao/EndpointPolicy/Endpoint_Acceptor_Filter.h"
#include "tao/PortableServer/Default_Acceptor_Filter.h"
#include "tao/PortableServer/POAManager.h"
#include "tao/EndpointPolicy/EndpointPolicyC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL


TAO_Endpoint_Acceptor_Filter_Factory::~TAO_Endpoint_Acceptor_Filter_Factory(void)
{
}

 TAO_Acceptor_Filter*
 TAO_Endpoint_Acceptor_Filter_Factory::create_object (TAO_POA_Manager& poamanager)
{
  CORBA::PolicyList& policies = poamanager.get_policies ();

  EndpointPolicy::EndpointList endpoints;
  TAO_Acceptor_Filter* filter = 0;

  for (CORBA::ULong i = 0; i < policies.length(); ++i)
    {
      if (policies[i]->policy_type() == EndpointPolicy::ENDPOINT_POLICY_TYPE)
        {
          EndpointPolicy::Policy_var cur_policy = EndpointPolicy::Policy::_narrow (policies[i]);
          if (CORBA::is_nil (cur_policy.in ()))
            return 0;
          
          EndpointPolicy::EndpointList_var cur_endpoints = cur_policy->value ();
          CORBA::ULong cur_num_endpoints = cur_endpoints->length ();

          CORBA::ULong num_endpoints = endpoints.length ();
          endpoints.length (num_endpoints + cur_num_endpoints);

          for (CORBA::ULong ii = 0; ii < cur_num_endpoints; ++ii)
            {
              endpoints[num_endpoints + ii] = cur_endpoints[ii];
            }
        }
    }

  if (endpoints.length () > 0)
    ACE_NEW_RETURN (filter,
                    TAO_Endpoint_Acceptor_Filter (endpoints),
                    0);
  else
    // no policy was in force, just return a default filter
    ACE_NEW_RETURN (filter,
                    TAO_Default_Acceptor_Filter (),
                    0);

  return filter;
}



ACE_FACTORY_DEFINE (TAO_EndpointPolicy, TAO_Endpoint_Acceptor_Filter_Factory)
ACE_STATIC_SVC_DEFINE (TAO_Endpoint_Acceptor_Filter_Factory,
                       ACE_TEXT ("TAO_Acceptor_Filter_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Endpoint_Acceptor_Filter_Factory),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
TAO_END_VERSIONED_NAMESPACE_DECL
