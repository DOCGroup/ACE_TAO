// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    POA_Policies.h
//
// = DESCRIPTION
//     POA
//
// = AUTHOR
//     Irfan Pyarali
//
// ============================================================================

#ifndef TAO_POA_POLICIES_H
#define TAO_POA_POLICIES_H
#include "ace/pre.h"

#include "PortableServerC.h"
#include "tao/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#if (TAO_HAS_MINIMUM_POA == 0)

class TAO_PortableServer_Export TAO_Thread_Policy :
  public PortableServer::ThreadPolicy,
  public TAO_Local_RefCounted_Object
{
public:
  TAO_Thread_Policy (PortableServer::ThreadPolicyValue value);

  PortableServer::ThreadPolicyValue value (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Policy_ptr copy (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void destroy (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::PolicyType policy_type (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Return the cached policy type for this policy.
  virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

  // Returns the scope at which this policy can be applied. See orbconf.h.
  virtual TAO_Policy_Scope _tao_scope (void) const;

protected:
  PortableServer::ThreadPolicyValue value_;
};

#endif /* TAO_HAS_MINIMUM_POA == 0 */

class TAO_PortableServer_Export TAO_Lifespan_Policy :
  public PortableServer::LifespanPolicy,
  public TAO_Local_RefCounted_Object
{
public:
  TAO_Lifespan_Policy (PortableServer::LifespanPolicyValue value);

  PortableServer::LifespanPolicyValue value (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Policy_ptr copy (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void destroy (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::PolicyType policy_type (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

    // Return the cached policy type for this policy.
  virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

  // Returns the scope at which this policy can be applied. See orbconf.h.
  virtual TAO_Policy_Scope _tao_scope (void) const;

protected:
  PortableServer::LifespanPolicyValue value_;
};

class TAO_PortableServer_Export TAO_Id_Uniqueness_Policy :
  public PortableServer::IdUniquenessPolicy,
  public TAO_Local_RefCounted_Object
{
public:
  TAO_Id_Uniqueness_Policy (PortableServer::IdUniquenessPolicyValue value);

  PortableServer::IdUniquenessPolicyValue value (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Policy_ptr copy (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void destroy (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::PolicyType policy_type (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Return the cached policy type for this policy.
  virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

  // Returns the scope at which this policy can be applied. See orbconf.h.
  virtual TAO_Policy_Scope _tao_scope (void) const;

protected:
  PortableServer::IdUniquenessPolicyValue value_;
};

class TAO_PortableServer_Export TAO_Id_Assignment_Policy :
  public PortableServer::IdAssignmentPolicy,
  public TAO_Local_RefCounted_Object
{
public:
  TAO_Id_Assignment_Policy (PortableServer::IdAssignmentPolicyValue value);

  PortableServer::IdAssignmentPolicyValue value (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Policy_ptr copy (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void destroy (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::PolicyType policy_type (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Return the cached policy type for this policy.
  virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

  // Returns the scope at which this policy can be applied. See orbconf.h.
  virtual TAO_Policy_Scope _tao_scope (void) const;

protected:
  PortableServer::IdAssignmentPolicyValue value_;
};

#if (TAO_HAS_MINIMUM_POA == 0)

class TAO_PortableServer_Export TAO_Implicit_Activation_Policy :
  public PortableServer::ImplicitActivationPolicy,
  public TAO_Local_RefCounted_Object
{
public:
  TAO_Implicit_Activation_Policy (PortableServer::ImplicitActivationPolicyValue value);

  PortableServer::ImplicitActivationPolicyValue value (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Policy_ptr copy (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void destroy (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::PolicyType policy_type (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Return the cached policy type for this policy.
  virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

  // Returns the scope at which this policy can be applied. See orbconf.h.
  virtual TAO_Policy_Scope _tao_scope (void) const;

protected:
  PortableServer::ImplicitActivationPolicyValue value_;
};

class TAO_PortableServer_Export TAO_Servant_Retention_Policy :
  public PortableServer::ServantRetentionPolicy,
  public TAO_Local_RefCounted_Object
{
public:
  TAO_Servant_Retention_Policy (PortableServer::ServantRetentionPolicyValue value);

  PortableServer::ServantRetentionPolicyValue value (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Policy_ptr copy (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void destroy (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::PolicyType policy_type (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Return the cached policy type for this policy.
  virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

  // Returns the scope at which this policy can be applied. See orbconf.h.
  virtual TAO_Policy_Scope _tao_scope (void) const;

protected:
  PortableServer::ServantRetentionPolicyValue value_;
};

class TAO_PortableServer_Export TAO_Request_Processing_Policy :
  public PortableServer::RequestProcessingPolicy,
  public TAO_Local_RefCounted_Object
{
public:
  TAO_Request_Processing_Policy (PortableServer::RequestProcessingPolicyValue value);

  PortableServer::RequestProcessingPolicyValue value (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Policy_ptr copy (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  void destroy (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::PolicyType policy_type (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Return the cached policy type for this policy.
  virtual TAO_Cached_Policy_Type _tao_cached_type (void) const;

  // Returns the scope at which this policy can be applied. See orbconf.h.
  virtual TAO_Policy_Scope _tao_scope (void) const;

protected:
  PortableServer::RequestProcessingPolicyValue value_;
};

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
# include "POA_Policies.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_POA_POLICIES_H */
