// $Id$

#include "Offer_Filter.h"

TAO_Offer_Filter::
TAO_Offer_Filter (SERVICE_TYPE_REPOS::TypeStruct* type_struct,
		  TAO_Policies& policies,
		  CORBA::Environment& _env)
  : search_card_ (policies.search_card (_env)),
    match_card_ (policies.match_card (_env)),
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
	this->mod_props_.insert (string (prop_seq[i].name));
    }
}

CORBA::Boolean
TAO_Offer_Filter::ok_to_consider (CosTrading::Offer* offer)
{
  CORBA::Boolean return_value = 1;
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
	      string prop_name (offer->properties[i].name);
	      if (this->mod_props_.find (prop_name) !=
		  this->mod_props_.end ())
		return_value = 0;
	    }

	  if (! this->dp_ && return_value)
	    {
	      // Determine if this property is dynamic.
	      if (prop_eval.is_dynamic_property (i))
		return_value = 0;
	    }

	  if (return_value == 0)
	    break;
	}
    }

  // If we're good to go, consider this offer considered and decrement 
  // the search cardinality counter. 
  if (return_value)
    this->search_card_--;

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
}
