//=============================================================================
/**
 *  @file    Default_Policy_Validator.h
 *
 *  $Id$
 *
 *  This file contains the declaration for the default POA policy validator.
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 *  @author  Frank Hunleth  <fhuntleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_DEFAULT_POLICY_VALIDATOR_H_
#define TAO_DEFAULT_POLICY_VALIDATOR_H_

#include "ace/pre.h"

#include "Policy_Validator.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_PortableServer_Export TAO_POA_Default_Policy_Validator
  : public TAO_POA_Policy_Validator
{
public:

  /// Constructor.
  TAO_POA_Default_Policy_Validator (TAO_ORB_Core &orb_core);

  /// Destructor.
  ~TAO_POA_Default_Policy_Validator (void);

protected:
  /**
   * Validate that the policies in the specified set
   * are consistent and legal.  Throw an appropriate exception
   * if that is not the case.
   */
  void validate_impl (TAO_Policy_Set &policies
                      TAO_ENV_ARG_DECL);

  /// Add/merge policies.
  void merge_policies_impl (TAO_Policy_Set &policies
                            TAO_ENV_ARG_DECL);

  /**
   * Return whether the specified policy type is legal for the
   * current configuration.  This is needed since the user can
   * potentially specify policies that are unknown to an
   * validate () routine, and these need to be caught.
   */
  virtual CORBA::Boolean legal_policy_impl (CORBA::PolicyType type);

};

#include "ace/post.h"
#endif /* TAO_DEFAULT_POLICY_VALIDATOR_H_ */
