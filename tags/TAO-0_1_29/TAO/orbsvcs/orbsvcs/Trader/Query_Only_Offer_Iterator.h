/* -*- C++ -*- */

// ============================================================================
// $Id$
// 
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Query_Only_Offer_Iterator.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ============================================================================

#ifndef TAO_QUERY_ONLY_OFFER_ITERATOR_H
#define TAO_QUERY_ONLY_OFFER_ITERATOR_H

#include "ace/Containers.h"
#include "Offer_Iterator.h"

class TAO_ORBSVCS_Export TAO_Query_Only_Offer_Iterator
  : public TAO_Offer_Iterator
  // = TITLE
  //     An implementation of the CosTrading::OfferIterator IDL
  //     interface that takes advantage of the fact that the trader is
  //     Query Only, and therefore its service offers are not being
  //     changed or removed.
  //
  // = DESCRIPTION
  //     The iterator stores pointers to Offers to be iterated over,
  //     and later accesses the Offers directly through the pointers.
  //
{
public:
  // = Initialization and termination methods.
  
  TAO_Query_Only_Offer_Iterator (const TAO_Property_Filter& pfilter);

  virtual ~TAO_Query_Only_Offer_Iterator (void);

  virtual CORBA::Boolean next_n (CORBA::ULong n, 
				 CosTrading::OfferSeq_out offers,
				 CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  // Deposit at maximum n offers into the return sequence and return 1,
  // or return 0 if the iterator is done and no offers are returned.
  
  virtual CORBA::ULong max_left (CORBA::Environment& _env) 
    TAO_THROW_SPEC ((CORBA::SystemException, 
		    CosTrading::UnknownMaxLeft));
  // Return the number of items left in the iterator.  

  void add_offer (CosTrading::OfferId offer_id,
		  const CosTrading::Offer* offer);
  // Add an offer the iterator should iterate over.
  
private:
  
  ACE_Unbounded_Queue <CosTrading::Offer *> offers_;
  // Structure that stores pointers to offers
  // to iterate over.
};


#endif /* TAO_QUERY_ONLY_OFFER_ITERATOR_H */
