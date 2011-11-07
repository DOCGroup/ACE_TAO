// -*- C++ -*-


//=============================================================================
/**
 *  @file    URL_Properties.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang
 */
//=============================================================================


#ifndef ACE_URL_PROPERTIES_H
#define ACE_URL_PROPERTIES_H

#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers.h"

/**
 * @class ACE_WString_Helper
 *
 * @brief Some helper functions for manipulate ACE_WString.
 *
 * These functions simplify encoding/decoding of
 * ACE_WString objects for network communication.
 */
class ACE_Svc_Export ACE_WString_Helper
{
public:
  /// Returns the actual size (in bytes) required to contain the
  /// ACE_WString.
  static size_t size (const ACE_WString &wstr);

  /// Encode <wstr> into <buf> for network communication.
  /// Return total octets consumed.
  static size_t encode (void *buf, const ACE_WString &wstr);

  /**
   * This function doesn't relate to ACE_WString directly.
   * It converts an ACE_UINT16 string from network
   * byte order to host byte order.  Returns size of the string.
   */
  static size_t decode (void *buf);
};

/**
 * @class ACE_URL_Property
 *
 * @brief Defines a property of a URL.
 *
 * A property contains a <name> and a <value>.
 * A URL may contain some properties and we can "locate"
 * the URL's we are looking for by examming URL for certain
 * properties that match our need.
 */
class ACE_Svc_Export ACE_URL_Property
{
public:
  /// Create a new property.
  ACE_URL_Property (const char *name = 0,
                    const char *value=0);

  /// Create a new property using wchar strings.  This is mostly used
  /// to support DBCS or UNICODE.
  ACE_URL_Property (const ACE_UINT16 *name,
                    const ACE_UINT16 *value);

  /// Copy constructor.
  ACE_URL_Property (const ACE_URL_Property &p);

  /// Destructor.
  ~ACE_URL_Property (void);

  /// Assignment operator.
  ACE_URL_Property &operator= (const ACE_URL_Property &rhs);

  /// Equals operator.
  bool operator== (const ACE_URL_Property &rhs) const;

  /// Inequality operator.
  bool operator!= (const ACE_URL_Property &rhs) const;

  // = Query property name.
  ACE_WString &name_rep (void);
  const ACE_WString &name (void) const;

  // = Set property name.
  void name (const ACE_UINT16 *n);
  void name (const char *n);

  // = Query property value.
  ACE_WString &value_rep (void);
  const ACE_WString &value (void) const;

  // = Set property value.
  void value (const ACE_UINT16 *v);
  void value (const char *v);

  // = Helper functions for encoding and decoding.
  /// Returns memory size (in bytes) required to encode this object.
  size_t size (void) const;

  /// Encodes this object into buf for network transmission.
  size_t encode (void *buf) const;

  /// Decodes buf and modifies this object, you should
  /// probably create this with default ctor.
  size_t decode (void *buf);

  /// Dump out this object for debug.
  void dump (void) const;

protected:
  /// Property name pointer.
  ACE_WString name_;

  /// Property value.
  ACE_WString value_;
} ;

typedef ACE_Array<ACE_URL_Property> ACE_URL_Property_Seq;
// type of URL_Property collections.

/**
 * @class ACE_URL_Offer
 *
 * @brief Defines a URL offer.
 *
 * A URL offer is defined by a <url> and an
 * <ACE_URL_Property_Seq>.  An offer is stored at server end
 * thru registering or reported back to querying client from the
 * sever.
 */
class ACE_Svc_Export ACE_URL_Offer
{
public:
  /// Create an offer.
  ACE_URL_Offer (const size_t size = 1, const char *url = 0);

  /// Copy ctor.
  ACE_URL_Offer (const ACE_URL_Offer &o);

  /// Default destructor.
  ~ACE_URL_Offer (void);

  /// Assignment operator.
  ACE_URL_Offer &operator= (const ACE_URL_Offer &rhs);

  /// Equality operator.
  bool operator== (const ACE_URL_Offer &rhs) const;

  /// Inequality operator.
  bool operator!= (const ACE_URL_Offer &rhs) const;

  // = Get URL string.
  ACE_WString &url_rep (void);
  const ACE_WString &url (void) const;

  // = Set URL.
  void url (const char *url);
  void url (const ACE_UINT16 *url);

  /// Get properties of this offer.
  ACE_URL_Property_Seq &url_properties (void);

  /// Set properties of this offer.  This operation virtually get a
  /// copy of the passed in prop.
  void url_properties (const ACE_URL_Property_Seq &prop);

  // = Helper functions for encoding and decoding.
  /// Returns memory size (in bytes) required to encode this object.
  size_t size (void) const;

  /// Encodes this object into buf for network transmission.
  size_t encode (void *buf) const;

  /// Decodes buf into current object, you better use
  /// the default ctor.
  size_t decode (void *buf);

  /// Dump this object for debug.
  void dump (void) const;

protected:
  /// URL of this offer.
  ACE_WString url_;

  /// Properties associate with this offer.
  ACE_URL_Property_Seq prop_;
};

typedef ACE_Array<ACE_URL_Offer> ACE_URL_Offer_Seq;
// type of URL offer collections.

#if defined (__ACE_INLINE__)
#include "URL_Properties.inl"
#endif /* __ACE_INLINE__ */

#endif /* ACE_WEB_PROPERTIES_H */
