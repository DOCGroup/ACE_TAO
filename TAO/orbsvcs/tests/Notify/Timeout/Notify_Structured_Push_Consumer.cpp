// $Id$

// ******************************************************************
// Include Section
// ******************************************************************

#include "ace/OS_NS_unistd.h"
#include "Notify_Structured_Push_Consumer.h"
#include "Notify_Test_Client.h"
#include "orbsvcs/Notify/Notify_Extensions.h"
#include "common.h"

// ******************************************************************
// Subroutine Section
// ******************************************************************

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


void
Notify_Structured_Push_Consumer::_connect (
                CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin)
{
  CosNotifyComm::StructuredPushConsumer_var objref = this->_this ();

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (
      CosNotifyChannelAdmin::STRUCTURED_EVENT,
      proxy_id_);

  this->proxy_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (
      proxysupplier.in ());

  this->proxy_->connect_structured_push_consumer (objref.in ());

  CosNotification::EventTypeSeq added (1);
  CosNotification::EventTypeSeq removed (1);
  added.length (1);
  removed.length (1);

  added[0].domain_name = CORBA::string_dup ("*");
  added[0].type_name = CORBA::string_dup ("examples");

  removed[0].domain_name = CORBA::string_dup ("*");
  removed[0].type_name = CORBA::string_dup ("*");

  this->subscription_change (added, removed);

  // give ownership to POA
  this->_remove_ref ();
}


void
Notify_Structured_Push_Consumer::push_structured_event (
  const CosNotification::StructuredEvent&)
{
  ACE_DEBUG ((LM_DEBUG, "-"));
  static const ACE_Time_Value sl (1, 0);

  this->count_++;
  if (this->count_ > this->expected_)
  {
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT ("Structured Consumer (%P|%t): ERROR: too ")
      ACE_TEXT ("many events received.\n")));
  }

  if (this->count_ >= this->expected_)
  {
    ACE_DEBUG ((LM_DEBUG, "\nConsumer received %u events.\n", count_));
    this->client_.consumer_done (this);
  }
  ACE_OS::sleep (sl);
}
