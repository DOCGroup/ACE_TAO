// $Id$

#include "Notify_Sequence_Push_Consumer.h"
#include "orbsvcs/TimeBaseC.h"
#include "common.h"

Notify_Sequence_Push_Consumer::Notify_Sequence_Push_Consumer (
                                            const char* name,
                                            unsigned int low,
                                            unsigned int high,
                                            CORBA::Boolean& done)
 : name_ (name),
   low_ (low),
   high_ (high),
   count_ (0),
   done_ (done)
{
}


void
Notify_Sequence_Push_Consumer::connect (
                CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin
                TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosNotifyComm::SequencePushConsumer_var objref =
    this->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (
      CosNotifyChannelAdmin::SEQUENCE_EVENT,
      proxy_supplier_id_
      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->proxy_supplier_ =
    CosNotifyChannelAdmin::SequenceProxyPushSupplier::_narrow (
      proxysupplier.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CosNotification::QoSProperties properties (4);
  properties.length (4);
  properties[0].name = CORBA::string_dup (CosNotification::MaximumBatchSize);
  properties[0].value <<= (CORBA::Long)5;
  properties[1].name = CORBA::string_dup (CosNotification::PacingInterval);
  properties[1].value <<= (TimeBase::TimeT)4;
  properties[3].name = CORBA::string_dup (CosNotification::MaxEventsPerConsumer);
  properties[3].value <<= (CORBA::Long)2;

  this->proxy_supplier_->set_qos (properties);
  this->proxy_supplier_->connect_sequence_push_consumer (objref.in ()
                                                         TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // give ownership to POA
  this->_remove_ref ();
}


void
Notify_Sequence_Push_Consumer::push_structured_events (
                          const CosNotification::EventBatch& events
                          TAO_ENV_ARG_DECL_NOT_USED /*TAO_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::ULong length = events.length ();

  ACE_DEBUG ((LM_DEBUG, "Received %u events:\n", length));

  for (CORBA::ULong e = 0; e < length; e++)
    {
      CORBA::ULong hlength = events[e].header.variable_header.length ();
      for (CORBA::ULong hi = 0; hi < hlength; hi++)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%s = %s\n",
                      (const char*)events[e].header.variable_header[hi].name,
                      Any_String (events[e].header.variable_header[hi].value)));
        }

      CORBA::ULong flength = events[e].filterable_data.length ();
      for (CORBA::ULong i = 0; i < flength; i++)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "%s = %s\n",
                      (const char*)events[e].filterable_data[i].name,
                      Any_String (events[e].filterable_data[i].value)));
        }
    }
  ACE_DEBUG ((LM_DEBUG,
              "-------------------------\n"));
  this->count_++;
  if (this->count_ > this->high_)
    {
      this->done_ = 2;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Sequence Consumer (%P|%t): ERROR: too "
                            "many events received.\n")));
    }
  else if (this->count_ == this->low_)
    {
      this->done_ = 1;
    }
  else
    {
      ACE_OS::sleep (1);
    }
}


