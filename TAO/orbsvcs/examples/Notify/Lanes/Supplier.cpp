#include "Supplier.h"

#include "tao/ORB_Core.h"

#include <string>

TAO_Notify_Lanes_Supplier::TAO_Notify_Lanes_Supplier (TAO_Notify_ORB_Objects& orb_objects)
  : orb_objects_ (orb_objects)
  , consumers_connected_ (lock_)
{
}

void
TAO_Notify_Lanes_Supplier::init (CosNotifyChannelAdmin::SupplierAdmin_ptr admin,
                                 int expected_consumer_count)
{
  // First initialize the class members.
  this->admin_ = admin;
  this->expected_consumer_count_ = expected_consumer_count;

  this->connect ();
}

void
TAO_Notify_Lanes_Supplier::run ()
{
  // The Priority at which we send the first event to the first consumer.
  RTCORBA::Priority priority {1};

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P, %t) Waiting for %d consumers to connect...\n"),
                this->expected_consumer_count_));

    // Wait till the consumers are ready to go.
    while (this->consumer_count_ != this->expected_consumer_count_)
      this->consumers_connected_.wait ();
  }

  // Send an event each to each consumer.
  // Each Consumer expects a different priority.
  for (int i {}; i < this->expected_consumer_count_; ++i, ++priority)
    {
      // Set this threads priority.
      this->orb_objects_.current_->the_priority (priority);

      // Make sure the priority was set, get the priority of the current thread.
      RTCORBA::Priority const thread_priority =
        this->orb_objects_.current_->the_priority ();

      // We will send this event.
      CosNotification::StructuredEvent event {};

      // Populate the Priority field so that the consumer can deduce the suppliers priority
      // to do a sanity check when it receives the event.
      CosNotification::PropertySeq& opt = event.header.variable_header;
      opt.length (1);

      CORBA::Any buffer {};
      buffer <<= static_cast<CORBA::Short> (thread_priority);

      opt[0].name = CORBA::string_dup (CosNotification::Priority);
      opt[0].value = buffer;

      // Set the domain and type nams in the event's fixed header.
      std::string const type {"TEST_TYPE_" + std::to_string (thread_priority)};

      event.header.fixed_header.event_type.domain_name = CORBA::string_dup ("TEST_DOMAIN");
      event.header.fixed_header.event_type.type_name = CORBA::string_dup (type.c_str ());

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P, %t) Supplier sends event type %C at priority %d\n"),
                  type.c_str (), thread_priority));

      // send the event
      this->send_event (event);
    } // repeat for the next consumer at the next priority.

  // Disconnect from the EC
  this->disconnect ();

  // Deactivate this object.
  this->deactivate ();

  // we're done. shutdown the ORB to exit the process.
  this->orb_objects_.orb_->shutdown (true);
}

void
TAO_Notify_Lanes_Supplier::connect ()
{
  // Activate the supplier object.
  CosNotifyComm::StructuredPushSupplier_var const objref = this->_this ();

  // Obtain the proxy.
  CosNotifyChannelAdmin::ProxyConsumer_var const proxyconsumer =
    this->admin_->obtain_notification_push_consumer (CosNotifyChannelAdmin::STRUCTURED_EVENT
                                                     , this->proxy_consumer_id_);

  ACE_ASSERT (!CORBA::is_nil (proxyconsumer.in ()));

  // narrow
  this->proxy_consumer_ =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (proxyconsumer.in ());

  ACE_ASSERT (!CORBA::is_nil (proxy_consumer_.in ()));

  // connect to the proxyconsumer.
  proxy_consumer_->connect_structured_push_supplier (objref.in ());
}

void
TAO_Notify_Lanes_Supplier::disconnect ()
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->disconnect_structured_push_consumer ();
}

void
TAO_Notify_Lanes_Supplier::deactivate ()
{
  PortableServer::POA_var const poa (this->_default_POA ());

  PortableServer::ObjectId_var const id (poa->servant_to_id (this));

  poa->deactivate_object (id.in ());
}

void
TAO_Notify_Lanes_Supplier::subscription_change (CosNotification::EventTypeSeq const& added,
                                                CosNotification::EventTypeSeq const& /*removed*/)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  // Count the number of consumers connect and signal the supplier thread when the expected count have connected.
  if (added.length () > 0)
    {
      if (++this->consumer_count_ == this->expected_consumer_count_)
        this->consumers_connected_.signal ();
    }
}

void
TAO_Notify_Lanes_Supplier::send_event (CosNotification::StructuredEvent const& event)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->push_structured_event (event);
}

void
TAO_Notify_Lanes_Supplier::disconnect_structured_push_supplier ()
{
  this->deactivate ();
}
