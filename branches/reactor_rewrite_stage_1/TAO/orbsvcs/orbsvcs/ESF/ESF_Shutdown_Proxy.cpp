// $Id$

#ifndef TAO_ESF_SHUTDOWN_PROXY_CPP
#define TAO_ESF_SHUTDOWN_PROXY_CPP

#include "ESF_Shutdown_Proxy.h"

#if ! defined (__ACE_INLINE__)
#include "ESF_Shutdown_Proxy.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(ESF, ESF_Shutdown_Proxy, "$Id$")

template<class PROXY> void
TAO_ESF_Shutdown_Proxy<PROXY>::work (PROXY *proxy
                                     TAO_ENV_ARG_DECL)
{
  ACE_TRY
    {
      proxy->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Do not propagate any exceptions
    }
  ACE_ENDTRY;
}

#endif /* TAO_ESF_SHUTDOWN_PROXY_CPP */
