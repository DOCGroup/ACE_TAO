// $Id$

#include "Supplier.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/RtecEventCommC.h"

ACE_RCSID(EC_Examples, Supplier, "$Id$")

Supplier::Supplier (RtecEventComm::EventSourceID id,
                    const RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer_proxy)
:id_ (id),
 consumer_proxy_ (consumer_proxy)
{
}

void
Supplier::timeout_occured (ACE_ENV_SINGLE_ARG_DECL)
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

  consumer_proxy_->push (event ACE_ENV_ARG_PARAMETER);
}

void
Supplier::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

Timeout_Consumer::Timeout_Consumer (Supplier* supplier)
  :supplier_impl_ (supplier)
{
}

void
Timeout_Consumer::push (const RtecEventComm::EventSet& events
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "TimeoutConsumer (%t) no events\n"));
      return;
    }

  ACE_DEBUG ((LM_DEBUG, "(%t) Timeout Event received\n"));
  supplier_impl_->timeout_occured (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
Timeout_Consumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

