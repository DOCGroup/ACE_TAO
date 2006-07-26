// -*- C++ -*-

//=============================================================================
/**
 *  @file     NIOP_Endpoint.h
 *
 *  $Id: NIOP_Endpoint.h,v 1.9 2006/04/19 11:38:50 jwillemsen Exp $
 *
 *   NIOP implementation of PP Framework Endpoint interface.
 *
 *  @author  Johnny Willemsen <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_NIOP_ENDPOINT_H
#define TAO_NIOP_ENDPOINT_H
#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_NIOP) && (TAO_HAS_NIOP != 0)

#include "tao/Strategies/strategies_export.h"
#include "tao/Endpoint.h"
#include "tao/CORBA_String.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_NIOP_Connection_Handler;

/**
 * @class TAO_NIOP_Endpoint
 *
 * @brief TAO_NIOP_Endpoint
 *
 * NIOP-specific implementation of PP Framework Endpoint interface.
 */
class TAO_Strategies_Export TAO_NIOP_Endpoint : public TAO_Endpoint
{
public:

  friend class TAO_NIOP_Profile;
  friend class TAO_SSLIOP_Profile;

  // = Initialization and termination methods.

  /// Default constructor.
  TAO_NIOP_Endpoint (void);

  /// Constructor.  This is the most efficient constructor since it
  /// does not require any address resolution processing.
  TAO_NIOP_Endpoint (const char *host,
                     CORBA::UShort port);
//                     const ACE_INET_Addr &addr,
//                     CORBA::Short priority = TAO_INVALID_PRIORITY);

  /// Constructor.
//  TAO_NIOP_Endpoint (const ACE_INET_Addr &addr,
//                     int use_dotted_decimal_addresses);

  /// Constructor.  This constructor is used when decoding endpoints.
//  TAO_NIOP_Endpoint (const char *host,
//                     CORBA::UShort port,
//                     CORBA::Short priority);

  /// Destructor.
  ~TAO_NIOP_Endpoint (void);

  /**
   * @name TAO_Endpoint Methods
   *
   * Please check the documentation in Endpoint.h for details.
   */
  //@{
  virtual TAO_Endpoint *next (void);
  virtual int addr_to_string (char *buffer, size_t length);
  virtual TAO_Endpoint *duplicate (void);

  /// Return true if this endpoint is equivalent to <other_endpoint>.  Two
  /// endpoints are equivalent iff their port and host are the same.
  CORBA::Boolean is_equivalent (const TAO_Endpoint *other_endpoint);

  /// Return a hash value for this object.
  CORBA::ULong hash (void);
  //@}

  // = NIOP_Endpoint-specific methods.

  /// Return a pointer to the host string.  This object maintains
  /// ownership of this string.
  const char *host (void) const;

  /// Copy the string <h> into <host_> and return the resulting pointer.
  /// This object maintains ownership of this string.
  const char *host (const char *h);

  /// Return the port number.
  CORBA::UShort port (void) const;

  /// Set the port number.
  CORBA::UShort port (CORBA::UShort p);

private:

  /// Helper method for setting INET_Addr.
//  int set (const ACE_INET_Addr &addr,
//           int use_dotted_decimal_addresses);

  /// String representing the host name.
  CORBA::String_var host_;

  /// TCP port number.
  CORBA::UShort port_;

  /// Flag to indicate if the address has been resolved and set.
//  int object_addr_set_;

  /// NIOP Endpoints can be stringed into a list.  Return the next
  /// endpoint in the list, if any.
  TAO_NIOP_Endpoint *next_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Strategies/NIOP_Endpoint.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_NIOP && TAO_HAS_NIOP != 0 */

#include /**/ "ace/post.h"
#endif  /* TAO_NIOP_PROFILE_H */
