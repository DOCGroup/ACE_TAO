//=============================================================================
/**
 *  @file    BiDirPolicy_Validator.h
 *
 *  $Id$
 *
 *  This file contains the declaration for the BiDir policy validator
 *  interface.
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 *  @author  Frank Hunleth  <fhuntleth@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_POLICY_VALIDATOR_H_
#define TAO_POLICY_VALIDATOR_H_

#include "ace/pre.h"
#include "bidirgiop_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "tao/Policy_Validator.h"


class TAO_Policy_Set;
class TAO_ORB_Core;

/**
 * @class TAO_BiDirPolicy_Validator
 *
 * @brief Policy Validator for BiDir GIOP class
 *
 */

class TAO_BiDirGIOP_Export TAO_BiDirPolicy_Validator : public TAO_Policy_Validator
{
public:

  TAO_BiDirPolicy_Validator (TAO_ORB_Core &orb_core);

  virtual void validate_impl (TAO_Policy_Set &policies
                              TAO_ENV_ARG_DECL);

  virtual void merge_policies_impl (TAO_Policy_Set &policies
                                    TAO_ENV_ARG_DECL);

  virtual CORBA::Boolean legal_policy_impl (CORBA::PolicyType type);

private:

};

#endif /**/
