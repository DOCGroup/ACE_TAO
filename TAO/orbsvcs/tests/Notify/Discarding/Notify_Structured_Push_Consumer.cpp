// $Id$

#include "Notify_Structured_Push_Consumer.h"
#include "common.h"

Notify_Structured_Push_Consumer::Notify_Structured_Push_Consumer (
                                            const char* name,
                                            CORBA::Short policy,
                                            unsigned int expected,
                                            CORBA::Boolean& done)
 : name_ (name),
   discard_policy_ (policy),
   expected_ (expected),
   count_ (0),
   done_ (done)
{
}


void
Notify_Structured_Push_Consumer::connect (
                CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin
                TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosNotifyComm::StructuredPushConsumer_var objref =
    this->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (
      CosNotifyChannelAdmin::STRUCTURED_EVENT,
      proxy_supplier_id_
      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->proxy_supplier_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (
      proxysupplier.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CosNotification::QoSProperties properties (2);
  properties.length (2);
  properties[0].name = CORBA::string_dup (CosNotification::DiscardPolicy);
  properties[0].value <<= this->discard_policy_;
  properties[1].name = CORBA::string_dup (CosNotification::MaxEventsPerConsumer);
  properties[1].value <<= (CORBA::Long)10;

  this->proxy_supplier_->set_qos (properties);
  this->proxy_supplier_->connect_structured_push_consumer (objref.in ()
                                                           TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // give ownership to POA
  this->_remove_ref ();
}


void
Notify_Structured_Push_Consumer::push_structured_event (
                          const CosNotification::StructuredEvent& event
                          TAO_ENV_ARG_DECL_NOT_USED /*TAO_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Received event:\n"));
  CORBA::ULong hlength = event.header.variable_header.length ();
  for (CORBA::ULong hi = 0; hi < hlength; hi++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s = %s\n",
                  (const char*)event.header.variable_header[hi].name,
                  Any_String (event.header.variable_header[hi].value)));
    }
  CORBA::ULong flength = event.filterable_data.length ();
  for (CORBA::ULong i = 0; i < flength; i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%s = %s\n",
                  (const char*)event.filterable_data[i].name,
                  Any_String (event.filterable_data[i].value)));
    }
  ACE_DEBUG ((LM_DEBUG,
              "-------------------------\n"));
  this->count_++;
  if (this->count_ > this->expected_)
    {
      this->done_ = 1;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Structured Consumer (%P|%t): ERROR: too "
                            "many events received.\n")));
    }
  else if (this->count_ == this->expected_)
    {
      this->done_ = 1;
    }
  else
    {
      ACE_OS::sleep (1);
    }
}

