// $Id$
#ifndef TAO_NS_Method_Request_Dispatch_T_CPP
#define TAO_NS_Method_Request_Dispatch_T_CPP

#include "Method_Request_Dispatch_T.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Dispatch_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Method_Request_Dispatch_T, "$id$")

template <class EVENT, class PROXY, class EVENT_PARAM, class PROXY_PARAM>
TAO_NS_Method_Request_Dispatch_T<EVENT, PROXY, EVENT_PARAM, PROXY_PARAM>::TAO_NS_Method_Request_Dispatch_T (EVENT_PARAM event, PROXY_PARAM proxy_supplier, CORBA::Boolean filtering)
  :event_ (event), proxy_supplier_ (proxy_supplier), filtering_ (filtering)
{
}

template <class EVENT, class PROXY, class EVENT_PARAM, class PROXY_PARAM>
TAO_NS_Method_Request_Dispatch_T<EVENT, PROXY, EVENT_PARAM, PROXY_PARAM>::~TAO_NS_Method_Request_Dispatch_T ()
{
}

#endif /* TAO_NS_Method_Request_Dispatch_T_CPP */
