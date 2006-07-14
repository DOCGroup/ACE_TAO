// -*- C++ -*-

//=============================================================================
/**
 *  @file   Policy_Manager.h
 *
 *  $Id$
 *
 * An implementation for the CORBA::PolicyManager interface.
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_POLICY_MANAGER_H
#define TAO_POLICY_MANAGER_H

#include /**/ "ace/pre.h"
#include "ace/Guard_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#if (TAO_HAS_CORBA_MESSAGING == 1)

#include "tao/PolicyC.h"
#include "tao/LocalObject.h"
#include "tao/Policy_Set.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Policy_Manager
  : public CORBA::PolicyManager
  , public TAO_Local_RefCounted_Object
{
public:
  /// constructor
  TAO_Policy_Manager (void);

  /// Obtain a single policy.
  CORBA::Policy_ptr get_policy (CORBA::PolicyType policy
                                ACE_ENV_ARG_DECL);

  /// Obtain a single cached policy.
  CORBA::Policy_ptr get_cached_policy (TAO_Cached_Policy_Type type
                                       ACE_ENV_ARG_DECL);

  // = The CORBA::PolicyManager operations

  virtual CORBA::PolicyList * get_policy_overrides (
        const CORBA::PolicyTypeSeq & ts
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void set_policy_overrides (const CORBA::PolicyList & policies,
                                     CORBA::SetOverrideType set_add
                                     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CORBA::InvalidPolicies));

private:

  // Disallow copying.
  TAO_Policy_Manager (TAO_Policy_Manager const &);
  TAO_Policy_Manager & operator= (TAO_Policy_Manager const &);

private:
  /// Protect access
  TAO_SYNCH_MUTEX mutex_;

  /// The implementation.
  TAO_Policy_Set impl_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#if defined (__ACE_INLINE__)
# include "tao/Policy_Manager.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_POLICY_MANAGER_H */
