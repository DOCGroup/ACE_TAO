#ifndef TAO_OFFER_ITERATORS_T_CPP
#define TAO_OFFER_ITERATORS_T_CPP

#include "orbsvcs/Trader/Offer_Iterators_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class MAP_LOCK_TYPE>
TAO_Register_Offer_Iterator<MAP_LOCK_TYPE>::
TAO_Register_Offer_Iterator (TAO_Offer_Database<MAP_LOCK_TYPE> &db,
                             const TAO_Property_Filter& pfilter)
  : TAO_Offer_Iterator (pfilter),
    db_ (db)
{
}

template <class MAP_LOCK_TYPE>
TAO_Register_Offer_Iterator<MAP_LOCK_TYPE>::
~TAO_Register_Offer_Iterator () = default;

template <class MAP_LOCK_TYPE> void
TAO_Register_Offer_Iterator<MAP_LOCK_TYPE>::
add_offer (CosTrading::OfferId id,
           const CosTrading::Offer* /* offer */)
{
  CORBA::String_var offer_id (id);
  this->offer_ids_.emplace_back (offer_id.in ());
}

template <class MAP_LOCK_TYPE> CORBA::ULong
TAO_Register_Offer_Iterator<MAP_LOCK_TYPE>::
max_left ()
{
  return static_cast<CORBA::ULong> (this->offer_ids_.size ());
}

template <class MAP_LOCK_TYPE> CORBA::Boolean
TAO_Register_Offer_Iterator<MAP_LOCK_TYPE>::
next_n (CORBA::ULong n,
        CosTrading::OfferSeq_out offers)
{
  CORBA::ULong ret_offers = 0;

  CORBA::ULong max_possible_offers_in_sequence =
    (n <  this->offer_ids_.size ()) ? n :
       static_cast<CORBA::ULong> (this->offer_ids_.size ());

  ACE_NEW_THROW_EX (offers,
                    CosTrading::OfferSeq,
                    CORBA::NO_MEMORY ());

  offers->length (max_possible_offers_in_sequence);

  // While there are entries left and we haven't filled <offers>
  // with requested number.
  while (! this->offer_ids_.empty ()
         && n > ret_offers)
    {
      // If offer is found, put it into the sequence.
      // remove this id irrespective of whether the offer is found
      // or not.
      std::string id = this->offer_ids_.front ();
      this->offer_ids_.pop_front ();
      CosTrading::Offer* offer = this->db_.lookup_offer (id.c_str ());

      if (offer != nullptr)
        this->pfilter_.filter_offer (offer,
                                     offers[ret_offers++]);
    }

  // Reset the length to the correct value
  offers->length (ret_offers);

  return static_cast<CORBA::Boolean> (ret_offers != 0);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_REGISTER_OFFER_ITERATOR_CPP */
