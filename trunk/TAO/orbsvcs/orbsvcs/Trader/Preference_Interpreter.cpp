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
      Preference_Info pref_info;
      TAO_Expression_Type expr_type = this->root_->expr_type ();
      
      pref_info.offer_ = offer;
      pref_info.offer_id_ = offer_id;
      pref_info.evaluated_ = CORBA::B_TRUE;

      if (evaluator.evaluate_preference (this->root_, pref_info.value_) == 0)
	{	  
	  // If the evaluation succeeds, insert the node into the
	  // correct place in the queue.
	  TAO_Expression_Type expr_type = this->root_->expr_type ();

	  if (expr_type == TAO_FIRST)
	    this->offers_.enqueue_tail (pref_info);
	  else
	    this->offers_.enqueue_head (pref_info);

	  if (expr_type == TAO_MIN || expr_type == TAO_MAX)
	    {	      
	      Ordered_Offers::ITERATOR offer_iter (this->offers_);

	      // Push the new item down the list until the min/max
	      // criterion is satisfied. Observe the evaluation
	      // failed / evaluation suceeded partion in the list.
	      offer_iter.advance ();
	      for (int i = 1;
		   ! offer_iter.done ();
		   offer_iter.advance (), i++)
		{
		  Preference_Info* current_offer;
		  offer_iter.next (current_offer);

		  // Maintain the sorted order in the first partition.
		  if (current_offer->evaluated_ == CORBA::B_TRUE &&
		      ((expr_type == TAO_MIN &&
		       pref_info.value_ > current_offer->value_) ||
		      (expr_type == TAO_MAX &&
		       pref_info.value_ < current_offer->value_)))
		    {
		      this->offers_.set (*current_offer, i - 1);
		      this->offers_.set (pref_info, i);
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
	  pref_info.evaluated_ = CORBA::B_FALSE;
	  this->offers_.enqueue_tail (pref_info);
	}
    }
}

int
TAO_Preference_Interpreter::
remove_offer (CosTrading::OfferId& offer_id,
	      CosTrading::Offer*& offer)
{
  int return_value = -1;
  Preference_Info pref_info;

  return_value = this->offers_.dequeue_head (pref_info);

  if (return_value == 0)
    {
      offer = pref_info.offer_;
      offer_id = pref_info.offer_id_;
    }
  
  return return_value;
}

int 
TAO_Preference_Interpreter::num_offers(void)
{
  return this->offers_.size();
}
