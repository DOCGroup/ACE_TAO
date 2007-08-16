// $Id$

#include "Notify_Structured_Push_Consumer.h"
#include "Notify_Test_Client.h"
#include "common.h"

#include "ace/OS_NS_sys_time.h"

Notify_Structured_Push_Consumer::Notify_Structured_Push_Consumer (
                                            const char* name,
                                            unsigned int expected,
                                            Notify_Test_Client& client)
 : name_ (name),
   expected_ (expected),
   count_ (0),
   client_ (client)
{
  this->client_.consumer_start (this);
}


Notify_Structured_Push_Consumer::~Notify_Structured_Push_Consumer ()
{
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

  proxy_->connect_structured_push_consumer (objref.in ());

  this->_remove_ref ();
}


void
Notify_Structured_Push_Consumer::push_structured_event(
                          const CosNotification::StructuredEvent& /*event*/)
{
  if ( this->count_ == 0 )
  {
      this->first_ = ACE_OS::gettimeofday();
  }

  if (this->count_ == this->expected_)
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: too many events received.\n")));
  }

  //ACE_DEBUG((LM_DEBUG, "-"));

  this->count_++;

  if (this->count_ == this->expected_)
    {
      this->client_.consumer_done (this);
      ACE_Time_Value totaltime = ( ACE_OS::gettimeofday() - this->first_ );
      double events_per_ms = static_cast<double>(this->count_) /
        static_cast<double>(totaltime.msec());
      ACE_DEBUG((LM_DEBUG, "Consumer %s received %d at %.2f events/ms\n", name_.c_str(), this->count_, events_per_ms ));
    }
}
