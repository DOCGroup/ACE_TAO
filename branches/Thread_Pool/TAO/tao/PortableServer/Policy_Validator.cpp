// @(#) $Id$

#include "Policy_Validator.h"
#include "tao/Environment.h"

ACE_RCSID(tao, POA, "$Id$")

TAO_POA_Policy_Validator *TAO_POA_Policy_Validator::last_ = 0;

TAO_POA_Policy_Validator::TAO_POA_Policy_Validator (void)
  : next_ (0)
{
  // No-Op.
}

TAO_POA_Policy_Validator::~TAO_POA_Policy_Validator (void)
{
  delete this->next_;
}

void
TAO_POA_Policy_Validator::add_validator (TAO_POA_Policy_Validator *validator)
{
  if (this->last_ == 0)
      this->last_ = this->next_ = validator;
  else
    this->last_ = this->last_->next_ = validator;
}


void
TAO_POA_Policy_Validator::validate (TAO_Policy_Set &policies,
                                         CORBA::Environment &ACE_TRY_ENV)
{
  this->validate_impl (policies, ACE_TRY_ENV);
  ACE_CHECK;

  if (this->next_ != 0)
    {
      this->next_->validate (policies, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

CORBA::Boolean
TAO_POA_Policy_Validator::legal_policy (CORBA::PolicyType type)
{
  return (this->legal_policy_impl (type) ||
          ((this->next_ != 0) && this->next_->legal_policy_impl (type)));
}


