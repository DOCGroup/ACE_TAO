/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Offer_Iterators.h
 *
 *  $Id$
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 *  @author Seth Widoff <sbw1@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_OFFER_ITERATORS_H
#define TAO_OFFER_ITERATORS_H
#include /**/ "ace/pre.h"

#include "Trader_Utils.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

  // *************************************************************
  // TAO_Offer_Iterator
  // *************************************************************

class TAO_Offer_Iterator :
  public virtual POA_CosTrading::OfferIterator,
  public virtual PortableServer::RefCountServantBase
{
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
public:
  // = Initialization and termination methods.

  TAO_Offer_Iterator (const TAO_Property_Filter& property_filter);

  virtual ~TAO_Offer_Iterator (void);

  /**
   * BEGIN SPEC
   * The destroy operation destroys the iterator. No further
   * operations can be invoked on an iterator after it has been
   * destroyed.
   * END SPEC
   */
  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Add an offer to the collection of offers the iterator will
  /// iterate over.
  virtual void add_offer (CosTrading::OfferId offer_id,
                          const CosTrading::Offer* offer) = 0;

  virtual CORBA::ULong max_left (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosTrading::UnknownMaxLeft)) = 0;

  // BEGIN SPEC
  // The max_left operation returns the number of service offers
  // remaining in the iterator. The exception UnknownMaxLeft is raised
  // if the iterator cannot determine the remaining number of service
  // offers (e.g., if the iterator determines its set of service
  // offers through lazy evaluation).
  // END SPEC

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
  virtual CORBA::Boolean next_n (CORBA::ULong n,
                                 CosTrading::OfferSeq_out offers
                                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
 protected:

  TAO_Offer_Iterator& operator=(const TAO_Offer_Iterator&);

  /// The filter through which each returned offer must pass. Used to
  /// strip offers of undesirable properties.
  TAO_Property_Filter pfilter_;
};

  // *************************************************************
  // TAO_Query_Only_Offer_Iterator
  // *************************************************************

class TAO_Query_Only_Offer_Iterator
  : public TAO_Offer_Iterator
  // = TITLE
  //     An implementation of the CosTrading::OfferIterator IDL
  //     interface that takes advantage of the fact that the trader is
  //     Query Only, and therefore its service offers are not being
  //     changed or removed.
  //
  // = DESCRIPTION
  //     The iterator stores pointers to Offers to be iterated over,
  //     and later accesses the Offers directly through the pointers.
  //
{
public:
  // = Initialization and termination methods.

  TAO_Query_Only_Offer_Iterator (const TAO_Property_Filter& pfilter);

  virtual ~TAO_Query_Only_Offer_Iterator (void);

  /// Deposit at maximum n offers into the return sequence and return 1,
  /// or return 0 if the iterator is done and no offers are returned.
  virtual CORBA::Boolean next_n (CORBA::ULong n,
                                 CosTrading::OfferSeq_out offers
                                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the number of items left in the iterator.
  virtual CORBA::ULong max_left (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosTrading::UnknownMaxLeft));

  /// Add an offer the iterator should iterate over.
  void add_offer (CosTrading::OfferId offer_id,
                  const CosTrading::Offer* offer);

private:

  TAO_Query_Only_Offer_Iterator (const TAO_Query_Only_Offer_Iterator&);
  TAO_Query_Only_Offer_Iterator& operator=(const TAO_Query_Only_Offer_Iterator&);

  /// Structure that stores pointers to offers
  /// to iterate over.
  ACE_Unbounded_Queue <CosTrading::Offer *> offers_;
};

  // *************************************************************
  // TAO_Offer_Iterator_Collection
  // *************************************************************

class TAO_Offer_Iterator_Collection :
  public virtual POA_CosTrading::OfferIterator,
  public virtual PortableServer::RefCountServantBase
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
//   the trader graph is deep enough.
{
public:

  // = Constructors.

  TAO_Offer_Iterator_Collection (void);

  virtual ~TAO_Offer_Iterator_Collection (void);

  /// Retrieve n offers from the set of iterators.
  virtual CORBA::Boolean next_n (CORBA::ULong n,
                                 CosTrading::OfferSeq_out offers
                                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Destroy the collection of iterators.
  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Determine how many offers are left in the collection.
  virtual CORBA::ULong max_left (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                    CosTrading::UnknownMaxLeft));

  /// Add an iterator to the collection.
  void add_offer_iterator (CosTrading::OfferIterator_ptr offer_iter);

private:

  TAO_Offer_Iterator_Collection (const TAO_Offer_Iterator_Collection&);
  TAO_Offer_Iterator_Collection& operator= (const TAO_Offer_Iterator_Collection&);

  typedef ACE_Unbounded_Queue <CosTrading::OfferIterator*> Offer_Iters;

  /// The iterator collection.
  Offer_Iters iters_;
};

  // *************************************************************
  // TAO_Offer_Id_Iterator
  // *************************************************************

class TAO_Offer_Id_Iterator :
  public virtual POA_CosTrading::OfferIdIterator,
  public virtual PortableServer::RefCountServantBase
// = TITLE
//   Silly little iterator that contains the overflow of offer ids
//   from the Admin list_offers method.
//
// = DESCRIPTION
//
//   BEGIN SPEC
//   The OfferIdIterator interface is used to return a set of offer
//   identifiers from the list_offers operation and the list_proxies
//   operation in the Admin interface by enabling the offer identifiers
//   to be extracted by successive operations on the OfferIdIterator
//   interface.
//   END SPEC
{
 public:

  /// No op constructor
  TAO_Offer_Id_Iterator(void);

  ~TAO_Offer_Id_Iterator (void);

  /**
   * The max_left operation returns the number of offer identifiers
   * remaining in the iterator. The exception UnknownMaxLeft is raised
   * if the iterator cannot determine the remaining number of offer
   * identifiers (e.g., if the iterator determines its set of offer
   * identifiers through lazy evaluation).
   */
  virtual CORBA::ULong max_left(ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosTrading::UnknownMaxLeft));

  /**
   * The destroy operation destroys the iterator. No further
   * operations can be invoked on an iterator after it has been
   * destroyed.
   */
  virtual void destroy(ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

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
  virtual CORBA::Boolean next_n(CORBA::ULong _n,
                                CosTrading::OfferIdSeq_out _ids
                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Insert a <new_id> into the contents of the iterator.
  void insert_id(CosTrading::OfferId new_id);

 private:

  TAO_Offer_Id_Iterator (const TAO_Offer_Id_Iterator&);
  TAO_Offer_Id_Iterator& operator= (TAO_Offer_Id_Iterator&);

  TAO_String_Queue ids_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_OFFER_ITERATOR */
