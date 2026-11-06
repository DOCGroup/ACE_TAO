#include "Notify_Structured_Push_Consumer.h"
#include "Notify_Test_Client.h"
#include "common.h"

#include "orbsvcs/TimeBaseC.h"

#include "tao/debug.h"

#include "ace/OS_NS_unistd.h"

Notify_Structured_Push_Consumer::Notify_Structured_Push_Consumer (
                                            char const* name,
                                            CORBA::Short policy,
                                            bool use_ordering,
                                            int expected,
                                            Notify_Test_Client& client)
 : name_ (name),
   order_policy_ (policy),
   use_ordering_ (use_ordering),
   expected_ (expected),
   client_ (client)
{
  this->client_.consumer_start (this);
}

void
Notify_Structured_Push_Consumer::_connect (
                CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin)
{
  CosNotifyComm::StructuredPushConsumer_var objref =
    this->_this ();

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (
      CosNotifyChannelAdmin::STRUCTURED_EVENT,
      proxy_id_);

  this->proxy_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (
      proxysupplier.in ());

  if (this->use_ordering_)
    {
      CosNotification::QoSProperties properties (1);
      properties.length (1);
      properties[0].name = CORBA::string_dup (CosNotification::OrderPolicy);
      properties[0].value <<= this->order_policy_;

      this->proxy_->set_qos (properties);
    }

  this->proxy_->connect_structured_push_consumer (objref.in ());

  // give ownership to POA
  this->_remove_ref ();
}

void
Notify_Structured_Push_Consumer::push_structured_event (
                          CosNotification::StructuredEvent const& event)
{
  ACE_DEBUG ((LM_DEBUG, "-"));
  if (this->count_ == 0)
    {
      // Sleep long enough to force the channel to back up, otherwise
      // there will be no ordering.
      ACE_OS::sleep (2);
    }

  ++count_;

  if (this->count_ > this->expected_)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: too many events received.\n")));
  }

  if (this->count_ >= this->expected_)
  {
    this->client_.consumer_done (this);
  }

  CORBA::Long id {};

  ACE_ASSERT (event.header.variable_header.length () == 3);
  ACE_ASSERT (ACE_OS::strcmp (event.header.variable_header[0].name.in (), "id") == 0);
  event.header.variable_header[0].value >>= id;

  // The first event won't necessarilly be in order, because we hadn't yet forced
  // the channel to queue events.
  if (this->count_ > 1)
    {
      if (this->order_policy_ == CosNotification::PriorityOrder
          || this->order_policy_ == CosNotification::DeadlineOrder)
        {
          int const expected_id = this->expected_ - this->count_ + 1;
          int const adjusted_expected_id = expected_id <= this->first_ ? expected_id - 1 : expected_id;

          if (id != adjusted_expected_id)
            ACE_ERROR ((LM_ERROR, "\nError: "));
          ACE_DEBUG ((LM_DEBUG, "Expected id:%d Received id:%d\n", adjusted_expected_id, id));
        }
      else if (id != this->count_ - 1)
        ACE_ERROR ((LM_ERROR,
                    "\nError: Expected id:%d Received id:%d\n",
                    this->count_ - 1, id));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "Ignoring first event. id=%d\n", id));
      this->first_ = id;
    }
}
