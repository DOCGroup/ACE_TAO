// -*- C++ -*-


//=============================================================================
/**
 *  @file    URL_Properties.h
 *
 *  @author Nanbor Wang
 */
//=============================================================================


#ifndef ACE_URL_PROPERTIES_H
#define ACE_URL_PROPERTIES_H

#include "ace/SString.h"
#include "ace/svc_export.h"

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
  explicit ACE_URL_Property (char const *name = nullptr,
                             char const *value = nullptr);

  /// Create a new property using wchar strings.  This is mostly used
  /// to support DBCS or UNICODE.
  ACE_URL_Property (ACE_UINT16 const *name,
                    ACE_UINT16 const *value);

  ACE_URL_Property (ACE_URL_Property const &) = default;

  ~ACE_URL_Property () = default;

  ACE_URL_Property &operator= (ACE_URL_Property const &) = default;

  /// Equals operator.
  bool operator== (ACE_URL_Property const &rhs) const;

  /// Inequality operator.
  bool operator!= (ACE_URL_Property const &rhs) const;

  // = Query property name.
  ACE_WString &name_rep ();
  ACE_WString const &name () const;

  // = Set property name.
  void name (ACE_UINT16 const *n);
  void name (char const *n);

  // = Query property value.
  ACE_WString &value_rep ();
  ACE_WString const &value () const;

  // = Set property value.
  void value (ACE_UINT16 const *v);
  void value (char const *v);

  // = Helper functions for encoding and decoding.
  /// Returns memory size (in bytes) required to encode this object.
  size_t size () const;

  /// Encodes this object into buf for network transmission.
  size_t encode (void *buf) const;

  /// Decodes buf and modifies this object, you should
  /// probably create this with default ctor.
  size_t decode (void *buf);

  /// Dump out this object for debug.
  void dump () const;

protected:
  /// Property name pointer.
  ACE_NS_WString name_;

  /// Property value.
  ACE_NS_WString value_;
} ;

using ACE_URL_Property_Seq = ACE_Array<ACE_URL_Property>;
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
  explicit ACE_URL_Offer (size_t size = 1, char const *url = nullptr);

  ACE_URL_Offer (ACE_URL_Offer const &) = default;

  ~ACE_URL_Offer () = default;

  ACE_URL_Offer &operator= (ACE_URL_Offer const &) = default;

  /// Equality operator.
  bool operator== (ACE_URL_Offer const &rhs) const;

  /// Inequality operator.
  bool operator!= (ACE_URL_Offer const &rhs) const;

  // = Get URL string.
  ACE_WString &url_rep ();
  ACE_WString const &url () const;

  // = Set URL.
  void url (char const *url);
  void url (ACE_UINT16 const *url);

  /// Get properties of this offer.
  ACE_URL_Property_Seq &url_properties ();

  /// Set properties of this offer.  This operation virtually get a
  /// copy of the passed in prop.
  void url_properties (ACE_URL_Property_Seq const &prop);

  // = Helper functions for encoding and decoding.
  /// Returns memory size (in bytes) required to encode this object.
  size_t size () const;

  /// Encodes this object into buf for network transmission.
  size_t encode (void *buf) const;

  /// Decodes buf into current object, you better use
  /// the default ctor.
  size_t decode (void *buf);

  /// Dump this object for debug.
  void dump () const;

protected:
  /// URL of this offer.
  ACE_NS_WString url_;

  /// Properties associate with this offer.
  ACE_URL_Property_Seq prop_;
};

using ACE_URL_Offer_Seq = ACE_Array<ACE_URL_Offer>;
// type of URL offer collections.

#if defined (__ACE_INLINE__)
#include "URL_Properties.inl"
#endif /* __ACE_INLINE__ */

#endif /* ACE_WEB_PROPERTIES_H */
