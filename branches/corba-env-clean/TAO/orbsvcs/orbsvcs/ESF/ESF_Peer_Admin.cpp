// $Id$

#ifndef TAO_ESF_PEER_ADMIN_CPP
#define TAO_ESF_PEER_ADMIN_CPP



#if ! defined (__ACE_INLINE__)
#include "ESF_Peer_Admin.i"
#endif /* __ACE_INLINE__ */

#include "ESF_Peer_Workers.h"

ACE_RCSID(ESF, ESF_Peer_Admin, "$Id$")

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
    peer_connected (PEER *peer
                    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ESF_Peer_Connected<PROXY,PEER> worker (peer);

  this->for_each (&worker TAO_ENV_ARG_PARAMETER);
}

template<class EVENT_CHANNEL, class PROXY, class INTERFACE, class PEER> void
TAO_ESF_Peer_Admin<EVENT_CHANNEL,PROXY,INTERFACE,PEER>::
    peer_reconnected (PEER *peer
                      TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ESF_Peer_Reconnected<PROXY,PEER> worker (peer);

  this->for_each (&worker TAO_ENV_ARG_PARAMETER);
}

template<class EVENT_CHANNEL, class PROXY, class INTERFACE, class PEER> void
TAO_ESF_Peer_Admin<EVENT_CHANNEL,PROXY,INTERFACE,PEER>::
    peer_disconnected (PEER *peer
                       TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ESF_Peer_Disconnected<PROXY,PEER> worker (peer);

  this->for_each (&worker TAO_ENV_ARG_PARAMETER);
}

#endif /* TAO_ESF_PEER_ADMIN_CPP */
