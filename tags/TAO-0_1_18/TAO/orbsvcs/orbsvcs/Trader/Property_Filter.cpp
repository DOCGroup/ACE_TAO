// $Id$

#include "Property_Filter.h"

TAO_Property_Filter::
TAO_Property_Filter (const SPECIFIED_PROPS& desired_props,
		     CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::IllegalPropertyName,
		   CosTrading::DuplicatePropertyName))
  : policy_  (desired_props._d ())
{
  if (this->policy_ == CosTrading::Lookup::some)
    {
      const PropertyNameSeq& prop_seq = desired_props.prop_names ();
      int length = prop_seq.length ();

      for (int i = 0; i < length; i++)
	{
	  const char* pname = prop_seq[i];

	  // Check for errors or duplicates
	  if (! TAO_Trader_Base::is_valid_identifier_name (pname))
	    TAO_THROW_SPEC (CosTrading::IllegalPropertyName (pname));
	  else
	    {
	      // Insert returns a pair whose second element is a flag
	      // indicating whether a collision occured.
	      string prop_name (pname);
	      if (this->props_.insert (prop_name).second == 0)
		TAO_THROW_SPEC (CosTrading::DuplicatePropertyName (pname));
	    }	    
	}
    }
}

TAO_Property_Filter::
TAO_Property_Filter (const TAO_Property_Filter& prop_filter)
  : props_ (prop_filter.props_),
    policy_ (prop_filter.policy_)
{  
}

void
TAO_Property_Filter::filter_offer (CosTrading::Offer& source,
				   CosTrading::Offer& destination)
{
  PROP_QUEUE prop_queue;
  CosTrading::PropertySeq& s_props = source.properties;
  CosTrading::PropertySeq& d_props = destination.properties;
  int length = s_props.length (), elem = 0;

  destination.reference = source.reference->_duplicate (source.reference);
  if (this->policy_ != CosTrading::Lookup::none)
    {
      for (int i = 0; i < length; i++)
	{
	  if (this->policy_ == CosTrading::Lookup::all)
	    prop_queue.push_back (&s_props[i]);
	  else
	    {
	      string prop_name (s_props[i].name);

	      // Save those property that match.
	      if (this->props_.find (prop_name) != this->props_.end ())
		prop_queue.push_back (&s_props[i]);
	    }
	}

      // Shove the matched properties into the destination property
      // sequence. 
      length = prop_queue.size ();
      d_props.length (prop_queue.size ());
      for (PROP_QUEUE::iterator prop_iter = prop_queue.begin ();
	   prop_iter != prop_queue.end ();
	   prop_iter++)      
	d_props[elem++] = *(*prop_iter);
    }
}

int
TAO_Property_Filter::verify_property_name (const char* name)
{
  int return_value = -1,
    length = ACE_OS::strlen (name);

  if (length >= 1 && isalpha (name[0]))
      {
	for (int i = 0; i < length; i++)
	  {
	    if (! (isalnum (name[i]) || name[i] == '_'))
	      break;
	  }

	return_value = 0;
      }

  return return_value;
}
