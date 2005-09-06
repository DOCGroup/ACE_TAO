// -*- C++ -*-

//=============================================================================
/**
 *  @file   Endpoint.h
 *
 *  $Id$
 *
 * Endpoint interface, part of TAO pluggable protocol framework.
 *
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_ENDPOINT_H
#define TAO_ENDPOINT_H

#include /**/ "ace/pre.h"
#include "ace/Thread_Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Export.h"
#include "tao/Basic_Types.h"
#include "tao/orbconf.h"

/*
 * Includes and forward decls for specializing TAO's
 * endpoint implementation.
 */
//@@ TAO_ENDPOINT_SPL_INCLUDE_FORWARD_DECL_ADD_HOOK
// Code woven by FOCUS:

#include "tao/IIOP_EndpointsC.h"
#include "ace/INET_Addr.h"

// END Code woven by FOCUS

/**
 * @class TAO_Endpoint
 *
 * @brief Defines the Endpoint interface in the Pluggable Protocol
 * framework.
 *
 * Lightweight encapsulation of addressing information for a
 * single acceptor endpoint.  In other words, Endpoint represents
 * a single point of contact for the server, and is the smallest
 * unit of addressing information necessary for a client to connect
 * to a server.
 * A Profile contains one or more Endpoints, i.e., knows of
 * one or more ways to contact server(s).
 */
class TAO_Export TAO_Endpoint
{
public:
  /// Constructor.
  TAO_Endpoint (CORBA::ULong tag,
                CORBA::Short priority = TAO_INVALID_PRIORITY);

  /// Destructor.
   ~TAO_Endpoint (void);

  /// IOP protocol tag accessor.
  CORBA::ULong tag (void) const;

  /// <priority_> attribute setter.
  void priority (CORBA::Short priority);

  /// <priority_> attribute getter.
  CORBA::Short priority (void) const;

  /**
   * @name TAO_Endpoint Template Methods
   *
   * Abstract methods to be implemented by concrete subclasses.
   */
  //@{
  /**
   * @return true if this endpoint is equivalent to @a other_endpoint.
   */
// Code woven by FOCUS:
   CORBA::Boolean is_equivalent (const TAO_Endpoint *other_endpoint) ;
// END Code woven by FOCUS

  /// Endpoints can be stringed in a list.
  /**
   * @return The next endpoint in the list, if any.
   */
// Code woven by FOCUS:
   TAO_Endpoint *next (void) ;
// END Code woven by FOCUS

  /// Return a string representation for the address.
  /**
   * The purpose of this method is to provide a general interface to
   * the underlying address object's @c addr_to_string method.  This
   * allows the protocol implementor to select the appropriate string
   * format.
   *
   * @return -1 if buffer is too small.
   */
// Code woven by FOCUS:
   int addr_to_string (char *buffer, size_t length) ;
// END Code woven by FOCUS

  /// This method returns a deep copy of the corresponding endpoints by
  /// allocating memory.
// Code woven by FOCUS:
   TAO_Endpoint *duplicate (void) ;
// END Code woven by FOCUS

  /// Return a hash value for this object.
// Code woven by FOCUS:
   CORBA::ULong hash (void) ;
// END Code woven by FOCUS

  /*
   * Hook to add public methods from derived class onto base
   * Endpoint class.
   */
  //@@ TAO_ENDPOINT_SPL_PUBLIC_METHODS_ADD_HOOK

  // @todo Lose these friends!
// Code woven by FOCUS:
  friend class TAO_Profile;
// END Code woven by FOCUS
  friend class TAO_SSLIOP_Profile;

  // = Initialization and termination methods.

  /// Default constructor.
// Code woven by FOCUS:
  TAO_Endpoint (void);
// END Code woven by FOCUS

  /// Constructor.  This is the most efficient constructor since it
  /// does not require any address resolution processing.
// Code woven by FOCUS:
  TAO_Endpoint (const char *host,
// END Code woven by FOCUS
                     CORBA::UShort port,
                     const ACE_INET_Addr &addr,
                     CORBA::Short priority = TAO_INVALID_PRIORITY);

  /// Constructor.
// Code woven by FOCUS:
  TAO_Endpoint (const ACE_INET_Addr &addr,
// END Code woven by FOCUS
                     int use_dotted_decimal_addresses);

  /// Constructor.  This constructor is used when decoding endpoints.
// Code woven by FOCUS:
  TAO_Endpoint (const char *host,
// END Code woven by FOCUS
                     CORBA::UShort port,
                     CORBA::Short priority);



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

  /// Do we have a preferred local network for the target?
  bool is_preferred_network (void) const;

  /// Return the preferred network if any.
  const char *preferred_network (void) const;

#if defined (ACE_HAS_IPV6)
  /// Does the host string represent an IPv6 decimal address.
  bool is_ipv6_decimal (void) const;
#endif /* ACE_HAS_IPV6 */


protected:

  /// Lock for the address lookup.
  /**
   * @todo This lock should be strategized so that we dont lock in
   *       single threaded configurations.  It is not possible to do
   *       this now as most of the information is available in the
   *       ORB_Core which is not available here.
   */
  mutable TAO_SYNCH_MUTEX addr_lookup_lock_;

  /// Cache the hash value
  CORBA::ULong hash_val_;

  /// IOP tag, identifying the protocol for which this endpoint
  /// contains addressing info.
  CORBA::ULong tag_;

  /**
   * CORBA priority of the acceptor this Endpoint is representing.
   * This is part of TAO 'prioritized endpoints' architecture, and is
   * currently used for RTCORBA only.
   */
  CORBA::Short priority_;

private:

  /// Endpoints should not be copied.

  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_Endpoint&))

  /*
   * Addition of private members from derived class.
   */
  //@@ TAO_ENDPOINT_SPL_PRIVATE_DATA_ADD_HOOK

  /// Helper method for setting INET_Addr.
  int set (const ACE_INET_Addr &addr,
           int use_dotted_decimal_addresses);

  /// Helper method for object_addr () call.
  void object_addr_i (void) const;

  /// Generate preferred interfaces from the options passed in by the
  /// user.
  CORBA::ULong preferred_interfaces (const char* csvPreferred, bool enforce);

  /// Chain a new duplicate of ourself with the specified
  /// local preferred interface.
// Code woven by FOCUS:
  TAO_Endpoint* add_local_endpoint(TAO_Endpoint* ep, const char* local);
// END Code woven by FOCUS

  /// Canonical copy constructor
  /**
   * In private section to prevent clients from invoking this
   * accidentally. Clients should only use duplicate () to make a depp
   * copy
   */
// Code woven by FOCUS:
  TAO_Endpoint (const TAO_Endpoint &);
// END Code woven by FOCUS

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
// Code woven by FOCUS:
  TAO_Endpoint *next_;
// END Code woven by FOCUS

};

//@@ TAO_ENDPOINT_SPL_EXTERN_ADD_HOOK

#if defined (__ACE_INLINE__)
# include "tao/Endpoint.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif  /* TAO_PROFILE_H */
