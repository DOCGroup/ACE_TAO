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

class ACE_Export ACE_URL_Locator
  // = TITLE
  //     Abstract Base class designates what interfaces a URL_Locator
  //     should provide.
  //
  // = DESCRIPTION
  //     This class defines the basic URL_Locator APIs.
{
public:
  enum ACE_Properties_Criteria
  { 
    NONE,
    SOME,
    ALL
  };
  // Specify how to select offers.

  virtual ~ACE_URL_Locator (void) = 0;
  // Default destructor.

  virtual int query (const ACE_Specified_Property how,
		     const ACE_URL_Property_Seq *pseq,
		     const int how_many,
		     ACE_URL_Offer_Seq *offer) = 0;
  // Query the locator for HTTP with designate properties (none, some,
  // or all).  The locator being queried will return a sequence of
  // offers with <how_many> offers in it.  This interface allocates
  // <offer> so users must deallocate it after use.

  virtual ACE_URL_OfferID export (const ACE_URL_Offer *offer) = 0;
  // Export an offer to the locator.

  virtual int withdraw (const ACE_URL_OfferID offer) = 0;
  // Withdraw an offer.  return 0 if succeed, -1 otherwise.

  virtual ACE_URL_Offer *describe (const ACE_URL_OfferID offer) = 0;
  // Query a specific offer.

  virtual int modify (const ACE_URL_OfferID offer,
		      const char *url = 0,
		      const ACE_URL_Property_Seq *del = 0,
		      const ACE_URL_Property_Seq *modify = 0) = 0;
  // Modify a previously registered offer.

  virtual LPCTSTR error_status (void) = 0;
  // Provide detailed human readable error status.

};

#if defined (__ACE_INLINE__)
#include "URL_Locator.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_WEB_LOCATOR_H */
