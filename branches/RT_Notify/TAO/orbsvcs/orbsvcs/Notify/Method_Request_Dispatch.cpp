// $Id$

#include "Method_Request_Dispatch.h"
#include "ProxySupplier.h"
#include "Consumer.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Dispatch.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Method_Request_Dispatch, "$Id$")

TAO_NS_Method_Request_Dispatch::TAO_NS_Method_Request_Dispatch (TAO_NS_Event_var& event, TAO_NS_ProxySupplier* proxy_supplier)
  : TAO_NS_Method_Request (event), proxy_supplier_ (proxy_supplier)
{
}

TAO_NS_Method_Request_Dispatch::~TAO_NS_Method_Request_Dispatch ()
{
}

TAO_NS_Method_Request*
TAO_NS_Method_Request_Dispatch::copy (void)
{
  /// @@use factory
  return new TAO_NS_Method_Request_Dispatch (this->event_, this->proxy_supplier_);
}

int
TAO_NS_Method_Request_Dispatch::call (void)
{
  ACE_DECLARE_NEW_CORBA_ENV; 

  this->event_->push (this->proxy_supplier_->consumer () ACE_ENV_ARG_PARAMETER);

  return 0;
}

