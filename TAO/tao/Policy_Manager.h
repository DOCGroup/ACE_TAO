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
#include "ace/pre.h"

#include "tao/orbconf.h"

#if (TAO_HAS_CORBA_MESSAGING == 1)

#include "tao/LocalObject.h"
#include "tao/PolicyC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

// Forward declarations.
class TAO_RelativeRoundtripTimeoutPolicy;
class TAO_Client_Priority_Policy;
class TAO_Sync_Scope_Policy;
class TAO_Buffering_Constraint_Policy;
class TAO_PriorityModelPolicy;
class TAO_ThreadpoolPolicy;
class TAO_PrivateConnectionPolicy;
class TAO_PriorityBandedConnectionPolicy;
class TAO_ServerProtocolPolicy;
class TAO_ClientProtocolPolicy;

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

  friend class TAO_Policy_Manager;

public:

  TAO_Policy_Manager_Impl (void);
  // Constructor

  ~TAO_Policy_Manager_Impl (void);
  // Destructor

  void copy_from (TAO_Policy_Manager_Impl* source,
                  CORBA::Environment &ACE_TRY_ENV);
  // Copy the state from <source>, it uses the copy() operator to
  // obtain independent copies of all the policies.

  void set_policy_overrides (const CORBA::PolicyList & policies,
                             CORBA::SetOverrideType set_add,
                             CORBA::Environment &ACE_TRY_ENV =
                             TAO_default_environment ()
    );
  // Modify the list of policies to include <policies>.
  // If <set_add> is CORBA::SET_OVERRIDE then we replace all the old
  // policies. If it is CORBA::ADD_OVERRIDE we simply add the policies
  // in <policies>.
  // No attempt is made to validate the policies for consistency.

  CORBA::PolicyList * get_policy_overrides (const CORBA::PolicyTypeSeq & types,
                                            CORBA::Environment &ACE_TRY_ENV =
                                            TAO_default_environment ()
    );
  // Get the values (if any) for the policies in <types>, if <types>
  // is an empty list the method returns *all* the current policies.

  CORBA::Policy_ptr get_policy (CORBA::PolicyType policy,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ()
    );
  // Obtain a single policy.

  // = Direct accesors to the policy implementations, for speedy
  //   lookups.

  CORBA::Policy *relative_roundtrip_timeout (void) const;

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

  TAO_Client_Priority_Policy *client_priority (void) const;

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

  CORBA::Policy *sync_scope (void) const;

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  TAO_Buffering_Constraint_Policy *buffering_constraint (void) const;

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if (TAO_HAS_RT_CORBA == 1)

  TAO_PriorityModelPolicy *priority_model (void) const;
  TAO_ThreadpoolPolicy *threadpool (void) const;
  TAO_PrivateConnectionPolicy *private_connection (void) const;
  TAO_PriorityBandedConnectionPolicy *priority_banded_connection (void) const;
  TAO_ServerProtocolPolicy *server_protocol (void) const;
  TAO_ClientProtocolPolicy *client_protocol (void) const;

  // = Speedy policy setters.  Take ownership of the argument policy
  // objects - no copying.

  void server_protocol (TAO_ServerProtocolPolicy *server_protocol);
  void client_protocol (TAO_ClientProtocolPolicy *client_protocol);

#endif /* TAO_HAS_RT_CORBA == 1 */

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

  CORBA::Policy *relative_roundtrip_timeout_;

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

  TAO_Client_Priority_Policy *client_priority_;

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

  CORBA::Policy *sync_scope_;

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  TAO_Buffering_Constraint_Policy *buffering_constraint_;

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if (TAO_HAS_RT_CORBA == 1)

  TAO_PriorityModelPolicy *priority_model_;
  TAO_ThreadpoolPolicy *threadpool_;
  TAO_PrivateConnectionPolicy *private_connection_;
  TAO_PriorityBandedConnectionPolicy *priority_banded_connection_;
  TAO_ServerProtocolPolicy *server_protocol_;
  TAO_ClientProtocolPolicy *client_protocol_;

#endif /* TAO_HAS_RT_CORBA == 1 */

  CORBA::PolicyList other_policies_;
  // Other policies that are not optimized for fast querying

  CORBA::ULong count_;
  // The number of non-nil policies
};

