// $Id$

#include "Offer_Modifier.h"

TAO_Offer_Modifier::
TAO_Offer_Modifier (const char* type_name,
		    CosTradingRepos::ServiceTypeRepository::TypeStruct* type_struct,
		    CosTrading::Offer& offer)
  : offer_ (offer),
    type_ (type_name)
{
  CosTrading::PropertySeq& prop_seq = this->offer_.properties;
  CosTradingRepos::ServiceTypeRepository::PropStructSeq&
    pstructs = type_struct->props;
  int pstructs_length = pstructs.length (),
    props_length = prop_seq.length ();

  // Separate the type defined properties into mandatory and readonly
  for (int i = 0; i < pstructs_length; i++)
    {
      if (pstructs[i].mode ==
	  CosTradingRepos::ServiceTypeRepository::PROP_MANDATORY)
	{
	  TAO_String_Hash_Key prop_name ((const char *) pstructs[i].name);
	  this->mandatory_.insert (prop_name);
	}
      else if (pstructs[i].mode ==
	       CosTradingRepos::ServiceTypeRepository::PROP_READONLY)
	{
	  TAO_String_Hash_Key prop_name ((const char *) pstructs[i].name);
	  this->readonly_.insert (prop_name);
	}
    }

  // Insert the properties of the offer into a map.
  for (i = 0; i < props_length; i++)
    {
      TAO_String_Hash_Key prop_name = (const char*) prop_seq[i].name;
      this->props_.bind (prop_name, &prop_seq[i]);
    }
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
  Prop_Names delete_me;
  // Validate that the listed property names can be deleted
  for (int i = 0, length = deletes.length (); i < length; i++)
    {
      const char* dname = (const char *) deletes[i];
      if (! TAO_Trader_Base::is_valid_identifier_name (dname))
	TAO_THROW (CosTrading::IllegalPropertyName (dname));
      else
	{
	  TAO_String_Hash_Key prop_name (dname);
	  if (this->mandatory_.find (prop_name) == 0)
	    TAO_THROW (CosTrading::Register::MandatoryProperty (this->type_, dname));
	  else if (delete_me.insert (prop_name) == 1)
	    TAO_THROW (CosTrading::DuplicatePropertyName (dname));
	  else if (this->props_.find (prop_name) == 0)
	    TAO_THROW (CosTrading::Register::UnknownPropertyName (dname));
	}
    }

  // Delete those properties from the offer.
  for (i = 0; i < length; i++)
    {
      TAO_String_Hash_Key prop_name = (const char *) deletes[i];
      this->props_.unbind (prop_name);
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
  Prop_Names modify_me;
  // Ensure that the proposed changes aren't to readonly properties or
  // otherwise invalid.
  for (int i = 0, length = modifies.length (); i < length; i++)
    {
      const char*  mname =(const char *) modifies[i].name;
      if (! TAO_Trader_Base::is_valid_identifier_name (mname))
	TAO_THROW (CosTrading::IllegalPropertyName (mname));
      else
	{
	  TAO_String_Hash_Key prop_name (mname);
	  if (this->readonly_.find (prop_name) == 0 &&
	      this->props_.find (prop_name) == 0)
	    TAO_THROW (CosTrading::Register::ReadonlyProperty (this->type_, mname));
	  else if (modify_me.insert (prop_name) == 1)
	    TAO_THROW (CosTrading::DuplicatePropertyName (mname));
	}
    }

  for (i = 0; i < length; i++)
    {
      // Add a property to the destination if it doesn't already exist.
      Props::ENTRY* existing_entry = 0;
      TAO_String_Hash_Key prop_name ((const char*) modifies[i].name);

      if (this->props_.bind (prop_name,
			     (CosTrading::Property *) &modifies[i],
			     existing_entry) == 1)
	{
	  // Modify a property if it already exists in the destination.
	  CosTrading::Property* prop = existing_entry->int_id_;	  
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
  prop_seq.length (this->props_.current_size ());
  for (Props::iterator props_iter (this->props_);
       ! props_iter.done ();
       props_iter++, elem++)
    {
      prop_seq[elem] = *(*props_iter).int_id_;
    }

  this->offer_.properties = prop_seq;
  return this->offer_;
}

