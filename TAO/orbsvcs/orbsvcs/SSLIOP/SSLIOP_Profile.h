// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     SSLIOP_Profile.h
//
// = DESCRIPTION
//     IIOP profile specific processing
//
// = AUTHOR
//     Fred Kuhns <fredk@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_SSLIOP_PROFILE_H
#define TAO_SSLIOP_PROFILE_H

#include "tao/IIOP_Profile.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SSLIOPC.h"

class TAO_SSLIOP_Client_Connection_Handler;

// TAO SSLIOP_Profile concrete Profile definitions
class TAO_Export TAO_SSLIOP_Profile : public TAO_IIOP_Profile
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

  CORBA::UShort ssl_port (void) const;
  // The port used for SSL communication.

  TAO_SSLIOP_Client_Connection_Handler *&ssl_hint (void);
  //  This is a hint for which connection handler to use.

  // = Please see Profile.h for the documentation of these methods.
  virtual int decode (TAO_InputCDR& cdr);
  virtual CORBA::Boolean is_equivalent (const TAO_Profile *other_profile);
  virtual void reset_hint (void);

private:
  TAO_SSLIOP_Profile (const TAO_SSLIOP_Profile &pfile);
  TAO_SSLIOP_Profile &operator= (const TAO_SSLIOP_Profile &src);
  // private & undefined

private:
  SSLIOP::SSL ssl_component_;
  // Cache the SSL tagged component in a decoded format. Notice that
  // we do not need to marshal this object!

  TAO_SSLIOP_Client_Connection_Handler *ssl_hint_;
  // Pointer to a connection handler which we successfully used
  // already.
};

#if defined (__ACE_INLINE__)
# include "SSLIOP_Profile.i"
#endif /* __ACE_INLINE__ */

#endif  /* TAO_SSLIOP_PROFILE_H */
