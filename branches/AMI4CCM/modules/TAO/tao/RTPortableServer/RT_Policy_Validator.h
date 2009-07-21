// -*- C++ -*-

//=============================================================================
/**
 *  @file    RT_Policy_Validator.h
 *
 *  $Id$
 *
 *  This file contains the declaration for the RTCORBA policy validator.
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 *  @author  Frank Hunleth  <fhuntleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_RT_POLICY_VALIDATOR_H_
#define TAO_RT_POLICY_VALIDATOR_H_

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/RTPortableServer/rtportableserver_export.h"
#include "tao/Policy_Validator.h"
#include "tao/RTCORBA/RTCORBA.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;
class TAO_Acceptor_Registry;
class TAO_Thread_Pool;

class TAO_RTPortableServer_Export TAO_POA_RT_Policy_Validator
  : public TAO_Policy_Validator
{
public:
  /// Constructor.
  TAO_POA_RT_Policy_Validator (TAO_ORB_Core &orb_core);

  /// Destructor.
  ~TAO_POA_RT_Policy_Validator (void);

  static RTCORBA::ServerProtocolPolicy_ptr server_protocol_policy_from_thread_pool (TAO_Thread_Pool *thread_pool,
                                                                                    TAO_ORB_Core &orb_core);

  static void server_protocol_policy_from_acceptor_registry (RTCORBA::ProtocolList &protocols,
                                                             TAO_Acceptor_Registry &acceptor_registry,
                                                             TAO_ORB_Core &orb_core);

  static TAO_Thread_Pool *extract_thread_pool (TAO_ORB_Core &orb_core,
                                               TAO_Policy_Set &policies);

protected:

  /**
   * Validate that the policies in the specified set
   * are consistent and legal.  Throw an appropriate exception
   * if that is not the case.
   */
  void validate_impl (TAO_Policy_Set &policies);

  /// Add/merge policies.
  void merge_policies_impl (TAO_Policy_Set &policies);

  /**
   * Return whether the specified policy type is legal for the
   * current configuration.  This is needed since the user can
   * potentially specify policies that are unknown to an
   * validate () routine, and these need to be caught.
   */
  virtual CORBA::Boolean legal_policy_impl (CORBA::PolicyType type);

private:
  void validate_server_protocol (TAO_Policy_Set &policies);

  void validate_priorities (TAO_Policy_Set &policies);

  void validate_thread_pool (TAO_Policy_Set &policies);

  TAO_Thread_Pool *thread_pool_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"

#endif /* TAO_RT_POLICY_VALIDATOR_H_ */
