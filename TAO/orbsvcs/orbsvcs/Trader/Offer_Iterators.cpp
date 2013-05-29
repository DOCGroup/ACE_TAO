// $Id$

#include "orbsvcs/Trader/Offer_Iterators.h"



TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Offer_Iterator::TAO_Offer_Iterator (const TAO_Property_Filter& pfilter)
  : pfilter_ (pfilter)
{
}

TAO_Offer_Iterator::~TAO_Offer_Iterator (void)
{
}

void
TAO_Offer_Iterator::destroy (void)
{
  // Remove self from POA

  PortableServer::POA_var poa =
    this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this);

  poa->deactivate_object (id.in ());
}

TAO_Query_Only_Offer_Iterator::
TAO_Query_Only_Offer_Iterator(const TAO_Property_Filter& pfilter)
  : TAO_Offer_Iterator (pfilter)
{
}

TAO_Query_Only_Offer_Iterator::~TAO_Query_Only_Offer_Iterator(void)
{
}

void
TAO_Query_Only_Offer_Iterator::add_offer (CosTrading::OfferId offer_id,
                                          const CosTrading::Offer* offer)
{
  this->offers_.enqueue_tail ((CosTrading::Offer*) offer);
  CORBA::string_free (offer_id);
}

CORBA::ULong
TAO_Query_Only_Offer_Iterator::max_left (void)
{
  return static_cast<CORBA::ULong> (this->offers_.size ());
}

CORBA::Boolean
TAO_Query_Only_Offer_Iterator::next_n (CORBA::ULong n,
                                       CosTrading::OfferSeq_out offers)
{
  offers = new CosTrading::OfferSeq;

  CORBA::ULong sequence_size = static_cast<CORBA::ULong> (this->offers_.size ());
  CORBA::ULong offers_in_sequence = (n < sequence_size) ? n : sequence_size;
  offers->length (offers_in_sequence);

  // populate the sequence.
  for (CORBA::ULong i = 0; i < offers_in_sequence; i++)
    {

      CosTrading::Offer *source = 0;
      this->offers_.dequeue_head (source);
      this->pfilter_.filter_offer (source, offers[i]);
    }

  return offers_in_sequence != 0;
}

TAO_Offer_Iterator_Collection::TAO_Offer_Iterator_Collection (void)
{
}

TAO_Offer_Iterator_Collection::~TAO_Offer_Iterator_Collection (void)
{
  while (! this->iters_.is_empty ())
    {
      CosTrading::OfferIterator* offer_iter = 0;
      this->iters_.dequeue_head (offer_iter);

      try
        {
          offer_iter->destroy ();

          CORBA::release (offer_iter);
        }
      catch (const CORBA::Exception&)
        {
          // Don't let the exceptions propagate since we're in a
          // destructor!
        }
    }
}

void
TAO_Offer_Iterator_Collection::
add_offer_iterator (CosTrading::OfferIterator_ptr offer_iter)
{
  if (! CORBA::is_nil (offer_iter))
    this->iters_.enqueue_tail (offer_iter);
}

CORBA::Boolean
TAO_Offer_Iterator_Collection::next_n (CORBA::ULong n,
                                       CosTrading::OfferSeq_out offers)
{
  CORBA::ULong offers_left = n;
  CORBA::Boolean return_value = 1;
  CosTrading::OfferSeq_var out_offers;

  ACE_NEW_THROW_EX (offers,
                    CosTrading::OfferSeq,
                    CORBA::NO_MEMORY ());

  while (offers_left > 0 && ! this->iters_.is_empty ())
    {
      CORBA::ULong offset = 0;
      CORBA::Boolean any_left = 0;
      CosTrading::OfferIterator* iter =  0;
      this->iters_.dequeue_head (iter);

      // Determine how many offers we should retrieve from this
      // iterator.

      // Retrieve the set of offers.
      any_left =
        iter->next_n (offers_left,
                      CosTrading::OfferSeq_out (out_offers.out ()));

      // If we've exhausted this iterator, destroy it.
      if (any_left == 0)
        {
          iter->destroy ();
          CORBA::release (iter);
        }
      else
        this->iters_.enqueue_head (iter);

      // Merge it with the passed set.
      offset = offers->length ();
      offers->length (out_offers->length () + offset);
      for (CORBA::ULong j = out_offers->length (); j > 0; j--)
        offers[j + offset - 1] = out_offers[j - 1];

      offers_left -= out_offers->length ();
    }

  // Determine if we have anything left to offer.
  if (this->iters_.is_empty ())
    return_value = 0;

  return return_value;
}

void
TAO_Offer_Iterator_Collection::destroy (void)
{
  // Destroy all iterators in the collection.
  for (Offer_Iters::ITERATOR iters_iter (this->iters_);
       ! iters_iter.done ();
       iters_iter.advance ())
    {
      CosTrading::OfferIterator** iter = 0;

      iters_iter.next (iter);
      (*iter)->destroy ();
    }

  // Remove self from POA

  PortableServer::POA_var poa =
    this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this);

  poa->deactivate_object (id.in ());
}

CORBA::ULong
TAO_Offer_Iterator_Collection::max_left (void)
{
  throw CosTrading::UnknownMaxLeft();
}

TAO_Offer_Id_Iterator::TAO_Offer_Id_Iterator (void)
{
}

TAO_Offer_Id_Iterator::~TAO_Offer_Id_Iterator (void)
{
  int return_value = 0;

  do
    {
      CosTrading::OfferId offer_id = 0;

      return_value = this->ids_.dequeue_head (offer_id);
      if (return_value == 0)
        CORBA::string_free (offer_id);
    }
  while (return_value == 0);
}

CORBA::ULong
TAO_Offer_Id_Iterator::max_left (void)
{
  return static_cast<CORBA::ULong> (this->ids_.size ());
}

void
TAO_Offer_Id_Iterator::destroy (void)
{
  // Remove self from POA

  PortableServer::POA_var poa =
    this->_default_POA ();

  PortableServer::ObjectId_var id =
    poa->servant_to_id (this);

  poa->deactivate_object (id.in ());
}

CORBA::Boolean
TAO_Offer_Id_Iterator::next_n (CORBA::ULong n,
                               CosTrading::OfferIdSeq_out _ids)
{
  // Calculate the number of Ids to be returned.
  CORBA::ULong items_left = static_cast<CORBA::ULong> (this->ids_.size());
  int difference = items_left - n;
  CORBA::ULong returnable_items = (difference >= 0) ? n : items_left;
  CORBA::Boolean return_value = (CORBA::Boolean) (difference > 0);

  // Allocate result sequence.
  ACE_NEW_RETURN (_ids,
                  CosTrading::OfferIdSeq(returnable_items),
                  (items_left > 0));
  _ids->length(returnable_items);                  

  // Transfer OfferIds chunk.
  for (CORBA::ULong i = 0; i < returnable_items; i++)
    {
      CosTrading::OfferId offer_id = 0;
      this->ids_.dequeue_head (offer_id);
      (*_ids)[i] = offer_id;
    }

  // Return true only if there are items left to be returned in
  // subsequent calls.
  return return_value;
}

void
TAO_Offer_Id_Iterator::insert_id (CosTrading::OfferId new_id)
{
  this->ids_.enqueue_tail (new_id);
}

TAO_END_VERSIONED_NAMESPACE_DECL
