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

#if !defined (ACE_URL_PROPERTIES_H)
#define ACE_URL_PROPERTIES_H

#include "ace/SString.h"
#include "ace/Array.h"

class ACE_Export ACE_WString_Helper
  // = TITLE
  //     Some helper functions for manipulate ACE_WString.
  //
  // = DESCRIPTION
  //     These functions simplify encoding/decoding of
  //     ACE_WString objects for network communication.
{
public:
  static size_t bsize (ACE_WString *wstr);
  // Returns the actual size required to contain the ACE_WString.

  static size_t encode (void *buf, ACE_WString *wstr);
  // Encode <wstr> into <buf> for network communication.
  // Return total octets consumed.

  static size_t decode (void *buf);
  // This function doesn't relate to ACE_WString directly.
  // It converts an ACE_USHORT16 string from network
  // byte order to host byte order.  Returns bsize of the string.
};

class ACE_Export ACE_URL_Property
  // = TITLE
  //     Defines a property of a URL.
  //
  // = DESCRIPTION
  //     A property contains a <name> and a <value>.
{
public:
  ACE_URL_Property (const char *name = 0, const char *value=0);
  // Create a property.

  ACE_URL_Property (const ACE_USHORT16 *name,
		    const ACE_USHORT16 *value);

  ACE_URL_Property (const ACE_URL_Property &p);
  // Copy constructor.
  
  ~ACE_URL_Property (void);
  // Destructor.

  ACE_URL_Property &operator= (const ACE_URL_Property &rhs);
  // Assignment operator.

  int operator== (const ACE_URL_Property &rhs) const;
  // Equals operator.

  int operator!= (const ACE_URL_Property &rhs) const;
  // Not equals operator.

  ACE_WString *name_rep (void);
  const ACE_WString *name (void) const;
  // Query property name.

  void name (const ACE_USHORT16 *n);
  void name (const char *n);
  // Set property name.

  ACE_WString *value_rep (void);
  const ACE_WString *value (void) const;
  // Query property value.

  void value (const ACE_USHORT16 *v);
  void value (const char *v);
  // Set property value. 

  // Some helper functions for encoding and decoding.
  size_t bsize (void) const;
  // Returns memory size required to encode this object.

  size_t encode (void *buf) const;
  // Encodes this object into buf for network transmission.

  size_t decode (void *buf);
  // Decodes buf and modifies this object, you should
  // probably create this with default ctor.  

  void dump (void) const;
  // Dump out this object for debug.
  
protected:
  ACE_WString *name_;
  // Property name pointer.
  
  ACE_WString *value_;
  // Property value.
} ;

typedef ACE_Array<ACE_URL_Property> ACE_URL_Property_Seq;

class ACE_Export ACE_URL_Offer
  // = TITLE
  //     Defines a URL offer.
  //
  // = DESCRIPTION
  //     A URL offer is defined by a <url> and an
  //     <ACE_URL_Property_Seq>.
{
public:
  ACE_URL_Offer (const size_t size = 1, const char *url = 0);
  // Create an offer.

  ACE_URL_Offer (const ACE_URL_Offer &o);
  // Copy ctor.

  ~ACE_URL_Offer (void);
  // Default destructor.

  ACE_URL_Offer &operator= (const ACE_URL_Offer &rhs);
  // Assignment operator.

  int operator== (const ACE_URL_Offer &rhs) const;
  // Equality operator.

  int operator!= (const ACE_URL_Offer &rhs) const;
  // Inequality operator.

  ACE_WString *url_rep (void);
  const ACE_WString *url (void) const;
  // Get URL string.

  void url (const char *url);
  void url (const ACE_USHORT16 *url);
  // Set URL.

  ACE_URL_Property_Seq &url_properties (void);
  // Get properties of this offer.
  
  void url_properties (const ACE_URL_Property_Seq &prop);
  // Set properties of this offer.  This operation
  // virtually get a copy of the passed in prop.

  // Some helper functions for encoding and decoding.
  size_t bsize (void) const;
  // Returns memory size required to encode this object.

  size_t encode (void *buf) const;
  // Encodes this object into buf for network transmission.

  size_t decode (void *buf);
  // Decodes buf into current object, you better use
  // the default ctor.

  void dump (void) const;
  // Dump this object for debug.

protected:
  ACE_WString *url_;
  // URL of this offer.

  ACE_URL_Property_Seq prop_;
  // Properties associate with this offer.
};

typedef ACE_Array<ACE_URL_Offer> ACE_URL_Offer_Seq;
typedef char *ACE_URL_OfferID;

#if defined (__ACE_INLINE__)
#include "URL_Properties.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_WEB_PROPERTIES_H */
