 /* -*- C++ -*- */

// ============================================================================
// $Id$
// 
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Query_Only_Offer_Iterator.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ============================================================================
#define ACE_BUILD_DLL

#include "Query_Only_Offer_Iterator.h"
#include <iostream.h>

TAO_Query_Only_Offer_Iterator::
TAO_Query_Only_Offer_Iterator(const TAO_Property_Filter& pfilter)
  : TAO_Offer_Iterator (pfilter)
{
}

TAO_Query_Only_Offer_Iterator::~TAO_Query_Only_Offer_Iterator(void)
{
  //  cerr << "In iterator destructor now" << endl;
}

void
TAO_Query_Only_Offer_Iterator::add_offer (CosTrading::OfferId offer_id,
					  CosTrading::Offer* offer)
{
  this->offers_.enqueue_tail (offer);
  CORBA::string_free (offer_id);
}

CORBA::ULong 
TAO_Query_Only_Offer_Iterator::max_left (CORBA::Environment& _env) 
  TAO_THROW_SPEC((CORBA::SystemException, 
		 CosTrading::UnknownMaxLeft))
{
  return this->offers_.size ();
}

CORBA::Boolean 
TAO_Query_Only_Offer_Iterator::next_n (CORBA::ULong n, 
                                       CosTrading::OfferSeq_out offers,
				       CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  offers = new CosTrading::OfferSeq;

  CORBA::ULong sequence_size = this->offers_.size ();
  CORBA::ULong offers_in_sequence = (n < sequence_size) ? n : sequence_size;
  offers->length (offers_in_sequence);
  
  // populate the sequence.
  for (CORBA::ULong i = 0; i < offers_in_sequence; i++)
    {
      CosTrading::Offer *source = 0;
      CosTrading::Offer& destination = (*offers)[i];

      this->offers_.dequeue_head (source);
      this->pfilter_.filter_offer (*source, destination);
    }
  
  return offers_in_sequence != 0;
}


