// $Id$

#include "RT_ProxySupplier.h"

#include "ace/Refcounted_Auto_Ptr.h"
#include "tao/debug.h"
#include "RT_Properties.h"
#include "Event.h"
#include "Structured/StructuredEvent.h"
#include "Method_Request_Dispatch.h"
#include "Method_Request_Dispatch_No_Filtering.h"
#include "Worker_Task.h"

#if ! defined (__ACE_INLINE__)
#include "RT_ProxySupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_RT_ProxySupplier, "$Id$")

TAO_NS_RT_ProxySupplier::TAO_NS_RT_ProxySupplier (void)
{
}

TAO_NS_RT_ProxySupplier::~TAO_NS_RT_ProxySupplier ()
{
}

void
TAO_NS_RT_ProxySupplier::init (TAO_NS_ConsumerAdmin* consumer_admin ACE_ENV_SINGLE_ARG_DECL)
{
  // Obtain our ref.
  CORBA::Object_var obj = this->ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->event_forwarder_ = Notify_Internal::Event_Forwarder::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RT_ProxySupplier::push (TAO_NS_Event_var &event)
{
  event->push (this->event_forwarder_.in () ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RT_ProxySupplier::push_no_filtering (TAO_NS_Event_var &event)
{
  event->push_no_filtering (this->event_forwarder_.in () ACE_ENV_ARG_PARAMETER);
}
