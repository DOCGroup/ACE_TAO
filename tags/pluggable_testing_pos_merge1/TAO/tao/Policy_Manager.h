// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO
//
// = FILENAME
//   Policy_Manager.h
//
// = DESCRIPTION
//   An implementation for the CORBA::PolicyManager interface.
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_POLICY_MANAGER_H
#define TAO_POLICY_MANAGER_H

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING)

#include "tao/POA_CORBA.h"
#include "tao/MessagingS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Export TAO_Policy_Manager_Impl
{
  //
  // = TITLE
  //   The policy manager implementation.
  //
  // = DESCRIPTION
  //   This class is used to implement both the CORBA::PolicyManager
  //   and the CORBA::PolicyCurrent interfaces.
  //
public:
  TAO_Policy_Manager_Impl (void);
  // Constructor

  void set_policy_overrides (
      const CORBA::PolicyList & policies,
      CORBA::SetOverrideType set_add,
      CORBA::Environment &ACE_TRY_ENV =
        CORBA::default_environment ()
    );
  // Modify the list of policies to include <policies>.
  // If <set_add> is CORBA::SET_OVERRIDE then we replace all the old
  // policies. If it is CORBA::ADD_OVERRIDE we simply add the policies
  // in <policies>.
  // No attempt is made to validate the policies for consitency.

  CORBA::PolicyList * get_policy_overrides (
      const CORBA::PolicyTypeSeq & types,
      CORBA::Environment &ACE_TRY_ENV =
        CORBA::default_environment ()
    );
  // Get the values (if any) for the policies in <types>, if <types>
  // is an empty list the method returns *all* the current policies.

  CORBA::Policy_ptr get_policy (
      CORBA::PolicyType policy,
      CORBA::Environment &ACE_TRY_ENV =
        CORBA::default_environment ()
    );
  // Obtain a single policy.

  // = Direct accesors to the policy implementations, for speedy
  //   lookups.
  POA_Messaging::RelativeRoundtripTimeoutPolicy*
      relative_roundtrip_timeout (void) const;

private:
  // The known policies are kept as pointers to the implementation
  // objects, this allow us to query the supported policies really
  // fast, without memory allocations.
  POA_Messaging::RelativeRoundtripTimeoutPolicy *relative_roundtrip_timeout_;

  CORBA::PolicyList other_policies_;
  // Other policies that are not optimized for fast querying

  CORBA::ULong count_;
  // The number of non-nil policies
};

// ****************************************************************

class TAO_Export TAO_Policy_Manager : public POA_CORBA::PolicyManager
{
public:
  TAO_Policy_Manager (void);
  // constructor

  CORBA::Policy_ptr get_policy (
      CORBA::PolicyType policy,
      CORBA::Environment &ACE_TRY_ENV =
        CORBA::default_environment ()
    );
  // Obtain a single policy.

  // = The CORBA::PolicyManager operations

  virtual CORBA::PolicyList * get_policy_overrides (
        const CORBA::PolicyTypeSeq & ts,
        CORBA::Environment &ACE_TRY_ENV =
          CORBA::Environment::default_environment ()
      );
  virtual void set_policy_overrides (
        const CORBA::PolicyList & policies,
        CORBA::SetOverrideType set_add,
        CORBA::Environment &ACE_TRY_ENV =
          CORBA::Environment::default_environment ()
      );
  POA_Messaging::RelativeRoundtripTimeoutPolicy*
      relative_roundtrip_timeout (void) const;

private:
  TAO_Policy_Manager_Impl impl_;
  // The implementation.
};

// ****************************************************************

class TAO_Export TAO_Policy_Current_Impl
{
public:
  CORBA::Policy_ptr get_policy (
      CORBA::PolicyType policy,
      CORBA::Environment &ACE_TRY_ENV =
        CORBA::default_environment ()
    );
  // Obtain a single policy.

  // = The CORBA::PolicyManager operations

  CORBA::PolicyList * get_policy_overrides (
        const CORBA::PolicyTypeSeq & ts,
        CORBA::Environment &ACE_TRY_ENV =
          CORBA::Environment::default_environment ()
      );
  void set_policy_overrides (
        const CORBA::PolicyList & policies,
        CORBA::SetOverrideType set_add,
        CORBA::Environment &ACE_TRY_ENV =
          CORBA::Environment::default_environment ()
      );
  POA_Messaging::RelativeRoundtripTimeoutPolicy*
      relative_roundtrip_timeout (void) const;

private:
  TAO_Policy_Manager_Impl manager_impl_;
  // The implementation.
};

// ****************************************************************

class TAO_Export TAO_Policy_Current : public POA_CORBA::PolicyCurrent
{
public:
  CORBA::Policy_ptr get_policy (
      CORBA::PolicyType policy,
      CORBA::Environment &ACE_TRY_ENV =
        CORBA::default_environment ()
    );
  // Obtain a single policy.

  // = The CORBA::PolicyManager operations

  virtual CORBA::PolicyList * get_policy_overrides (
        const CORBA::PolicyTypeSeq & ts,
        CORBA::Environment &ACE_TRY_ENV =
          CORBA::Environment::default_environment ()
      );
  virtual void set_policy_overrides (
        const CORBA::PolicyList & policies,
        CORBA::SetOverrideType set_add,
        CORBA::Environment &ACE_TRY_ENV =
          CORBA::Environment::default_environment ()
      );
  POA_Messaging::RelativeRoundtripTimeoutPolicy*
      relative_roundtrip_timeout (void) const;

  // = Set and get the implementation.
  TAO_Policy_Current_Impl &implementation (void) const;
  TAO_Policy_Current_Impl &implementation (TAO_Policy_Current_Impl &);
};

#if defined (__ACE_INLINE__)
# include "tao/Policy_Manager.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_CORBA_MESSAGING */

#endif /* TAO_POLICY_MANAGER_H */
