// $Id$

#include "Notify_Sequence_Push_Consumer.h"
#include "orbsvcs/TimeBaseC.h"
#include "common.h"
#include "tao/debug.h"

Notify_Sequence_Push_Consumer::Notify_Sequence_Push_Consumer (
                                            const char* name,
                                            CORBA::Short policy,
                                            unsigned int expected,
                                            CORBA::Boolean& done)
 : name_ (name),
   order_policy_ (policy),
   expected_ (expected),
   count_ (0),
   done_ (done)
{
}


void
Notify_Sequence_Push_Consumer::connect (
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

  CosNotification::QoSProperties properties (3);
  properties.length (3);
  properties[0].name = CORBA::string_dup (CosNotification::MaximumBatchSize);
  properties[0].value <<= (CORBA::Long)5;
  properties[1].name = CORBA::string_dup (CosNotification::PacingInterval);
  properties[1].value <<= (TimeBase::TimeT)4;
  properties[2].name = CORBA::string_dup (CosNotification::OrderPolicy);
  properties[2].value <<= this->order_policy_;

  this->proxy_->set_qos (properties);
  this->proxy_->connect_sequence_push_consumer (objref.in ()
                                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // give ownership to POA
  this->_remove_ref ();
}


void
Notify_Sequence_Push_Consumer::push_structured_events (
                          const CosNotification::EventBatch& events
                          ACE_ENV_ARG_DECL_NOT_USED /*ACE_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  static long previous = 0;
  static CORBA::Boolean first = 1;

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

          if (this->order_policy_ == CosNotification::PriorityOrder)
            {
              if (ACE_OS::strcmp (
                                  events[e].header.variable_header[hi].name, "Priority") == 0)
                {
                  CORBA::Short current;
                  events[e].header.variable_header[hi].value >>= current;
                  if (first)
                    {
                      first = 0;
                    }
                  else
                    {
                      if (current >
                          ACE_static_cast (CORBA::Short, previous))
                        {
                          this->done_ = 1;
                          ACE_ERROR ((LM_ERROR,
                                      ACE_TEXT ("ERROR: Priority Ordering failed\n")));
                        }
                    }
                  previous = ACE_static_cast (long, current);
                }
            }
          else if (this->order_policy_ == CosNotification::DeadlineOrder)
            {
              if (ACE_OS::strcmp (
                                  events[e].header.variable_header[hi].name, "Timeout") == 0)
                {
                  TimeBase::TimeT current;
                  events[e].header.variable_header[hi].value >>= current;
                  if (first)
                    {
                      first = 0;
                    }
                  else
                    {
                      if (current <
                          ACE_static_cast (TimeBase::TimeT, previous))
                        {
                          this->done_ = 1;
                          ACE_ERROR ((LM_ERROR,
                                      ACE_TEXT ("ERROR: Deadline Ordering failed\n")));
                        }
                    }
# if defined (ACE_CONFIG_WIN32_H)
                  previous = ACE_static_cast (long, current);
# else
                  // Convert ACE_ULong_Long to 32-bit integer
                  previous = (current / 1);
# endif /* ACE_CONFIG_WIN32_H */
                }
            }
        }

      CORBA::ULong flength = events[e].filterable_data.length ();
      for (CORBA::ULong i = 0; i < flength; i++)
        {
          if (TAO_debug_level)
            ACE_DEBUG ((LM_DEBUG,
                        "%s = %s\n",
                        (const char*)events[e].filterable_data[i].name,
                        Any_String (events[e].filterable_data[i].value)));

          if (this->order_policy_ == CosNotification::FifoOrder)
            {
              if (ACE_OS::strcmp (events[e].filterable_data[i].name, "enum") == 0)
                {
                  CORBA::ULong current;
                  events[e].filterable_data[i].value >>= current;
                  if (first)
                    {
                      first = 0;
                    }
                  else
                    {
                      if (current <
                          ACE_static_cast (CORBA::ULong, previous))
                        {
                          this->done_ = 1;
                          ACE_ERROR ((LM_ERROR,
                                      ACE_TEXT ("ERROR: FIFO Ordering failed.\n")));
                        }
                    }
                  previous = ACE_static_cast (long, current);
                }
            }
        }
    }

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
                "-------------------------\n"));

  this->count_+= events.length ();

  if (this->count_ > this->expected_)
    {
      this->done_ = 1;

      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Sequence Consumer (%P|%t): ERROR: too "
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
