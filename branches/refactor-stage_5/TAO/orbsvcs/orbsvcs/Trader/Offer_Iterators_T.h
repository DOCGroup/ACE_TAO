/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Offer_Iterators_T.h
 *
 *  $Id$
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 *  @author Seth Widoff <sbw1@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_REGISTER_OFFER_ITERATOR_H
#define TAO_REGISTER_OFFER_ITERATOR_H
#include /**/ "ace/pre.h"

#include "Offer_Iterators.h"
#include "Offer_Database.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */
/**
 * @class TAO_Register_Offer_Iterator
 *
 * @brief An implementation of CosTrading::OfferIterator IDL interface
 * appropriate when trader has Register functionality.
 *
 * Stores ids of offers to be iterated over.  Before returning
 * an offer, checks if the offer is still there (since it may
 * have been removed by the Register).
 */
template <class MAP_LOCK_TYPE>
class TAO_Register_Offer_Iterator : public TAO_Offer_Iterator
{
public:

  // = Initialization and termination methods.

  /// Takes service type and trader reference in order to
  /// later locate offers using their ids.
  TAO_Register_Offer_Iterator (TAO_Offer_Database<MAP_LOCK_TYPE> &db,
                               const TAO_Property_Filter& pfilter);

  /// Destructor.
  virtual ~TAO_Register_Offer_Iterator (void);

  /// Deposit at maximum n offers into the return sequence and return 1,
  /// or return 0 if the iterator is done and no offers are returned.
  virtual CORBA::Boolean next_n (CORBA::ULong n,
                                 CosTrading::OfferSeq_out offers
                                 ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Throws CosTrading::UnknownMaxLeft since with the presence of
   * "Register" functionality, the iterator cannot guarantee that
   * the trader will have all the offers it has now when the time
   * to return them comes.
   */
  virtual CORBA::ULong max_left (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    CosTrading::UnknownMaxLeft));

  /// Add an offer the iterator should iterate over.
  void add_offer (CosTrading::OfferId id,
                  const CosTrading::Offer *);

private:

  /// A reference to the trader is needed for access to the map of offers.
  TAO_Offer_Database<MAP_LOCK_TYPE> &db_;

  /// Offer ids of offers to iterate over.
  TAO_String_Queue offer_ids_;
};


#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Offer_Iterators_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_REGISTER_OFFER_ITERATOR_H */
