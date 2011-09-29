// $Id$

#include "tao/Policy_Validator.h"
#include "tao/debug.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Policy_Validator::TAO_Policy_Validator (TAO_ORB_Core &orb_core)
  : orb_core_ (orb_core),
    next_ (0)
{
}

TAO_Policy_Validator::~TAO_Policy_Validator (void)
{
  delete this->next_;
}

TAO_ORB_Core &
TAO_Policy_Validator::orb_core() const
{
  return this->orb_core_;
}

void
TAO_Policy_Validator::add_validator (TAO_Policy_Validator *validator)
{
  // The validator we're adding can't be part of another list
  ACE_ASSERT (validator->next_ == 0);

  // Why would we want to add ourself to our list
  if (this != validator)
    {
      // Get to the end of the list and make sure that the
      // new validator isn't already part of our list
      TAO_Policy_Validator* current = this;
      while (current->next_ != 0)
        {
          if (current->next_ == validator)
            {
              if (TAO_debug_level > 3)
                {
                  ACE_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("(%P|%t) Skipping validator [%@] ")
                              ACE_TEXT ("since it would create a circular list\n"),
                              validator));
                }

              return;
            }
          current = current->next_;
        }

      // Add the new validator to the end of the list
      current->next_ = validator;
    }
}


void
TAO_Policy_Validator::validate (TAO_Policy_Set &policies)
{
  this->validate_impl (policies);

  if (this->next_ != 0)
    {
      this->next_->validate (policies);
    }
}

void
TAO_Policy_Validator::merge_policies (TAO_Policy_Set &policies)
{
  this->merge_policies_impl (policies);

  if (this->next_)
    {
      this->next_->merge_policies (policies);
    }
}

CORBA::Boolean
TAO_Policy_Validator::legal_policy (CORBA::PolicyType type)
{
  return (this->legal_policy_impl (type)
          || ((this->next_ != 0)
              && this->next_->legal_policy_impl (type)));
}

TAO_END_VERSIONED_NAMESPACE_DECL
