/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//    none
//
// = FILENAME
//    URL_Locator.h
//
// = AUTHOR
//    Nanbor Wang
//
// ============================================================================

#if !defined (ACE_URL_LOCATOR_H)
#define ACE_URL_LOCATOR_H

#include "URL_Properties.h"

class ACE_Export ACE_URL_Locator
  // = TITLE
  //     Abstract Base class designates what interfaces a URL_Locator
  //     should provide.
  //
  // = DESCRIPTION
  //     This class defines the basic URL_Locator APIs.
// Nanbor, please make sure that you explain more about what a URL
// Locator is in the description here.
{
public:

  // = Specify how to select offers.
  // Nanbor, please add comments to each of these enumerals.
  enum ACE_Selection_Criteria
  { 
    NONE,
    SOME,
    ALL,
    INVALID_SELECTION
  };

  enum ACE_URL_Locator_Error
    // Nanbor, I don't think that you need to use this enum.  I
    // recommend that you return -1 from methods that fail and set
    // errno accordingly.
  {
    OK = 0,			// everything's OK so far.
    OFFER_EXIST,		// trying to register an offer.
				// that is already exist in repository.
    NOMEM,			// no memory available.
    NO_SUCH_OFFER,		// No such offer in the repository.
    INVALID_ARGUMENT,		// Invalid argument encountered.
    UNIMPLEMENTED,		// function not implemented.
    UNKNOWN,			// Unknown error.
    MAX_URL_ERROR
  };
  // Possible error code of URL_Locator

  virtual ~ACE_URL_Locator (void) = 0;
  // Default destructor.

  virtual int url_query (const ACE_Selection_Criteria how,
			 const ACE_URL_Property_Seq *pseq,
			 const size_t how_many,
			 size_t &num_query,
			 ACE_URL_Offer_Seq *offer) = 0;
  // Query the locator for HTTP with designate properties (none, some,
  // or all).  The locator being queried will return a sequence of
  // offers with <how_many> offers in it.  This interface allocates
  // <offer> so users must deallocate it after use.

  virtual int export_offer (ACE_URL_Offer *offer,
			    ACE_WString &offer_id) = 0;
  // Export an offer to the locator.

  virtual int withdraw_offer (const ACE_WString &offer_id) = 0;
  // Withdraw an offer.  return 0 if succeed, -1 otherwise.

  virtual int describe_offer (const ACE_WString &offer_id,
			      ACE_URL_Offer *offer) = 0;
  // Query a specific offer.

  virtual int modify_offer (const ACE_WString &offer_id,
			    const char *url = 0,
			    const ACE_URL_Property_Seq *del = 0,
			    const ACE_URL_Property_Seq *modify = 0) = 0;
  // Modify a previously registered offer.

  virtual char *error_status (void);
  // Provide a human readable error status.
  // Nanbor, please make sure that you explain who is responsible for
  // the memory returned from this method.  Also, I recommend that you
  // make this return a *const* char *.
};

#if defined (__ACE_INLINE__)
#include "URL_Locator.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_WEB_LOCATOR_H */
