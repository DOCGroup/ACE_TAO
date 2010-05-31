// $Id$

#ifndef TAO_ESF_PEER_WORKERS_CPP
#define TAO_ESF_PEER_WORKERS_CPP

#include "orbsvcs/ESF/ESF_Peer_Workers.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Peer_Workers.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class P, class R> void
TAO_ESF_Peer_Connected<P,R>::work (P *proxy)
{
  proxy->connected (this->peer_);
// Shouldn't happen, just following the discipline

  this->peer_->connected (proxy);
// Shouldn't happen, just following the discipline
}

// ****************************************************************

template<class P, class R> void
TAO_ESF_Peer_Reconnected<P,R>::work (P *proxy)
{
  proxy->reconnected (this->peer_);
// Shouldn't happen, just following the discipline

  this->peer_->reconnected (proxy);
// Shouldn't happen, just following the discipline
}

// ****************************************************************

template<class P, class R> void
TAO_ESF_Peer_Disconnected<P,R>::work (P *proxy)
{
  proxy->disconnected (this->peer_);
// Shouldn't happen, just following the discipline

  this->peer_->disconnected (proxy);
// Shouldn't happen, just following the discipline
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_ESF_PEER_WORKERS_CPP */
