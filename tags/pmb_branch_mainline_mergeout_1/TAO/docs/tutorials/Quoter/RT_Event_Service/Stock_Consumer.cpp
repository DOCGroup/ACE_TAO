//
// $Id$
//

#include "Stock_Consumer.h"
#include "QuoterC.h"

Stock_Consumer::Stock_Consumer ()
{
}

void
Stock_Consumer::connect (RtecEventChannelAdmin::EventChannel_ptr event_channel,
                         const RtecEventChannelAdmin::ConsumerQOS &subscriptions)
{
  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    event_channel->for_consumers ();

  this->supplier_proxy_ =
    consumer_admin->obtain_push_supplier ();

  RtecEventComm::PushConsumer_var myself = this->_this ();
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
Stock_Consumer::push (const RtecEventComm::EventSet &data
                      ACE_ENV_ARG_DECL_NOT_USED)
  throw (CORBA::SystemException)
{
  for (CORBA::ULong i = 0; i != data.length (); ++i) {
    const RtecEventComm::Event &e = data[i];

    Quoter::Event *event;
    if ((e.data.any_value >>= event) == 0)
      continue; // Invalid event

    cout << "The new price for one stock in \""
         << event->full_name.in ()
         << "\" (" << event->symbol.in ()
         << ") is " << event->price << endl;
  }
}

void
Stock_Consumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    throw (CORBA::SystemException)
{
  this->supplier_proxy_ =
    RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
}
