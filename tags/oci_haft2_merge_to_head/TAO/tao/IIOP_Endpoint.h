// -*- C++ -*-

//=============================================================================
/**
 *  @file     IIOP_Endpoint.h
 *
 *  $Id$
 *
    IIOP implementation of PP Framework Endpoint interface.
 *
 *  @author  Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_IIOP_ENDPOINT_H
#define TAO_IIOP_ENDPOINT_H

#include /**/ "ace/pre.h"

#include "tao/Endpoint.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CORBA_String.h"
#include "ace/INET_Addr.h"


class TAO_IIOP_Connection_Handler;

/**
 * @class TAO_IIOP_Endpoint
 *
 * @brief TAO_IIOP_Endpoint
 *
 * IIOP-specific implementation of PP Framework Endpoint interface.
 */
class TAO_Export TAO_IIOP_Endpoint : public TAO_Endpoint
{
public:

  // @todo Lose these friends!
  friend class TAO_IIOP_Profile;
  friend class TAO_SSLIOP_Profile;

  // = Initialization and termination methods.

  /// Default constructor.
  TAO_IIOP_Endpoint (void);

  /// Constructor.  This is the most efficient constructor since it
  /// does not require any address resolution processing.
  TAO_IIOP_Endpoint (const char *host,
                     CORBA::UShort port,
                     const ACE_INET_Addr &addr,
                     CORBA::Short priority = TAO_INVALID_PRIORITY);

  /// Constructor.
  TAO_IIOP_Endpoint (const ACE_INET_Addr &addr,
                     int use_dotted_decimal_addresses);

  /// Constructor.  This constructor is used when decoding endpoints.
  TAO_IIOP_Endpoint (const char *host,
                     CORBA::UShort port,
                     CORBA::Short priority);

  /// Destructor.
  ~TAO_IIOP_Endpoint (void);

  // = Implementation of abstract TAO_Endpoint methods.  See
  // Endpoint.h for their documentation.

  virtual TAO_Endpoint *next (void);
  virtual int addr_to_string (char *buffer, size_t length);

  /// Makes a copy of <this>
  virtual TAO_Endpoint *duplicate (void);

  /// Return true if this endpoint is equivalent to <other_endpoint>.  Two
  /// endpoints are equivalent iff their port and host are the same.
  virtual CORBA::Boolean is_equivalent (const TAO_Endpoint *other_endpoint);

  /// Return a hash value for this object.
  virtual CORBA::ULong hash (void);

  // = IIOP_Endpoint-specific methods.

  /// Return a reference to the <object_addr>.
  const ACE_INET_Addr &object_addr (void) const;

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
  int set (const ACE_INET_Addr &addr,
           int use_dotted_decimal_addresses);

  /// String representing the host name.
  CORBA::String_var host_;

  /// TCP port number.
  CORBA::UShort port_;

  /// Cached instance of <ACE_INET_Addr> for use in making
  /// invocations, etc.
  mutable ACE_INET_Addr object_addr_;

  /// Flag to indicate if the address has been resolved and set.
  mutable int object_addr_set_;

  /// IIOP Endpoints can be stringed into a list.  Return the next
  /// endpoint in the list, if any.
  TAO_IIOP_Endpoint *next_;

};


#if defined (__ACE_INLINE__)
# include "tao/IIOP_Endpoint.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif  /* TAO_IIOP_PROFILE_H */
