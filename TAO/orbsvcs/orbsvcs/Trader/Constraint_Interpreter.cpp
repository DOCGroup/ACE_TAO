#include "orbsvcs/Trader/Constraint_Interpreter.h"
#include "orbsvcs/Trader/Trader_Constraint_Visitors.h"
#include "orbsvcs/Trader/Constraint_Tokens.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Constraint_Interpreter::TAO_Constraint_Interpreter (
    const CosTradingRepos::ServiceTypeRepository::TypeStruct& ts,
    const char* constraints
  )
  : TAO_Interpreter ()
{
  // @@ Throwing Exception from constructor is very nasty situation to
  // deal with.

  TAO_Trader_Constraint_Validator type_checker (ts);

  if (TAO_Interpreter::is_empty_string (constraints))
    {
      ACE_NEW_THROW_EX (this->root_,
                        TAO_Literal_Constraint ((CORBA::Boolean) 1),
                        CORBA::NO_MEMORY ());
    }
  else
    {
      if (this->build_tree (constraints) != 0)
        throw CosTrading::IllegalConstraint (constraints);

      if (type_checker.validate (this->root_) == -1)
        throw CosTrading::IllegalConstraint (constraints);
    }
}

TAO_Constraint_Interpreter::
TAO_Constraint_Interpreter (TAO_Constraint_Validator& validator,
                           const char* constraints)
{
  if (TAO_Interpreter::is_empty_string (constraints))
    {
      ACE_NEW_THROW_EX (this->root_,
                        TAO_Literal_Constraint ((CORBA::Boolean) 1),
                        CORBA::NO_MEMORY ());
    }
  else
    {
      if (this->build_tree (constraints) != 0)
        throw CosTrading::IllegalConstraint (constraints);

      if (validator.validate (this->root_) == -1)
        throw CosTrading::IllegalConstraint (constraints);
    }
}

TAO_Constraint_Interpreter::~TAO_Constraint_Interpreter ()
{
}

CORBA::Boolean
TAO_Constraint_Interpreter::evaluate (CosTrading::Offer* offer)
{
  TAO_Trader_Constraint_Evaluator evaluator (offer);
  return evaluator.evaluate_constraint (this->root_);
}

CORBA::Boolean
TAO_Constraint_Interpreter::evaluate (TAO_Constraint_Evaluator& evaluator)
{
  return evaluator.evaluate_constraint (this->root_);
}

TAO_Preference_Interpreter::TAO_Preference_Interpreter (
    const CosTradingRepos::ServiceTypeRepository::TypeStruct& ts,
    const char* preference)
    : TAO_Interpreter ()
{
  TAO_Trader_Constraint_Validator type_checker (ts);

  if (TAO_Interpreter::is_empty_string (preference))
    {
      ACE_NEW_THROW_EX (this->root_,
                        TAO_Noop_Constraint (TAO_FIRST),
                        CORBA::NO_MEMORY ());
    }
  else
    {
      if (this->build_tree (preference) != 0)
        throw CosTrading::Lookup::IllegalPreference (preference);

      if (type_checker.validate (this->root_) == -1)
        throw CosTrading::Lookup::IllegalPreference (preference);
    }
}

TAO_Preference_Interpreter::
TAO_Preference_Interpreter (TAO_Constraint_Validator& validator,
                           const char* preference)
    : TAO_Interpreter ()
{
  if (TAO_Interpreter::is_empty_string (preference))
    {
      ACE_NEW_THROW_EX (this->root_,
                        TAO_Noop_Constraint (TAO_FIRST),
                        CORBA::NO_MEMORY ());
    }
  else
    {
      if (this->build_tree (preference) != 0)
        throw CosTrading::Lookup::IllegalPreference (preference);

      if (validator.validate (this->root_) == -1)
        throw CosTrading::Lookup::IllegalPreference (preference);
    }
}

TAO_Preference_Interpreter::~TAO_Preference_Interpreter ()
{
}

void
TAO_Preference_Interpreter::
order_offer (CosTrading::Offer* offer,
             CosTrading::OfferId offer_id)
{
  TAO_Trader_Constraint_Evaluator evaluator (offer);
  this->order_offer (evaluator, offer, offer_id);
}

void
TAO_Preference_Interpreter::
order_offer (TAO_Constraint_Evaluator& evaluator,
             CosTrading::Offer* offer,
             CosTrading::OfferId offer_id)
{
  if (this->root_ != nullptr)
    {
      Preference_Info pref_info;

      pref_info.offer_ = offer;
      pref_info.offer_id_ = offer_id;
      pref_info.evaluated_ = true;

      if (evaluator.evaluate_preference (this->root_, pref_info.value_) == 0)
        {
          // If the evaluation succeeds, insert the node into the
          // correct place in the queue.
          TAO_Expression_Type expr_type = this->root_->expr_type ();

          if (expr_type == TAO_FIRST
              || (expr_type == TAO_WITH
                  && ! static_cast<CORBA::Boolean> (pref_info.value_)))
            this->offers_.push_back (pref_info);
          else
            this->offers_.push_front (pref_info);

          if (expr_type == TAO_MIN || expr_type == TAO_MAX)
            {
              // Push the new item down the list until the min/max
              // criterion is satisfied. Observe the evaluation failed
              // / evaluation suceeded partion in the list.
              for (size_t i = 1; i < this->offers_.size (); ++i)
                {
                  Preference_Info const &current_offer = this->offers_[i];

                  // Maintain the sorted order in the first partition.
                  if (current_offer.evaluated_
                      && ((expr_type == TAO_MIN
                        && pref_info.value_ > current_offer.value_)
                       || (expr_type == TAO_MAX
                           && pref_info.value_ < current_offer.value_)))
                    {
                      // Swap the out of order pair
                      this->offers_[i - 1] = current_offer;
                      this->offers_[i] = pref_info;
                    }
                  else
                    break;
                }
            }
        }
      else
        {
          // If the evaluation fails, just tack the sucker onto the
          // end of the queue.
          pref_info.evaluated_ = false;
          this->offers_.push_back (pref_info);
        }
    }
}

int
TAO_Preference_Interpreter::
remove_offer (CosTrading::Offer*& offer,
              CosTrading::OfferId& offer_id)
{
  int return_value = -1;
  Preference_Info pref_info;

  if (! this->offers_.empty ())
    {
      pref_info = this->offers_.front ();
      this->offers_.pop_front ();
      return_value = 0;
    }

  if (return_value == 0)
    {
      offer = pref_info.offer_;
      offer_id = pref_info.offer_id_;
    }

  return return_value;
}

int
TAO_Preference_Interpreter::
remove_offer (CosTrading::Offer*& offer)
{
  CosTrading::OfferId offer_id = 0;
  return this->remove_offer (offer, offer_id);
}


size_t
TAO_Preference_Interpreter::num_offers ()
{
  return this->offers_.size ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
