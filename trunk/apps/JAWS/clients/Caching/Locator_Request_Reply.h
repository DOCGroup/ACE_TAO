/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//    none
//
// = FILENAME
//    Locator_Request_Response.h
//
// = AUTHOR
//    Nanbor Wang
//
// ============================================================================

#if !defined (ACE_LOCATOR_REQUEST_REPLY_H)
#define ACE_LOCATOR_REQUEST_REPLY_H

#include "URL_Properties.h"

class ACE_Export ACE_URL_Locator_Request
// Nanbor, please make sure you explain what this class is *used* for!
  // = TITLE
  //     A URL request message formater/wrapper.
  //
  // = DESCRIPTION
  //     This class defines a URL request data.
{
public:
  enum ACE_URL_Locator_Request_Type
  {
    QUERY,
    EXPORT,
    WITHDRAW,
    DESCRIBE,
    MODIFY,
    INVALID_REQUEST		// LAST
  };
  // Request type

  ACE_URL_Locator_Request (void);
  // Default ctor.

  ~ACE_URL_Locator_Request (void);
  // Default dtor.

  int url_query (const int how,
		 const ACE_URL_Property_Seq &pseq,
		 const int how_many);
  // Query the locator for HTTP with designate properties (none, some,
  // or all).  The locator being queried will return a sequence of
  // offers with <how_many> offers in it.  This interface allocates
  // <offer> so users must deallocate it after use.

  int export_offer (const ACE_URL_Offer &offer);
  // Export an offer to the locator.

  int withdraw_offer (const ACE_WString &offer_id);
  // Withdraw an offer.  return 0 if succeed, -1 otherwise.

  int describe_offer (const ACE_WString &offer_id);
  // Query a specific offer.

  int modify_offer (const ACE_WString &offer_id,
		    const char *url = 0,
		    const ACE_URL_Property_Seq &del = 0,
		    const ACE_URL_Property_Seq &modify = 0);
  // Modify a previously registered offer.

  int modify_offer (const ACE_WString &offer_id,
		    const ACE_WString *url = 0,
		    const ACE_URL_Property_Seq &del = 0,
		    const ACE_URL_Property_Seq &modify = 0);
  // Modify a previously registered offer.

  int encode (void);
  // Encode request for network communication.

  int decode (void);
  // Restore from network data.

  // = A bunch of methods to access internal data.
  int how (void);
  int how_many (void);
  ACE_URL_Property_Seq &seq (void);
  ACE_URL_Property_Seq &del (void);
  ACE_URL_Property_Seq &modify (void);
  ACE_WString &id (void);
  ACE_WString &url (void);
  void *buffer (void);

  size_t encode (void);
  // Encode request for network communication.  If succeed,
  // returns the size of the buffer, otherwise, return 0.

  size_t decode (void *buffer);
  // Restore from network data.  Returns size of the buffer
  // if succeed, 0 otherwise.

  const int how (void) const;
  const int how_many (void) const;
  const ACE_URL_Property_Seq *seq (void) const;
  const ACE_URL_Property_Seq *del (void) const;
  const ACE_URL_Property_Seq *modify (void) const;
  const ACE_URL_Offer *offer (void) const;
  const ACE_WString &id (void) const;
  const ACE_WString &url (void) const;
  const char *buffer (void) const;
  // A bunch of methods to access internal data.

  void dump (void);
  // Print out this object.

protected:
  u_int code_;
  // Request type code.

  // Nanbor, please make sure that you comment all of these data
  // members.
  size_t bsize (void) const;
  // Return the size of buffer required to encode
  // this request.

  int how_;
  int how_many_;
  ACE_URL_Property_Seq *seq1_;
  // For query or del in modify_offer.

  ACE_URL_Property_Seq *seq2_;
  // For modify seq. in modify_offer.

  ACE_URL_Offer *offer_;

  ACE_WString id_;

  ACE_WString url_;

  char *buffer_;
};

#if defined (__ACE_INLINE__)
#include "Locator_Request_Reply.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_LOCATOR_REQUEST_REPLY_H */
