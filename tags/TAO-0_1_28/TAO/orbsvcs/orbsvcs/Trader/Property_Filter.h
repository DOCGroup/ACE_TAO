/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = LIBRARY 
//    orbsvcs
//  
// = FILENAME 
//    Property_Filter.h
// 
// = AUTHOR 
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ======================================================================= 

#ifndef TAO_PROPERTY_FILTER_H
#define TAO_PROPERTY_FILTER_H

#include "Trader.h"
#include "ace/Containers.h"

class TAO_Property_Filter
// = TITLE
//
//   The Ace_Property_Filter copies those properties specified in a
//   CosTrading::Lookup::SpecifiedProps from a source
//   CosTrading::Offer to a destination CosTrading::Offer. 
{
public:

  typedef CosTrading::Lookup::SpecifiedProps SPECIFIED_PROPS;
  
  TAO_Property_Filter (const SPECIFIED_PROPS& desired_props,
		       CORBA::Environment& env)
    TAO_THROW_SPEC ((CosTrading::IllegalPropertyName,
		     CosTrading::DuplicatePropertyName));
  // Verify that the specified properties are correct.

  TAO_Property_Filter (const TAO_Property_Filter& prop_filter);

  void filter_offer (CosTrading::Offer& source,
		     CosTrading::Offer& destination);
  // Copy the desired properties from the source offer to the
  // destination offer.
  
private:

  typedef ACE_Unbounded_Set< TAO_String_Hash_Key > Prop_Names;
  typedef ACE_Unbounded_Queue< CosTrading::Property* > Prop_Queue;
  
  int verify_property_name (const char* name);

  Prop_Names props_;
  CosTrading::Lookup::HowManyProps policy_;
};

#endif /* TAO_PROPERTY_FILTER_H */
