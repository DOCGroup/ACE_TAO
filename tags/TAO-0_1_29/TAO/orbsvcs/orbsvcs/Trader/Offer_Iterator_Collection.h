/* -*- C++ -*- */

// ========================================================================
// $Id$
//
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Offer_Iterator_Collection.h
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ========================================================================

#ifndef TAO_OFFER_ITERATOR_COLLECTION_H
#define TAO_OFFER_ITERATOR_COLLECTION_H

#include "orbsvcs/CosTradingS.h"

class TAO_Offer_Iterator_Collection : public POA_CosTrading::OfferIterator
// = TITLE
//   A collection of offer iterator to query in turn.
//
// = DESCRIPTION
//   Since a query could conceivable query several other traders, it
//   needs a way to merge the results into a single set of results
//   suitable for being returned to the user. And since all the query
//   method can return to the user is a sequence and an iterator, and
//   the size of the sequence is constrained, there needs to be some
//   way to collect all the returned offer_iterators into a single
//   offer_iterator. This is that collection. The results of
//   collecting all the iterators in this way is a distributed tree of 
//   iterators, which could conceivably become hugely inefficient if
//   the trader graphs is deep enough.
{
public:

  // = Constructors.
  
  TAO_Offer_Iterator_Collection (void);

  virtual ~TAO_Offer_Iterator_Collection (void) {}

  virtual CORBA::Boolean next_n (CORBA::ULong n,
				 CosTrading::OfferSeq_out offers,
				 CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException));
  // Retrieve n offers from the set of iterators.
  
  virtual void destroy (CORBA::Environment& _env)  
    TAO_THROW_SPEC ((CORBA::SystemException));
  // Destroy the collection of iterators.
  
  virtual CORBA::ULong max_left (CORBA::Environment &env)
    TAO_THROW_SPEC ((CORBA::SystemException,
		    CosTrading::UnknownMaxLeft));
  // Determine how many offers are left in the collection.
 
  void add_offer_iterator (CosTrading::OfferIterator* offer_iter);
  // Add an iterator to the collection.

private:

  struct Iter_Info
  {
    CosTrading::OfferIterator_var iter_;
    // Object reference for this iterator in the collection.
    
    CORBA::ULong num_left_;
    // Cached value of the number of elements left in this iterator. 
  };
  
  typedef ACE_Unbounded_Queue <Iter_Info> Offer_Iters;

  Offer_Iters iters_;
  // The iterator collection.
  
  CORBA::ULong total_left_;
  // Cached value of the number of offers left in the collection.
};

#endif /* TAO_OFFER_ITERATOR_COLLECTION_H */
