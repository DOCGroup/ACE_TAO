/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//    none
//
// = FILENAME
//    Local_Locator.h
//
// = AUTHOR
//    Nanbor Wang
//
// ============================================================================

#if !defined (ACE_LOCAL_LOCATOR_H)
#define ACE_LOCAL_LOCATOR_H

#include "URL_Locator.h"
#include "ace/Containers.h"
#include "ID_Generator.h"

class ACE_URL_Record
  // = TITLE
  //     A URL Record.
  //
  // = DESCRIPTION
  //     A record placed in URL repository.  Notice that
  //     both member pointers are own by URL_Record.
  //     They will get deallocated when the object goes
  //     out of scope.
{
  friend class ACE_URL_Local_Locator;
  // Nanbor, is it really necessary to have ACE_Node<> as a friend?
  // That seems like it breaks encapsulation. 
  friend class ACE_Node<ACE_URL_Record>;
public:
  ACE_URL_Record (ACE_URL_Offer *offer);
  // ctor.

  ~ACE_URL_Record (void);
  // dtor.

  int operator== (const ACE_URL_Record &rhs) const;
  // Two records are equal if they have the same offer id.

  int operator!= (const ACE_URL_Record &rhs) const;
  // Unequal, complement of equal.

private:
  ACE_URL_Record (void);
  // Nanbor, please explain why this is private.
  // default ctor.

  ACE_WString *id_;
  // Offer ID in the repository.

  ACE_URL_Offer *offer_;
  // Offer (and its properties).
};

class ACE_Export ACE_URL_Local_Locator
  // = TITLE
  //     A simple URL repository to store URL offer locally.
  //
  // = DESCRIPTION
  //     This class manage a collection of URL offers
  //     for local query and registration.  But we should
  //     really use it within a server.
{
  virtual ~ACE_URL_Local_Locator (void);
  // Default destructor.

  virtual int url_query (const ACE_URL_Locator::ACE_Selection_Criteria how,
			 const ACE_URL_Property_Seq *pseq,
			 const size_t how_many,
			 size_t &num_query,
			 ACE_URL_Offer_Seq *offer);
  // Query the locator for HTTP with designate properties (none, some,
  // or all).  The locator being queried will return a sequence of
  // offers with <how_many> offers in it.  This interface allocates
  // <offer> so users must deallocate it after use.

  virtual int export_offer (ACE_URL_Offer *offer,
			    ACE_WString &offer_id);
  // Export an offer to the locator.

  virtual int withdraw_offer (const ACE_WString &offer_id);
  // Withdraw an offer.  return 0 if succeed, -1 otherwise.

  virtual int describe_offer (const ACE_WString &offer_id,
			      ACE_URL_Offer *offer);
  // Query a specific offer.

  virtual int modify_offer (const ACE_WString &offer_id,
			    const char *url = 0,
			    const ACE_URL_Property_Seq *del = 0,
			    const ACE_URL_Property_Seq *modify = 0);
  // Modify a previously registered offer.

protected:
  ACE_Unbounded_Stack<ACE_URL_Record> repository_;
  // Nanbor, please add a comment here.  In particular, why do you use
  // a stack rather than, e.g., a set?
};

#if defined (__ACE_INLINE__)
#include "Local_Locator.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_LOCAL_LOCATOR_H */
