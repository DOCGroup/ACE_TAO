/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//    none
//
// = FILENAME
//    Location_Server.h
//
// = AUTHOR
//    Nanbor Wang
//
// ============================================================================

#if !defined (ACE_LOCATION_SERVER_H)
#define ACE_LOCATION_SERVER_H

#include "ace/Containers.h"
#include "Web_Locator.h"

class ACE_URL_Record
  // = TITLE
  //     An offer record in the offer repository.
  //
  // = DESCRIPTION
  //     This class holds an offer record in offer repository
{
public:
  ACE_URL_Record (ACE_URL_OfferID id, ACE_URL_Offer &offer);
  // Construct an URL record.

  ~ACE_URL_Record (void) const;
  // Destructor.

  ACE_URL_OfferID id (void);
  // Get internal id string.

  void id (ACE_URL_OfferID id);
  // Set id string.

Protected:
  ACE_URL_OfferID id_;
  // Offer ID of this record.

  ACE_URL_Offer offer_;
  // URL Offer in this record.
};

typedef ACE_Unbounded_Queue<ACE_URL_Record> ACE_URL_Repository;
typedef ACE_Unbounded_Queue_Iterator<ACE_URL_Record> ACE_URL_Repository_Iterator;

class ACE_Location_Service
  // = TITLE
  //     Location Servers maintain a collection of offers other URL
  //     servers willing to provide.
  //
  // = DESCRIPTION
  //     This class implements the location server's end point.  Clients
  //     connect to it to query offer information.  Providers (servers) 
  //     connect to it to register their offer.  
{
public:
  ACE_URL_OfferID export (ACE_URL_Offer &offer);
  // Register an offer to the location server.  Operation will fail
  // if there is an offer with identical URL string exist in the 
  // repository and the function returns NULL.  On success, it returns
  // an OfferID.

  int query (const ACE_Specified_Property how,
	     const ACE_URL_Property_Seq *pseq,
	     const int how_many,
	     ACE_URL_Offer_Seq *offer);
  // Query the offer repository whether there're offers that match our
  // requirement specified in <how> and <pseq>.  Create an offer sequence
  // that contains at most <how_many> offers in it.  Offers are picked out 
  // from the beginning.  So if there are too many offers that meet our
  // requirement, some may never get picked.  On success, the function
  // return 0, non-zero indicates error condition.  

  int withdraw (const ACE_URL_OfferID offer);
  // Withdraw an image offer.  return 0 if succeed, -1 otherwise (most
  // likely, there isn't an offer with id:<offer>.

  ACE_URL_Offer *describe (const ACE_URL_OfferID offer);
  // Query a specific offer.  This function returns a pointer
  // points to the ACE_URL_Offer field with corresponding <offer> id.

  int modify (const ACE_URL_OfferID offer,
	      const char *url = 0,
	      const ACE_URL_Property_Seq *del = 0,
	      const ACE_URL_Property_Seq *modify = 0);
  // Modify an already registered offer.  If <url> != 0, the URL of
  // the offer will be replaced by string pointed by <url>.  <del>
  // points to a sequence of properties to be delete from the offer.
  // <modify> points to a sequence of properties to be modified.  It
  // this offer contains a property with the same name, the value of this
  // property will be replaced by the new value.  If the offer doesn't
  // contain the same property, this property will be added to the offer.

protected:
  ACE_URL_Repository repository_;
  // URL back store.

  unsigned long id_counter_;
  // This number is used to maintain the uniqueness of OfferID.
  // An offer ID is derived from concatenating two strings.  First number is
  // the ASCII representation of number from ACE_OS::time() and the second
  // number is the ASCII representation of <id_counter_>.  The <id_counter_>
  // get incremented everytime a new offer ID is generated.  The length of
  // Offer ID is yet to be determined.
};

#if define (__ACE_INLINE__)
#include "Location_Server.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_LOCATION_SERVER_H */
