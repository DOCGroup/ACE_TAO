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
                      const SSLIOP::SSL *ssl_component);
  // Profile constructor, same as above except the object_key has
  // already been marshaled.

  TAO_SSLIOP_Profile (const char *host,
                      CORBA::UShort port,
                      const TAO_ObjectKey &object_key,
                      const ACE_INET_Addr &addr,
                      const TAO_GIOP_Version &version,
                      TAO_ORB_Core *orb_core,
                      const SSLIOP::SSL *ssl_component);
  //  Profile constructor, this is the most efficient since it
  // doesn't require any address resolution processing.

  TAO_SSLIOP_Profile (const char *string,
                      TAO_ORB_Core *orb_core,
                      const SSLIOP::SSL *ssl_component,
                      CORBA::Environment &ACE_TRY_ENV);
  // Create object using a string ior.

  TAO_SSLIOP_Profile (TAO_ORB_Core *orb_core);
  // Profile constructor, default.

  ~TAO_SSLIOP_Profile (void);
  // Destructor is to be called only through <_decr_refcnt>.

  // = Please see Profile.h for the documentation of these methods.
  virtual int decode (TAO_InputCDR& cdr);
  virtual int encode_endpoints (void);
  // Encodes this profile's endpoints into a tagged component.
  // This is done only if RTCORBA is enabled, since currently this is
  // the only case when we have more than one endpoint per profile.
  //
  // SSL endpoints are transmitted using TAO-proprietory tagged component.
  // Component tag is TAO_TAG_SSL_ENDPOINTS and component data is an
  // encapsulation of a sequence of structs, each representing a
  // single endpoint.  Data format is specified in ssl_endpoins.pidl.

  virtual CORBA::Boolean is_equivalent (const TAO_Profile *other_profile);

  virtual TAO_Endpoint *endpoint (void);
  // Return pointer to the head of this profile's endpoints list,
  // i.e., <ssl_endpoint_> accessor.

  void add_endpoint (TAO_SSLIOP_Endpoint *endp);
  // Add <endp> to this profile's list of endpoints (it is inserted
  // next to the head of the list).  This profiles takes ownership of
  // <endp>.  If <endp>'s <iiop_endpoint_> member is not 0, it is
  // added to our parent's class endpoint list.

private:

  int decode_endpoints (void);
  // Helper for <decode>.  Decodes endpoints from a tagged component.
  // Decode only if RTCORBA is enabled.  Return 0 on success and -1 on
  // failure.

  TAO_SSLIOP_Endpoint ssl_endpoint_;
  // Head of this profile's list of endpoints.  This endpoint is not
  // dynamically allocated because a profile always contains at least
  // one endpoint.
  //
  // Currently, a profile contains more than one endpoint, i.e., list
  // contains more than just the head, only when RTCORBA is enabled.
  // However, in the near future, this will be used in nonRT mode as
  // well, e.g., to support TAG_ALTERNATE_IIOP_ADDRESS feature.
  //
  // Since SSLIOP profile is an extension of IIOP profile, its
  // addressing info is contained in two places:  IIOP parent class
  // contains all iiop addressing while this class contains
  // ssl-specific addressing additions to iiop.  This means that
  // there are two lists of endpoints: one maintained in the parent
  // class and one maintained here.  Each ssl endpoint maintains a
  // pointer to its counterpart in the parent class endpoint list.
  //
  // For transmission of iiop addressing information, see
  // TAO_IIOP_Profile.  Addressing info of the default ssl endpoint,
  // i.e., head of the list, is transmitted using standard
  // SSLIOP::TAG_SSL_SEC_TRANS tagged component.  See
  // <encode_endpoints> method documentation above for how the rest of
  // the ssl endpoint list is transmitted.
};

#if defined (__ACE_INLINE__)
# include "SSLIOP_Profile.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"

#endif  /* TAO_SSLIOP_PROFILE_H */
