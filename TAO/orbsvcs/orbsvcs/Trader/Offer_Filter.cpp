// $Id$

#include "Offer_Filter.h"

TAO_Offer_Filter::
TAO_Offer_Filter (SERVICE_TYPE_REPOS::TypeStruct* type_struct,
		  TAO_Policies& policies,
		  CORBA::Environment& _env)
  : search_card_ (policies.search_card (_env)),
    match_card_ (policies.match_card (_env)),
    return_card_ (policies.return_card (_env)),
    dp_ (policies.use_dynamic_properties (_env)),
    mod_ (policies.use_modifiable_properties (_env))
{
  SERVICE_TYPE_REPOS::PropStructSeq& prop_seq = type_struct->props;

  // Take note of modifiable properties in the type_struct
  for (int i = prop_seq.length () - 1; i >= 0; i--)
    {
      SERVICE_TYPE_REPOS::PropertyMode mode = prop_seq[i].mode;
      if (mode != SERVICE_TYPE_REPOS::PROP_MANDATORY_READONLY ||
	  mode != SERVICE_TYPE_REPOS::PROP_READONLY)
	{
	  TAO_String_Hash_Key prop_name ((const char*) prop_seq[i].name);
	  this->mod_props_.insert (prop_name);
	}
    }

  if (policies.exact_type_match (_env) == CORBA::B_TRUE)
    {
      TAO_String_Hash_Key exact_match
	(TAO_Policies::POLICY_NAMES[TAO_Policies::EXACT_TYPE_MATCH]);
      this->limits_.insert (exact_match);
    }
    
}

CORBA::Boolean
TAO_Offer_Filter::ok_to_consider (CosTrading::Offer* offer)
{
  TAO_String_Hash_Key use_mods =
    TAO_Policies::POLICY_NAMES[TAO_Policies::USE_MODIFIABLE_PROPERTIES];
  TAO_String_Hash_Key use_dyns =
    TAO_Policies::POLICY_NAMES[TAO_Policies::USE_DYNAMIC_PROPERTIES];
  CORBA::Boolean return_value = CORBA::B_TRUE;
  TAO_Property_Evaluator prop_eval (*offer);

  // If we should screen offers, determine if this offer is unworthy
  // for consideration. 
  if (! (this->mod_ && this->dp_))
    {
      for (int i = offer->properties.length () - 1;
	   i >= 0 && return_value;
	   i--)
	{
	  // Winnow away the unwanted offers with modifiable or
	  // dynamic properties.
	  if (! this->mod_)
	    {
	      // Determine if this property name is found in the set
	      // of modifiable properties for the type being considered.
	      TAO_String_Hash_Key prop_name ((const char*) offer->properties[i].name);
	      if (this->mod_props_.find (prop_name) == 0)
		{
		  this->limits_.insert (use_mods);
		  return_value = 0;
		}
	    }

	  if (! this->dp_ && return_value)
	    {
	      // Determine if this property is dynamic.
	      if (prop_eval.is_dynamic_property (i))
		{
		  this->limits_.insert (use_dyns);
		  return_value = 0;
		}
	    }

	  if (return_value == 0)
	    break;
	}
    }

  // If we're good to go, consider this offer considered and decrement 
  // the search cardinality counter. 
  if (return_value)
    {
      this->search_card_--;
      if (this->search_card_ == 0)
	{
	  TAO_String_Hash_Key search_card =
	    TAO_Policies::POLICY_NAMES[TAO_Policies::SEARCH_CARD];
	  this->limits_.insert (search_card);
	}
    }

  return return_value;
}

CORBA::Boolean
TAO_Offer_Filter::ok_to_consider_more (void)
{
  return this->search_card_ > 0 && this->match_card_ > 0;
}

void
TAO_Offer_Filter::matched_offer (void)
{
  this->match_card_--;
  this->return_card_--;

  if (this->match_card_ == 0)
    {
      TAO_String_Hash_Key match_card =
	TAO_Policies::POLICY_NAMES[TAO_Policies::MATCH_CARD];
      this->limits_.insert (match_card);
    }

  if (this->return_card_ == 0)
    {
      TAO_String_Hash_Key return_card =
	TAO_Policies::POLICY_NAMES[TAO_Policies::MATCH_CARD];
      this->limits_.insert (return_card);
    }
}

CosTrading::PolicyNameSeq*
TAO_Offer_Filter::limits_applied (void)
{
  int i = 0;
  CORBA::ULong size = this->limits_.size ();
  CosTrading::PolicyName* temp =
    CosTrading::PolicyNameSeq::allocbuf (size);

  for (Names::iterator p_iter (this->limits_.begin());
       ! p_iter.done ();
       p_iter.advance ())
    {
      TAO_String_Hash_Key* policy_name_ptr = 0;
      p_iter.next (policy_name_ptr);
      temp[i++] = CORBA::string_dup ((const char*) *policy_name_ptr);
    }
  
  return new CosTrading::PolicyNameSeq (i, i, temp, 1);
}
