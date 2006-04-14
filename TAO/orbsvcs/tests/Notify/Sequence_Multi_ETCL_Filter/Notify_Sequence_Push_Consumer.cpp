// $Id$

#include "ace/OS_NS_unistd.h"
#include "Notify_Sequence_Push_Consumer.h"
#include "Notify_Test_Client.h"
#include "orbsvcs/TimeBaseC.h"
#include "common.h"
#include "tao/debug.h"

Notify_Sequence_Push_Consumer::Notify_Sequence_Push_Consumer (
                                            const char* name,
                                            unsigned int low,
                                            unsigned int high,
                                            Notify_Test_Client& client)
 : name_ (name),
   low_ (low),
   high_ (high),
   count_ (0),
   client_ (client)
{
  this->client_.consumer_start (this);
}


void
Notify_Sequence_Push_Consumer::_connect (
                CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin
                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosNotifyComm::SequencePushConsumer_var objref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (
      CosNotifyChannelAdmin::SEQUENCE_EVENT,
      proxy_id_
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->proxy_ =
    CosNotifyChannelAdmin::SequenceProxyPushSupplier::_narrow (
      proxysupplier.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CosNotification::QoSProperties properties (3); //(4);
  properties.length (3); //(4);
  properties[0].name = CORBA::string_dup (CosNotification::MaximumBatchSize);
  properties[0].value <<= (CORBA::Long)5;
  properties[1].name = CORBA::string_dup (CosNotification::PacingInterval);
  properties[1].value <<= (TimeBase::TimeT)4;
  //properties[3].name = CORBA::string_dup (CosNotification::MaxEventsPerConsumer);
  //properties[3].value <<= (CORBA::Long)2;

  this->proxy_->set_qos (properties);
  this->proxy_->connect_sequence_push_consumer (objref.in ()
                                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // give ownership to POA
  this->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}


void
Notify_Sequence_Push_Consumer::push_structured_events (
                          const CosNotification::EventBatch& events
                          ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG((LM_DEBUG, "-"));

  CORBA::ULong length = events.length ();

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Received %u events:\n", length));

  for (CORBA::ULong e = 0; e < length; e++)
    {
      CORBA::ULong hlength = events[e].header.variable_header.length ();
      for (CORBA::ULong hi = 0; hi < hlength; hi++)
        {
          if (TAO_debug_level)
            ACE_DEBUG ((LM_DEBUG,
                        "%s = %s\n",
                        (const char*)events[e].header.variable_header[hi].name,
                        Any_String (events[e].header.variable_header[hi].value)));
        }

      CORBA::ULong flength = events[e].filterable_data.length ();
      for (CORBA::ULong i = 0; i < flength; i++)
        {
          if (TAO_debug_level)
            ACE_DEBUG ((LM_DEBUG,
                        "%s = %s\n",
                        (const char*)events[e].filterable_data[i].name,
                        Any_String (events[e].filterable_data[i].value)));
        }
    }

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
                "-------------------------\n"));

  this->count_++;
  if (this->count_ > this->high_)
    {
      this->client_.consumer_done (this);
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Sequence Consumer (%P|%t): ERROR: too ")
                  ACE_TEXT ("many events received.\n")));
      ACE_THROW (CORBA::INTERNAL ());
    }
  else if (this->count_ == this->low_)
    {
      this->client_.consumer_done (this);
    }
  else
    {
      ACE_OS::sleep (1);
    }
}
