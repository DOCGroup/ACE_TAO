// $Id$

#include "Method_Request_Lookup.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Lookup.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Method_Request_Lookup, "$Id$")

#include "tao/debug.h"
#include "Consumer_Map.h"
#include "ProxySupplier.h"
#include "ProxyConsumer.h"
#include "Proxy.h"
#include "Admin.h"
#include "SupplierAdmin.h"

TAO_NS_Method_Request_Lookup::TAO_NS_Method_Request_Lookup (const TAO_NS_Event_var& event, TAO_NS_ProxyConsumer* proxy_consumer)
  : TAO_NS_Method_Request_Lookup_Base (event, proxy_consumer)
{
  this->init (event);
}

TAO_NS_Method_Request_Lookup::~TAO_NS_Method_Request_Lookup ()
{
}

int
TAO_NS_Method_Request_Lookup::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

/******************************************************************************************************/

TAO_NS_Method_Request_Lookup_No_Copy::TAO_NS_Method_Request_Lookup_No_Copy (const TAO_NS_Event* event, TAO_NS_ProxyConsumer* proxy_consumer)
  : TAO_NS_Method_Request_Lookup_No_Copy_Base (event, proxy_consumer)
{
}

TAO_NS_Method_Request_Lookup_No_Copy::~TAO_NS_Method_Request_Lookup_No_Copy ()
{
}

int
TAO_NS_Method_Request_Lookup_No_Copy::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

TAO_NS_Method_Request*
TAO_NS_Method_Request_Lookup_No_Copy::copy (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_NS_Method_Request* request;

  TAO_NS_Event* event_copy = this->event_->copy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  TAO_NS_Event_Copy_var event_var (event_copy);

  ACE_NEW_THROW_EX (request,
                    TAO_NS_Method_Request_Lookup (event_var, this->proxy_consumer_),
                    CORBA::INTERNAL ());

  return request;
}
