// $Id$

#include "ServantRetentionPolicyFactoryImpl.h"
#include "ace/Dynamic_Service.h"
#include "ServantRetentionPolicy.h"

ACE_RCSID (PortableServer,
           ServantRetentionPolicyFactoryImpl,
           "$Id$")

#if (TAO_HAS_MINIMUM_POA == 0)

namespace TAO
{
  namespace Portable_Server
  {
    ServantRetentionPolicyFactoryImpl::~ServantRetentionPolicyFactoryImpl (void)
    {
    }

    ::PortableServer::ServantRetentionPolicy_ptr
    ServantRetentionPolicyFactoryImpl::create (
      ::PortableServer::ServantRetentionPolicyValue value
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException))
    {
      ServantRetentionPolicy* policy = 0;

      ACE_NEW_THROW_EX (policy,
                        ServantRetentionPolicy,
                        CORBA::NO_MEMORY ());

      ACE_CHECK_RETURN (::PortableServer::ServantRetentionPolicy::_nil ());

      (void) policy->init (value);

      return policy;
    }

    ::PortableServer::ServantRetentionPolicy_ptr
    ServantRetentionPolicyFactoryImpl::create (
      const CORBA::Any &value
      ACE_ENV_ARG_DECL)
        ACE_THROW_SPEC ((CORBA::SystemException,
                         CORBA::PolicyError))
    {
      ServantRetentionPolicy* policy = 0;

      ACE_NEW_THROW_EX (policy,
                        ServantRetentionPolicy,
                        CORBA::NO_MEMORY ());

      ACE_CHECK_RETURN (::PortableServer::ServantRetentionPolicy::_nil ());

      (void) policy->init (value ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (::PortableServer::ServantRetentionPolicy::_nil ());

      return policy;
    }

    ACE_STATIC_SVC_DEFINE (
        ServantRetentionPolicyFactoryImpl,
        ACE_TEXT ("ServantRetentionPolicyFactory"),
        ACE_SVC_OBJ_T,
        &ACE_SVC_NAME (ServantRetentionPolicyFactoryImpl),
        ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
        0
      )

    ACE_FACTORY_DEFINE (TAO_PortableServer, ServantRetentionPolicyFactoryImpl)

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<ServantRetentionPolicyFactoryImpl>;

    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

    #pragma instantiate ACE_Dynamic_Service<ServantRetentionPolicyFactoryImpl>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

