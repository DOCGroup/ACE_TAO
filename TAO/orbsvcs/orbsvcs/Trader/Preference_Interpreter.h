/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = LIBRARY 
//    orbsvcs
//  
// = FILENAME 
//    Preference_Interpreter.h
// 
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ========================================================================

#ifndef TAO_PREFERENCE_INTERPRETER_H
#define TAO_PREFERENCE_INTERPRETER_H

#include <map>
#include "Interpreter.h"
#include "Constraint_Evaluator.h"
#include "Constraint_Validator.h"

class TAO_Preference_Interpreter : public TAO_Interpreter
// = TITLE
//   The TAO_Preference_Interpreter will, given a valid preference
//   string and offers, will order the offers based on the offers'
//   compliance with the preferences.
//
// = DESCRIPTION
//   Each time the order_offer method is invoked, the
//   TAO_Preference_Interpreter stores the offer reference in the
//   order dictated by its evaluation of the preference string. After
//   the TAO_Preference_Interpreter client has finished ordering all
//   the offers, it will extract the offers in order using the
//   remove_offer method.
{
public:

  TAO_Preference_Interpreter(CosTradingRepos::ServiceTypeRepository::TypeStruct* ts,
			     const char* preference,
			     CORBA::Environment& env)
    TAO_THROW_SPEC ((CosTrading::Lookup::IllegalPreference));

  TAO_Preference_Interpreter(TAO_Constraint_Validator& validator,
			     const char* preference,
			     CORBA::Environment& env)
    TAO_THROW_SPEC ((CosTrading::Lookup::IllegalPreference));

  // Parse the preference string, determining first if it's
  // valid. Throw an IllegalPreference exception if the preference
  // doesn't conform to the BNF grammar for preferences. 
  
  ~TAO_Preference_Interpreter(void);
  // Destructor

#if defined TAO_HAS_DYNAMIC_PROPERTY_BUG
  void order_offer (CosTrading::OfferId offer_id,
		    CosTrading::Offer* offer,
		    CORBA::ORB_ptr orb);
#else
  void order_offer (CosTrading::OfferId offer_id,
		    CosTrading::Offer* offer);
#endif /* TAO_HAS_DYNAMIC_PROPERTY_BUG */
  
  void order_offer (CosTrading::OfferId offer_id,
		    CosTrading::Offer* offer,
		    TAO_Constraint_Evaluator& evaluator);
  // Evaluate the offer, and order it internally based on the results
  // of the evaluation.

  int remove_offer (CosTrading::OfferId& offer_id,
		    CosTrading::Offer*& offer);
  // Remove the next offer. The offer returned will be the next in the 
  // ordering determined by the preference string. 

  int num_offers(void);
  // Return the number of offers remaining in the ordering.

private:

  typedef multimap
  < TAO_Literal_Constraint,
    pair < CosTrading::OfferId, CosTrading::Offer* >,
    less < TAO_Literal_Constraint > >
  ORDERED_OFFERS;
  // The map of ordered offers to their order value.
    
  ORDERED_OFFERS offers_;
};

#endif /* TAO_PREFERENCE_INTERPRETER_H */
