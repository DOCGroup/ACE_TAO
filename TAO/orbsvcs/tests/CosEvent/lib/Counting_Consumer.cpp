// $Id$

#include "Counting_Consumer.h"

ACE_RCSID(CEC_Tests, CEC_Count_Consumer, "$Id$")

CEC_Counting_Consumer::CEC_Counting_Consumer (const char* name)
  : event_count (0),
    disconnect_count (0),
    name_ (name)
{
}

void
CEC_Counting_Consumer::connect (CosEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
                   CORBA::Environment &ACE_TRY_ENV)
{
  // The canonical protocol to connect to the EC

  CosEventComm::PushConsumer_var consumer =
    this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  if (CORBA::is_nil (this->supplier_proxy_.in ()))
    {
      this->supplier_proxy_ =
        consumer_admin->obtain_push_supplier (ACE_TRY_ENV);
      ACE_CHECK;
    }

  this->supplier_proxy_->connect_push_consumer (consumer.in (),
                                                ACE_TRY_ENV);
  ACE_CHECK;
}

void
CEC_Counting_Consumer::disconnect (CORBA::Environment &ACE_TRY_ENV)
{
  if (!CORBA::is_nil (this->supplier_proxy_.in ()))
    {
      this->supplier_proxy_->disconnect_push_supplier (ACE_TRY_ENV);
      ACE_CHECK;
    }

  PortableServer::POA_var consumer_poa =
    this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;
  PortableServer::ObjectId_var consumer_id =
    consumer_poa->servant_to_id (this, ACE_TRY_ENV);
  ACE_CHECK;
  consumer_poa->deactivate_object (consumer_id.in (), ACE_TRY_ENV);
  ACE_CHECK;

  this->supplier_proxy_ =
    CosEventChannelAdmin::ProxyPushSupplier::_nil ();
}

void
CEC_Counting_Consumer::dump_results (int expected_count, int tolerance)
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
CEC_Counting_Consumer::push (const CORBA::Any&,
                             CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
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
CEC_Counting_Consumer::disconnect_push_consumer (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->disconnect_count++;
  this->supplier_proxy_ =
    CosEventChannelAdmin::ProxyPushSupplier::_nil ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
