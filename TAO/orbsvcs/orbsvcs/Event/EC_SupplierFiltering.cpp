// $Id$

#include "EC_SupplierFiltering.h"
#include "EC_Event_Channel.h"
#include "EC_ConsumerAdmin.h"
#include "EC_ProxySupplier.h"
#include "EC_QOS_Info.h"

#if ! defined (__ACE_INLINE__)
#include "EC_SupplierFiltering.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_SupplierFiltering, "$Id$")

TAO_EC_SupplierFiltering::~TAO_EC_SupplierFiltering (void)
{
}

// ****************************************************************

TAO_EC_Null_SupplierFiltering::
    TAO_EC_Null_SupplierFiltering (TAO_EC_Event_Channel* ec)
  :  event_channel_ (ec)
{
}

void
TAO_EC_Null_SupplierFiltering::bind (TAO_EC_ProxyPushConsumer*)
{
}

void
TAO_EC_Null_SupplierFiltering::unbind (TAO_EC_ProxyPushConsumer*)
{
}

void
TAO_EC_Null_SupplierFiltering::connected (TAO_EC_ProxyPushSupplier*,
                                          CORBA::Environment &)
{
}

void
TAO_EC_Null_SupplierFiltering::disconnected (TAO_EC_ProxyPushSupplier*,
                                             CORBA::Environment &)
{
}

void
TAO_EC_Null_SupplierFiltering::push (const RtecEventComm::EventSet& event,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  TAO_EC_ConsumerAdmin* consumer_admin =
    this->event_channel_->consumer_admin ();

  ACE_GUARD_THROW (TAO_EC_ConsumerAdmin::Busy_Lock,
      ace_mon, consumer_admin->busy_lock (),
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

  TAO_EC_ConsumerAdmin::SupplierSetIterator end =
    consumer_admin->end ();

  for (CORBA::ULong j = 0; j < event.length (); ++j)
    {
      const RtecEventComm::Event& e = event[j];
      RtecEventComm::Event* buffer = 
        ACE_const_cast(RtecEventComm::Event*, &e);
      RtecEventComm::EventSet single_event (1, 1, buffer, 0);

      for (TAO_EC_ConsumerAdmin::SupplierSetIterator i =
             consumer_admin->begin ();
           i != end;
           ++i)
        {
          TAO_EC_QOS_Info qos_info;

          (*i)->filter (single_event, qos_info, ACE_TRY_ENV);
          ACE_CHECK;
        }
    }
}
