// $Id$

#include "Notify_Structured_Push_Consumer.h"
#include "orbsvcs/Notify/Notify_Extensions.h"
#include "common.h"
#include "tao/debug.h"

Notify_Structured_Push_Consumer::Notify_Structured_Push_Consumer (
                                            const char* name,
                                            TimeBase::TimeT block,
                                            unsigned int expected,
                                            CORBA::Boolean& done)
 : name_ (name),
   blocking_timeout_ (block),
   expected_ (expected),
   count_ (0),
   done_ (done)
{
}


void
Notify_Structured_Push_Consumer::connect (
                CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin
                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosNotifyComm::StructuredPushConsumer_var objref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (
      CosNotifyChannelAdmin::STRUCTURED_EVENT,
      proxy_id_
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->proxy_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (
      proxysupplier.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CosNotification::QoSProperties properties (2);
  properties.length (2);
  properties[0].name = CORBA::string_dup (CosNotification::MaxEventsPerConsumer);
  properties[0].value <<= (CORBA::Long)10;
  properties[1].name = CORBA::string_dup (TAO_Notify_Extensions::BlockingPolicy);
  properties[1].value <<= this->blocking_timeout_;

  this->proxy_->set_qos (properties);
  this->proxy_->connect_structured_push_consumer (objref.in ()
                                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // give ownership to POA
  this->_remove_ref ();
}


void
Notify_Structured_Push_Consumer::push_structured_event (
                          const CosNotification::StructuredEvent& event
                          ACE_ENV_ARG_DECL_NOT_USED /*ACE_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Received event:\n"));

  CORBA::ULong hlength = event.header.variable_header.length ();
  for (CORBA::ULong hi = 0; hi < hlength; hi++)
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG,
                    "%s = %s\n",
                    (const char*)event.header.variable_header[hi].name,
                    Any_String (event.header.variable_header[hi].value)));
    }
  CORBA::ULong flength = event.filterable_data.length ();
  for (CORBA::ULong i = 0; i < flength; i++)
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG,
                    "%s = %s\n",
                    (const char*)event.filterable_data[i].name,
                    Any_String (event.filterable_data[i].value)));
    }
  if (TAO_debug_level)
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