// ****************************************************************

class TAO_Export TAO_Policy_Manager :
  public CORBA::PolicyManager,
  public TAO_Local_RefCounted_Object
{
public:
  TAO_Policy_Manager (void);
  // constructor

  CORBA::Policy_ptr get_policy (CORBA::PolicyType policy,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ()
    );
  // Obtain a single policy.

  // = The CORBA::PolicyManager operations

  virtual CORBA::PolicyList * get_policy_overrides (
        const CORBA::PolicyTypeSeq & ts,
        CORBA::Environment &ACE_TRY_ENV =
          CORBA::Environment::default_environment ()
      )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void set_policy_overrides (const CORBA::PolicyList & policies,
                                     CORBA::SetOverrideType set_add,
                                     CORBA::Environment &ACE_TRY_ENV =
                                     CORBA::Environment::default_environment ()
                                     )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CORBA_InvalidPolicies));

  CORBA::Policy *relative_roundtrip_timeout (void);

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

  TAO_Client_Priority_Policy *client_priority (void);

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

  CORBA::Policy *sync_scope (void);

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  TAO_Buffering_Constraint_Policy *buffering_constraint (void);

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if (TAO_HAS_RT_CORBA == 1)

  TAO_PriorityModelPolicy *priority_model (void);
  TAO_ThreadpoolPolicy *threadpool (void);
  TAO_PrivateConnectionPolicy *private_connection (void);
  TAO_PriorityBandedConnectionPolicy *priority_banded_connection (void);
  TAO_ServerProtocolPolicy *server_protocol (void);
  TAO_ClientProtocolPolicy *client_protocol (void);

#endif /* TAO_HAS_RT_CORBA == 1 */

private:
  ACE_SYNCH_MUTEX mutex_;
  // Protect access

  TAO_Policy_Manager_Impl impl_;
  // The implementation.
};

// ****************************************************************

class TAO_Export TAO_Policy_Current_Impl
{
public:
  CORBA::Policy_ptr get_policy (CORBA::PolicyType policy,
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
  void set_policy_overrides (const CORBA::PolicyList & policies,
                             CORBA::SetOverrideType set_add,
                             CORBA::Environment &ACE_TRY_ENV =
                             CORBA::Environment::default_environment ());

  CORBA::Policy *relative_roundtrip_timeout (void) const;

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

  TAO_Client_Priority_Policy *client_priority (void) const;

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

  CORBA::Policy *sync_scope (void) const;

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  TAO_Buffering_Constraint_Policy *buffering_constraint (void) const;

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if (TAO_HAS_RT_CORBA == 1)

  TAO_PrivateConnectionPolicy *private_connection (void) const;
  TAO_PriorityBandedConnectionPolicy *priority_banded_connection (void) const;
  TAO_ClientProtocolPolicy *client_protocol (void) const;

#endif /* TAO_HAS_RT_CORBA == 1 */

private:
  TAO_Policy_Manager_Impl manager_impl_;
  // The implementation.
};

// ****************************************************************

class TAO_Export TAO_Policy_Current :
  public CORBA::PolicyCurrent,
  public TAO_Local_RefCounted_Object
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
      )
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void set_policy_overrides (
        const CORBA::PolicyList & policies,
        CORBA::SetOverrideType set_add,
        CORBA::Environment &ACE_TRY_ENV =
          CORBA::Environment::default_environment ()
      )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CORBA_InvalidPolicies));

  CORBA::Policy *relative_roundtrip_timeout (void) const;

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

  TAO_Client_Priority_Policy *client_priority (void) const;

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

  CORBA::Policy *sync_scope (void) const;

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  TAO_Buffering_Constraint_Policy *buffering_constraint (void) const;

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if (TAO_HAS_RT_CORBA == 1)

  TAO_PrivateConnectionPolicy *private_connection (void) const;
  TAO_PriorityBandedConnectionPolicy *priority_banded_connection (void) const;
  TAO_ClientProtocolPolicy *client_protocol (void) const;

#endif /* TAO_HAS_RT_CORBA == 1 */

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
