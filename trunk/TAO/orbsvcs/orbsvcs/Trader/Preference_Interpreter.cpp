// Preference_Interpreter.cpp

// ========================================================================
// $Id$
//
// = LIBRARY 
//    orbsvcs
//  
// = FILENAME 
//    Preference_Interpreter.cpp
// 
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ========================================================================

#include "Preference_Interpreter.h"
#include <stdio.h>

const CORBA::Double TRADER_MAX_DOUBLE = 1.79769313486231570e+308;

TAO_Preference_Interpreter::
TAO_Preference_Interpreter(CosTradingRepos::ServiceTypeRepository::TypeStruct* ts,
			   const char* preference,
			   CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::Lookup::IllegalPreference))
  : TAO_Interpreter ()
{
  TAO_Constraint_Validator type_checker(ts);

  if (TAO_Interpreter::is_empty_string(preference))
    this->root_ = new TAO_Noop_Constraint(TAO_FIRST);
  else
    {
      if (this->build_tree (preference) != 0)
	TAO_THROW (CosTrading::Lookup::IllegalPreference (preference));	  

      if (type_checker.validate (this->root_) == -1)
	TAO_THROW (CosTrading::Lookup::IllegalPreference (preference));
    }
}

TAO_Preference_Interpreter::
TAO_Preference_Interpreter(TAO_Constraint_Validator& validator,
			   const char* preference,
			   CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::Lookup::IllegalPreference))
    : TAO_Interpreter ()
{
  if (TAO_Interpreter::is_empty_string(preference))
    this->root_ = new TAO_Noop_Constraint(TAO_FIRST);
  else
    {
      if (this->build_tree (preference) != 0)
	TAO_THROW (CosTrading::Lookup::IllegalPreference (preference));	  
      
      if (validator.validate (this->root_) == -1)
	TAO_THROW (CosTrading::Lookup::IllegalPreference (preference));
    }
}

TAO_Preference_Interpreter::~TAO_Preference_Interpreter()
{
}

void 
TAO_Preference_Interpreter::
order_offer (CosTrading::OfferId offer_id,
	     CosTrading::Offer* offer)
{
  TAO_Constraint_Evaluator evaluator(offer);
  this->order_offer (offer_id, offer, evaluator);
}

void 
TAO_Preference_Interpreter::
order_offer (CosTrading::OfferId offer_id,
	     CosTrading::Offer* offer,
	     TAO_Constraint_Evaluator& evaluator)
{
  if (this->root_ != 0)
    {
      TAO_Literal_Constraint return_value;	  
      TAO_Expression_Type expr_type = this->root_->expr_type ();
      if (evaluator.evaluate_preference (this->root_, return_value) == 0)
	this->offers_.insert (return_value, make_pair (offer_id, offer));
      else
	{
	  TAO_Literal_Constraint end ((CORBA::Double) TRADER_MAX_DOUBLE);
	  this->offers_.insert (end, make_pair (offer_id, offer));      
	}
    }
}

int
TAO_Preference_Interpreter::
remove_offer (CosTrading::OfferId& offer_id,
	      CosTrading::Offer*& offer)
{
  int return_value = 0;
  TAO_Expression_Type expr_type = this->root_->expr_type ();
  ORDERED_OFFERS::iterator offer_beg = this->offers_.begin ();
  ORDERED_OFFERS::iterator offer_end = this->offers_.end ();

  if (offer_beg != offer_end)
    {
      if (expr_type == TAO_WITH || expr_type == TAO_MAX)
	{
	  offer_end--;
	  offer_id = (*offer_end).second.first;
	  offer = (*offer_end).second.second;
	  this->offers_.erase (offer_end);
	}
      else
	{
	  offer_id = (*offer_beg).second.first;
	  offer = (*offer_beg).second.second;
	  this->offers_.erase (offer_beg);
	}
      
      return_value = 1;
    }
  
  return return_value;
}

int 
TAO_Preference_Interpreter::num_offers(void)
{
  return this->offers_.size();
}
