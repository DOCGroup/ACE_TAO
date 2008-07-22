// -*- C -*-

//=============================================================================
/**
 *  @file    ZIOP_Policy_Validator.h
 *
 *  $Id$
 *
 *  This file contains the declaration for the ZIOP policy validator
 *  interface.
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ZIOP_POLICY_VALIDATOR_H_
#define TAO_ZIOP_POLICY_VALIDATOR_H_

#include /**/ "ace/pre.h"
#include "tao/ZIOP/ZIOP_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "tao/Policy_Validator.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Policy_Set;
class TAO_ORB_Core;

/**
 * @class TAO_ZIOPPolicy_Validator
 *
 * @brief Policy Validator for ZIOP policies
 *
 */
class TAO_ZIOP_Export TAO_ZIOPPolicy_Validator : public TAO_Policy_Validator
{
public:

  TAO_ZIOPPolicy_Validator (TAO_ORB_Core &orb_core);

  virtual void validate_impl (TAO_Policy_Set &policies);

  virtual void merge_policies_impl (TAO_Policy_Set &policies);

  virtual CORBA::Boolean legal_policy_impl (CORBA::PolicyType type);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_ZIOP_POLICY_VALIDATOR_H_ */


