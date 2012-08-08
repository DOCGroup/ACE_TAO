// -*- C++ -*-

//===========================================================================
/**
 *  @file     SCIOP_Endpoint.h
 *
 *  SCIOP implementation of PP Framework Endpoint interface.
 *
 *  $Id$
 *
 *  @author  Jason Cohen, Lockheed Martin ATL  <jcohen@atl.lmco.com>
 *  @author  Keith O'Hara, Lockheed Martin ATL
 */
//==========================================================================


#ifndef TAO_SCIOP_ENDPOINT_H
#define TAO_SCIOP_ENDPOINT_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if TAO_HAS_SCIOP == 1
#include "tao/Endpoint.h"
#include "tao/Strategies/strategies_export.h"
#include "tao/CORBA_String.h"
#include "ace/INET_Addr.h"
#include "tao/IIOP_EndpointsC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_SCIOP_Connection_Handler;

/**
 * @class TAO_SCIOP_Endpoint
 *
 * @brief TAO_SCIOP_Endpoint
 *
 * SCIOP-specific implementation of PP Framework Endpoint interface.
 */
class TAO_Strategies_Export TAO_SCIOP_Endpoint : public TAO_Endpoint
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
                      const ACE_INET_Addr &addr,
                      CORBA::Short priority = TAO_INVALID_PRIORITY);

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
  virtual TAO_Endpoint *duplicate (void);

  /// Return true if this endpoint is equivalent to @a other_endpoint.  Two
  /// endpoints are equivalent if their port and host are the same.
  virtual CORBA::Boolean is_equivalent (const TAO_Endpoint *other_endpoint);

  /// Return a hash value for this object.
  virtual CORBA::ULong hash (void);

  // = SCIOP_Endpoint-specific methods.

  /// Return a reference to the <object_addr>.
  const ACE_INET_Addr &object_addr (void) const;

  /// Return a pointer to the host string.  This object maintains
  /// ownership of this string.
  const char *host (void) const;

  /// Copy the string @a h into <host_> and return the resulting pointer.
  /// This object maintains ownership of this string.
  const char *host (const char *h);

  /// Return the port number.
  CORBA::UShort port (void) const;

  /// Set the port number.
  CORBA::UShort port (CORBA::UShort p);

  /// Do we have a preferred local network for the target?
  bool is_preferred_network (void) const;

  /// Return the preferred network if any.
  const char *preferred_network (void) const;

private:

  /// Helper method for setting INET_Addr.
  int set (const ACE_INET_Addr &addr,
           int use_dotted_decimal_addresses);

  /// Helper method for object_addr ().
  void object_addr_i (void) const;

  /// Generate preferred interfaces from the options passed in by the
  /// user.
  CORBA::ULong preferred_interfaces (TAO_ORB_Core *oc);

  /// Canonical copy constructor
  /**
   * In private section to prevent clients from invoking this
   * accidentally. Clients should only use duplicate () to make a deep
   * copy
   */
  TAO_SCIOP_Endpoint (const TAO_SCIOP_Endpoint &);


  /// String representing the host name.
  CORBA::String_var host_;

  /// TCP port number.
  CORBA::UShort port_;

  /// Cached instance of <ACE_INET_Addr> for use in making
  /// invocations, etc.
  mutable ACE_INET_Addr object_addr_;

  /// Flag to indicate if the address has been resolved and set.
  mutable int object_addr_set_;

  /// Preferred path for this endpoint.
  TAO::IIOP_Endpoint_Info preferred_path_;

  /// Is this endpoint created encodable as part of the IOR?
  bool is_encodable_;

  /// SCIOP Endpoints can be strung into a list.  Return the next
  /// endpoint in the list, if any.
  TAO_SCIOP_Endpoint *next_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Strategies/SCIOP_Endpoint.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_SCIOP == 1 */

#include /**/ "ace/post.h"
#endif  /* TAO_SCIOP_PROFILE_H */
