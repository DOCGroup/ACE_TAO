/* -*- C++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//    none
//
// = FILENAME
//    Web_Locator.h
//
// = AUTHOR
//    Nanbor Wang
//
// ============================================================================

// @@ Nanbor, should we change this name from "Web_Locator" to
// "Content_Locator" or "URL_Locator"?  It seems like "Web" is too general!

#if !defined (ACE_WEB_LOCATOR_H)
#define ACE_WEB_LOCATOR_H

#include "ace/OS.h"
#include "ace/Array.h"

class ACE_Export ACE_URL_Property
  // = TITLE
  //     Defines a property of a URL.
  //
  // = DESCRIPTION
  //     A property contains a <name> and a <value>.
{
public:
  ACE_URL_Property (LPCTSTR name = 0, LPCTSTR value=0);
  // Create a property.

  ACE_URL_Property (ACE_URL_Property &p);
  // Copy constructor.
  
  ~ACE_URL_Property (void);
  // Destructor.

  ACE_URL_Property &operator= (ACE_URL_Property &rhs);
  // Assignment operator.

  int operator== (ACE_URL_Property &rhs) const;
  // Equals operator.

  int operator!= (ACE_URL_Property &rhs) const;
  // Not equals operator.

  LPCTSTR name (void) const;
  // Query property name.

  void name (LPCTSTR name);
  // Set property name.

  LPCTSTR value (void) const;
  // Query property value.

  void value (LPCTSTR value);
  // Set property value.

protected:
  LPTSTR name_;
  // Property name pointer.
  
  LPTSTR value_;
  // Property value.
} ;

typedef ACE_Array<ACE_URL_Property> ACE_URL_Property_Seq;

class ACE_Export ACE_URL_Offer
  // = TITLE
  //     Defines a URL offer.
  //
  // = DESCRIPTION
  //     A URL offer is defined by an <url> and an
  //     <ACE_URL_Property_Seq>.
{
public:
  ACE_URL_Offer (LPCTSTR url, ACE_URL_Property_Seq &prop_seq);
  // Create an offer.

  ACE_URL_Offer (ACE_URL_Offer &o);
  // Copy ctor.

  ~ACE_URL_Offer (void);
  // Default destructor.

  ACE_URL_Offer &operator= (ACE_URL_Offer &rhs);
  // Assignment operator.

  int operator== (ACE_URL_Offer &rhs) const;
  // Equality operator.

  int operator!= (ACE_URL_Offer &rhs) const;
  // Inequality operator.

  LPCTSTR url (void) const;
  // Get URL string.

  Void url (LPCTSTR url);
  // Set URL.

  ACE_URL_Property_Seq &url_properties (void);
  // Get properties of this offer.
  
  void url_properties (ACE_URL_Property_Seq &prop);
  // Set properties of this offer.

protected:
  char *url_;
  // URL of this offer.

  ACE_URL_Property_Seq prop_;
  // Properties associate with this offer.

private:
  // @@ Nanbor, please make sure you move this definition into the *.cpp file.
  ACE_URL_Offer (void) {}
  // Do not allow access to default constructor
};

typedef ACE_Array<ACE_URL_Offer> ACE_URL_Offer_Seq;
typedef char *ACE_URL_OfferID;

class ACE_Export ACE_URL_Locator
  // = TITLE
  //     Base class for the URL locator.
  //
  // = DESCRIPTION
  //     This class provide an interface for clients to communicate
  //     with a URL locator thru a stream specified in <handle>.
{
public:
  enum ACE_Specified_Property 
  { 
    NONE,
    SOME,
    ALL
  };
  // Specify how to select offers.

  Ace_Url_Locator (ACE_HANDLE handle = ACE_INVALID_HANDLE,
		   const ACE_Time_Value &timeout = ACE_Time_Value::zero);
  // Try to "Bind" to a URL locator.

  ~ACE_URL_Locator (void);
  // Default destructor.

  void set_timeout (const ACE_Time_Value &timeout);
  // Set timeout value.

  ACE_Time_Value get_timeout (void) const;
  // Get timeout value for this locator.

  void set_handle (ACE_HANDLE handle);
  // Set the handle used to connect to locator.

  ACE_HANDLE get_handle (void) const;
  // Get the handle used to connect to locator.

  LPCTSTR error_status (void);
  // Provide detailed human readable error status.

protected:
  int errno_;
  // Record operation error (to avoid using exception.)

  ACE_Time_Value timeout_;
  // Timeout value when communicate with the connected location
  // server.

  ACE_HANDLE handle_;
  // Handle used to communicate with locator.
};

// @@ Nanbor, I'm not really sure why you are using inheritance here?
// Is this so that you don't have to reimplement the accessor methods
// in ACE_URL_Locator?  There doesn't seem to be any virtual functions
// in the base class.  Can you please explain this design choice to
// me?!

class ACE_Export ACE_URL_Locator_Query : public ACE_URL_Locator
  // = TITLE
  //     A query interface to locate a URL.
  //
  // = DESCRIPTION
  //     This class provide an interface for clients to
  //     query a URL locator with certain properties.
{
public:
  ACE_URL_Locator_Query (ACE_HANDLE handle = ACE_INVALID_HANDLE,
			 const ACE_Time_Value &timeout = ACE_Time_Value::zero);
  // Try to "Bind" to a URL locator.

  ~ACE_URL_Locator_Query (void);
  // Default destructor.

  int query (const ACE_Specified_Property how,
	     const ACE_URL_Property_Seq *pseq,
	     const int how_many,
	     ACE_URL_Offer_Seq *offer);
  // Query the locator for HTTP with designate properties (none, some,
  // or all).  The locator being queried will return a sequence of
  // offers with <how_many> offers in it.  This interface allocates
  // <offer> so users must deallocate it after use.
};

class ACE_Export ACE_URL_Locator_Register : public ACE_URL_Locator
  // = TITLE
  //     This class provides interfaces for HTTP servers to export
  //     their services.
  //
  // = DESCRIPTION
  //     HTTP servers can use this class to register their offers to a
  //     URL locator so clients can locate resources provided by
  //     servers thru the locator.
{
public:
  ACE_URL_Locator_Register (ACE_HANDLE handle = ACE_INVALID_HANDLE,
			    const ACE_Time_Value &timeout = ACE_Time_Value::zero);
  // Instantiate a registration interface to "locator."

  ~ACE_URL_Locator_Register (void);
  // Default destructor.

  ACE_URL_OfferID export (const ACE_URL_Offer *offer);
  // Export an offer to the locator.

  int withdraw (const ACE_URL_OfferID offer);
  // Withdraw an offer.  return 0 if succeed, -1 otherwise.

  ACE_URL_Offer *describe (const ACE_URL_OfferID offer);
  // Query a specific offer.

  int modify (const ACE_URL_OfferID offer,
	      const char *url = 0,
	      const ACE_URL_Property_Seq *del = 0,
	      const ACE_URL_Property_Seq *modify = 0);
  // Modify a previously registered offer.
};

#if defined (__ACE_INLINE__)
#include "Web_Locator.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_WEB_LOCATOR_H */
