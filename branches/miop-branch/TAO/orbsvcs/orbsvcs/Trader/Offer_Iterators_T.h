/* -*- C++ -*- */
// $Id$

// ========================================================================
//
// = LIBRARY
//    orbsvcs
//
// = FILENAME
//    Offer_Iterators_T.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// ========================================================================

#ifndef TAO_REGISTER_OFFER_ITERATOR_H
#define TAO_REGISTER_OFFER_ITERATOR_H
#include "ace/pre.h"

#include "Offer_Iterators.h"
#include "Offer_Database.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */
template <class MAP_LOCK_TYPE>
class TAO_Register_Offer_Iterator : public TAO_Offer_Iterator
{
  // = TITLE
  //     An implementation of CosTrading::OfferIterator IDL interface
  //     appropriate when trader has Register functionality.
  //
  // = DESCRIPTION
  //     Stores ids of offers to be iterated over.  Before returning
  //     an offer, checks if the offer is still there (since it may
  //     have been removed by the Register).
public:

  // = Initialization and termination methods.

  TAO_Register_Offer_Iterator (TAO_Offer_Database<MAP_LOCK_TYPE> &db,
                               const TAO_Property_Filter& pfilter);
  // Takes service type and trader reference in order to
  // later locate offers using their ids.

  virtual ~TAO_Register_Offer_Iterator (void);
  // destructor.

  virtual CORBA::Boolean next_n (CORBA::ULong n,
                                 CosTrading::OfferSeq_out offers
                                 TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Deposit at maximum n offers into the return sequence and return 1,
  // or return 0 if the iterator is done and no offers are returned.

  virtual CORBA::ULong max_left (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    CosTrading::UnknownMaxLeft));
  // Throws CosTrading::UnknownMaxLeft since with the presence of
  // "Register" functionality, the iterator cannot guarantee that
  // the trader will have all the offers it has now when the time
  // to return them comes.

  void add_offer (CosTrading::OfferId id,
                  const CosTrading::Offer *);
  // Add an offer the iterator should iterate over.

private:

  TAO_Offer_Database<MAP_LOCK_TYPE> &db_;
  // A reference to the trader is needed for access to the map of offers.

  TAO_String_Queue offer_ids_;
  // Offer ids of offers to iterate over.
};


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Offer_Iterators_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_REGISTER_OFFER_ITERATOR_H */
