// $Id$

#include "EC_Trivial_Supplier_Filter.h"
#include "EC_Event_Channel.h"
#include "EC_ConsumerAdmin.h"
#include "EC_ProxySupplier.h"
#include "EC_QOS_Info.h"
#include "EC_Scheduling_Strategy.h"
#include "EC_ProxyConsumer.h" // @@ MSVC 6 bug

#if ! defined (__ACE_INLINE__)
#include "EC_Trivial_Supplier_Filter.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Trivial_Supplier_Filter, "$Id$")

// ****************************************************************

TAO_EC_Trivial_Supplier_Filter::
    TAO_EC_Trivial_Supplier_Filter (TAO_EC_Event_Channel* ec)
  :  event_channel_ (ec)
{
}

void
TAO_EC_Trivial_Supplier_Filter::bind (TAO_EC_ProxyPushConsumer*)
{
}

void
TAO_EC_Trivial_Supplier_Filter::unbind (TAO_EC_ProxyPushConsumer*)
{
}

void
TAO_EC_Trivial_Supplier_Filter::connected (TAO_EC_ProxyPushSupplier*
                                           TAO_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_EC_Trivial_Supplier_Filter::reconnected (TAO_EC_ProxyPushSupplier*
                                             TAO_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_EC_Trivial_Supplier_Filter::disconnected (TAO_EC_ProxyPushSupplier*
                                              TAO_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_EC_Trivial_Supplier_Filter::shutdown (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
{
}

void
TAO_EC_Trivial_Supplier_Filter::push (const RtecEventComm::EventSet& event,
                                      TAO_EC_ProxyPushConsumer *consumer
                                      TAO_ENV_ARG_DECL)
{
  TAO_EC_Scheduling_Strategy* scheduling_strategy =
    this->event_channel_->scheduling_strategy ();
  scheduling_strategy->schedule_event (event,
                                       consumer,
                                       this
                                       TAO_ENV_ARG_PARAMETER);
}

void
TAO_EC_Trivial_Supplier_Filter::push_scheduled_event (RtecEventComm::EventSet &event,
                                                      const TAO_EC_QOS_Info &event_info
                                                      TAO_ENV_ARG_DECL)
{
  TAO_EC_Filter_Worker worker (event, event_info);
  this->event_channel_->consumer_admin ()->for_each (&worker
                                                     TAO_ENV_ARG_PARAMETER);
}

CORBA::ULong
TAO_EC_Trivial_Supplier_Filter::_incr_refcnt (void)
{
  return 1;
}

CORBA::ULong
TAO_EC_Trivial_Supplier_Filter::_decr_refcnt (void)
{
  return 1;
}

// ****************************************************************

TAO_EC_Trivial_Supplier_Filter_Builder::
  TAO_EC_Trivial_Supplier_Filter_Builder (TAO_EC_Event_Channel *ec)
  :  filter_ (ec)
{
}

TAO_EC_Supplier_Filter*
TAO_EC_Trivial_Supplier_Filter_Builder::create (
    RtecEventChannelAdmin::SupplierQOS&)
{
  return &this->filter_;
}

void
TAO_EC_Trivial_Supplier_Filter_Builder::destroy (
    TAO_EC_Supplier_Filter*)
{
}
