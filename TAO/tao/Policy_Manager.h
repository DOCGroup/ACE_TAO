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

#if (TAO_HAS_CORBA_MESSAGING == 1)

#include "tao/POA_CORBA.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// Forward declarations.
class TAO_RelativeRoundtripTimeoutPolicy_i;
class TAO_Client_Priority_Policy;
class TAO_Sync_Scope_Policy;
class TAO_Buffering_Constraint_Policy;

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

  ~TAO_Policy_Manager_Impl (void);
  // Destructor

  void copy_from (TAO_Policy_Manager_Impl* source,
                  CORBA::Environment &ACE_TRY_ENV);
  // Copy the state from <source>, it uses the copy() operator to
  // obtain independent copies of all the policies.

  void set_policy_overrides (
      const CORBA::PolicyList & policies,
      CORBA::SetOverrideType set_add,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  // Modify the list of policies to include <policies>.
  // If <set_add> is CORBA::SET_OVERRIDE then we replace all the old
  // policies. If it is CORBA::ADD_OVERRIDE we simply add the policies
  // in <policies>.
  // No attempt is made to validate the policies for consitency.

  CORBA::PolicyList * get_policy_overrides (
      const CORBA::PolicyTypeSeq & types,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  // Get the values (if any) for the policies in <types>, if <types>
  // is an empty list the method returns *all* the current policies.

  CORBA::Policy_ptr get_policy (
      CORBA::PolicyType policy,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  // Obtain a single policy.

  // = Direct accesors to the policy implementations, for speedy
  //   lookups.
  TAO_RelativeRoundtripTimeoutPolicy_i *relative_roundtrip_timeout (void) const;

  TAO_Client_Priority_Policy *client_priority (void) const;

  TAO_Sync_Scope_Policy *sync_scope (void) const;

  TAO_Buffering_Constraint_Policy *buffering_constraint (void) const;

private:
  ACE_UNIMPLEMENTED_FUNC (TAO_Policy_Manager_Impl operator=(const TAO_Policy_Manager_Impl&))
  ACE_UNIMPLEMENTED_FUNC (TAO_Policy_Manager_Impl(const TAO_Policy_Manager_Impl&))

  void cleanup_i (CORBA::Environment &ACE_TRY_ENV);
  // Remove and destroy all the policy objects owned by this policy
  // manager.

private:
  // The known policies are kept as pointers to the implementation
  // objects, this allow us to query the supported policies really
  // fast, without memory allocations.
  TAO_RelativeRoundtripTimeoutPolicy_i *relative_roundtrip_timeout_;

  TAO_Client_Priority_Policy *client_priority_;

  TAO_Sync_Scope_Policy *sync_scope_;

  TAO_Buffering_Constraint_Policy *buffering_constraint_;

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
        TAO_default_environment ()
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
  TAO_RelativeRoundtripTimeoutPolicy_i *relative_roundtrip_timeout (void) const;

  TAO_Client_Priority_Policy *client_priority (void) const;

  TAO_Sync_Scope_Policy *sync_scope (void) const;

  TAO_Buffering_Constraint_Policy *buffering_constraint (void) const;

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
        TAO_default_environment ()
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
  TAO_RelativeRoundtripTimeoutPolicy_i *relative_roundtrip_timeout (void) const;

  TAO_Client_Priority_Policy *client_priority (void) const;

  TAO_Sync_Scope_Policy *sync_scope (void) const;

  TAO_Buffering_Constraint_Policy *buffering_constraint (void) const;

private:
  TAO_Policy_Manager_Impl manager_impl_;
  // The implementation.
};

// ****************************************************************

class TAO_Export TAO_Policy_Current : public POA_CORBA::PolicyCurrent
{
public:
  TAO_Policy_Current (void);
  // Constructor

  CORBA::Policy_ptr get_policy (
      CORBA::PolicyType policy,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
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
  TAO_RelativeRoundtripTimeoutPolicy_i *relative_roundtrip_timeout (void) const;

  TAO_Client_Priority_Policy *client_priority (void) const;

  TAO_Sync_Scope_Policy *sync_scope (void) const;

  TAO_Buffering_Constraint_Policy *buffering_constraint (void) const;

  // = Set and get the implementation.
  TAO_Policy_Current_Impl &implementation (void) const;
  TAO_Policy_Current_Impl &implementation (TAO_Policy_Current_Impl &);
};

#if defined (__ACE_INLINE__)
# include "tao/Policy_Manager.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

#endif /* TAO_POLICY_MANAGER_H */
