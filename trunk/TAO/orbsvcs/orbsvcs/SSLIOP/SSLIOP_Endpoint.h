// This may look like C, but it's really -*- C++ -*-
// $Id$


// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     SSLIOP_Endpoint.h
//
// = AUTHOR
//     Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_SSLIOP_ENDPOINT_H
#define TAO_SSLIOP_ENDPOINT_H

#include "ace/pre.h"

#include "tao/Endpoint.h"

#if defined (ACE_HAS_SSL) && ACE_HAS_SSL == 1

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"
#include "SSLIOP_Export.h"
#include "orbsvcs/SSLIOPC.h"

// Tag for storing multiple endpoints within a single profile.
#define TAO_TAG_SSL_ENDPOINTS 0x54414f01U

class TAO_SSLIOP_Client_Connection_Handler;
class TAO_IIOP_Endpoint;

class TAO_SSLIOP_Export TAO_SSLIOP_Endpoint : public TAO_Endpoint
{
  // = TITLE
  //   This class defines the protocol specific attributes required
  //   for locating ORBs over a TCP/IP network.
  //
  // = DESCRIPTION
  //   This class defines the IIOP profile as specified in the CORBA
  //   specification.
public:

  friend class TAO_SSLIOP_Profile;

  TAO_SSLIOP_Endpoint (u_short ssl_port,
                       TAO_IIOP_Endpoint *iiop_endp);
  // Endpoint constructor, default.

  ~TAO_SSLIOP_Endpoint (void);
  // Destructor is to be called only through <_decr_refcnt>.

  // = Abstract Endpoint interface methods.

  TAO_Endpoint *next (void);
  // Return the next endpoint in the list.

  virtual int addr_to_string (char *buffer, size_t length);
  // Return a string representation for the address.

  virtual void reset_hint (void);
  //  Reset the hint's value.

  // = SSLIOP Endpoint methods.

  TAO_IIOP_Endpoint *iiop_endpoint (void) const;
  // Accessor to our IIOP counterpart.

  CORBA::Boolean is_equivalent (const TAO_SSLIOP_Endpoint *other_endpoint);
  // Return true if this profile is equivalent to other_profile.  Two
  // profiles are equivalent iff their key, port, host, object_key and
  // version are the same.

  CORBA::UShort ssl_port (void) const;
  // The port used for SSL communication.

  TAO_SSLIOP_Client_Connection_Handler *&ssl_hint (void);
  //  This is a hint for which connection handler to use.

private:

  SSLIOP::SSL ssl_component_;
  // Cache the SSL tagged component in a decoded format. Notice that
  // we do not need to marshal this object!

  TAO_SSLIOP_Client_Connection_Handler *ssl_hint_;
  // Pointer to a connection handler which we successfully used
  // already.

  TAO_SSLIOP_Endpoint *next_;
  // Next endpoint in the list.

  TAO_IIOP_Endpoint *iiop_endpoint_;
  //
};

#if defined (__ACE_INLINE__)
#include "SSLIOP_Endpoint.i"
#endif /* __ACE_INLINE__ */

#endif  /* ACE_HAS_SSL */

#include "ace/post.h"
#endif  /* TAO_SSLIOP_ENDPOINT_H */
