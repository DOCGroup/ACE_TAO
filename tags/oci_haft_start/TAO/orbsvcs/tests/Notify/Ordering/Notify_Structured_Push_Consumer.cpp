// $Id$

#include "Notify_Structured_Push_Consumer.h"
#include "orbsvcs/TimeBaseC.h"
#include "common.h"
#include "tao/debug.h"

Notify_Structured_Push_Consumer::Notify_Structured_Push_Consumer (
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
Notify_Structured_Push_Consumer::_connect (
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

  CosNotification::QoSProperties properties (1);
  properties.length (1);
  properties[0].name = CORBA::string_dup (CosNotification::OrderPolicy);
  properties[0].value <<= this->order_policy_;

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
  static long previous = 0;
  static CORBA::Boolean first = 1;
  // Since the Notification Service begins to dispatch before all the
  // events are queued, the first 2 events are not in the "expected"
  // order.
  static const unsigned int expected_out_of_order = 2;

  if (this->count_ >= expected_out_of_order)
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

          if (this->order_policy_ == CosNotification::PriorityOrder)
            {
              if (ACE_OS::strcmp (
                      event.header.variable_header[hi].name, "Priority") == 0)
                {
                  CORBA::Short current;
                  event.header.variable_header[hi].value >>= current;
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
                                      ACE_TEXT ("ERROR: Priority Ordering failed.\n")));
                        }
                    }
                  previous = ACE_static_cast (long, current);
                }
            }
          else if (this->order_policy_ == CosNotification::DeadlineOrder)
            {
              if (ACE_OS::strcmp (
                      event.header.variable_header[hi].name, "Timeout") == 0)
                {
                  TimeBase::TimeT current;
                  event.header.variable_header[hi].value >>= current;
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
                                      ACE_TEXT ("ERROR: Deadline Ordering failed.\n")));
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

      CORBA::ULong flength = event.filterable_data.length ();
      for (CORBA::ULong i = 0; i < flength; i++)
        {
          if (TAO_debug_level)
            ACE_DEBUG ((LM_DEBUG,
                        "%s = %s\n",
                        (const char*)event.filterable_data[i].name,
                        Any_String (event.filterable_data[i].value)));

          if (this->order_policy_ == CosNotification::FifoOrder)
            {
              if (ACE_OS::strcmp (event.filterable_data[i].name, "enum") == 0)
                {
                  CORBA::ULong current;
                  event.filterable_data[i].value >>= current;
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
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG,
                    "-------------------------\n"));
    }
  this->count_++;
  if (this->count_ > this->expected_)
    {
      this->done_ = 1;
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ERROR: too "
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
