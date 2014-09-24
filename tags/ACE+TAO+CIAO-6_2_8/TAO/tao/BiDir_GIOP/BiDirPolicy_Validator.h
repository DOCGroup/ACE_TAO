// -*- C++ -*-

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

#ifndef TAO_BIDIR_POLICY_VALIDATOR_H_
#define TAO_BIDIR_POLICY_VALIDATOR_H_

#include /**/ "ace/pre.h"
#include "tao/BiDir_GIOP/bidirgiop_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "tao/Policy_Validator.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Policy_Set;
class TAO_ORB_Core;

/**
 * @class TAO_BiDirPolicy_Validator
 *
 * @brief Policy Validator for BiDir GIOP class
 *
 */
class TAO_BIDIRGIOP_Export TAO_BiDirPolicy_Validator : public TAO_Policy_Validator
{
public:
  TAO_BiDirPolicy_Validator (TAO_ORB_Core &orb_core);

  virtual void validate_impl (TAO_Policy_Set &policies);

  virtual void merge_policies_impl (TAO_Policy_Set &policies);

  virtual CORBA::Boolean legal_policy_impl (CORBA::PolicyType type);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_BIDIR_POLICY_VALIDATOR_H_ */
