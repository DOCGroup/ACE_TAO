/**
 * @file Auto_Functor.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */
#ifndef TAO_PERF_RTEC_AUTO_FUNCTOR_CPP
#define TAO_PERF_RTEC_AUTO_FUNCTOR_CPP

#include "Auto_Functor.h"
#include "tao/Environment.h"
#include "ace/Swap.h"

#if !defined(__ACE_INLINE__)
#include "Auto_Functor.inl"
#endif /* __ACE_INLINE__ */

template<class Client, class Functor>
Auto_Functor<Client,Functor>::Auto_Functor (const Auto_Functor<Client,Functor> &rhs)
  : client_ (0)
{
  ACE_Swap<Client*>::swap (this->client_, rhs.client_);
}

template<class Client, class Functor> Auto_Functor<Client,Functor>&
Auto_Functor<Client,Functor>::operator= (const Auto_Functor<Client,Functor> &rhs)
{
  Auto_Functor<Client,Functor> tmp (rhs);
  ACE_Swap<Client*>::swap (this->client_, tmp.client_);
  return *this;
}

template<class Client, class Functor>
Auto_Functor<Client,Functor>::~Auto_Functor ()
{
  if (this->client_ == 0)
    return;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY {
    Functor functor;
    functor (this->client_ ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
  } ACE_CATCHANY {
    // @@ TODO This event should be logged. Cannot throw because that
    //    would make it impossible to use this class effectively.
    //    Read Servant_var.cpp for more details.
  } ACE_ENDTRY;
}

template<class Client, class Functor> Auto_Functor<Client,Functor>&
Auto_Functor<Client,Functor>::operator= (Client *client)
{
  Auto_Functor<Client,Functor> tmp (client);
  ACE_Swap<Client*>::swap (this->client_, tmp.client_);
  return *this;
}

#endif /* TAO_PERF_RTEC_AUTO_FUNCTOR_CPP */
