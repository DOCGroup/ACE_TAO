/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Event Service Framework
//
// = FILENAME
//   ESF_Peer_Admin
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   http://www.cs.wustl.edu/~coryan/EC/index.html
//
// ============================================================================

#ifndef TAO_ESF_PEER_ADMIN_H
#define TAO_ESF_PEER_ADMIN_H

#include "ESF_Proxy_Admin.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template<class EVENT_CHANNEL, class PROXY, class PEER>
class TAO_ESF_Peer_Admin : public TAO_ESF_Proxy_Admin<EVENT_CHANNEL,PROXY>
{
  // = TITLE
  //   ESF_Peer_Admin
  //
  // = DESCRIPTION
  //   Some Event Services that perform filtering have to propagate
  //   the consumer connect/disconnect activities to the suppliers,
  //   and vice-versa.
  //   In that scenario the ESF_Proxy_Admin<> interface is augmented
  //   with connected()/reconnected()/disconnected() operations for
  //   the proxy peers (i.e. the ProxySuppliers for the ProxyConsumers
  //   and vice-versa).
  //
  // = REQUIREMENTS
  //   In addition to the requirements imposed by ESF_Proxy_Admin<>
  //   the PROXY interface must implement:
  //
  //   void connected (PEER *peer, CORBA::Environment&) throw ();
  //   void reconnected (PEER *peer, CORBA::Environment&) throw ();
  //   void disconnected (PEER *peer, CORBA::Environment&) throw ();
  //
  //   Similarly, the PEER interface must implement:
  //
  //   void connected (PROXY *proxy, CORBA::Environment&) throw ();
  //   void reconnected (PROXY *proxy, CORBA::Environment&) throw ();
  //   void disconnected (PROXY *proxy, CORBA::Environment&) throw ();
  //
public:
  TAO_ESF_Peer_Admin (EVENT_CHANNEL *ec);
  // Constructor

  virtual ~TAO_ESF_Peer_Admin (void);
  // destructor

  virtual void peer_connected (PEER *peer,
                               CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC (());
  // A <peer> has connected, this is invoked when the peer's client
  // has invoked the connect_xxx_yyy() method.
  // The default implementation is a no-op.

  virtual void peer_reconnected (PEER *peer,
                                 CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC (());
  // A <peer> has reconnected, i.e. its client has invoked the
  // connect_xxx_yyy() method, but the peer was connected already.
  // The default implementation delegates on the collection
  // <reconnected> method

  virtual void peer_disconnected (PEER *peer,
                                  CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC (());
  // A <peer> has been disconnected. The default implementation
  // removes the object from the collection and deactivates the
  // proxy.
};

// ****************************************************************

#if defined (__ACE_INLINE__)
#include "ESF_Peer_Admin.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ESF_Peer_Admin.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Peer_Admin.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_PEER_ADMIN_H */
