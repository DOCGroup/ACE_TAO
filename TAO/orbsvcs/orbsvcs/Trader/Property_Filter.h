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

#include "stl.h"
#include "Trader.h"

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

  typedef set< string,  less <string> > PROP_NAMES;
  typedef deque< CosTrading::Property* > PROP_QUEUE;
  
  int verify_property_name (const char* name);

  PROP_NAMES props_;
  CosTrading::Lookup::HowManyProps policy_;
};

#endif /* TAO_PROPERTY_FILTER_H */
