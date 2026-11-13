// -*- C++ -*-

/**
 *  @file    Offer_Iterators.h
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 *  @author Seth Widoff <sbw1@cs.wustl.edu>
 */

#ifndef TAO_OFFER_ITERATORS_H
#define TAO_OFFER_ITERATORS_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Trader/Trader_Utils.h"

#include <deque>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

  // *************************************************************
  // TAO_Offer_Iterator
  // *************************************************************

/**
 * @brief Implements the CosTrading::OfferIterator IDL interface.
 *
 * This abstract base class allows for different OfferIterator
 * implementations. The interface returns service offers from a query
 * through successive operations on the iterator.
 */
class TAO_Offer_Iterator
  : public virtual POA_CosTrading::OfferIterator
{
public:
  TAO_Offer_Iterator (TAO_Offer_Iterator const&) = delete;
  TAO_Offer_Iterator& operator= (TAO_Offer_Iterator const&) = delete;
  TAO_Offer_Iterator (TAO_Offer_Iterator&&) = delete;
  TAO_Offer_Iterator& operator= (TAO_Offer_Iterator&&) = delete;

  explicit TAO_Offer_Iterator (TAO_Property_Filter const& property_filter);

  ~TAO_Offer_Iterator () override = default;

  /**
   * BEGIN SPEC
   * The destroy operation destroys the iterator. No further
   * operations can be invoked on an iterator after it has been
   * destroyed.
   * END SPEC
   */
  void destroy () override;

  /// Add an offer to the collection of offers the iterator will
  /// iterate over.
  virtual void add_offer (CosTrading::OfferId offer_id,
                          const CosTrading::Offer* offer) = 0;

  /// The max_left operation returns the number of service offers
  /// remaining in the iterator. The exception UnknownMaxLeft is raised
  /// if the iterator cannot determine the remaining number of service
  /// offers (e.g., if the iterator determines its set of service
  /// offers through lazy evaluation).

  CORBA::ULong max_left () override = 0;

  /**
   * BEGIN SPEC
   * The next_n operation returns a set of service offers in the
   * output parameter "offers." The operation returns n service offers
   * if there are at least n service offers remaining in the
   * iterator. If there are fewer than n service offers in the
   * iterator, then all remaining service offers are returned. The
   * actual number of service offers returned can be determined from
   * the length of the "offers" sequence. The next_n operation returns
   * TRUE if there are further service offers to be extracted from the
   * iterator. It returns FALSE if there are no further service offers
   * to be extracted.
   * END SPEC
   */
  CORBA::Boolean next_n (CORBA::ULong n,
                         CosTrading::OfferSeq_out offers) override = 0;
 protected:
  /// The filter through which each returned offer must pass. Used to
  /// strip offers of undesirable properties.
  TAO_Property_Filter pfilter_;
};

  // *************************************************************
  // TAO_Query_Only_Offer_Iterator
  // *************************************************************

/**
 * @brief Implements an OfferIterator for a query-only trader.
 *
 * The iterator stores pointers to Offers and accesses them directly
 * because the trader's service offers are not changed or removed.
 */
class TAO_Query_Only_Offer_Iterator
  : public TAO_Offer_Iterator
{
public:
  TAO_Query_Only_Offer_Iterator (TAO_Query_Only_Offer_Iterator const&) = delete;
  TAO_Query_Only_Offer_Iterator& operator= (TAO_Query_Only_Offer_Iterator const&) = delete;
  TAO_Query_Only_Offer_Iterator (TAO_Query_Only_Offer_Iterator&&) = delete;
  TAO_Query_Only_Offer_Iterator& operator= (TAO_Query_Only_Offer_Iterator&&) = delete;

  explicit TAO_Query_Only_Offer_Iterator (TAO_Property_Filter const& pfilter);

  ~TAO_Query_Only_Offer_Iterator () override = default;

  /// Deposit at most n offers into the return sequence and return true,
  /// or return false if the iterator is done and no offers are returned.
  CORBA::Boolean next_n (CORBA::ULong n,
                         CosTrading::OfferSeq_out offers) override;

  /// Return the number of items left in the iterator.
  CORBA::ULong max_left () override;

  /// Add an offer the iterator should iterate over.
  void add_offer (CosTrading::OfferId offer_id,
                  CosTrading::Offer const* offer) override;

private:
  /// Structure that stores pointers to offers
  /// to iterate over.
  std::deque<CosTrading::Offer *> offers_;
};

  // *************************************************************
  // TAO_Offer_Iterator_Collection
  // *************************************************************

