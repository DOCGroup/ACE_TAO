
#include "Stock_Consumer.h"
#include "QuoterC.h"

#include <iostream>

void
Stock_Consumer::connect (RtecEventChannelAdmin::EventChannel_ptr event_channel,
                         RtecEventChannelAdmin::ConsumerQOS const &subscriptions)
{
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin (
    event_channel->for_consumers ());

  this->supplier_proxy_ =
    consumer_admin->obtain_push_supplier ();

  RtecEventComm::PushConsumer_var myself (this->_this ());
  this->supplier_proxy_->connect_push_consumer (myself.in (),
                                                subscriptions);
}

void
Stock_Consumer::disconnect ()
{
  // Do not receive any more events...
  this->supplier_proxy_->disconnect_push_supplier ();
}

void
Stock_Consumer::push (RtecEventComm::EventSet const &data)
{
  for (CORBA::ULong i = 0; i != data.length (); ++i)
    {
      RtecEventComm::Event const &e = data[i];

      Quoter::Event const *event {};
      if ((e.data.any_value >>= event) == 0)
        continue; // Invalid event

      std::cout << "The new price for one stock in \""
                << event->full_name.in ()
                << "\" (" << event->symbol.in ()
                << ") is " << event->price << std::endl;
    }
}

void
Stock_Consumer::disconnect_push_consumer ()
{
  this->supplier_proxy_ = RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
}
