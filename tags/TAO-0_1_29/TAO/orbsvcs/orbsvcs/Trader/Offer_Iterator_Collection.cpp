// $Id$

#include "Offer_Iterator_Collection.h"

TAO_Offer_Iterator_Collection::TAO_Offer_Iterator_Collection (void)
  : total_left_ (0)
{
}

void
TAO_Offer_Iterator_Collection::
add_offer_iterator (CosTrading::OfferIterator* offer_iter)
{
  Iter_Info iter_info;

  if (offer_iter != CosTrading::OfferIterator::_nil ())
    {  
      TAO_TRY
	{
	  iter_info.iter_ = offer_iter;
	  iter_info.num_left_ = offer_iter->max_left (TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	}
      TAO_CATCHANY
	{
	  // I don't know what to do here if the offer_iterator throws
	  // an UnknownMaxLeft exception.
	}
      TAO_ENDTRY;

      this->total_left_ += iter_info.num_left_;
      this->iters_.enqueue_tail (iter_info);
    }
}

CORBA::Boolean
TAO_Offer_Iterator_Collection::next_n (CORBA::ULong n,
				       CosTrading::OfferSeq_out offers,
				       CORBA::Environment &env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong offers_left = n;
  CORBA::Boolean return_value = CORBA::B_TRUE;
  CosTrading::OfferSeq_var out_offers;
  
  ACE_NEW_RETURN (offers, CosTrading::OfferSeq, return_value);
  while (offers_left > 0 && ! this->iters_.is_empty ())
    {
      Iter_Info iter_info;
      this->iters_.dequeue_head (iter_info);

      // Determine how many offers we should retrieve from this
      // iterator. 
      CORBA::ULong offers_to_retrieve =
	(offers_left > iter_info.num_left_)
	? iter_info.num_left_
	: offers_left;

      // Retrieve the set of offers.
      iter_info.iter_->next_n
	(offers_to_retrieve,
	 CosTrading::OfferSeq_out (out_offers.out ()),
	 env);

      // Merge it with the passed set.
      CORBA::ULong offset = offers->length ();
      offers->length (out_offers->length () + offset);
      for (int j = out_offers->length () - 1; j >= 0; j--)
	offers[j + offset] = out_offers[j];

      // Adjust the offer counters.
      offers_left -= offers_to_retrieve;
      iter_info.num_left_ -= offers_to_retrieve;
      this->total_left_ -= offers_to_retrieve;

      // If we've exhausted this iterator, destroy it.
      if (iter_info.num_left_ == 0)
	iter_info.iter_->destroy (env);
      else
	this->iters_.enqueue_head (iter_info);
    }

  // Determine if we have anything left to offer.
  if (this->iters_.is_empty ())
    return_value = CORBA::B_FALSE;

  return return_value;
}

void
TAO_Offer_Iterator_Collection::destroy (CORBA::Environment& _env)  
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  // Destroy all iterators in the collection.
  for (Offer_Iters::ITERATOR iters_iter (this->iters_);
       ! iters_iter.done ();
       iters_iter.advance ())
    {
      Iter_Info* iter_info = 0;

      iters_iter.next (iter_info);
      iter_info->iter_->destroy (_env);
    }

  this->total_left_ = 0;

  // Remove self from POA
  //
  // Note that there is no real error checking here as we can't do
  // much about errors here anyway
  //
  
  TAO_TRY
    {
      PortableServer::POA_var poa = this->_default_POA (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      PortableServer::ObjectId_var id =
	poa->servant_to_id (this, TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      poa->deactivate_object (id.in (), TAO_TRY_ENV);
    }
  TAO_CATCHANY
    {      
    }
  TAO_ENDTRY;
  
  delete this;  
}

CORBA::ULong
TAO_Offer_Iterator_Collection::max_left (CORBA::Environment &env)
  TAO_THROW_SPEC ((CORBA::SystemException,
		   CosTrading::UnknownMaxLeft))
{
  return this->total_left_;
}
