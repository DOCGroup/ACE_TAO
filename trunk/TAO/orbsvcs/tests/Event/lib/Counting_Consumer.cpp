// $Id$

#include "Counting_Consumer.h"

ACE_RCSID(EC_Tests, EC_Count_Consumer, "$Id$")

EC_Counting_Consumer::EC_Counting_Consumer (const char* name)
  : event_count (0),
    disconnect_count (0),
    name_ (name)
{
}

void
EC_Counting_Consumer::connect (RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
                   const RtecEventChannelAdmin::ConsumerQOS &qos
                   ACE_ENV_ARG_DECL)
{
  // The canonical protocol to connect to the EC

  RtecEventComm::PushConsumer_var consumer =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->supplier_proxy_.in ()))
    {
      this->supplier_proxy_ =
        consumer_admin->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  this->supplier_proxy_->connect_push_consumer (consumer.in (),
                                                qos
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
EC_Counting_Consumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  if (!CORBA::is_nil (this->supplier_proxy_.in ()))
    {
      this->supplier_proxy_->disconnect_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      this->supplier_proxy_ =
        RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
    }
  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
EC_Counting_Consumer::deactivate (ACE_ENV_SINGLE_ARG_DECL)
{
  PortableServer::POA_var consumer_poa =
    this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::ObjectId_var consumer_id =
    consumer_poa->servant_to_id (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  consumer_poa->deactivate_object (consumer_id.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

void
EC_Counting_Consumer::dump_results (int expected_count, int tolerance)
{
  int diff = this->event_count - expected_count;
  if (diff > tolerance || diff < -tolerance)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "ERROR - %s unexpected number of events  <%d>\n",
                  this->name_,
                  this->event_count));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s - number of events <%d> within margins\n",
                  this->name_,
                  this->event_count));
    }
}

void
EC_Counting_Consumer::push (const RtecEventComm::EventSet& events
                ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
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
EC_Counting_Consumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->disconnect_count++;
  this->supplier_proxy_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
}
