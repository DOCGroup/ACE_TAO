// $Id$

#ifndef TAO_ESF_PEER_ADMIN_CPP
#define TAO_ESF_PEER_ADMIN_CPP

#include "ESF_Peer_Admin.h"

#if ! defined (__ACE_INLINE__)
#include "ESF_Peer_Admin.i"
#endif /* __ACE_INLINE__ */

#include "ESF_Peer_Workers.h"

ACE_RCSID(ESF, ESF_Peer_Admin, "$Id$")

template<class EC, class P,class I, class PEER>
TAO_ESF_Peer_Admin<EC,P,I,PEER>::~TAO_ESF_Peer_Admin (void)
{
}

template<class EC, class P,class I, class PEER> void
TAO_ESF_Peer_Admin<EC,P,I,PEER>::peer_connected (PEER *peer,
                                                 CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC (())
{
  TAO_ESF_Peer_Connected<P,PEER> worker (peer);

  this->for_each (&worker, ACE_TRY_ENV);
}

template<class EC, class P,class I, class PEER> void
TAO_ESF_Peer_Admin<EC,P,I,PEER>::peer_reconnected (PEER *peer,
                                                   CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC (())
{
  TAO_ESF_Peer_Reconnected<P,PEER> worker (peer);

  this->for_each (&worker, ACE_TRY_ENV);
}

template<class EC, class P,class I, class PEER> void
TAO_ESF_Peer_Admin<EC,P,I,PEER>::peer_disconnected (PEER *peer,
                                                    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC (())
{
  TAO_ESF_Peer_Disconnected<P,PEER> worker (peer);

  this->for_each (&worker, ACE_TRY_ENV);
}

#endif /* TAO_ESF_PEER_ADMIN_CPP */
