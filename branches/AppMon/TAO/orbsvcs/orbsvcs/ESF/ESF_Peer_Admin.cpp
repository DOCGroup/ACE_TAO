// $Id$

#ifndef TAO_ESF_PEER_ADMIN_CPP
#define TAO_ESF_PEER_ADMIN_CPP

#include "orbsvcs/ESF/ESF_Peer_Workers.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class EVENT_CHANNEL, class PROXY, class INTERFACE, class PEER>
TAO_ESF_Peer_Admin<EVENT_CHANNEL,PROXY,INTERFACE,PEER>::
    TAO_ESF_Peer_Admin (EVENT_CHANNEL *ec)
  :  TAO_ESF_Proxy_Admin<EVENT_CHANNEL,PROXY,INTERFACE> (ec)
{
}

template<class EVENT_CHANNEL, class PROXY, class INTERFACE, class PEER>
TAO_ESF_Peer_Admin<EVENT_CHANNEL,PROXY,INTERFACE,PEER>::
    ~TAO_ESF_Peer_Admin (void)
{
}

template<class EVENT_CHANNEL, class PROXY, class INTERFACE, class PEER> void
TAO_ESF_Peer_Admin<EVENT_CHANNEL,PROXY,INTERFACE,PEER>::
    peer_connected (PEER *peer)
{
  TAO_ESF_Peer_Connected<PROXY,PEER> worker (peer);

  this->for_each (&worker);
}

template<class EVENT_CHANNEL, class PROXY, class INTERFACE, class PEER> void
TAO_ESF_Peer_Admin<EVENT_CHANNEL,PROXY,INTERFACE,PEER>::
    peer_reconnected (PEER *peer)
{
  TAO_ESF_Peer_Reconnected<PROXY,PEER> worker (peer);

  this->for_each (&worker);
}

template<class EVENT_CHANNEL, class PROXY, class INTERFACE, class PEER> void
TAO_ESF_Peer_Admin<EVENT_CHANNEL,PROXY,INTERFACE,PEER>::
    peer_disconnected (PEER *peer)
{
  TAO_ESF_Peer_Disconnected<PROXY,PEER> worker (peer);

  this->for_each (&worker);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_ESF_PEER_ADMIN_CPP */
