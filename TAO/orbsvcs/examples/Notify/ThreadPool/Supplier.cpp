#include "Supplier.h"

TAO_Notify_ThreadPool_Supplier::TAO_Notify_ThreadPool_Supplier (
  TAO_Notify_ORB_Objects& orb_objects)
  : orb_objects_ (orb_objects)
  , consumers_connected_ (lock_)
{
}

void
TAO_Notify_ThreadPool_Supplier::init (
  CosNotifyChannelAdmin::SupplierAdmin_ptr admin,
  int expected_consumer_count,
  int max_events,
  int proxy_consumer_thread_count)
{
  // First initialize the class members.
  this->admin_ = admin;
  this->expected_consumer_count_ = expected_consumer_count;
  this->max_events_ = max_events;
  this->proxy_consumer_thread_count_ = proxy_consumer_thread_count;

  this->connect ();
}

void
TAO_Notify_ThreadPool_Supplier::run ()
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

    ACE_DEBUG ((LM_DEBUG,
                "(%P, %t) Waiting for %d consumers to connect...\n",
                this->expected_consumer_count_ - 1));

    // Wait till the consumers are ready to go.
    while (this->consumer_count_ != this->expected_consumer_count_)
      this->consumers_connected_.wait ();
  }

  ACE_DEBUG ((LM_DEBUG,
              "(%P, %t) Supplier is sending an events...\n"));

  // Send events to each consumer.
  for (int i {}; i < this->max_events_; ++i)
    {
      for (int j {}; j < this->expected_consumer_count_; ++j)
        {
          // send the event
          this->send_event (this->events_[j]);
        }
    }

  // Disconnect from the EC
  this->disconnect ();

  // Deactivate this object.
  this->deactivate ();

  // we're done. shutdown the ORB to exit the process.
  this->orb_objects_.orb_->shutdown (true);
}

void
TAO_Notify_ThreadPool_Supplier::connect ()
{
  // Activate the supplier object.
  CosNotifyComm::StructuredPushSupplier_var objref = this->_this ();

  CosNotifyChannelAdmin::ProxyConsumer_var proxyconsumer;

  if (this->proxy_consumer_thread_count_ != 0)
    {
      // Narrow to the extended interface.
      NotifyExt::SupplierAdmin_var admin_ext = NotifyExt::SupplierAdmin::_narrow (this->admin_.in ());

      NotifyExt::ThreadPoolParams tp_params {};
      tp_params.priority_model = NotifyExt::CLIENT_PROPAGATED;
      tp_params.static_threads =
        static_cast<CORBA::ULong> (this->proxy_consumer_thread_count_);

      CosNotification::QoSProperties qos (1);
      qos.length (1);
      qos[0].name = CORBA::string_dup (NotifyExt::ThreadPool);
      qos[0].value <<= tp_params;

      // Obtain the proxy. The QoS is applied to the POA in which the Proxy is hosted.
      proxyconsumer = admin_ext->obtain_notification_push_consumer_with_qos (
        CosNotifyChannelAdmin::STRUCTURED_EVENT,
        this->proxy_consumer_id_,
        qos);
    }
  else
    {
      // Obtain the proxy.
      proxyconsumer = this->admin_->obtain_notification_push_consumer (
        CosNotifyChannelAdmin::STRUCTURED_EVENT,
        this->proxy_consumer_id_);
    }

  ACE_ASSERT (!CORBA::is_nil (proxyconsumer.in ()));

  // narrow
  this->proxy_consumer_ =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (proxyconsumer.in ());

  ACE_ASSERT (!CORBA::is_nil (proxy_consumer_.in ()));

  // connect to the proxyconsumer.
  this->proxy_consumer_->connect_structured_push_supplier (objref.in ());

  ACE_DEBUG ((LM_DEBUG,
              "(%P,%t) Created Supplier %d with %d threads at the ProxyConsumer\n",
              this->proxy_consumer_id_,
              this->proxy_consumer_thread_count_));
}

void
TAO_Notify_ThreadPool_Supplier::disconnect ()
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->disconnect_structured_push_consumer ();
}

void
TAO_Notify_ThreadPool_Supplier::deactivate ()
{
  PortableServer::POA_var poa (this->_default_POA ());

  PortableServer::ObjectId_var id (poa->servant_to_id (this));

  poa->deactivate_object (id.in ());
}

void
TAO_Notify_ThreadPool_Supplier::subscription_change (
  CosNotification::EventTypeSeq const& added,
  CosNotification::EventTypeSeq const& /*removed*/)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  // Count connected consumers and signal the supplier thread when all are ready.
  // Only 1 consumer connects at a time.
  if (added.length () > 0 && this->consumer_count_ < this->expected_consumer_count_)
    {
      // Set the domain and type names in the event's fixed header.
      this->events_[this->consumer_count_].header.fixed_header.event_type.domain_name =
        CORBA::string_dup (added[0].domain_name.in ());
      this->events_[this->consumer_count_].header.fixed_header.event_type.type_name =
        CORBA::string_dup (added[0].type_name.in ());

      ++this->consumer_count_;

      ACE_DEBUG ((LM_DEBUG,
                  "(%P,%t) Received Type %d: (%s)\n",
                  this->consumer_count_,
                  added[0].type_name.in ()));

      if (this->consumer_count_ == this->expected_consumer_count_)
        this->consumers_connected_.signal ();
    }
}

void
TAO_Notify_ThreadPool_Supplier::send_event (CosNotification::StructuredEvent const& event)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->push_structured_event (event);
}

void
TAO_Notify_ThreadPool_Supplier::disconnect_structured_push_supplier ()
{
  this->deactivate ();
}
