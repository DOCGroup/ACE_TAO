// $Id$

#include "RTCORBA_ProxyConsumer.h"

#include "tao/debug.h"
#include "RTCORBA_Properties.h"
#include "Method_Request_Lookup.h"
#include "Event_Manager.h"

#if ! defined (__ACE_INLINE__)
#include "RTCORBA_ProxyConsumer.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_RTCORBA_ProxyConsumer, "$Id$")

TAO_NS_RTCORBA_ProxyConsumer::TAO_NS_RTCORBA_ProxyConsumer (void)
{
}

TAO_NS_RTCORBA_ProxyConsumer::~TAO_NS_RTCORBA_ProxyConsumer ()
{
}

void
TAO_NS_RTCORBA_ProxyConsumer::push (TAO_NS_Event_var &event)
{
  if (TAO_debug_level > 0)
  ACE_DEBUG ((LM_DEBUG, "(%x,%P,%t) received event priority %d\n", this,
          TAO_NS_RTCORBA_PROPERTIES::instance()->current ()->the_priority ()));

  TAO_NS_Method_Request_Lookup request (event, this, this->event_manager_->consumer_map ());

  request.call ();
}
