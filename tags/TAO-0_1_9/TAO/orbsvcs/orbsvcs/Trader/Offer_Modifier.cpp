// $Id$

#include "Offer_Modifier.h"

TAO_Offer_Modifier::
TAO_Offer_Modifier (const char* type_name,
		    TYPE_STRUCT* type_struct,
		    CosTrading::Offer& offer)
  : offer_ (offer),
    type_ (type_name)
{
  typedef CosTradingRepos::ServiceTypeRepository SERVICE_TYPE_REPOS;

  CosTrading::PropertySeq& prop_seq = this->offer_.properties;
  SERVICE_TYPE_REPOS::PropStructSeq& pstructs = type_struct->props;
  int pstructs_length = pstructs.length (),
    props_length = prop_seq.length ();

  // Separate the type defined properties into mandatory and readonly
  for (int i = 0; i < pstructs_length; i++)
    {
      if (pstructs[i].mode == SERVICE_TYPE_REPOS::PROP_MANDATORY)
	this->mandatory_.insert (string (pstructs[i].name));
      else if (pstructs[i].mode == SERVICE_TYPE_REPOS::PROP_READONLY)
	this->readonly_.insert (string (pstructs[i].name));
    }

  // Insert the properties of the offer into a map.
  for (i = 0; i < props_length; i++)
    this->props_[string (prop_seq[i].name)] = &prop_seq[i];
}

void
TAO_Offer_Modifier::
delete_properties (const CosTrading::PropertyNameSeq& deletes,
		   CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::Register::UnknownPropertyName, 
		  CosTrading::Register::MandatoryProperty,
		  CosTrading::IllegalPropertyName,
		  CosTrading::DuplicatePropertyName))
{
  PROP_NAMES delete_me;
  // Validate that the listed property names can be deleted
  for (int i = 0, length = deletes.length (); i < length; i++)
    {
      CosTrading::PropertyName dname = (char*)((const char *) deletes[i]);
      if (! TAO_Trader_Base::is_valid_identifier_name (dname))
	TAO_THROW (CosTrading::IllegalPropertyName (dname));
      else
	{
	  string prop_name (dname);
	  if (this->mandatory_.find (prop_name) != this->mandatory_.end ())
	    TAO_THROW (CosTrading::Register::MandatoryProperty (this->type_, dname));
	  else if ((delete_me.insert (prop_name)).second == 0)
	    TAO_THROW (CosTrading::DuplicatePropertyName (dname));
	  else if (this->props_.find (prop_name) == this->props_.end ())
	    TAO_THROW (CosTrading::Register::UnknownPropertyName (dname));
	}
    }

  // Delete those properties from the offer.
  for (i = 0; i < length; i++)
    {
      string prop_name = (const char *) deletes[i];
      this->props_.erase (prop_name);
    } 
}

void
TAO_Offer_Modifier::
merge_properties (const CosTrading::PropertySeq& modifies,
		  CORBA::Environment& _env)
  TAO_THROW_SPEC ((CosTrading::IllegalPropertyName,
		   CosTrading::DuplicatePropertyName,
		   CosTrading::Register::ReadonlyProperty))
{
  PROP_NAMES modify_me;
  // Ensure that the proposed changes aren't to readonly properties or
  // otherwise invalid.
  for (int i = 0, length = modifies.length (); i < length; i++)
    {
      CosTrading::PropertyName mname =
	(char*) ((const char *) modifies[i].name);
      if (! TAO_Trader_Base::is_valid_identifier_name (mname))
	TAO_THROW (CosTrading::IllegalPropertyName (mname));
      else
	{
	  string prop_name (mname);
	  if (this->readonly_.find (prop_name) != this->readonly_.end () &&
	      this->props_.find (prop_name) == this->props_.end ())
	    TAO_THROW (CosTrading::Register::ReadonlyProperty (this->type_, mname));
	  else if ((modify_me.insert (prop_name)).second == 0)
	    TAO_THROW (CosTrading::DuplicatePropertyName (mname));
	}
    }

  for (i = 0; i < length; i++)
    {
      // Add a property to the destination if it doesn't already exist.
      pair <PROPS::iterator, bool> insert_return = 
	this->props_.insert (make_pair (string (modifies[i].name),
					(CosTrading::Property *) &modifies[i]));

      // Modify a property if it already exists in the destination.
      if (! insert_return.second)
	{
	  PROPS::iterator& dup = insert_return.first;
	  CosTrading::Property* prop = (*dup).second;

	  prop->value = modifies[i].value;
	}
    }
}

CosTrading::Offer&
TAO_Offer_Modifier::affect_change (void)
{
  int elem = 0;
  CosTrading::PropertySeq prop_seq;

  // Create a new property list reflecting the deletes, modifies, and
  // add operations performed, and place this property list in the
  // offer. 
  prop_seq.length (this->props_.size ());
  for (PROPS::iterator props_iter = this->props_.begin ();
       props_iter != this->props_.end ();
       props_iter++, elem++)
    {
      prop_seq[elem] = *(*props_iter).second;
    }

  this->offer_.properties = prop_seq;
  return this->offer_;
}

