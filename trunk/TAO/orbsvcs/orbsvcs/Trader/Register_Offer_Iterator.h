/* -*- C++ -*- */

// ========================================================================
// $Id$
//
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Register_Offer_Iterator.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ========================================================================

#ifndef TAO_REGISTER_OFFER_ITERATOR_H
#define TAO_REGISTER_OFFER_ITERATOR_H

#include "Offer_Iterator.h"

template <class TRADER>
class TAO_Register_Offer_Iterator : public TAO_Offer_Iterator
  // = TITLE
  //     An implementation of CosTrading::OfferIterator IDL interface
  //     appropriate when trader has Register functionality.
  //
  // = DESCRIPTION
  //     Stores ids of offers to be iterated over.  Before returning
  //     an offer, checks if the offer is still there (since it may
  //     have been removed by the Register). 
{
public:

  // = Initialization and termination methods.

  TAO_Register_Offer_Iterator (TRADER &trader,
			       const TAO_Property_Filter& pfilter);
  // Takes service type and trader reference in order to 
  // later locate offers using their ids.

  virtual ~TAO_Register_Offer_Iterator (void);
  // destructor.

  virtual CORBA::Boolean next_n (CORBA::ULong n, 
				 CosTrading::OfferSeq_out offers,
				 CORBA::Environment& _env)  
    TAO_THROW_SPEC ((CORBA::SystemException));
  // Deposit at maximum n offers into the return sequence and return 1,
  // or return 0 if the iterator is done and no offers are returned.
  
  virtual CORBA::ULong max_left (CORBA::Environment& _env) 
    TAO_THROW_SPEC ((CORBA::SystemException, 
		    CosTrading::UnknownMaxLeft));
  // Throws CosTrading::UnknownMaxLeft since with the presence of 
  // "Register" functionality, the iterator cannot guarantee that 
  // the trader will have all the offers it has now when the time
  // to return them comes.

  void add_offer (CosTrading::OfferId id,
		  const CosTrading::Offer* offer);
  // Add an offer the iterator should iterate over.

private:

  TRADER &trader_;
  // A reference to the trader is needed for access to the map of offers.

  ACE_Unbounded_Queue <CosTrading::OfferId> offer_ids_;
  // Offer ids of offers to iterate over.
};


#if defined (TAO_TEMPLATES_REQUIRE_SOURCE)
#include "Register_Offer_Iterator.cpp"
#endif /* TAO_TEMPLATES_REQUIRE_SOURCE */

#endif /* TAO_REGISTER_OFFER_ITERATOR_H */
