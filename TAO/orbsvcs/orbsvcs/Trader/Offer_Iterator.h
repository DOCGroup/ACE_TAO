/* -*- C++ -*- */

// ============================================================================
// $Id$
//
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Offer_Iterator.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ============================================================================

#ifndef TAO_OFFER_ITERATOR_H
#define TAO_OFFER_ITERATOR_H

#include "Property_Filter.h"

class TAO_ORBSVCS_Export  TAO_Offer_Iterator :
  public POA_CosTrading::OfferIterator
  // = TITLE
  //     This class implements CosTrading::OfferIterator IDL
  //     interface. 

  // = DESCRIPTION
  //     This is an abstract base class  to allow for different
  //     implementations of OfferIterator.
  // BEGIN SPEC
  //   The OfferIterator interface is used to return a set of service
  //   offers from the query operation by enabling the service offers
  //   to be extracted by successive operations on the OfferIterator
  //   interface.
  // END SPEC
{
public:
  // = Initialization and termination methods.

  TAO_Offer_Iterator (const TAO_Property_Filter& property_filter);

  virtual ~TAO_Offer_Iterator (void);
  
  virtual void destroy (CORBA::Environment& _env)  
    TAO_THROW_SPEC ((CORBA::SystemException));
  // BEGIN SPEC
  // The destroy operation destroys the iterator. No further
  // operations can be invoked on an iterator after it has been
  // destroyed. 
  // END SPEC
  
  virtual void add_offer (CosTrading::OfferId offer_id,
			  CosTrading::Offer* offer) = 0;
  // Add an offer to the collection of offers the iterator will
  // iterate over. 

  virtual CORBA::ULong max_left (CORBA::Environment &env) = 0
    TAO_THROW_SPEC ((CORBA::SystemException,
		    CosTrading::UnknownMaxLeft));

  // BEGIN SPEC
  // The max_left operation returns the number of service offers
  // remaining in the iterator. The exception UnknownMaxLeft is raised
  // if the iterator cannot determine the remaining number of service
  // offers (e.g., if the iterator determines its set of service
  // offers through lazy evaluation). 
  // END SPEC
  
  virtual CORBA::Boolean next_n (CORBA::ULong n,
				 CosTrading::OfferSeq_out offers,
				 CORBA::Environment &env) = 0
    TAO_THROW_SPEC ((CORBA::SystemException));
  // BEGIN SPEC
  // The next_n operation returns a set of service offers in the
  // output parameter "offers." The operation returns n service offers
  // if there are at least n service offers remaining in the
  // iterator. If there are fewer than n service offers in the
  // iterator, then all remaining service offers are returned. The
  // actual number of service offers returned can be determined from
  // the length of the "offers" sequence. The next_n operation returns
  // TRUE if there are further service offers to be extracted from the
  // iterator. It returns FALSE if there are no further service offers
  // to be extracted.  
  // END SPEC
 protected:

  TAO_Property_Filter pfilter_;
};

#endif /* TAO_OFFER_ITERATOR */
