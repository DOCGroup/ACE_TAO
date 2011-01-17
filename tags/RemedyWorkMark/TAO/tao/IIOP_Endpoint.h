// -*- C++ -*-

//=============================================================================
/**
 *  @file     IIOP_Endpoint.h
 *
 *  $Id$
 *
 *  IIOP implementation of PP Framework Endpoint interface.
 *
 *  @author  Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_IIOP_ENDPOINT_H
#define TAO_IIOP_ENDPOINT_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)

#include "tao/CORBA_String.h"
#include "tao/IIOP_EndpointsC.h"
#include "tao/Endpoint.h"

#include "ace/INET_Addr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_IIOP_Connection_Handler;
class TAO_IIOP_Profile;

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

  //@@ TAO_ENDPOINT_SPL_PUBLIC_METHODS_COPY_HOOK_START

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


  //@@ TAO_ENDPOINT_SPL_PUBLIC_METHODS_COPY_HOOK_END

  /// Destructor.
  ~TAO_IIOP_Endpoint (void);


  // = Implementation of abstract TAO_Endpoint methods.  See
  // Endpoint.h for their documentation.

  virtual TAO_Endpoint *next (void);

  /**
   * Return the next endpoint in the list, but use protocol-specific
   * filtering to constrain the value. The orb core is needed to supply
   * any sort of filter arguments, and the root endpoint is needed in case
   * the algorithm needs to rewind. If the supplied root is 0, then this
   * is assumed to be the candidate next endpoint.
   *
   * To use this, the caller starts off the change with root == 0. This
   * is a bit of a violation in logic, a more correct implementation would
   * accept this == 0 and a non-null root.
   * To do iteration using next_filtered, do:
   *   for (TAO_Endpoint *ep = root_endpoint->next_filtered (orb_core, 0);
   *        ep != 0;
   *        ep = ep->next_filtered(orb_core, root_endpoint)) { }
   */
  virtual TAO_Endpoint *next_filtered (TAO_ORB_Core *, TAO_Endpoint *root);

  virtual int addr_to_string (char *buffer, size_t length);

  /// Makes a copy of @c this
  virtual TAO_Endpoint *duplicate (void);

  /// Return true if this endpoint is equivalent to @a other_endpoint.  Two
  /// endpoints are equivalent if their port and host are the same.
  virtual CORBA::Boolean is_equivalent (const TAO_Endpoint *other_endpoint);

  /// Return a hash value for this object.
  virtual CORBA::ULong hash (void);

  // = IIOP_Endpoint-specific methods.

  /*
   * Hook to copy only the non virtual concrete methods implemented
   * in this class to the derived class in the specialization.
   */
  //@@ TAO_ENDPOINT_SPL_PUBLIC_METHODS_COPY_HOOK_START

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

#if defined (ACE_HAS_IPV6)
  /// Does the host string represent an IPv6 decimal address.
  bool is_ipv6_decimal (void) const;
#endif /* ACE_HAS_IPV6 */

  //@@ TAO_ENDPOINT_SPL_PUBLIC_METHODS_COPY_HOOK_END


  /// Need to have an assignment operator since the IIOP_Profile class may
  /// have to reorder its list of endpoints based on filtering by the EndpointPolicy.
  TAO_IIOP_Endpoint & operator= (const TAO_IIOP_Endpoint& other);

private:
  TAO_IIOP_Endpoint *next_filtered_i (TAO_IIOP_Endpoint *root,
                                      bool ipv6_only,
                                      bool prefer_ipv6,
                                      bool want_ipv6);


  //@@ TAO_ENDPOINT_SPL_PRIVATE_DATA_COPY_HOOK_START

  /// Helper method for setting INET_Addr.
  int set (const ACE_INET_Addr &addr,
           int use_dotted_decimal_addresses);

  /// Helper method for object_addr () call.
  void object_addr_i (void) const;

  /// Generate preferred interfaces from the options passed in by the
  /// user.
  CORBA::ULong preferred_interfaces (const char *csvPreferred,
                                     bool enforce,
                                     TAO_IIOP_Profile &profile);

  /// Chain a new duplicate of ourself with the specified
  /// local preferred interface.
  TAO_IIOP_Endpoint *add_local_endpoint (TAO_IIOP_Endpoint *ep,
                                         const char *local,
                                         TAO_IIOP_Profile &profile);

  /// Canonical copy constructor
  /**
   * In private section to prevent clients from invoking this
   * accidentally. Clients should only use duplicate () to make a deep
   * copy
   */
  TAO_IIOP_Endpoint (const TAO_IIOP_Endpoint &);

private:

  /// String representing the host name.
  CORBA::String_var host_;

  /// TCP port number.
  CORBA::UShort port_;

#if defined (ACE_HAS_IPV6)
  /// Does the host string represent an IPv6 decimal address.
  bool is_ipv6_decimal_;
#endif /* ACE_HAS_IPV6 */

  /// Is this endpoint created encodable as part of the IOR?
  bool is_encodable_;

  /// Flag to indicate if the address has been resolved and set.
  mutable bool object_addr_set_;

  /// Cached instance of ACE_INET_Addr for use in making
  /// invocations, etc.
  mutable ACE_INET_Addr object_addr_;

  /// Preferred path for this endpoint.
  TAO::IIOP_Endpoint_Info preferred_path_;

  /// IIOP Endpoints can be stringed into a list.  Return the next
  /// endpoint in the list, if any.
  TAO_IIOP_Endpoint *next_;

  //@@ TAO_ENDPOINT_SPL_PRIVATE_DATA_COPY_HOOK_END
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/IIOP_Endpoint.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */

#include /**/ "ace/post.h"
#endif  /* TAO_IIOP_PROFILE_H */
