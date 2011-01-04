// $Id$
// Reused from: $TAO_ROOT/orbsvcs/examples/RtEC/MCast

#include "Consumer.h"
#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/Event_Service_Constants.h"

Consumer::Consumer (void)
  : event_count_ (0)
{
}

void
Consumer::connect (RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin)
{
  this->proxy_ =
    consumer_admin->obtain_push_supplier ();

  RtecEventComm::PushConsumer_var me =
    this->_this ();

  // Simple subscription, but usually the helper classes in
  // $TAO_ROOT/orbsvcs/Event_Utils.h are a better way to do this.
  RtecEventChannelAdmin::ConsumerQOS qos;
  qos.is_gateway = 0;

  qos.dependencies.length (2);
  RtecEventComm::EventHeader& h0 =
    qos.dependencies[0].event.header;
  h0.type   = ACE_ES_DISJUNCTION_DESIGNATOR;
  h0.source = 1; // The disjunction has one element

  RtecEventComm::EventHeader& h1 =
    qos.dependencies[1].event.header;
  h1.type   = ACE_ES_EVENT_UNDEFINED;  // first free event type
  h1.source = ACE_ES_EVENT_SOURCE_ANY; // Any source is OK

  this->proxy_->connect_push_consumer (me.in (), qos);
}

void
Consumer::disconnect (void)
{
  try
    {
      // Disconnect from the proxy
      this->proxy_->disconnect_push_supplier ();
    }
  catch (const CORBA::Exception&)
    {
      // Ignore exceptions
    }
  this->proxy_ = RtecEventChannelAdmin::ProxyPushSupplier::_nil ();

  // Deactivate this object
  PortableServer::POA_var poa =
    this->_default_POA ();
  // Get the Object Id used for the servant..
  PortableServer::ObjectId_var oid =
    poa->servant_to_id (this);
  // Deactivate the object
  poa->deactivate_object (oid.in ());
}

void
Consumer::push (const RtecEventComm::EventSet& events)
{
  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Consumer (%P|%t) no events\n"));
      return;
    }

  this->event_count_ += events.length ();
  if (this->event_count_ % 10000 == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Consumer (%P|%t): %d events received\n",
                  this->event_count_));
    }
}

void
Consumer::disconnect_push_consumer (void)
{
}

