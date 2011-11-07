// $Id$

#include "Consumer.h"

TAO_Notify_Lanes_Consumer::TAO_Notify_Lanes_Consumer (TAO_Notify_ORB_Objects& orb_objects)
  : orb_objects_ (orb_objects)
{
}

TAO_Notify_Lanes_Consumer::~TAO_Notify_Lanes_Consumer (void)
{
}

void
TAO_Notify_Lanes_Consumer::init (PortableServer::POA_var& poa, CosNotifyChannelAdmin::ConsumerAdmin_var& admin, ACE_CString& event_type)
{
  this->default_POA_ = poa;
  this->admin_ = admin;
  this->event_type_ = event_type;

  this->connect ();
}

PortableServer::POA_ptr
TAO_Notify_Lanes_Consumer::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_Notify_Lanes_Consumer::run (void)
{
  // Nothing to do.
}

void
TAO_Notify_Lanes_Consumer::connect (void)
{
  // Activate the consumer with the default_POA_
  CosNotifyComm::StructuredPushConsumer_var objref = this->_this ();

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    this->admin_->obtain_notification_push_supplier (CosNotifyChannelAdmin::STRUCTURED_EVENT
                                                     , proxy_supplier_id_);

  ACE_ASSERT (!CORBA::is_nil (proxysupplier.in ()));

  // narrow
  this->proxy_supplier_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (proxysupplier.in ());

  ACE_ASSERT (!CORBA::is_nil (proxy_supplier_.in ()));

  this->proxy_supplier_->connect_structured_push_consumer (objref.in ());

  // Call subscription_change to inform the supplier that this consumer is available.
  CosNotification::EventTypeSeq added (1);
  CosNotification::EventTypeSeq removed;

  added.length (1);
  added[0].domain_name = CORBA::string_dup ("TEST_DOMAIN");
  added[0].type_name = CORBA::string_dup (this->event_type_.c_str ());

  this->proxy_supplier_->subscription_change (added, removed);
}

void
TAO_Notify_Lanes_Consumer::disconnect (void)
{
  this->proxy_supplier_->disconnect_structured_push_supplier();
}

void
TAO_Notify_Lanes_Consumer::offer_change (const CosNotification::EventTypeSeq & /*added*/,
                               const CosNotification::EventTypeSeq & /*removed*/
                               )
{
  // No-Op.
}

void
TAO_Notify_Lanes_Consumer::push_structured_event (const CosNotification::StructuredEvent & notification)
{
  try
    {
      // Check the current threads priority.
      RTCORBA::Priority thread_priority =
        this->orb_objects_.current_->the_priority ();

      const CosNotification::PropertySeq& prop_seq = notification.header.variable_header;

      // Extract the priority at which the supplier send it.
      RTCORBA::Priority event_priority = 0;

      for (CORBA::ULong i = 0; i < prop_seq.length (); ++i)
        {
          if (ACE_OS::strcmp (prop_seq[i].name.in (), CosNotification::Priority) == 0)
            prop_seq[i].value >>= event_priority;
        }

      ACE_DEBUG ((LM_DEBUG, "(%P, %t) Consumer received event with priority = %d and thread priority = %d\n",
                  event_priority, thread_priority));

      // The current thread priority and the event priority must match.
      if (event_priority != thread_priority)
        ACE_DEBUG ((LM_DEBUG, "(%P, %t) Error: Event priority and thread priority are different.\n"));

      // Disconnect from the EC
      this->disconnect ();

      // Deactivate this object.
      this->deactivate ();

      // We received the event, shutdown the ORB.
      this->orb_objects_.orb_->shutdown (1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Consumer error "));

      return;
    }
}

void
TAO_Notify_Lanes_Consumer::deactivate (void)
{
  PortableServer::POA_var poa (this->_default_POA ());

  PortableServer::ObjectId_var id (poa->servant_to_id (this));

  poa->deactivate_object (id.in());
}

void
TAO_Notify_Lanes_Consumer::disconnect_structured_push_consumer (void)
{
  this->deactivate ();
}
