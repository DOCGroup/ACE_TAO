// -*- C++ -*-

//=============================================================================
/**
 *  @file   Policy_Current.h
 *
 * An implementation for the CORBA::PolicyCurrent interface.
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_POLICY_CURRENT_H
#define TAO_POLICY_CURRENT_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_CORBA_MESSAGING == 1)

#include "tao/Policy_CurrentC.h"
#include "tao/LocalObject.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Policy_Current_Impl;

class TAO_Policy_Current
  : public CORBA::PolicyCurrent
  , public ::CORBA::LocalObject
{
public:
  /// Constructor
  TAO_Policy_Current () = default;

  /// Obtain a single policy.
  CORBA::Policy_ptr get_policy (CORBA::PolicyType policy);

  /// Obtain a single cached policy.
  CORBA::Policy_ptr get_cached_policy (TAO_Cached_Policy_Type type);

  // = The CORBA::PolicyManager operations

  CORBA::PolicyList *get_policy_overrides (
        CORBA::PolicyTypeSeq const &ts) override;

  void set_policy_overrides (
        CORBA::PolicyList const &policies,
        CORBA::SetOverrideType set_add) override;

  // = Set and get the implementation.
  TAO_Policy_Current_Impl &implementation () const;
  TAO_Policy_Current_Impl &implementation (TAO_Policy_Current_Impl &);

private:
  TAO_Policy_Current (TAO_Policy_Current const &) = delete;
  TAO_Policy_Current &operator= (TAO_Policy_Current const &) = delete;
  TAO_Policy_Current (TAO_Policy_Current &&) = delete;
  TAO_Policy_Current &operator= (TAO_Policy_Current &&) = delete;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_POLICY_CURRENT_H */
