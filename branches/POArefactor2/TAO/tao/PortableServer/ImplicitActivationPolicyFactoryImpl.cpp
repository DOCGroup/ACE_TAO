// $Id$

#include "ImplicitActivationPolicyFactoryImpl.h"
#include "ace/Dynamic_Service.h"
#include "ImplicitActivationPolicy.h"

ACE_RCSID (PortableServer,
           ImplicitActivationPolicyFactoryImpl,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ImplicitActivationPolicyFactoryImpl::~ImplicitActivationPolicyFactoryImpl (void)
    {
    }

    ::PortableServer::ImplicitActivationPolicy_ptr
    ImplicitActivationPolicyFactoryImpl::create (
      ::PortableServer::ImplicitActivationPolicyValue value
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
    {
      ImplicitActivationPolicy* policy = 0;

      ACE_NEW_THROW_EX (policy,
                        ImplicitActivationPolicy,
                        CORBA::NO_MEMORY ());

      ACE_CHECK_RETURN (::PortableServer::ImplicitActivationPolicy::_nil ());

      (void) policy->init (value);

      return policy;
    }

    ::PortableServer::ImplicitActivationPolicy_ptr
    ImplicitActivationPolicyFactoryImpl::create (
      const CORBA::Any &value
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         CORBA::PolicyError))
    {
      ImplicitActivationPolicy* policy = 0;

      ACE_NEW_THROW_EX (policy,
                        ImplicitActivationPolicy,
                        CORBA::NO_MEMORY ());

      ACE_CHECK_RETURN (::PortableServer::ImplicitActivationPolicy::_nil ());

      (void) policy->init (value ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (::PortableServer::ImplicitActivationPolicy::_nil ());

      return policy;
    }

    ACE_STATIC_SVC_DEFINE (
        ImplicitActivationPolicyFactoryImpl,
        ACE_TEXT ("ImplicitActivationPolicyFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ImplicitActivationPolicyFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, ImplicitActivationPolicyFactoryImpl)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<ImplicitActivationPolicyFactoryImpl>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<ImplicitActivationPolicyFactoryImpl>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */
