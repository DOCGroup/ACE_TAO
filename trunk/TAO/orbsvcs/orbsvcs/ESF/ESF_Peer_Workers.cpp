// $Id$

#ifndef TAO_ESF_PEER_WORKERS_CPP
#define TAO_ESF_PEER_WORKERS_CPP

#include "orbsvcs/ESF/ESF_Peer_Workers.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Peer_Workers.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class P, class R> void
TAO_ESF_Peer_Connected<P,R>::work (P *proxy
                                   ACE_ENV_ARG_DECL)
{
  proxy->connected (this->peer_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK; // Shouldn't happen, just following the discipline

  this->peer_->connected (proxy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK; // Shouldn't happen, just following the discipline
}

// ****************************************************************

template<class P, class R> void
TAO_ESF_Peer_Reconnected<P,R>::work (P *proxy
                                     ACE_ENV_ARG_DECL)
{
  proxy->reconnected (this->peer_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK; // Shouldn't happen, just following the discipline

  this->peer_->reconnected (proxy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK; // Shouldn't happen, just following the discipline
}

// ****************************************************************

template<class P, class R> void
TAO_ESF_Peer_Disconnected<P,R>::work (P *proxy
                                      ACE_ENV_ARG_DECL)
{
  proxy->disconnected (this->peer_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK; // Shouldn't happen, just following the discipline

  this->peer_->disconnected (proxy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK; // Shouldn't happen, just following the discipline
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_ESF_PEER_WORKERS_CPP */
