/* -*- C++ -*- */

// ========================================================================
// $Id$
//
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//   Offer_Modifier
//
// = AUTHOR
//    Seth Widoff   <sbw1@cs.wustl.edu>
// 
// ========================================================================

#ifndef ACE_OFFER_MODIFIER_H
#define ACE_OFFER_MODIFIER_H

#include "Trader.h"
#include "ace/Containers.h"

class TAO_Offer_Modifier
// = TITLE
//   This class deletes, modifies, and adds properties to a given
//   offer according to the rules of the modify method on the Register 
//   interface.
{
public:

  TAO_Offer_Modifier (const char* type,
		      CosTradingRepos::ServiceTypeRepository::TypeStruct* type_struct,
		      CosTrading::Offer& offer);
  // Modify an <offer> of type <type>, whose properties are described
  // by <type_struct>
  
  void delete_properties (const CosTrading::PropertyNameSeq& deletes,
			  CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::Register::UnknownPropertyName, 
		    CosTrading::Register::MandatoryProperty,
		    CosTrading::IllegalPropertyName,
		    CosTrading::DuplicatePropertyName));
  // Delete the properties whose names were given to the
  // constructor. Ensure we don't delete mandatory properties.

  void merge_properties (const CosTrading::PropertySeq& modifies,
			 CORBA::Environment& _env)
    TAO_THROW_SPEC ((CosTrading::IllegalPropertyName,
		    CosTrading::DuplicatePropertyName,
		    CosTrading::Register::ReadonlyProperty));
  // Copy to the destination the union of the source and destination
  // properties. In the case of duplicate properties, update the
  // destination with the source's value.

  CosTrading::Offer& affect_change (void);
  // Return a reference to the Offer with the changes affected.
  
private:

  typedef ACE_Unbounded_Set<TAO_String_Hash_Key> Prop_Names;

  typedef ACE_Hash_Map_Manager
    <
    TAO_String_Hash_Key,
    CosTrading::Property*,
    ACE_Null_Mutex
    >
    Props;
  
  const char* type_;
  // The type of the offer.
  
  Props props_;
  // The map of properties in the offer.
  
  Prop_Names readonly_, mandatory_;
  // The set of readonly and mandatory property names in the offer's
  // type.
  
  CosTrading::Offer& offer_;
  // A reference to the offer undergoing change.
};

#endif /* TAO_OFFER_MODIFIER_H */
