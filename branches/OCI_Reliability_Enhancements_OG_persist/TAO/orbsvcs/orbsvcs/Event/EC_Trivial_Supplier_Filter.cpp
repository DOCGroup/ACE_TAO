// $Id$

#include "orbsvcs/Event/EC_Trivial_Supplier_Filter.h"
#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "orbsvcs/Event/EC_ConsumerAdmin.h"
#include "orbsvcs/Event/EC_ProxySupplier.h"
#include "orbsvcs/Event/EC_QOS_Info.h"
#include "orbsvcs/Event/EC_Scheduling_Strategy.h"
#include "orbsvcs/Event/EC_ProxyConsumer.h" // @@ MSVC 6 bug



// ****************************************************************

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Trivial_Supplier_Filter::
    TAO_EC_Trivial_Supplier_Filter (TAO_EC_Event_Channel_Base* ec)
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
TAO_EC_Trivial_Supplier_Filter::connected (TAO_EC_ProxyPushSupplier*)
{
}

void
TAO_EC_Trivial_Supplier_Filter::reconnected (TAO_EC_ProxyPushSupplier*)
{
}

void
TAO_EC_Trivial_Supplier_Filter::disconnected (TAO_EC_ProxyPushSupplier*)
{
}

void
TAO_EC_Trivial_Supplier_Filter::shutdown (void)
{
}

void
TAO_EC_Trivial_Supplier_Filter::push (const RtecEventComm::EventSet& event,
                                      TAO_EC_ProxyPushConsumer *consumer)
{
  TAO_EC_Scheduling_Strategy* scheduling_strategy =
    this->event_channel_->scheduling_strategy ();
  scheduling_strategy->schedule_event (event,
                                       consumer,
                                       this);
}

void
TAO_EC_Trivial_Supplier_Filter::push_scheduled_event (RtecEventComm::EventSet &event,
                                                      const TAO_EC_QOS_Info &event_info)
{
  TAO_EC_Filter_Worker worker (event, event_info);
  this->event_channel_->for_each_consumer (&worker);
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
  TAO_EC_Trivial_Supplier_Filter_Builder (TAO_EC_Event_Channel_Base *ec)
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

TAO_END_VERSIONED_NAMESPACE_DECL
