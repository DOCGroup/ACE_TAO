/**
 * @file Auto_Disconnect.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */
#ifndef TAO_PERF_RTEC_AUTO_DISCONNECT_CPP
#define TAO_PERF_RTEC_AUTO_DISCONNECT_CPP

#include "Auto_Disconnect.h"

#if !defined(__ACE_INLINE__)
#include "Auto_Disconnect.inl"
#endif /* __ACE_INLINE__ */

template<class Client> Auto_Disconnect<Client>&
Auto_Disconnect<Client>::operator= (Client *client)
{
  this->Auto_Functor<Client,Disconnect<Client> >::operator= (client);
  return *this;
}

#endif /* TAO_PERF_RTEC_AUTO_DISCONNECT_CPP */
