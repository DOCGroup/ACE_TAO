/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = LIBRARY 
//    orbsvcs
//  
// = FILENAME 
//    Offer_Id_terator.h
//
// = AUTHOR 
//    Seth Widoff <sbw1@cs.wustl.edu>
//  
// ======================================================================= 

#ifndef TAO_OFFER_ID_ITERATOR_H
#define TAO_OFFER_ID_ITERATOR_H

#include "ace/Containers.h"
#include "orbsvcs/CosTradingS.h"

class TAO_Offer_Id_Iterator : public POA_CosTrading::OfferIdIterator
// = TITLE
//   Silly little iterator that contains the overflow of offer ids
//   from the Admin list_offers method.
// 
// = DESCRIPTION
//
//   BEGIN SPEC
//   The OfferIdIterator interface is used to return a set of offer
//   identifiers from the list_offers operation and the list_proxies
//   operation in the Admin interface by enabling the offer identifiers
//   to be extracted by successive operations on the OfferIdIterator
//   interface. 
//   END SPEC
{
 public:
  
  TAO_Offer_Id_Iterator(void);
  // No op constructor

  ~TAO_Offer_Id_Iterator (void);
  
  virtual CORBA::ULong max_left(CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException,
		     CosTrading::UnknownMaxLeft));
  // The max_left operation returns the number of offer identifiers
  // remaining in the iterator. The exception UnknownMaxLeft is raised
  // if the iterator cannot determine the remaining number of offer
  // identifiers (e.g., if the iterator determines its set of offer
  // identifiers through lazy evaluation). 
  
  virtual void destroy(CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  // The destroy operation destroys the iterator. No further
  // operations can be invoked on an iterator after it has been
  // destroyed. 
  
  virtual CORBA::Boolean next_n(CORBA::ULong _n,
				CosTrading::OfferIdSeq_out _ids,
				CORBA::Environment& env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  // The next_n operation returns a set of offer identifiers in the
  // output parameter "ids." The operation returns n offer identifiers
  // if there are at least n offer identifiers remaining in the
  // iterator. If there are fewer than n offer identifiers in the
  // iterator, then all remaining offer identifiers are returned. The
  // actual number of offer identifiers returned can be determined
  // from the length of the "ids" sequence. The next_n operation
  // returns TRUE if there are further offer identifiers to be
  // extracted from the iterator. It returns FALSE if there are no
  // further offer identifiers to be extracted. 
  
  void insert_id(CosTrading::OfferId new_id);
  // Insert a <new_id> into the contents of the iterator.
  
 private:

  typedef ACE_Unbounded_Queue
    <
    CosTrading::OfferId
    >
    Offer_Id_Queue;
  
  
  Offer_Id_Queue ids_;
};

#endif /* TAO_OFFER_ID_ITERATOR_H */
