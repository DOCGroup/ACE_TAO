
#include "PortableServer_PolicyFactory.h"
#include "IdAssignmentPolicyC.h"
#include "IdUniquenessPolicyC.h"
#include "ImplicitActivationPolicyC.h"
#include "LifespanPolicyC.h"
#include "RequestProcessingPolicyC.h"
#include "ServantRetentionPolicyC.h"
#include "ThreadPolicyC.h"
#include "Policy_Creator_T.h"

#include "ThreadPolicyFactory.h"
#include "LifespanPolicyFactory.h"
#include "IdAssignmentPolicyFactory.h"
#include "IdUniquenessPolicyFactory.h"
#include "ImplicitActivationPolicyFactory.h"
#include "RequestProcessingPolicyFactory.h"
#include "ServantRetentionPolicyFactory.h"

ACE_RCSID (PortableServer,
           PortableServer_PolicyFactory,
           "$Id$")

CORBA::Policy_ptr
TAO_PortableServer_PolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &value
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CORBA::PolicyError))
{
#if (TAO_HAS_MINIMUM_POA == 0)

  if (type == ::PortableServer::THREAD_POLICY_ID)
    return TAO::Portable_Server::Policy_Creator<
            TAO::Portable_Server::ThreadPolicyFactory,
            CORBA::Policy,
            PortableServer::ThreadPolicyValue>::create (
              "ThreadPolicyFactory",
              TAO::Portable_Server::ace_svc_desc_ThreadPolicyFactory,
              value ACE_ENV_ARG_PARAMETER);

#endif /* TAO_HAS_MINIMUM_POA == 0 */
  if (type == PortableServer::LIFESPAN_POLICY_ID)
    return TAO::Portable_Server::Policy_Creator<
            TAO::Portable_Server::LifespanPolicyFactory,
            CORBA::Policy,
            PortableServer::LifespanPolicyValue>::create (
              "LifespanPolicyFactory",
              TAO::Portable_Server::ace_svc_desc_LifespanPolicyFactory,
              value ACE_ENV_ARG_PARAMETER);

  if (type == PortableServer::ID_UNIQUENESS_POLICY_ID)
    return TAO::Portable_Server::Policy_Creator<
            TAO::Portable_Server::IdUniquenessPolicyFactory,
            CORBA::Policy,
            PortableServer::IdUniquenessPolicyValue>::create (
              "IdUniquenessPolicyFactory",
              TAO::Portable_Server::ace_svc_desc_IdUniquenessPolicyFactory,
              value ACE_ENV_ARG_PARAMETER);

  if (type == PortableServer::ID_ASSIGNMENT_POLICY_ID)
    return TAO::Portable_Server::Policy_Creator<
            TAO::Portable_Server::IdAssignmentPolicyFactory,
            CORBA::Policy,
            PortableServer::IdAssignmentPolicyValue>::create (
              "IdAssignmentPolicyFactory",
              TAO::Portable_Server::ace_svc_desc_IdAssignmentPolicyFactory,
              value ACE_ENV_ARG_PARAMETER);

#if (TAO_HAS_MINIMUM_POA == 0)

  if (type == PortableServer::IMPLICIT_ACTIVATION_POLICY_ID)
    return TAO::Portable_Server::Policy_Creator<
            TAO::Portable_Server::ImplicitActivationPolicyFactory,
            CORBA::Policy,
            PortableServer::ImplicitActivationPolicyValue>::create (
              "ImplicitActivationPolicyFactory",
              TAO::Portable_Server::ace_svc_desc_ImplicitActivationPolicyFactory,
              value ACE_ENV_ARG_PARAMETER);

  if (type == PortableServer::SERVANT_RETENTION_POLICY_ID)
    return TAO::Portable_Server::Policy_Creator<
            TAO::Portable_Server::ServantRetentionPolicyFactory,
            CORBA::Policy,
            PortableServer::ServantRetentionPolicyValue>::create (
              "ServantRetentionPolicyFactory",
              TAO::Portable_Server::ace_svc_desc_ServantRetentionPolicyFactory,
              value ACE_ENV_ARG_PARAMETER);

  if (type == PortableServer::REQUEST_PROCESSING_POLICY_ID)
    return TAO::Portable_Server::Policy_Creator<
            TAO::Portable_Server::RequestProcessingPolicyFactory,
            CORBA::Policy,
            PortableServer::RequestProcessingPolicyValue>::create (
              "RequestProcessingPolicyFactory",
              TAO::Portable_Server::ace_svc_desc_RequestProcessingPolicyFactory,
              value ACE_ENV_ARG_PARAMETER);

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                    CORBA::Policy::_nil ());
}
