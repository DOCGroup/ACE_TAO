// -*- C++ -*-

//=============================================================================
/**
 *  @file   Policy_Manager.h
 *
 *  $Id$
 *
 * An implementation for the CORBA::PolicyManager interface.
 *
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_POLICY_MANAGER_H
#define TAO_POLICY_MANAGER_H
#include "ace/pre.h"

#include "tao/orbconf.h"

#if (TAO_HAS_CORBA_MESSAGING == 1)

#include "tao/Policy_Set.h"
#include "tao/LocalObject.h"
#include "tao/PolicyC.h"
#include "tao/debug.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


class TAO_Export TAO_Policy_Manager :
  public CORBA::PolicyManager,
  public TAO_Local_RefCounted_Object
{
public:
  /// constructor
  TAO_Policy_Manager (void);

  /// Obtain a single policy.
  CORBA::Policy_ptr get_policy (CORBA::PolicyType policy
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS);


  // = The CORBA::PolicyManager operations

  virtual CORBA::PolicyList * get_policy_overrides (
        const CORBA::PolicyTypeSeq & ts
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void set_policy_overrides (const CORBA::PolicyList & policies,
                                     CORBA::SetOverrideType set_add
                                     ACE_ENV_ARG_DECL_WITH_DEFAULTS
                                     )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CORBA_InvalidPolicies));

  /// Obtain a single cached policy.
  CORBA::Policy_ptr get_cached_policy (TAO_Cached_Policy_Type type);

private:
  /// Protect access
  TAO_SYNCH_MUTEX mutex_;

  /// The implementation.
  TAO_Policy_Set impl_;
};

// ****************************************************************

class TAO_Export TAO_Policy_Current_Impl
{
public:

  TAO_Policy_Current_Impl (void);

  /// Obtain a single policy.
  CORBA::Policy_ptr get_policy (CORBA::PolicyType policy
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS);


  // = The CORBA::PolicyManager operations

  CORBA::PolicyList * get_policy_overrides (
        const CORBA::PolicyTypeSeq & ts
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      );
  void set_policy_overrides (const CORBA::PolicyList & policies,
                             CORBA::SetOverrideType set_add
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Obtain a single cached policy.
  CORBA::Policy_ptr get_cached_policy (TAO_Cached_Policy_Type type);

private:
  /// The implementation.
  TAO_Policy_Set manager_impl_;
};

// ****************************************************************

class TAO_Export TAO_Policy_Current :
  public CORBA::PolicyCurrent,
  public TAO_Local_RefCounted_Object
{
public:
  /// Constructor
  TAO_Policy_Current (void);

  /// Obtain a single policy.
  CORBA::Policy_ptr get_policy (
      CORBA::PolicyType policy
      ACE_ENV_ARG_DECL_WITH_DEFAULTS);


  // = The CORBA::PolicyManager operations

  virtual CORBA::PolicyList * get_policy_overrides (
        const CORBA::PolicyTypeSeq & ts
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void set_policy_overrides (
        const CORBA::PolicyList & policies,
        CORBA::SetOverrideType set_add
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CORBA_InvalidPolicies));

  /// Obtain a single cached policy.
  CORBA::Policy_ptr get_cached_policy (TAO_Cached_Policy_Type type);

  // = Set and get the implementation.
  TAO_Policy_Current_Impl &implementation (void) const;
  TAO_Policy_Current_Impl &implementation (TAO_Policy_Current_Impl &);
};

#if defined (__ACE_INLINE__)
# include "tao/Policy_Manager.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

#include "ace/post.h"
#endif /* TAO_POLICY_MANAGER_H */

