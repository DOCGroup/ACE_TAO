// This may look like C, but it's really -*- C++ -*-


//=============================================================================
/**
 *  @file     DIOP_Endpoint.h
 *
 *  $Id$
 *
 *   DIOP implementation of PP Framework Endpoint interface.
 *
 *  @author  Michael Kircher
 */
//=============================================================================


#ifndef TAO_DIOP_ENDPOINT_H
#define TAO_DIOP_ENDPOINT_H
#include /**/ "ace/pre.h"
#include "tao/Endpoint.h"


#if defined (TAO_HAS_DIOP) && (TAO_HAS_DIOP != 0)



#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "tao/CORBA_String.h"
#include "ace/INET_Addr.h"
#include "strategies_export.h"


class TAO_DIOP_Connection_Handler;

/**
 * @class TAO_DIOP_Endpoint
 *
 * @brief TAO_DIOP_Endpoint
 *
 * DIOP-specific implementation of PP Framework Endpoint interface.
 */
class TAO_Strategies_Export TAO_DIOP_Endpoint : public TAO_Endpoint
{
public:

  friend class TAO_DIOP_Profile;
  friend class TAO_SSLIOP_Profile;

  // = Initialization and termination methods.

  /// Default constructor.
  TAO_DIOP_Endpoint (void);

  /// Constructor.  This is the most efficient constructor since it
  /// does not require any address resolution processing.
  TAO_DIOP_Endpoint (const char *host,
                     CORBA::UShort port,
                     const ACE_INET_Addr &addr,
                     CORBA::Short priority = TAO_INVALID_PRIORITY);

  /// Constructor.
  TAO_DIOP_Endpoint (const ACE_INET_Addr &addr,
                     int use_dotted_decimal_addresses);

  /// Constructor.  This constructor is used when decoding endpoints.
  TAO_DIOP_Endpoint (const char *host,
                     CORBA::UShort port,
                     CORBA::Short priority);

  /// Destructor.
  ~TAO_DIOP_Endpoint (void);

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

  // Allocates memory and returns a copy of <this>

  // = DIOP_Endpoint-specific methods.

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

  //TAO_DIOP_Connection_Handler *&hint (void);
  // Access to our <hint_>.

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
  ACE_INET_Addr object_addr_;

  /// Flag to indicate if the address has been resolved and set.
  int object_addr_set_;

  /// DIOP Endpoints can be stringed into a list.  Return the next
  /// endpoint in the list, if any.
  TAO_DIOP_Endpoint *next_;
};

#if defined (__ACE_INLINE__)
# include "DIOP_Endpoint.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_DIOP && TAO_HAS_DIOP != 0 */

#include /**/ "ace/post.h"
#endif  /* TAO_DIOP_PROFILE_H */
