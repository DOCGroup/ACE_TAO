// $Id$

#include "RequestProcessingPolicyFactoryImpl.h"
#include "ace/Dynamic_Service.h"
#include "RequestProcessingPolicy.h"

ACE_RCSID (PortableServer,
           RequestProcessingPolicyFactoryImpl,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    RequestProcessingPolicyFactoryImpl::~RequestProcessingPolicyFactoryImpl (void)
    {
    }

    ::PortableServer::RequestProcessingPolicy_ptr
    RequestProcessingPolicyFactoryImpl::create (
      ::PortableServer::RequestProcessingPolicyValue value
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
    {
      RequestProcessingPolicy* policy = 0;

      ACE_NEW_THROW_EX (policy,
                        RequestProcessingPolicy,
                        CORBA::NO_MEMORY ());

      ACE_CHECK_RETURN (::PortableServer::RequestProcessingPolicy::_nil ());

      (void) policy->init (value);

      return policy;
    }

    ::PortableServer::RequestProcessingPolicy_ptr
    RequestProcessingPolicyFactoryImpl::create (
      const CORBA::Any &value
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         CORBA::PolicyError))
    {
      RequestProcessingPolicy* policy = 0;

      ACE_NEW_THROW_EX (policy,
                        RequestProcessingPolicy,
                        CORBA::NO_MEMORY ());

      ACE_CHECK_RETURN (::PortableServer::RequestProcessingPolicy::_nil ());

      (void) policy->init (value ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (::PortableServer::RequestProcessingPolicy::_nil ());

      return policy;
    }

    ACE_STATIC_SVC_DEFINE (
        RequestProcessingPolicyFactoryImpl,
        ACE_TEXT ("RequestProcessingPolicyFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (RequestProcessingPolicyFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, RequestProcessingPolicyFactoryImpl)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<RequestProcessingPolicyFactoryImpl>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<RequestProcessingPolicyFactoryImpl>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

