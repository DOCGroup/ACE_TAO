// This may look like C, but it's really -*- C++ -*-
// $Id$


// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     IIOP_Endpoint.h
//
// = AUTHOR
//     Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_IIOP_ENDPOINT_H
#define TAO_IIOP_ENDPOINT_H
#include "ace/pre.h"

#include "tao/Endpoint.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"

#include "ace/INET_Addr.h"


class TAO_IIOP_Client_Connection_Handler;

class TAO_Export TAO_IIOP_Endpoint : public TAO_Endpoint
{
  // = TITLE
  //   This class defines the protocol specific attributes required
  //   for locating ORBs over a TCP/IP network.
  //
  // = DESCRIPTION
  //   This class defines the IIOP profile as specified in the CORBA
  //   specification.
public:

  friend class TAO_IIOP_Profile;

  TAO_IIOP_Endpoint (const ACE_INET_Addr &addr,
                     int use_dotted_decimal_addresses);
  // Profile constructor

  TAO_IIOP_Endpoint (const char *host,
                     CORBA::UShort port,
                     const ACE_INET_Addr &addr);
  // Endpoint constructor, this is the most efficient since it
  // doesn't require any address resolution processing.

  TAO_IIOP_Endpoint (const char *host,
                     CORBA::UShort port,
                     CORBA::Short priority);
  // Endpoint constructor - used when decoding endpoints from the wire.

  TAO_IIOP_Endpoint (void);
  // Endpoint constructor, default.

  ~TAO_IIOP_Endpoint (void);
  // Destructor is to be called only through <_decr_refcnt>.

  CORBA::Boolean is_equivalent (const TAO_IIOP_Endpoint *other_endpoint);
  // Return true if this profile is equivalent to other_profile.  Two
  // profiles are equivalent iff their key, port, host, object_key and
  // version are the same.

  CORBA::ULong hash (void);
  // Return a hash value for this object.

  // = Abstract Endpoint interface methods.

  TAO_Endpoint *next (void);
  // Return the next endpoint in the list.

  virtual int addr_to_string (char *buffer, size_t length);
  // Return a string representation for the address.

  virtual void reset_hint (void);
  //  Reset the hint's value.

  // = IIOP Endpoint methods.

  const ACE_INET_Addr &object_addr (void) const;
  //  return a reference to the object_addr.

  const char *host (void) const;
  // Return a pointer to the host string.  This object maintains
  // ownership of this string.

  const char *host (const char *h);
  // Copy the string h into <host_> and return the resulting pointer.
  // This object maintains ownership of this string.

  CORBA::UShort port (void) const;
  // Return the port number.

  CORBA::UShort port (CORBA::UShort p);
  // Set the port number.

  TAO_IIOP_Client_Connection_Handler *&hint (void);
  //  This is a hint for which connection handler to use.

private:
  int set (const ACE_INET_Addr &addr,
           int use_dotted_decimal_addresses);
  // helper method to set the INET_Addr.

  CORBA::String_var host_;
  // String representing the hosts name.

  CORBA::UShort port_;
  // TCP port number

  ACE_INET_Addr object_addr_;
  // Cached instance of <ACE_INET_Addr> for use in making
  // invocations, etc.

  TAO_IIOP_Client_Connection_Handler *hint_;
  // Pointer to a connection handler which we successfully used
  // already.

  TAO_IIOP_Endpoint *next_;
  // Next endpoint in the list.
};

#if defined (__ACE_INLINE__)
# include "tao/IIOP_Endpoint.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_IIOP_PROFILE_H */
