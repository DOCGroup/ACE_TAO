// -*- C++ -*-
// $Id$

#include "tao/PI_Server/PortableServer_PolicyFactory.h"
#include "tao/PortableServer/ThreadPolicy.h"
#include "tao/PortableServer/LifespanPolicy.h"
#include "tao/PortableServer/IdAssignmentPolicy.h"
#include "tao/PortableServer/IdUniquenessPolicy.h"
#include "tao/PortableServer/ImplicitActivationPolicy.h"
#include "tao/PortableServer/RequestProcessingPolicy.h"
#include "tao/PortableServer/ServantRetentionPolicy.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PI_Server/Policy_Creator_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CORBA::Policy_ptr
TAO_PortableServer_PolicyFactory::create_policy (
    CORBA::PolicyType type,
    const CORBA::Any &value)
{
#if !defined (CORBA_E_MICRO)
#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT)

  if (type == ::PortableServer::THREAD_POLICY_ID)
    {
      TAO::Portable_Server::ThreadPolicy *thread_policy = 0;
      PortableServer::ThreadPolicyValue thr_value;

      TAO::Portable_Server::create_policy (thread_policy, thr_value, value);

      return thread_policy;
    }

#endif /* TAO_HAS_MINIMUM_POA == 0 */
  if (type == PortableServer::LIFESPAN_POLICY_ID)
    {
      TAO::Portable_Server::LifespanPolicy *lifespan_policy = 0;
      PortableServer::LifespanPolicyValue lifespan_value;

      TAO::Portable_Server::create_policy (lifespan_policy, lifespan_value, value);

      return lifespan_policy;
    }

  if (type == PortableServer::ID_UNIQUENESS_POLICY_ID)
    {
      TAO::Portable_Server::IdUniquenessPolicy *id_uniqueness_policy = 0;
      PortableServer::IdUniquenessPolicyValue id_uniqueness_value;

      TAO::Portable_Server::create_policy (id_uniqueness_policy, id_uniqueness_value, value);

      return id_uniqueness_policy;
    }

  if (type == PortableServer::ID_ASSIGNMENT_POLICY_ID)
    {
      TAO::Portable_Server::IdAssignmentPolicy *id_assignment_policy = 0;
      PortableServer::IdAssignmentPolicyValue id_assignment_value;

      TAO::Portable_Server::create_policy (id_assignment_policy, id_assignment_value, value);

      return id_assignment_policy;
    }

#if (TAO_HAS_MINIMUM_POA == 0) && !defined (CORBA_E_COMPACT)

  if (type == PortableServer::IMPLICIT_ACTIVATION_POLICY_ID)
    {
      TAO::Portable_Server::ImplicitActivationPolicy *implicit_activatation_policy = 0;
      PortableServer::ImplicitActivationPolicyValue implicit_activation_value;

      TAO::Portable_Server::create_policy (implicit_activatation_policy, implicit_activation_value, value);

      return implicit_activatation_policy;
    }

  if (type == PortableServer::SERVANT_RETENTION_POLICY_ID)
    {
      TAO::Portable_Server::ServantRetentionPolicy *servant_retention_policy = 0;
      PortableServer::ServantRetentionPolicyValue servant_retention_value;

      TAO::Portable_Server::create_policy (servant_retention_policy, servant_retention_value, value);

      return servant_retention_policy;
    }

  if (type == PortableServer::REQUEST_PROCESSING_POLICY_ID)
    {
      TAO::Portable_Server::RequestProcessingPolicy *request_processing_policy = 0;
      PortableServer::RequestProcessingPolicyValue request_processing_value;

      TAO::Portable_Server::create_policy (request_processing_policy, request_processing_value, value);

      return request_processing_policy;
    }

#endif /* TAO_HAS_MINIMUM_POA == 0 */

  throw ::CORBA::PolicyError (CORBA::BAD_POLICY_TYPE);
#else
  ACE_UNUSED_ARG (value);
  ACE_UNUSED_ARG (type);

  throw ::CORBA::PolicyError (CORBA::BAD_POLICY_TYPE);
#endif
}

TAO_END_VERSIONED_NAMESPACE_DECL
