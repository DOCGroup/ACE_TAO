// $Id$
#ifndef TAO_Notify_Method_Request_Lookup_T_CPP
#define TAO_Notify_Method_Request_Lookup_T_CPP

#include "Method_Request_Lookup_T.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Lookup_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_Method_Request_Lookup_T, "$id$")

template <class EVENT, class PROXY, class EVENT_PARAM, class PROXY_PARAM>
TAO_Notify_Method_Request_Lookup_T<EVENT, PROXY, EVENT_PARAM, PROXY_PARAM>::TAO_Notify_Method_Request_Lookup_T (EVENT_PARAM event, PROXY_PARAM proxy_consumer)
  :event_ (event), proxy_consumer_ (proxy_consumer)
{
}

template <class EVENT, class PROXY, class EVENT_PARAM, class PROXY_PARAM>
TAO_Notify_Method_Request_Lookup_T<EVENT, PROXY, EVENT_PARAM, PROXY_PARAM>::~TAO_Notify_Method_Request_Lookup_T ()
{
}


template <class EVENT, class PROXY, class EVENT_PARAM, class PROXY_PARAM> void
TAO_Notify_Method_Request_Lookup_T<EVENT, PROXY, EVENT_PARAM, PROXY_PARAM>::work (TAO_Notify_ProxySupplier* proxy_supplier ACE_ENV_ARG_DECL)
{
  proxy_supplier->push (this->event_ ACE_ENV_ARG_PARAMETER);
}

#endif /* TAO_Notify_Method_Request_Lookup_T_CPP */
