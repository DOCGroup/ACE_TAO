// -*- C++ -*-

/**
 *  @file   ESF_Peer_Workers.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  http://doc.ece.uci.edu/~coryan/ESF/index.html
 */

#ifndef TAO_ESF_PEER_WORKERS_H
#define TAO_ESF_PEER_WORKERS_H

#include "orbsvcs/ESF/ESF_Worker.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_ESF_Peer_Connected
 *
 * Helper class.
 * Used to iterate over a Proxy_Collection and invoke:
 * PROXY->connected (peer);
 * peer->connected (proxy);
 */
template<class PROXY, class PEER>
class TAO_ESF_Peer_Connected : public TAO_ESF_Worker<PROXY>
{
public:
  TAO_ESF_Peer_Connected (PEER *peer);

  void work (PROXY *proxy);

private:
  PEER* peer_;
};

// ****************************************************************

/**
 * @class TAO_ESF_Peer_Reconnected
 *
 * Helper class.
 * Used to iterate over a Proxy_Collection and invoke:
 * PROXY->reconnected (peer);
 * peer->reconnected (proxy);
 */
template<class PROXY, class PEER>
class TAO_ESF_Peer_Reconnected : public TAO_ESF_Worker<PROXY>
{
public:
  TAO_ESF_Peer_Reconnected (PEER *peer);

  void work (PROXY *proxy);

private:
  PEER* peer_;
};

// ****************************************************************

/**
 * @class TAO_ESF_Peer_Disconnected
 *
 * Helper class.
 * Used to iterate over a Proxy_Collection and invoke:
 * PROXY->disconnected (peer);
 * peer->disconnected (proxy);
 */
template<class PROXY, class PEER>
class TAO_ESF_Peer_Disconnected : public TAO_ESF_Worker<PROXY>
{
public:
  TAO_ESF_Peer_Disconnected (PEER *peer);

  void work (PROXY *proxy);

private:
  PEER* peer_;
};

// ****************************************************************

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Peer_Workers.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "orbsvcs/ESF/ESF_Peer_Workers.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Peer_Workers.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_PEER_WORKERS_H */
