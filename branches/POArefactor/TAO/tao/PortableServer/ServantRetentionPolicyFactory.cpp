// $Id$

#include "ServantRetentionPolicyFactory.h"
#include "ace/Dynamic_Service.h"
#include "ServantRetentionPolicy.h"

ACE_RCSID (PortableServer,
           ServantRetentionPolicyFactory,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ServantRetentionPolicyFactory::~ServantRetentionPolicyFactory (void)
    {
    }

    ::PortableServer::ServantRetentionPolicy_ptr
    ServantRetentionPolicyFactory::create (
      ::PortableServer::ServantRetentionPolicyValue value)
    {
      ServantRetentionPolicy* policy = 0;

      ACE_NEW_RETURN (policy,
                      ServantRetentionPolicy,
                      ::PortableServer::ServantRetentionPolicy::_nil ());

      (void) policy->init (value);

      return policy;
    }

    ::PortableServer::ServantRetentionPolicy_ptr
    ServantRetentionPolicyFactory::create (
      const CORBA::Any &value
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::PolicyError))
    {
      ServantRetentionPolicy* policy = 0;

      ACE_NEW_RETURN (policy,
                      ServantRetentionPolicy,
                      ::PortableServer::ServantRetentionPolicy::_nil ());

      (void) policy->init (value ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (::PortableServer::ServantRetentionPolicy::_nil ());

      return policy;
    }

    ACE_STATIC_SVC_DEFINE (
        ServantRetentionPolicyFactory,
        ACE_TEXT ("ServantRetentionPolicyFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ServantRetentionPolicyFactory),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, ServantRetentionPolicyFactory)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<ServantRetentionPolicyFactory>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<ServantRetentionPolicyFactory>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

