// @(#) $Id$

#include "Policy_Validator.h"
#include "Environment.h"
#include "ORB_Core.h"

ACE_RCSID(tao, Policy_Validator, "$Id$")

TAO_Policy_Validator *TAO_Policy_Validator::last_ = 0;

TAO_Policy_Validator::TAO_Policy_Validator (TAO_ORB_Core &orb_core)
  : orb_core_ (orb_core),
    next_ (0)
{
  // No-Op.
}

TAO_Policy_Validator::~TAO_Policy_Validator (void)
{
  delete this->next_;
}

void
TAO_Policy_Validator::add_validator (TAO_Policy_Validator *validator)
{
  if (this->last_ == 0)
    this->last_ = this->next_ = validator;
  else
    this->last_ = this->last_->next_ = validator;
}


void
TAO_Policy_Validator::validate (TAO_Policy_Set &policies
                                    TAO_ENV_ARG_DECL)
{
  this->validate_impl (policies TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->next_ != 0)
    {
      this->next_->validate (policies TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_Policy_Validator::merge_policies (TAO_Policy_Set &policies
                                      TAO_ENV_ARG_DECL)
{
  this->merge_policies_impl (policies TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->next_ != 0)
    {
      this->next_->merge_policies (policies TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

CORBA::Boolean
TAO_Policy_Validator::legal_policy (CORBA::PolicyType type)
{
  return (this->legal_policy_impl (type) ||
          ((this->next_ != 0) && this->next_->legal_policy_impl (type)));
}
