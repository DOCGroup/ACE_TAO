// $Id$

#include "RTCORBA_ProxySupplier.h"

#include "ace/Refcounted_Auto_Ptr.h"
#include "tao/debug.h"
#include "RTCORBA_Properties.h"
#include "Event.h"
#include "Types.h"

#if ! defined (__ACE_INLINE__)
#include "RTCORBA_ProxySupplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_RTCORBA_ProxySupplier, "$Id$")

TAO_NS_RTCORBA_ProxySupplier::TAO_NS_RTCORBA_ProxySupplier (void)
{
}

TAO_NS_RTCORBA_ProxySupplier::~TAO_NS_RTCORBA_ProxySupplier ()
{
}

void
TAO_NS_RTCORBA_ProxySupplier::init_ps (ACE_ENV_SINGLE_ARG_DECL)
{
  // Obtain our ref.
  CORBA::Object_var obj = this->ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->event_forwarder_ = Notify_Internal::Event_Forwarder::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
}

void
TAO_NS_RTCORBA_ProxySupplier::push (TAO_NS_Event_var &event)
{
  event->push (this->event_forwarder_.in () ACE_ENV_ARG_PARAMETER);
}

void 
TAO_NS_RTCORBA_ProxySupplier::forward (const CosNotification::StructuredEvent& event ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
		   CORBA::SystemException
		   ))
{
  if (TAO_debug_level > 0)
  ACE_DEBUG ((LM_DEBUG, "(%x,%P,%t) TAO_NS_RTCORBA_ProxySupplier::forward received event priority %d\n", this,
	      TAO_NS_RTCORBA_PROPERTIES::instance()->current ()->the_priority ()));

  // final dispatch to consumer
  this->consumer_->push (event ACE_ENV_ARG_PARAMETER);
}


