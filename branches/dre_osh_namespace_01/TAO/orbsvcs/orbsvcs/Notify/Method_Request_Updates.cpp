// $Id$

#include "Method_Request_Updates.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Updates.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_Method_Request_Updates, "$Id$")

#include "tao/debug.h"
#include "Proxy.h"
#include "Peer.h"

TAO_Notify_Method_Request_Updates::TAO_Notify_Method_Request_Updates (const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed, TAO_Notify_Proxy* proxy)
  : TAO_Notify_Method_Request_Updates_Base (added, removed, proxy)
{
}

TAO_Notify_Method_Request_Updates::~TAO_Notify_Method_Request_Updates ()
{
}

int
TAO_Notify_Method_Request_Updates::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

/********************************************************************************************************/

TAO_Notify_Method_Request_Updates_No_Copy::TAO_Notify_Method_Request_Updates_No_Copy (const TAO_Notify_EventTypeSeq& added
                                                                              , const TAO_Notify_EventTypeSeq& removed, TAO_Notify_Proxy* proxy)
  : TAO_Notify_Method_Request_Updates_No_Copy_Base (added, removed, proxy)
{
}

TAO_Notify_Method_Request_Updates_No_Copy::~TAO_Notify_Method_Request_Updates_No_Copy ()
{
}

TAO_Notify_Method_Request*
TAO_Notify_Method_Request_Updates_No_Copy::copy (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Notify_Method_Request* request;

  ACE_NEW_THROW_EX (request,
                    TAO_Notify_Method_Request_Updates (this->added_, this->removed_, this->proxy_),
                    CORBA::INTERNAL ());

  return request;
}

int
TAO_Notify_Method_Request_Updates_No_Copy::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Notify_Method_Request_Updates_T<const TAO_Notify_EventTypeSeq
, TAO_Notify_Proxy_Guard
, const TAO_Notify_EventTypeSeq&
, TAO_Notify_Proxy*
>;

template class TAO_Notify_Method_Request_Updates_T<const TAO_Notify_EventTypeSeq&
, TAO_Notify_Proxy*
, const TAO_Notify_EventTypeSeq&
, TAO_Notify_Proxy*
>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Notify_Method_Request_Updates_T<const TAO_Notify_EventTypeSeq
, TAO_Notify_Proxy_Guard
, const TAO_Notify_EventTypeSeq&
, TAO_Notify_Proxy*
>

#pragma instantiate TAO_Notify_Method_Request_Updates_T<const TAO_Notify_EventTypeSeq&
, TAO_Notify_Proxy*
, const TAO_Notify_EventTypeSeq&
, TAO_Notify_Proxy*
>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
