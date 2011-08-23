// $Id$

#ifndef TAO_ESF_SHUTDOWN_PROXY_CPP
#define TAO_ESF_SHUTDOWN_PROXY_CPP

#include "orbsvcs/ESF/ESF_Shutdown_Proxy.h"
#include "tao/Exception.h"
#include "ace/CORBA_macros.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Shutdown_Proxy.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class PROXY> void
TAO_ESF_Shutdown_Proxy<PROXY>::work (PROXY *proxy)
{
  try
    {
      proxy->shutdown ();
    }
  catch (const CORBA::Exception&)
    {
      // Do not propagate any exceptions
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_ESF_SHUTDOWN_PROXY_CPP */
