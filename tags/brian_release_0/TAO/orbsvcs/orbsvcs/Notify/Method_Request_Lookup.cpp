// $Id$

#include "Method_Request_Lookup.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Lookup.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_Notify_Method_Request_Lookup, "$Id$")

#include "tao/debug.h"
#include "Consumer_Map.h"
#include "ProxySupplier.h"
#include "ProxyConsumer.h"
#include "Proxy.h"
#include "Admin.h"
#include "SupplierAdmin.h"

TAO_Notify_Method_Request_Lookup::TAO_Notify_Method_Request_Lookup (const TAO_Notify_Event_var& event, TAO_Notify_ProxyConsumer* proxy_consumer)
  : TAO_Notify_Method_Request_Lookup_Base (event, proxy_consumer)
{
  this->init (event);
}

TAO_Notify_Method_Request_Lookup::~TAO_Notify_Method_Request_Lookup ()
{
}

int
TAO_Notify_Method_Request_Lookup::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

/******************************************************************************************************/

TAO_Notify_Method_Request_Lookup_No_Copy::TAO_Notify_Method_Request_Lookup_No_Copy (const TAO_Notify_Event* event, TAO_Notify_ProxyConsumer* proxy_consumer)
  : TAO_Notify_Method_Request_Lookup_No_Copy_Base (event, proxy_consumer)
{
}

TAO_Notify_Method_Request_Lookup_No_Copy::~TAO_Notify_Method_Request_Lookup_No_Copy ()
{
}

int
TAO_Notify_Method_Request_Lookup_No_Copy::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

TAO_Notify_Method_Request*
TAO_Notify_Method_Request_Lookup_No_Copy::copy (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Notify_Method_Request* request;

  TAO_Notify_Event* event_copy = this->event_->copy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  TAO_Notify_Event_Copy_var event_var (event_copy);

  ACE_NEW_THROW_EX (request,
                    TAO_Notify_Method_Request_Lookup (event_var, this->proxy_consumer_),
                    CORBA::INTERNAL ());

  return request;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Notify_Method_Request_Lookup_T<const TAO_Notify_Event_var
, TAO_Notify_ProxyConsumer_Guard
, const TAO_Notify_Event_var&
, TAO_Notify_ProxyConsumer*>;

template class TAO_Notify_Method_Request_Lookup_T<const TAO_Notify_Event*
, TAO_Notify_ProxyConsumer*
, const TAO_Notify_Event*
, TAO_Notify_ProxyConsumer*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Notify_Method_Request_Lookup_T<const TAO_Notify_Event_var
, TAO_Notify_ProxyConsumer_Guard
, const TAO_Notify_Event_var&
, TAO_Notify_ProxyConsumer*>

#pragma instantiate TAO_Notify_Method_Request_Lookup_T<const TAO_Notify_Event*
, TAO_Notify_ProxyConsumer*
, const TAO_Notify_Event*
, TAO_Notify_ProxyConsumer*>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
