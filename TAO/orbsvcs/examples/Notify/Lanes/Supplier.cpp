// $Id$

#include "Supplier.h"

#if ! defined (__ACE_INLINE__)
#include "Supplier.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Supplier, "$id$")

#include "tao/ORB_Core.h"

TAO_NS_Supplier::TAO_NS_Supplier  (TAO_NS_ORB_Objects& orb_objects)
  : orb_objects_ (orb_objects)
  , proxy_consumer_id_ (0)
  , expected_consumer_count_ (2)
  , consumers_connected_ (lock_)
  , consumer_count_ (0)
{
}

TAO_NS_Supplier::~TAO_NS_Supplier ()
{
}

void
TAO_NS_Supplier::init (CosNotifyChannelAdmin::SupplierAdmin_var& admin, int expected_consumer_count ACE_ENV_ARG_DECL)
{
  // First initialize the class members.
  this->admin_ = admin;
  this->expected_consumer_count_ = expected_consumer_count;

  this->connect (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_NS_Supplier::run (ACE_ENV_SINGLE_ARG_DECL)
{
  // The Priority at which we send the first event to the first consumer.
  RTCORBA::Priority priority = 1;

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

    ACE_DEBUG ((LM_DEBUG, "(%P, %t) Waiting for %d consumers to connect...\n", this->expected_consumer_count_));

    // Wait till the consumers are ready to go.
    while (this->consumer_count_ != this->expected_consumer_count_)
      this->consumers_connected_.wait ();
  }

  // Send an event each to each consumer.
  // Each Consumer expects a different priority.
  for (int i = 0; i < this->expected_consumer_count_; ++i, ++priority)
    {
      // Set this threads priority.
      this->orb_objects_.current_->the_priority (priority ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Make sure the priority was set, get the priority of the current thread.
      RTCORBA::Priority thread_priority =
        this->orb_objects_.current_->the_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      // We will send this event.
      CosNotification::StructuredEvent event;

      // Populate the Priority field so that the consumer can deduce the suppliers priority
      // to do a sanity check when it receives the event.
      CosNotification::PropertySeq& opt =  event.header.variable_header;
      opt.length (1);

      CORBA::Any buffer;
      buffer <<= (CORBA::Short) thread_priority;

      opt[0].name = CORBA::string_dup (CosNotification::Priority);
      opt[0].value = buffer;

      // Set the domain and type nams in the event's fixed header.
      char type[BUFSIZ];
      ACE_OS::sprintf (type, "TEST_TYPE_%d", thread_priority);

      event.header.fixed_header.event_type.domain_name = CORBA::string_dup("TEST_DOMAIN");
      event.header.fixed_header.event_type.type_name = CORBA::string_dup(type);

      ACE_DEBUG ((LM_DEBUG,
                  "(%P, %t) Supplier is sending an event of type %s at priority %d\n", type, thread_priority));

      // send the event
      this->send_event (event ACE_ENV_ARG_PARAMETER);
    } // repeat for the next consumer at the next priority.

  // Disconnect from the EC
  this->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);

  // Deactivate this object.
  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);

  // we're done. shutdown the ORB to exit the process.
  this->orb_objects_.orb_->shutdown (1);
}

void
TAO_NS_Supplier::connect (ACE_ENV_SINGLE_ARG_DECL)
{
  // Activate the supplier object.
  CosNotifyComm::StructuredPushSupplier_var objref = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Obtain the proxy.
  CosNotifyChannelAdmin::ProxyConsumer_var proxyconsumer =
    this->admin_->obtain_notification_push_consumer (CosNotifyChannelAdmin::STRUCTURED_EVENT
                                                     , proxy_consumer_id_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxyconsumer.in ()));

  // narrow
  this->proxy_consumer_ =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (proxyconsumer.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_consumer_.in ()));

  // connect to the proxyconsumer.
  proxy_consumer_->connect_structured_push_supplier (objref.in ()
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Supplier::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->disconnect_structured_push_consumer(ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_NS_Supplier::deactivate (ACE_ENV_SINGLE_ARG_DECL)
{
  PortableServer::POA_var poa (this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER));
  ACE_CHECK;

  PortableServer::ObjectId_var id (poa->servant_to_id (this
                                                       ACE_ENV_ARG_PARAMETER));
  ACE_CHECK;

  poa->deactivate_object (id.in()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Supplier::subscription_change (const CosNotification::EventTypeSeq & added,
                                      const CosNotification::EventTypeSeq & /*removed */
                                      ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
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
TAO_NS_Supplier::send_event (const CosNotification::StructuredEvent& event ACE_ENV_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  proxy_consumer_->push_structured_event (event ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Supplier::disconnect_structured_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}
