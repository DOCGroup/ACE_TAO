// $Id$

#include "Consumer.h"

ACE_RCSID(Notify, TAO_Notify_Lanes_Consumer, "$id$")

TAO_Notify_Lanes_Consumer::TAO_Notify_Lanes_Consumer (TAO_Notify_ORB_Objects& orb_objects)
  : orb_objects_ (orb_objects)
{
}

TAO_Notify_Lanes_Consumer::~TAO_Notify_Lanes_Consumer (void)
{
}

void
TAO_Notify_Lanes_Consumer::init (PortableServer::POA_var& poa, CosNotifyChannelAdmin::ConsumerAdmin_var& admin, ACE_CString& event_type ACE_ENV_ARG_DECL)
{
  this->default_POA_ = poa;
  this->admin_ = admin;
  this->event_type_ = event_type;

  this->connect (ACE_ENV_SINGLE_ARG_PARAMETER);
}

PortableServer::POA_ptr
TAO_Notify_Lanes_Consumer::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_Notify_Lanes_Consumer::run (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  // Nothing to do.
}

void
TAO_Notify_Lanes_Consumer::connect (ACE_ENV_SINGLE_ARG_DECL)
{
  // Activate the consumer with the default_POA_
  CosNotifyComm::StructuredPushConsumer_var objref = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    this->admin_->obtain_notification_push_supplier (CosNotifyChannelAdmin::STRUCTURED_EVENT
                                                     , proxy_supplier_id_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxysupplier.in ()));

  // narrow
  this->proxy_supplier_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (proxysupplier.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_supplier_.in ()));

  this->proxy_supplier_->connect_structured_push_consumer (objref.in ()
                                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Call subscription_change to inform the supplier that this consumer is available.
  CosNotification::EventTypeSeq added (1);
  CosNotification::EventTypeSeq removed;

  added.length (1);
  added[0].domain_name = CORBA::string_dup ("TEST_DOMAIN");
  added[0].type_name = CORBA::string_dup (this->event_type_.c_str ());

  this->proxy_supplier_->subscription_change (added, removed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_Lanes_Consumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  this->proxy_supplier_->disconnect_structured_push_supplier(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_Lanes_Consumer::offer_change (const CosNotification::EventTypeSeq & /*added*/,
                               const CosNotification::EventTypeSeq & /*removed*/
                               ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  // No-Op.
}

void
TAO_Notify_Lanes_Consumer::push_structured_event (const CosNotification::StructuredEvent & notification
                                        ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
{
  ACE_TRY_NEW_ENV
    {
      // Check the current threads priority.
      RTCORBA::Priority thread_priority =
        this->orb_objects_.current_->the_priority (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

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
        ACE_DEBUG ((LM_DEBUG, "(%P, %t) Error: Event priority and thread priority are different. \n"));

      // Disconnect from the EC
      this->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);

      // Deactivate this object.
      this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);

      // We received the event, shutdown the ORB.
      this->orb_objects_.orb_->shutdown (1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION,
                          ACE_TEXT ("Consumer error "));

      return;
    }
  ACE_ENDTRY;
}

void
TAO_Notify_Lanes_Consumer::deactivate (ACE_ENV_SINGLE_ARG_DECL)
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
TAO_Notify_Lanes_Consumer::disconnect_structured_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
}
