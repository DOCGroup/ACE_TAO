//=============================================================================
/**
 *  @file    RT_Stub.h
 *
 *  $Id$
 *
 *  This file contains the declaration for the RTCORBA Stub.
 *
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_RT_STUB_H_
#define TAO_RT_STUB_H_

#include "ace/pre.h"
#include "tao/corbafwd.h"
#include "tao/Stub.h"

#if (TAO_HAS_RT_CORBA == 1)

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


class TAO_PriorityModelPolicy;
class TAO_PriorityBandedConnectionPolicy;
class TAO_ClientProtocolPolicy;
class TAO_PrivateConnectionPolicy;



/**
 * @class TAO_RT_Stub
 *
 * This class reprent a stub with extended functionality, which are needed for RTCORBA.
 */
class TAO_Export TAO_RT_Stub : public TAO_Stub
{
public:

  // -- Ctor/Dtor --
  TAO_RT_Stub (const char *repository_id,
               const TAO_MProfile &profiles,
               TAO_ORB_Core *orb_core);

  virtual ~TAO_RT_Stub (void);


#if (TAO_HAS_CORBA_MESSAGING == 1)

  /**
   * Returns the effective policy if <type> is a known client-exposed
   * policy type.  Returns the effective override for all other policy
   * types.
   */
  CORBA::Policy_ptr get_policy (CORBA::PolicyType type,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ()
    );


  CORBA::Policy_ptr get_client_policy (CORBA::PolicyType type,
                                       CORBA::Environment &ACE_TRY_ENV =
                                       TAO_default_environment ()
    );

  virtual TAO_Stub* set_policy_overrides (const CORBA::PolicyList & policies,
                                             CORBA::SetOverrideType set_add,
                                             CORBA::Environment &ACE_TRY_ENV =
                                             TAO_default_environment ());

#endif /* TAO_HAS_CORBA_MESSAGING */

  /// Creates a Stub Object.

  /// Returns the CORBA::Policy (which will be narrowed to be
  /// used as RTCORBA::PriorityModelPolicy) exported
  /// in object's IOR.
  CORBA::Policy_ptr exposed_priority_model (CORBA::Environment &ACE_TRY_ENV);

  /// Returns the CORBA::Policy (which will be narrowed and used
  /// as RTCORBA::PriorityBandedConnectionPolicy) exported
  /// in object's IOR.
  CORBA::Policy_ptr exposed_priority_banded_connection (CORBA::Environment &ACE_TRY_ENV);

  /// Returns the CORBA::Policy (which will be narrowed and used
  /// as RTCORBA::ClientProtocolPolicy) exported
  /// in object's IOR.
  CORBA::Policy_ptr exposed_client_protocol (CORBA::Environment &ACE_TRY_ENV);


  CORBA::Policy *private_connection (void);

  CORBA::Policy *priority_banded_connection (void);

  CORBA::Policy *client_protocol (void);

  // = Methods for obtaining effective policies.
  //
  //   Effective policy is the one that would be used if a request
  //   were made.  The policy is determined by obtaining the effective
  //   override for a given policy type, and then reconciling it with
  //   the policy value exported in the Object's IOR.

  CORBA::Policy *
  effective_priority_banded_connection (CORBA::Environment
                                        &ACE_TRY_ENV =
                                        TAO_default_environment ());
  CORBA::Policy *
  effective_client_protocol (CORBA::Environment &ACE_TRY_ENV =
                             TAO_default_environment ());



private:

  /// Helper method used to parse the policies.
  void parse_policies (CORBA::Environment &ACE_TRY_ENV);

  void exposed_priority_model (CORBA::Policy_ptr policy);

  void exposed_priority_banded_connection (CORBA::Policy_ptr policy);

  void exposed_client_protocol (CORBA::Policy_ptr policy);

private:

  // The following attribute are used to cache
  // the different kind of policies and avoid to
  // parse the MProfile's policy list each time we
  // are asked about a given policy.

  CORBA::Policy *priority_model_policy_;

  CORBA::Policy *priority_banded_connection_policy_;

  CORBA::Policy *client_protocol_policy_;

  CORBA::Boolean are_policies_parsed_;



private:
  // = Disallow copy constructor and assignment operator.
  ACE_UNIMPLEMENTED_FUNC (TAO_RT_Stub (const TAO_RT_Stub &))
  ACE_UNIMPLEMENTED_FUNC (TAO_RT_Stub &operator = (const TAO_RT_Stub &))

#if defined (__GNUG__)
  // G++ (even 2.6.3) stupidly thinks instances can't be created.
  // This de-warns.
  friend class everyone_needs_a_friend;
#endif /* __GNUG__ */
};

#endif /* TAO_HAS_RT_CORBA == 1 */


#include "ace/post.h"
#endif /* TAO_RT_STUB_FACTORY_H_ */
