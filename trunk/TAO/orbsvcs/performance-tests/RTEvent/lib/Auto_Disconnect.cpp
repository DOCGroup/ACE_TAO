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
#include "tao/Environment.h"

#if !defined(__ACE_INLINE__)
#include "Auto_Disconnect.inl"
#endif /* __ACE_INLINE__ */

template<class SERVANT>
Auto_Disconnect<SERVANT>::~Auto_Disconnect ()
{
  if (this->client_ == 0)
    return;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY {
    this->client_->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
  } ACE_CATCHANY {
    // @@ TODO This event should be logged. Cannot throw because that
    //    would make it impossible to use this class effectively.
    //    Read Servant_var.cpp for more details.
  } ACE_ENDTRY;
}

#endif /* TAO_PERF_RTEC_AUTO_DISCONNECT_CPP */
