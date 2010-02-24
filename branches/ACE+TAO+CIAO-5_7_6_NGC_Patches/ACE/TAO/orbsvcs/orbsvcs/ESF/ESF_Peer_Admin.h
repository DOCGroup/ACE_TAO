// -*- C++ -*-

/**
 *  @file   ESF_Peer_Admin.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_ESF_PEER_ADMIN_H
#define TAO_ESF_PEER_ADMIN_H

#include "orbsvcs/ESF/ESF_Proxy_Admin.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_ESF_Peer_Admin
 *
 * @brief Extend the Proxy_Admin interface to provide methods for
 *        filtering
 *
 * Some Event Services that perform filtering have to propagate
 * the consumer connect/disconnect activities to the suppliers,
 * and vice-versa.
 * In that scenario the ESF_Proxy_Admin<> interface is augmented
 * with connected()/reconnected()/disconnected() operations for
 * the proxy peers (i.e. the ProxySuppliers for the ProxyConsumers
 * and vice-versa).
 *
 * <H2>Requirements</H2>
 *
 * In addition to the requirements imposed by ESF_Proxy_Admin<>
 * the PROXY interface must implement:
 *
 * @verbatim
 * void connected (PEER *peer) throw ();
 * void reconnected (PEER *peer) throw ();
 * void disconnected (PEER *peer) throw ();
 * @endverbatim
 *
 * Similarly, the PEER interface must implement:
 *
 * @verbatim
 * void connected (PROXY *proxy) throw ();
 * void reconnected (PROXY *proxy) throw ();
 * void disconnected (PROXY *proxy) throw ();
 * @endverbatim
 */
template<class EVENT_CHANNEL, class PROXY, class INTERFACE, class PEER>
class TAO_ESF_Peer_Admin : public TAO_ESF_Proxy_Admin<EVENT_CHANNEL,PROXY,INTERFACE>
{
public:
  /// Constructor
  TAO_ESF_Peer_Admin (EVENT_CHANNEL *ec);

  /// destructor
  virtual ~TAO_ESF_Peer_Admin (void);

  /**
   * A <peer> has connected, this is invoked when the peer's client
   * has invoked the connect_xxx_yyy() method.
   * The default implementation is a no-op.
   */
  virtual void peer_connected (PEER *peer);

  /**
   * A <peer> has reconnected, i.e. its client has invoked the
   * connect_xxx_yyy() method, but the peer was connected already.
   * The default implementation delegates on the collection
   * <reconnected> method
   */
  virtual void peer_reconnected (PEER *peer);

  /**
   * A <peer> has been disconnected. The default implementation
   * removes the object from the collection and deactivates the
   * proxy.
   */
  virtual void peer_disconnected (PEER *peer);
};

// ****************************************************************

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/ESF/ESF_Peer_Admin.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Peer_Admin.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_PEER_ADMIN_H */