/**
 * @brief Collects several offer iterators into one iterator.
 *
 * A query may query several other traders, and this collection merges
 * their results into a single set. Since a query returns a sequence and
 * an iterator, this class collects returned iterators into one iterator.
 * The result is a distributed tree of iterators, which can become
 * inefficient when the trader graph is deep.
 */
class TAO_Offer_Iterator_Collection :
  public virtual POA_CosTrading::OfferIterator
{
public:
  TAO_Offer_Iterator_Collection (TAO_Offer_Iterator_Collection const&) = delete;
  TAO_Offer_Iterator_Collection& operator= (TAO_Offer_Iterator_Collection const&) = delete;
  TAO_Offer_Iterator_Collection (TAO_Offer_Iterator_Collection&&) = delete;
  TAO_Offer_Iterator_Collection& operator= (TAO_Offer_Iterator_Collection&&) = delete;

  TAO_Offer_Iterator_Collection () = default;

  ~TAO_Offer_Iterator_Collection () override;

  /// Retrieve n offers from the set of iterators.
  CORBA::Boolean next_n (CORBA::ULong n,
                         CosTrading::OfferSeq_out offers) override;

  /// Destroy the collection of iterators.
  void destroy () override;

  /// Determine how many offers are left in the collection.
  CORBA::ULong max_left () override;

  /// Add an iterator to the collection.
  void add_offer_iterator (CosTrading::OfferIterator_ptr offer_iter);

private:
  using Offer_Iters = std::deque<CosTrading::OfferIterator*>;

  /// The iterator collection.
  Offer_Iters iters_;
};

  // *************************************************************
  // TAO_Offer_Id_Iterator
  // *************************************************************

/**
 * @brief Stores the overflow of offer IDs from the Admin list_offers method.
 *
 * The OfferIdIterator interface returns offer identifiers from the
 * list_offers and list_proxies operations through successive operations.
 */
class TAO_Offer_Id_Iterator :
  public virtual POA_CosTrading::OfferIdIterator
{
 public:
  TAO_Offer_Id_Iterator (TAO_Offer_Id_Iterator const&) = delete;
  TAO_Offer_Id_Iterator& operator= (TAO_Offer_Id_Iterator const&) = delete;
  TAO_Offer_Id_Iterator (TAO_Offer_Id_Iterator&&) = delete;
  TAO_Offer_Id_Iterator& operator= (TAO_Offer_Id_Iterator&&) = delete;

  TAO_Offer_Id_Iterator () = default;

  ~TAO_Offer_Id_Iterator () override = default;

  /**
   * The max_left operation returns the number of offer identifiers
   * remaining in the iterator. The exception UnknownMaxLeft is raised
   * if the iterator cannot determine the remaining number of offer
   * identifiers (e.g., if the iterator determines its set of offer
   * identifiers through lazy evaluation).
   */
  CORBA::ULong max_left () override;

  /**
   * The destroy operation destroys the iterator. No further
   * operations can be invoked on an iterator after it has been
   * destroyed.
   */
  void destroy () override;

  /**
   * The next_n operation returns a set of offer identifiers in the
   * output parameter "ids." The operation returns n offer identifiers
   * if there are at least n offer identifiers remaining in the
   * iterator. If there are fewer than n offer identifiers in the
   * iterator, then all remaining offer identifiers are returned. The
   * actual number of offer identifiers returned can be determined
   * from the length of the "ids" sequence. The next_n operation
   * returns TRUE if there are further offer identifiers to be
   * extracted from the iterator. It returns FALSE if there are no
   * further offer identifiers to be extracted.
   */
  CORBA::Boolean next_n (CORBA::ULong _n,
                         CosTrading::OfferIdSeq_out _ids) override;

  /// Insert a @a new_id into the contents of the iterator.
  void insert_id(CosTrading::OfferId new_id);

 private:
  TAO_String_Queue ids_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_OFFER_ITERATOR */
