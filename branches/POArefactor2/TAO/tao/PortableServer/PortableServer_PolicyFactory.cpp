
#include "PortableServer_PolicyFactory.h"

#include "ThreadPolicyFactory.h"
#include "LifespanPolicyFactory.h"
#include "IdAssignmentPolicyFactory.h"
#include "IdUniquenessPolicyFactory.h"
#include "ImplicitActivationPolicyFactory.h"
#include "RequestProcessingPolicyFactory.h"
#include "ServantRetentionPolicyFactory.h"
#include "PortableServer.h"

#include "Policy_Creator_T.h"

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
            CORBA::Policy_ptr,
            PortableServer::ThreadPolicyValue>::create (
              "ThreadPolicyFactory",
              ACE_TEXT("dynamic ThreadPolicyFactory Service_Object *")
              ACE_TEXT("TAO_PortableServer:_make_ThreadPolicyFactoryImpl()"),
              value ACE_ENV_ARG_PARAMETER);

#endif /* TAO_HAS_MINIMUM_POA == 0 */
  if (type == PortableServer::LIFESPAN_POLICY_ID)
    return TAO::Portable_Server::Policy_Creator<
            TAO::Portable_Server::LifespanPolicyFactory,
            CORBA::Policy_ptr,
            PortableServer::LifespanPolicyValue>::create (
              "LifespanPolicyFactory",
              ACE_TEXT("dynamic LifespanPolicyFactory Service_Object *")
              ACE_TEXT("TAO_PortableServer:_make_LifespanPolicyFactoryImpl()"),
              value ACE_ENV_ARG_PARAMETER);

  if (type == PortableServer::ID_UNIQUENESS_POLICY_ID)
    return TAO::Portable_Server::Policy_Creator<
            TAO::Portable_Server::IdUniquenessPolicyFactory,
            CORBA::Policy_ptr,
            PortableServer::IdUniquenessPolicyValue>::create (
              "IdUniquenessPolicyFactory",
              ACE_TEXT("dynamic IdUniquenessPolicyFactory Service_Object *")
              ACE_TEXT("TAO_PortableServer:_make_IdUniquenessPolicyFactoryImpl()"),
              value ACE_ENV_ARG_PARAMETER);

  if (type == PortableServer::ID_ASSIGNMENT_POLICY_ID)
    return TAO::Portable_Server::Policy_Creator<
            TAO::Portable_Server::IdAssignmentPolicyFactory,
            CORBA::Policy_ptr,
            PortableServer::IdAssignmentPolicyValue>::create (
              "IdAssignmentPolicyFactory",
              ACE_TEXT("dynamic IdAssignmentPolicyFactory Service_Object *")
              ACE_TEXT("TAO_PortableServer:_make_IdAssignmentPolicyFactoryImpl()"),
              value ACE_ENV_ARG_PARAMETER);

#if (TAO_HAS_MINIMUM_POA == 0)

  if (type == PortableServer::IMPLICIT_ACTIVATION_POLICY_ID)
    return TAO::Portable_Server::Policy_Creator<
            TAO::Portable_Server::ImplicitActivationPolicyFactory,
            CORBA::Policy_ptr,
            PortableServer::ImplicitActivationPolicyValue>::create (
              "ImplicitActivationPolicyFactory",
              ACE_TEXT("dynamic ImplicitActivationPolicyFactory Service_Object *")
              ACE_TEXT("TAO_PortableServer:_make_ImplicitActivationPolicyFactoryImpl()"),
              value ACE_ENV_ARG_PARAMETER);

  if (type == PortableServer::SERVANT_RETENTION_POLICY_ID)
    return TAO::Portable_Server::Policy_Creator<
            TAO::Portable_Server::ServantRetentionPolicyFactory,
            CORBA::Policy_ptr,
            PortableServer::ServantRetentionPolicyValue>::create (
              "ServantRetentionPolicyFactory",
              ACE_TEXT("dynamic ServantRetentionPolicyFactory Service_Object *")
              ACE_TEXT("TAO_PortableServer:_make_ServantRetentionPolicyFactoryImpl()"),
              value ACE_ENV_ARG_PARAMETER);

  if (type == PortableServer::REQUEST_PROCESSING_POLICY_ID)
    return TAO::Portable_Server::Policy_Creator<
            TAO::Portable_Server::RequestProcessingPolicyFactory,
            CORBA::Policy_ptr,
            PortableServer::RequestProcessingPolicyValue>::create (
              "RequestProcessingPolicyFactory",
              ACE_TEXT("dynamic RequestProcessingPolicyFactory Service_Object *")
              ACE_TEXT("TAO_PortableServer:_make_RequestProcessingPolicyFactoryImpl()"),
              value ACE_ENV_ARG_PARAMETER);

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                    CORBA::Policy::_nil ());
}
