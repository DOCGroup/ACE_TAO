// $Id$
//===========================================================================
/**
 *  @file     SCIOP_Endpoint.h
 *
 *   SCIOP implementation of PP Framework Endpoint interface.
 *
 *
 *  @author  Jason Cohen, Lockheed Martin ATL  <jcohen@atl.lmco.com>
 *  @author  Keith O'Hara, Lockheed Martin ATL
 *  @author  based on IIOP_Endpoint by
 *  @author  Marina Spivak <marina@cs.wustl.edu>
 */
//==========================================================================


#ifndef TAO_SCIOP_ENDPOINT_H
#define TAO_SCIOP_ENDPOINT_H

#include "ace/pre.h"

#include "tao/Endpoint.h"

#if TAO_HAS_SCIOP == 1

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CORBA_String.h"
#include "ace/INET_Addr.h"


class TAO_SCIOP_Connection_Handler;

/**
 * @class TAO_SCIOP_Endpoint
 *
 * @brief TAO_SCIOP_Endpoint
 *
 * SCIOP-specific implementation of PP Framework Endpoint interface.
 */
class TAO_Export TAO_SCIOP_Endpoint : public TAO_Endpoint
{
public:

  // @todo Lose these friends!
  friend class TAO_SCIOP_Profile;
  friend class TAO_SSLIOP_Profile;

  // = Initialization and termination methods.

  /// Default constructor.
  TAO_SCIOP_Endpoint (void);

  /// Constructor.  This is the most efficient constructor since it
  /// does not require any address resolution processing.
  TAO_SCIOP_Endpoint (const char *host,
                     CORBA::UShort port,
                     const ACE_INET_Addr &addr);

  /// Constructor.
  TAO_SCIOP_Endpoint (const ACE_INET_Addr &addr,
                     int use_dotted_decimal_addresses);

  /// Constructor.  This constructor is used when decoding endpoints.
  TAO_SCIOP_Endpoint (const char *host,
                     CORBA::UShort port,
                     CORBA::Short priority);

  /// Destructor.
  ~TAO_SCIOP_Endpoint (void);

  // = Implementation of abstract TAO_Endpoint methods.  See
  // Endpoint.h for their documentation.

  virtual TAO_Endpoint *next (void);
  virtual int addr_to_string (char *buffer, size_t length);
  virtual void reset_hint (void);

  /// Makes a copy of <this>
  virtual TAO_Endpoint *duplicate (void);

  /// Return true if this endpoint is equivalent to <other_endpoint>.  Two
  /// endpoints are equivalent iff their port and host are the same.
  virtual CORBA::Boolean is_equivalent (const TAO_Endpoint *other_endpoint);

  /// Return a hash value for this object.
  virtual CORBA::ULong hash (void);

  // = SCIOP_Endpoint-specific methods.

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

  /// SCIOP Endpoints can be stringed into a list.  Return the next
  /// endpoint in the list, if any.
  TAO_SCIOP_Endpoint *next_;

};


#if defined (__ACE_INLINE__)
# include "SCIOP_Endpoint.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_SCIOP == 1 */

#include "ace/post.h"
#endif  /* TAO_SCIOP_PROFILE_H */
