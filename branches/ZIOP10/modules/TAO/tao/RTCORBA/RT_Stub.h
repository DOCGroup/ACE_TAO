// -*- C++ -*-

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
 *  @author  Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_RT_STUB_H_
#define TAO_RT_STUB_H_

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/rtcorba_export.h"

#include "tao/Stub.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward references.
class TAO_PriorityModelPolicy;
class TAO_PriorityBandedConnectionPolicy;
class TAO_ClientProtocolPolicy;
class TAO_PrivateConnectionPolicy;

/**
 * @class TAO_RT_Stub
 *
 * This class represent a stub with extended functionality, which are needed
 * for RTCORBA.
 */
class TAO_RTCORBA_Export TAO_RT_Stub : public TAO_Stub
{
public:

  TAO_RT_Stub (const char *repository_id,
               const TAO_MProfile &profiles,
               TAO_ORB_Core *orb_core);

  virtual ~TAO_RT_Stub (void);


#if (TAO_HAS_CORBA_MESSAGING == 1)

  /**
   * Returns the effective policy if @a type is a known client-exposed
   * policy type.  Returns the effective override for all other policy
   * types.
   */
  CORBA::Policy_ptr get_policy (CORBA::PolicyType type);

  CORBA::Policy_ptr get_cached_policy (TAO_Cached_Policy_Type type);

  TAO_Stub* set_policy_overrides (const CORBA::PolicyList & policies,
                                  CORBA::SetOverrideType set_add);

#endif /* TAO_HAS_CORBA_MESSAGING */

private:

  /// Helper method used to parse the policies.
  void parse_policies (void);

  void exposed_priority_model (CORBA::Policy_ptr policy);

  void exposed_priority_banded_connection (CORBA::Policy_ptr policy);

  void exposed_client_protocol (CORBA::Policy_ptr policy);

  /// Returns the CORBA::Policy (which will be narrowed to be
  /// used as RTCORBA::PriorityModelPolicy) exported
  /// in object's IOR.
  CORBA::Policy_ptr exposed_priority_model (void);

  /// Returns the CORBA::Policy (which will be narrowed and used
  /// as RTCORBA::PriorityBandedConnectionPolicy) exported
  /// in object's IOR.
  CORBA::Policy_ptr exposed_priority_banded_connection (void);

  /// Returns the CORBA::Policy (which will be narrowed and used
  /// as RTCORBA::ClientProtocolPolicy) exported
  /// in object's IOR.
  CORBA::Policy_ptr exposed_client_protocol (void);

  // = Methods for obtaining effective policies.
  //
  //   Effective policy is the one that would be used if a request
  //   were made.  The policy is determined by obtaining the effective
  //   override for a given policy type, and then reconciling it with
  //   the policy value exported in the Object's IOR.

  CORBA::Policy_ptr effective_priority_banded_connection (void);
  CORBA::Policy_ptr effective_client_protocol (void);

  // The following attribute are used to cache
  // the different kind of policies and avoid to
  // parse the MProfile's policy list each time we
  // are asked about a given policy.

  CORBA::Policy_var priority_model_policy_;

  CORBA::Policy_var priority_banded_connection_policy_;

  CORBA::Policy_var client_protocol_policy_;

  bool are_policies_parsed_;

private:
  // = Disallow copying and assignment.
  TAO_RT_Stub (const TAO_RT_Stub &);
  TAO_RT_Stub &operator = (const TAO_RT_Stub &);

};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"

#endif /* TAO_RT_STUB_FACTORY_H_ */
