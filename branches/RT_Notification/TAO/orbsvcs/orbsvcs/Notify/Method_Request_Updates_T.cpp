// $Id$
#ifndef TAO_NS_Method_Request_Updates_T_CPP
#define TAO_NS_Method_Request_Updates_T_CPP

#include "Method_Request_Updates_T.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Updates_T.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Method_Request_Updates_T, "$id$")

template <class SEQ, class PROXY, class SEQ_PARAM, class PROXY_PARAM>
TAO_NS_Method_Request_Updates_T<SEQ, PROXY, SEQ_PARAM, PROXY_PARAM>::TAO_NS_Method_Request_Updates_T (SEQ_PARAM added, SEQ_PARAM removed,
                                                                                                      PROXY_PARAM proxy)
  : added_ (added), removed_ (removed), proxy_ (proxy)
{
}

template <class SEQ, class PROXY, class SEQ_PARAM, class PROXY_PARAM>
TAO_NS_Method_Request_Updates_T<SEQ, PROXY, SEQ_PARAM, PROXY_PARAM>::~TAO_NS_Method_Request_Updates_T ()
{
}

#endif /* TAO_NS_Method_Request_Updates_T_CPP */
