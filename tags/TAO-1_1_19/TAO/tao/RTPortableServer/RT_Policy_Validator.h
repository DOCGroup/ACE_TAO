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

#include "ace/pre.h"
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "rtportableserver_export.h"
#include "tao/PortableServer/Policy_Validator.h"

class TAO_ORB_Core;

class TAO_RTPortableServer_Export TAO_POA_RT_Policy_Validator
  : public TAO_POA_Policy_Validator
{
public:
  /// Constructor.
  TAO_POA_RT_Policy_Validator (TAO_ORB_Core *orb_core);

  /// Destructor.
  ~TAO_POA_RT_Policy_Validator (void);

protected:

  /**
   * Validate that the policies in the specified set
   * are consistent and legal.  Throw an appropriate exception
   * if that is not the case.
   */
  virtual void validate_impl (TAO_Policy_Set &policies,
                              CORBA::Environment &ACE_TRY_ENV);

  /**
   * Return whether the specified policy type is legal for the
   * current configuration.  This is needed since the user can
   * potentially specify policies that are unknown to an
   * validate () routine, and these need to be caught.
   */
  virtual CORBA::Boolean legal_policy_impl (CORBA::PolicyType type);

private:
  void validate_server_protocol (TAO_Policy_Set &policies,
                                 CORBA::Environment &ACE_TRY_ENV);

  void validate_priorities (TAO_Policy_Set &policies,
                            CORBA::Environment &ACE_TRY_ENV);


private:
  TAO_ORB_Core *orb_core_;
};

#include "ace/post.h"
#endif /* TAO_RT_POLICY_VALIDATOR_H_ */


