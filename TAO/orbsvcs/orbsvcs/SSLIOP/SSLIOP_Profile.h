// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO_SSLIOP
//
// = FILENAME
//     SSLIOP_Profile.h
//
// = DESCRIPTION
//     SSLIOP profile specific processing
//
// = AUTHOR
//     Carlos O'Ryan <coryan@ece.uci.edu>
//     Ossama Othman <ossama@ece.uci.edu>
//
// ============================================================================

#ifndef TAO_SSLIOP_PROFILE_H
#define TAO_SSLIOP_PROFILE_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if defined (ACE_HAS_SSL) && ACE_HAS_SSL == 1

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SSLIOP_Endpoint.h"
#include "tao/IIOP_Profile.h"

class TAO_SSLIOP_Client_Connection_Handler;

// TAO SSLIOP_Profile concrete Profile definitions
class TAO_SSLIOP_Export TAO_SSLIOP_Profile : public TAO_IIOP_Profile
{
  // = TITLE
  //   This class defines the protocol specific attributes required
  //   for locating ORBs over a TCP/IP network, using either IIOP or
  //   IIOP/SSL for communication.
  //
  // = DESCRIPTION
  //   This class extends TAO_IIOP_Profile to support secure
  //   communication using SSL.
  //
public:
  TAO_SSLIOP_Profile (const ACE_INET_Addr &addr,
                      const TAO_ObjectKey &object_key,
                      const TAO_GIOP_Version &version,
                      TAO_ORB_Core *orb_core,
                      u_short ssl_port = 0);
  // Profile constructor, same as above except the object_key has
  // already been marshaled.

  TAO_SSLIOP_Profile (const char *host,
                      CORBA::UShort port,
                      const TAO_ObjectKey &object_key,
                      const ACE_INET_Addr &addr,
                      const TAO_GIOP_Version &version,
                      TAO_ORB_Core *orb_core,
                      u_short ssl_port = 0);
  //  Profile constructor, this is the most efficient since it
  // doesn't require any address resolution processing.

  TAO_SSLIOP_Profile (const char *string,
                      TAO_ORB_Core *orb_core,
                      u_short ssl_port,
                      CORBA::Environment &ACE_TRY_ENV =
                          TAO_default_environment ());
  // Create object using a string ior.

  TAO_SSLIOP_Profile (TAO_ORB_Core *orb_core);
  // Profile constructor, default.

  ~TAO_SSLIOP_Profile (void);
  // Destructor is to be called only through <_decr_refcnt>.

  virtual TAO_Endpoint *endpoint (void);
  // Head of the list of endpoints for this profile.

  void add_endpoint (TAO_SSLIOP_Endpoint *endp);
  //

  // = Please see Profile.h for the documentation of these methods.
  virtual int decode (TAO_InputCDR& cdr);
  virtual int encode (TAO_OutputCDR &stream) const;
  // Encode this profile in a stream, i.e. marshal it.
  virtual CORBA::Boolean is_equivalent (const TAO_Profile *other_profile);

private:

  int encode_endpoints (void);
  // Encodes endpoints from this profile into a tagged component.

  int decode_endpoints (void);
  // Decodes endpoints of this profile from a tagged component.


  TAO_SSLIOP_Endpoint ssl_endpoint_;
  //
};

#if defined (__ACE_INLINE__)
# include "SSLIOP_Profile.i"
#endif /* __ACE_INLINE__ */

#endif  /* ACE_HAS_SSL */

#include "ace/post.h"

#endif  /* TAO_SSLIOP_PROFILE_H */
