// $Id$

#include "Supplier.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/RtecEventCommC.h"



Supplier::Supplier (RtecEventComm::EventSourceID id,
                    const RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer_proxy)
:id_ (id),
 consumer_proxy_ (consumer_proxy)
{
}

void
Supplier::timeout_occured (void)
{
  RtecEventComm::EventSet event (1);
  if (id_ == 1)
    {
      event.length (1);
      event[0].header.type   = ACE_ES_EVENT_UNDEFINED;
      event[0].header.source = id_;
      event[0].header.ttl    = 1;
    }
  else
    {
      event.length (1);
      event[0].header.type   = ACE_ES_EVENT_UNDEFINED + 1;
      event[0].header.source = id_;
      event[0].header.ttl    = 1;
    }

  consumer_proxy_->push (event);
}

void
Supplier::disconnect_push_supplier (void)
{
}

Timeout_Consumer::Timeout_Consumer (Supplier* supplier)
  :supplier_impl_ (supplier)
{
}

void
Timeout_Consumer::push (const RtecEventComm::EventSet& events)
{
  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TimeoutConsumer (%t) no events\n"));
      return;
    }

  ACE_DEBUG ((LM_DEBUG, "(%t) Timeout Event received\n"));
  supplier_impl_->timeout_occured ();
}

void
Timeout_Consumer::disconnect_push_consumer (void)
{
}

