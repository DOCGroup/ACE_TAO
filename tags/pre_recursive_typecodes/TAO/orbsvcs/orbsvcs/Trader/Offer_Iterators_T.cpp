// ============================================================================
// $Id$
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Offer_Iterators_T.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
//    Irfan Pyarali <irfan@cs.wustl.edu>
// ============================================================================

#ifndef TAO_OFFER_ITERATORS_T_C
#define  TAO_OFFER_ITERATORS_T_C

#include "Offer_Iterators_T.h"

ACE_RCSID(Trader, Offer_Iterators_T, "$Id$")

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
~TAO_Register_Offer_Iterator (void)
{
  while (! this->offer_ids_.is_empty ())
    {
      CosTrading::OfferId offer_id = 0;
      this->offer_ids_.dequeue_head (offer_id);

      CORBA::string_free (offer_id);
    }
}

template <class MAP_LOCK_TYPE> void
TAO_Register_Offer_Iterator<MAP_LOCK_TYPE>::
add_offer (CosTrading::OfferId id,
           const CosTrading::Offer* offer)
{
  this->offer_ids_.enqueue_tail (id);
}

template <class MAP_LOCK_TYPE> CORBA::ULong
TAO_Register_Offer_Iterator<MAP_LOCK_TYPE>::
max_left (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException,
                  CosTrading::UnknownMaxLeft))
{
  return this->offer_ids_.size ();
}

template <class MAP_LOCK_TYPE> CORBA::Boolean
TAO_Register_Offer_Iterator<MAP_LOCK_TYPE>::
next_n (CORBA::ULong n,
        CosTrading::OfferSeq_out offers,
        CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong ret_offers = 0;

  CORBA::ULong max_possible_offers_in_sequence =
    (n <  this->offer_ids_.size ()) ? n : this->offer_ids_.size ();

  ACE_NEW_RETURN (offers, CosTrading::OfferSeq, 0);
  offers->length (max_possible_offers_in_sequence);

  // While there are entries left and we haven't filled <offers>
  // with requested number.
  while (! this->offer_ids_.is_empty ()
         && n > ret_offers)
    {
      // If offer is found, put it into the sequence.
      // remove this id irrespective of whether the offer is found
      // or not.
      CosTrading::OfferId id;
      this->offer_ids_.dequeue_head (id);

      TAO_TRY
        {
          CosTrading::OfferId_var offerid_var (id);
          CosTrading::Offer* offer = this->db_.lookup_offer (id, TAO_TRY_ENV);
          TAO_CHECK_ENV;

          if (offer != 0)
            this->pfilter_.filter_offer (offer, offers[ret_offers++]);
        }
      TAO_CATCHANY {}
      TAO_ENDTRY;
    }

  // Reset the length to the correct value
  offers->length (ret_offers);

  return ACE_static_cast (CORBA::Boolean, ret_offers != 0);
}

#endif /* TAO_REGISTER_OFFER_ITERATOR_C */
