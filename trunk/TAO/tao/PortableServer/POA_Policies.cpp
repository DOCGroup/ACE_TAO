// @(#) $Id$

#include "POA_Policies.h"

#if !defined (__ACE_INLINE__)
# include "POA_Policies.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(PortableServer, POA_Policies, "$Id$")


#if (TAO_HAS_MINIMUM_POA == 0)

TAO_Thread_Policy::TAO_Thread_Policy (PortableServer::ThreadPolicyValue value)
  : value_ (value)
{
}

PortableServer::ThreadPolicyValue
TAO_Thread_Policy::value (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->value_;
}

CORBA::Policy_ptr
TAO_Thread_Policy::copy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Thread_Policy *thread_policy_copy = 0;
  ACE_NEW_THROW_EX (thread_policy_copy,
                    TAO_Thread_Policy (this->value_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return thread_policy_copy;
}

void
TAO_Thread_Policy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

CORBA::PolicyType
TAO_Thread_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return PortableServer::THREAD_POLICY_ID;
}

TAO_Cached_Policy_Type
TAO_Thread_Policy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_THREAD;
}

TAO_Policy_Scope
TAO_Thread_Policy::_tao_scope (void) const
{
  return TAO_POLICY_POA_SCOPE;
}

CORBA::Policy_ptr
TAO_Thread_Policy::create (const CORBA::Any &val
                           ACE_ENV_ARG_DECL)
{
  PortableServer::ThreadPolicyValue value;
  if ((val >>= value) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                      CORBA::Policy::_nil ());

  TAO_Thread_Policy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_Thread_Policy (value),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

/////////////////////////////////////////////////////////////////////////////

TAO_Lifespan_Policy::TAO_Lifespan_Policy (PortableServer::LifespanPolicyValue value)
  : value_ (value)
{
}

PortableServer::LifespanPolicyValue
TAO_Lifespan_Policy::value (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->value_;
}

CORBA::Policy_ptr
TAO_Lifespan_Policy::copy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Lifespan_Policy *lifespan_policy_copy = 0;
  ACE_NEW_THROW_EX (lifespan_policy_copy,
                    TAO_Lifespan_Policy (this->value_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return lifespan_policy_copy;
}

void
TAO_Lifespan_Policy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

CORBA::PolicyType
TAO_Lifespan_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return PortableServer::LIFESPAN_POLICY_ID;
}

TAO_Cached_Policy_Type
TAO_Lifespan_Policy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_LIFESPAN;
}

TAO_Policy_Scope
TAO_Lifespan_Policy::_tao_scope (void) const
{
  return TAO_POLICY_POA_SCOPE;
}

CORBA::Policy_ptr
TAO_Lifespan_Policy::create (const CORBA::Any &val
                             ACE_ENV_ARG_DECL)
{
  PortableServer::LifespanPolicyValue value;
  if ((val >>= value) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                      CORBA::Policy::_nil ());

  TAO_Lifespan_Policy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_Lifespan_Policy (value),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

/////////////////////////////////////////////////////////////////////////////

TAO_Id_Uniqueness_Policy::TAO_Id_Uniqueness_Policy (PortableServer::IdUniquenessPolicyValue value)
  : value_ (value)
{
}

PortableServer::IdUniquenessPolicyValue
TAO_Id_Uniqueness_Policy::value (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->value_;
}

CORBA::Policy_ptr
TAO_Id_Uniqueness_Policy::copy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Id_Uniqueness_Policy *id_uniqueness_policy_copy = 0;
  ACE_NEW_THROW_EX (id_uniqueness_policy_copy,
                    TAO_Id_Uniqueness_Policy (this->value_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return id_uniqueness_policy_copy;
}

void
TAO_Id_Uniqueness_Policy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

CORBA::PolicyType
TAO_Id_Uniqueness_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return PortableServer::ID_UNIQUENESS_POLICY_ID;
}

TAO_Cached_Policy_Type
TAO_Id_Uniqueness_Policy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_ID_UNIQUENESS;
}

TAO_Policy_Scope
TAO_Id_Uniqueness_Policy::_tao_scope (void) const
{
  return TAO_POLICY_POA_SCOPE;
}

CORBA::Policy_ptr
TAO_Id_Uniqueness_Policy::create (const CORBA::Any &val
                                  ACE_ENV_ARG_DECL)
{
  PortableServer::IdUniquenessPolicyValue value;
  if ((val >>= value) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                      CORBA::Policy::_nil ());

  TAO_Id_Uniqueness_Policy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_Id_Uniqueness_Policy (value),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

/////////////////////////////////////////////////////////////////////////////

TAO_Id_Assignment_Policy::TAO_Id_Assignment_Policy (PortableServer::IdAssignmentPolicyValue value)
  : value_ (value)
{
}

PortableServer::IdAssignmentPolicyValue
TAO_Id_Assignment_Policy::value (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->value_;
}

CORBA::Policy_ptr
TAO_Id_Assignment_Policy::copy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Id_Assignment_Policy *id_assignment_policy_copy = 0;
  ACE_NEW_THROW_EX (id_assignment_policy_copy,
                    TAO_Id_Assignment_Policy (this->value_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return id_assignment_policy_copy;
}

void
TAO_Id_Assignment_Policy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

CORBA::PolicyType
TAO_Id_Assignment_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return PortableServer::ID_ASSIGNMENT_POLICY_ID;
}

TAO_Cached_Policy_Type
TAO_Id_Assignment_Policy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_ID_ASSIGNMENT;
}

