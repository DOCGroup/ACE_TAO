// This may look like C, but it's really -*- C++ -*-


//=============================================================================
/**
 *  @file     UIPMC_Endpoint.h
 *
 *  $Id$
 *
 *   UIPMC implementation of PP Framework Endpoint interface.
 *
 *
 *  @author  Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_UIPMC_ENDPOINT_H
#define TAO_UIPMC_ENDPOINT_H
#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Endpoint.h"
#include "ace/INET_Addr.h"

#include "portablegroup_export.h"

/**
 * @class TAO_UIPMC_Endpoint
 *
 * @brief TAO_UIPMC_Endpoint
 *
 * UIPMC-specific implementation of PP Framework Endpoint interface.
 */
class TAO_PortableGroup_Export TAO_UIPMC_Endpoint : public TAO_Endpoint
{
public:

  friend class TAO_UIPMC_Profile;

  // = Initialization and termination methods.

  /// Default constructor.
  TAO_UIPMC_Endpoint (void);

  /// Constructor.
  TAO_UIPMC_Endpoint (const ACE_INET_Addr &addr);

  /// Constructor.
  TAO_UIPMC_Endpoint (const CORBA::Octet class_d_address[4],
                      CORBA::UShort port);

  /// Destructor.
  ~TAO_UIPMC_Endpoint (void);

  // = Implementation of abstract TAO_Endpoint methods.  See
  // Endpoint.h for their documentation.

  virtual TAO_Endpoint *next (void);
  virtual int addr_to_string (char *buffer, size_t length);
  virtual void reset_hint (void);

  /// Makes a copy of <this>
  virtual TAO_Endpoint *duplicate (void) const;

  /// Return true if this endpoint is equivalent to <other_endpoint>.  Two
  /// endpoints are equivalent iff their port and host are the same.
  CORBA::Boolean is_equivalent (const TAO_Endpoint *other_endpoint);

  /// Return a hash value for this object.
  CORBA::ULong hash (void);

  // Allocates memory and returns a copy of <this>

  // = UIPMC_Endpoint-specific methods.

  /// Return a reference to the <object_addr>.
  const ACE_INET_Addr &object_addr (void) const;

  /// Set the IP multicast address and port.
  void object_addr (const ACE_INET_Addr &addr);

  /// Return the port number.
  CORBA::UShort port (void) const;

  /// Set the port number.
  CORBA::UShort port (CORBA::UShort p);

  //TAO_UIPMC_Connection_Handler *&hint (void);
  // Access to our <hint_>.


private:

  /// Helper methods for getting and setting the IP address.
  ACE_UINT32 uint_ip_addr (void) const;
  void uint_ip_addr (ACE_UINT32 ip_addr);

  /// Helper method to update the cached object address.
  void update_object_addr (void);

  /// IP Multicast address.
  CORBA::Octet class_d_address_[4];

  /// UDP port number.
  CORBA::UShort port_;

  /// Cached instance of <ACE_INET_Addr> for use in making
  /// invocations, etc.
  ACE_INET_Addr object_addr_;

  /// UIPMC Endpoints can be stringed into a list.  Return the next
  /// endpoint in the list, if any.
  TAO_UIPMC_Endpoint *next_;
};

#if defined (__ACE_INLINE__)
# include "tao/UIPMC_Endpoint.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_UIPMC_PROFILE_H */
