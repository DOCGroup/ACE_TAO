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

#include "stl.h"
#include "Trader.h"

class TAO_Offer_Modifier
// = TITLE
//   This class deletes, modifies, and adds properties to a given
//   offer according to the rules of the modify method on the Register 
//   interface.
{
public:

  typedef CosTradingRepos::ServiceTypeRepository::TypeStruct TYPE_STRUCT;  
  
  TAO_Offer_Modifier (const char* type,
		      TYPE_STRUCT* type_struct,
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

  typedef set< string, less <string> > PROP_NAMES;
  typedef map< string, CosTrading::Property*, less <string> > PROPS;

  const char* type_;
  // The type of the offer.
  
  PROPS props_;
  // The map of properties in the offer.
  
  PROP_NAMES readonly_, mandatory_;
  // The set of readonly and mandatory property names in the offer's
  // type.
  
  CosTrading::Offer& offer_;
  // A reference to the offer undergoing change.
};

#endif /* TAO_OFFER_MODIFIER_H */
