// $Id$

#include "Counting_Consumer.h"



EC_Counting_Consumer::EC_Counting_Consumer (const char* name)
  : event_count (0),
    disconnect_count (0),
    name_ (name)
{
}

void
EC_Counting_Consumer::connect (RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
                   const RtecEventChannelAdmin::ConsumerQOS &qos)
{
  // The canonical protocol to connect to the EC

  RtecEventComm::PushConsumer_var consumer =
    this->_this ();

  if (CORBA::is_nil (this->supplier_proxy_.in ()))
    {
      this->supplier_proxy_ =
        consumer_admin->obtain_push_supplier ();
    }

  this->supplier_proxy_->connect_push_consumer (consumer.in (),
                                                qos);
}

void
EC_Counting_Consumer::disconnect (void)
{
  if (!CORBA::is_nil (this->supplier_proxy_.in ()))
    {
      this->supplier_proxy_->disconnect_push_supplier ();
      this->supplier_proxy_ =
        RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
    }
  this->deactivate ();
}

void
EC_Counting_Consumer::deactivate (void)
{
  PortableServer::POA_var consumer_poa =
    this->_default_POA ();
  PortableServer::ObjectId_var consumer_id =
    consumer_poa->servant_to_id (this);
  consumer_poa->deactivate_object (consumer_id.in ());

}

void
EC_Counting_Consumer::dump_results (int expected_count, int tolerance)
{
  int diff = this->event_count - expected_count;
  if (diff > tolerance || diff < -tolerance)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "ERROR - %s unexpected number of events <%d> instead of <%d>\n",
                  this->name_,
                  this->event_count,
                  expected_count));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s - number of events <%d> within margins expected <%d>\n",
                  this->name_,
                  this->event_count,
                  expected_count));
    }
}

void
EC_Counting_Consumer::push (const RtecEventComm::EventSet& events)
{
  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s (%P|%t) no events\n", this->name_));
      return;
    }

  this->event_count ++;
#if 0
  if (this->event_count % 10 == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s (%P|%t): %d events received\n",
                  this->name_,
                  this->event_count));
    }
#endif /* 0 */
}

void
EC_Counting_Consumer::disconnect_push_consumer (void)
{
  this->disconnect_count++;
  this->supplier_proxy_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
}
