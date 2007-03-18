// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class P, class R>
TAO_ESF_Peer_Connected<P,R>::TAO_ESF_Peer_Connected (R *peer)
  :  peer_ (peer)
{
}

// ****************************************************************

template<class P, class R>
TAO_ESF_Peer_Reconnected<P,R>::TAO_ESF_Peer_Reconnected (R *peer)
  :  peer_ (peer)
{
}

// ****************************************************************

template<class P, class R>
TAO_ESF_Peer_Disconnected<P,R>::TAO_ESF_Peer_Disconnected (R *peer)
  :  peer_ (peer)
{
}

// ****************************************************************

TAO_END_VERSIONED_NAMESPACE_DECL