TAO_Policy_Scope
TAO_Id_Assignment_Policy::_tao_scope (void) const
{
  return TAO_POLICY_POA_SCOPE;
}

CORBA::Policy_ptr
TAO_Id_Assignment_Policy::create (const CORBA::Any &val
                                  ACE_ENV_ARG_DECL)
{
  PortableServer::IdAssignmentPolicyValue value;
  if ((val >>= value) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                      CORBA::Policy::_nil ());

  TAO_Id_Assignment_Policy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_Id_Assignment_Policy (value),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

/////////////////////////////////////////////////////////////////////////////

#if (TAO_HAS_MINIMUM_POA == 0)

TAO_Implicit_Activation_Policy::TAO_Implicit_Activation_Policy (PortableServer::ImplicitActivationPolicyValue value)
  : value_ (value)
{
}

PortableServer::ImplicitActivationPolicyValue
TAO_Implicit_Activation_Policy::value (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->value_;
}

CORBA::Policy_ptr
TAO_Implicit_Activation_Policy::copy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Implicit_Activation_Policy *implicit_activation_policy_copy = 0;
  ACE_NEW_THROW_EX (implicit_activation_policy_copy,
                    TAO_Implicit_Activation_Policy (this->value_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return implicit_activation_policy_copy;
}

void
TAO_Implicit_Activation_Policy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

CORBA::PolicyType
TAO_Implicit_Activation_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return PortableServer::IMPLICIT_ACTIVATION_POLICY_ID;
}

TAO_Cached_Policy_Type
TAO_Implicit_Activation_Policy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_IMPLICIT_ACTIVATION;
}

TAO_Policy_Scope
TAO_Implicit_Activation_Policy::_tao_scope (void) const
{
  return TAO_POLICY_POA_SCOPE;
}

CORBA::Policy_ptr
TAO_Implicit_Activation_Policy::create (const CORBA::Any &val
                                        ACE_ENV_ARG_DECL)
{
  PortableServer::ImplicitActivationPolicyValue value;
  if ((val >>= value) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                      CORBA::Policy::_nil ());

  TAO_Implicit_Activation_Policy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_Implicit_Activation_Policy (value),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

/////////////////////////////////////////////////////////////////////////////

TAO_Servant_Retention_Policy::TAO_Servant_Retention_Policy (PortableServer::ServantRetentionPolicyValue value)
  : value_ (value)
{
}

PortableServer::ServantRetentionPolicyValue
TAO_Servant_Retention_Policy::value (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->value_;
}

CORBA::Policy_ptr
TAO_Servant_Retention_Policy::copy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Servant_Retention_Policy *servant_retention_policy_copy = 0;
  ACE_NEW_THROW_EX (servant_retention_policy_copy,
                    TAO_Servant_Retention_Policy (this->value_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return servant_retention_policy_copy;
}

void
TAO_Servant_Retention_Policy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

CORBA::PolicyType
TAO_Servant_Retention_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return PortableServer::SERVANT_RETENTION_POLICY_ID;
}

TAO_Cached_Policy_Type
TAO_Servant_Retention_Policy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_SERVANT_RETENTION;
}

TAO_Policy_Scope
TAO_Servant_Retention_Policy::_tao_scope (void) const
{
  return TAO_POLICY_POA_SCOPE;
}

CORBA::Policy_ptr
TAO_Servant_Retention_Policy::create (const CORBA::Any &val
                                      ACE_ENV_ARG_DECL)
{
  PortableServer::ServantRetentionPolicyValue value;
  if ((val >>= value) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                      CORBA::Policy::_nil ());

  TAO_Servant_Retention_Policy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_Servant_Retention_Policy (value),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

/////////////////////////////////////////////////////////////////////////////

TAO_Request_Processing_Policy::TAO_Request_Processing_Policy (PortableServer::RequestProcessingPolicyValue value)
  : value_ (value)
{
}

PortableServer::RequestProcessingPolicyValue
TAO_Request_Processing_Policy::value (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->value_;
}

CORBA::Policy_ptr
TAO_Request_Processing_Policy::copy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_Request_Processing_Policy *request_processing_policy_copy = 0;
  ACE_NEW_THROW_EX (request_processing_policy_copy,
                    TAO_Request_Processing_Policy (this->value_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return request_processing_policy_copy;
}

void
TAO_Request_Processing_Policy::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

CORBA::PolicyType
TAO_Request_Processing_Policy::policy_type (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return PortableServer::REQUEST_PROCESSING_POLICY_ID;
}

TAO_Cached_Policy_Type
TAO_Request_Processing_Policy::_tao_cached_type (void) const
{
  return TAO_CACHED_POLICY_REQUEST_PROCESSING;
}

TAO_Policy_Scope
TAO_Request_Processing_Policy::_tao_scope (void) const
{
  return TAO_POLICY_POA_SCOPE;
}

CORBA::Policy_ptr
TAO_Request_Processing_Policy::create (const CORBA::Any &val
                                       ACE_ENV_ARG_DECL)
{
  PortableServer::RequestProcessingPolicyValue value;
  if ((val >>= value) == 0)
    ACE_THROW_RETURN (CORBA::PolicyError (CORBA::BAD_POLICY_TYPE),
                      CORBA::Policy::_nil ());

  TAO_Request_Processing_Policy *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    TAO_Request_Processing_Policy (value),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  return tmp;
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
