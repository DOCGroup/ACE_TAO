// $Id$

#include "EC_Trivial_Supplier_Filter.h"
#include "EC_Event_Channel.h"
#include "EC_ConsumerAdmin.h"
#include "EC_ProxySupplier.h"
#include "EC_QOS_Info.h"

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
TAO_EC_Trivial_Supplier_Filter::connected (TAO_EC_ProxyPushSupplier*,
                                           CORBA::Environment &)
{
}

void
TAO_EC_Trivial_Supplier_Filter::reconnected (TAO_EC_ProxyPushSupplier*,
                                             CORBA::Environment &)
{
}

void
TAO_EC_Trivial_Supplier_Filter::disconnected (TAO_EC_ProxyPushSupplier*,
                                              CORBA::Environment &)
{
}

void
TAO_EC_Trivial_Supplier_Filter::shutdown (CORBA::Environment &)
{
}

void
TAO_EC_Trivial_Supplier_Filter::push (const RtecEventComm::EventSet& event,
                                      CORBA::Environment &ACE_TRY_ENV)
{
  TAO_EC_ConsumerAdmin* consumer_admin =
    this->event_channel_->consumer_admin ();

  for (CORBA::ULong j = 0; j < event.length (); ++j)
    {
      const RtecEventComm::Event& e = event[j];
      RtecEventComm::Event* buffer =
        ACE_const_cast(RtecEventComm::Event*, &e);
      RtecEventComm::EventSet single_event (1, 1, buffer, 0);

      TAO_EC_QOS_Info qos_info;

      TAO_EC_Filter_Worker worker (single_event, qos_info);
      consumer_admin->for_each (&worker, ACE_TRY_ENV);
      ACE_CHECK;
    }
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
