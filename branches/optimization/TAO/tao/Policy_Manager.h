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

/**
 * @class TAO_Policy_Manager_Impl
 *
 * @brief The policy manager implementation.
 *
 * This class is used to implement both the CORBA::PolicyManager
 * and the CORBA::PolicyCurrent interfaces.
 */
class TAO_Export TAO_Policy_Manager_Impl
{

  friend class TAO_Policy_Manager;

public:

  /// Constructor
  TAO_Policy_Manager_Impl (void);

  /// Destructor
  ~TAO_Policy_Manager_Impl (void);

  /// Copy the state from <source>, it uses the copy() operator to
  /// obtain independent copies of all the policies.
  void copy_from (TAO_Policy_Manager_Impl* source,
                  CORBA::Environment &ACE_TRY_ENV);

  /**
   * Modify the list of policies to include <policies>.
   * If <set_add> is CORBA::SET_OVERRIDE then we replace all the old
   * policies. If it is CORBA::ADD_OVERRIDE we simply add the policies
   * in <policies>.
   * No attempt is made to validate the policies for consistency.
   */
  void set_policy_overrides (const CORBA::PolicyList & policies,
                             CORBA::SetOverrideType set_add,
                             CORBA::Environment &ACE_TRY_ENV =
                             TAO_default_environment ()
    );

  /// Get the values (if any) for the policies in <types>, if <types>
  /// is an empty list the method returns *all* the current policies.
  CORBA::PolicyList * get_policy_overrides (const CORBA::PolicyTypeSeq & types,
                                            CORBA::Environment &ACE_TRY_ENV =
                                            TAO_default_environment ()
    );

  /// Obtain a single policy.
  CORBA::Policy_ptr get_policy (CORBA::PolicyType policy,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ()
    );

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

  CORBA::Policy *priority_model (void) const;
  CORBA::Policy *threadpool (void) const;
  CORBA::Policy *private_connection (void) const;
  CORBA::Policy *priority_banded_connection (void) const;
  CORBA::Policy *server_protocol (void) const;
  CORBA::Policy *client_protocol (void) const;

  // = Speedy policy setters.  Take ownership of the argument policy
  // objects - no copying.

  void server_protocol (TAO_ServerProtocolPolicy *server_protocol);
  void client_protocol (TAO_ClientProtocolPolicy *client_protocol);

#endif /* TAO_HAS_RT_CORBA == 1 */

private:
  ACE_UNIMPLEMENTED_FUNC (TAO_Policy_Manager_Impl operator=(const TAO_Policy_Manager_Impl&))
  ACE_UNIMPLEMENTED_FUNC (TAO_Policy_Manager_Impl(const TAO_Policy_Manager_Impl&))

  /// Remove and destroy all the policy objects owned by this policy
  /// manager.
  void cleanup_i (CORBA::Environment &ACE_TRY_ENV);

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

  CORBA::Policy *priority_model_;
  CORBA::Policy *threadpool_;
  CORBA::Policy *private_connection_;
  CORBA::Policy *priority_banded_connection_;
  CORBA::Policy *server_protocol_;
  CORBA::Policy *client_protocol_;

#endif /* TAO_HAS_RT_CORBA == 1 */

  /// Other policies that are not optimized for fast querying
  CORBA::PolicyList other_policies_;

  /// The number of non-nil policies
  CORBA::ULong count_;
};

// ****************************************************************

class TAO_Export TAO_Policy_Manager :
  public CORBA::PolicyManager,
  public TAO_Local_RefCounted_Object
{
public:
  /// constructor
  TAO_Policy_Manager (void);

  /// Obtain a single policy.
  CORBA::Policy_ptr get_policy (CORBA::PolicyType policy,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ()
    );

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

  CORBA::Policy *priority_model (void);
  CORBA::Policy *threadpool (void);
  CORBA::Policy *private_connection (void);
  CORBA::Policy *priority_banded_connection (void);
  CORBA::Policy *server_protocol (void);
  CORBA::Policy *client_protocol (void);

#endif /* TAO_HAS_RT_CORBA == 1 */

private:
  /// Protect access
  ACE_SYNCH_MUTEX mutex_;

  /// The implementation.
  TAO_Policy_Manager_Impl impl_;
};

// ****************************************************************

class TAO_Export TAO_Policy_Current_Impl
{
public:
  /// Obtain a single policy.
  CORBA::Policy_ptr get_policy (CORBA::PolicyType policy,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ()
    );

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

  CORBA::Policy *private_connection (void) const;
  CORBA::Policy *priority_banded_connection (void) const;
  CORBA::Policy *client_protocol (void) const;

#endif /* TAO_HAS_RT_CORBA == 1 */

private:
  /// The implementation.
  TAO_Policy_Manager_Impl manager_impl_;
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
      CORBA::PolicyType policy,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );

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

  CORBA::Policy *private_connection (void) const;
  CORBA::Policy *priority_banded_connection (void) const;
  CORBA::Policy *client_protocol (void) const;

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
