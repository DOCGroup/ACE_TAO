// $Id$

#include "RequestProcessingPolicyFactory.h"
#include "ace/Dynamic_Service.h"
#include "RequestProcessingPolicy.h"

ACE_RCSID (PortableServer,
           RequestProcessingPolicyFactory,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingPolicyFactory::~RequestProcessingPolicyFactory (void)
    {
    }

    ::PortableServer::RequestProcessingPolicy_ptr
    RequestProcessingPolicyFactory::create (
      ::PortableServer::RequestProcessingPolicyValue value)
    {
      RequestProcessingPolicy* policy = 0;

      ACE_NEW_RETURN (policy,
                      RequestProcessingPolicy,
                      ::PortableServer::RequestProcessingPolicy::_nil ());

      (void) policy->init (value);

      return policy;
    }

    ::PortableServer::RequestProcessingPolicy_ptr
    RequestProcessingPolicyFactory::create (
      const CORBA::Any &value
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::PolicyError))
    {
      RequestProcessingPolicy* policy = 0;

      ACE_NEW_RETURN (policy,
                      RequestProcessingPolicy,
                      ::PortableServer::RequestProcessingPolicy::_nil ());

      (void) policy->init (value ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (::PortableServer::RequestProcessingPolicy::_nil ());

      return policy;
    }

    ACE_STATIC_SVC_DEFINE (
        RequestProcessingPolicyFactory,
        ACE_TEXT ("RequestProcessingPolicyFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (RequestProcessingPolicyFactory),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, RequestProcessingPolicyFactory)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<RequestProcessingPolicyFactory>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<RequestProcessingPolicyFactory>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

