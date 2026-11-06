#ifndef testListener_h
#define testListener_h

#include "bug3688_export.h"

#include <string>
#include <vector>

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"


struct bug3688_Export TestListener
:
  public POA_CosNotifyComm::StructuredPushConsumer
{
  TestListener(
    CORBA::ORB* orb,
    std::string const& channel_ior,
    std::string const& filter_expression = "");

  TestListener (TestListener const&) = delete;
  TestListener (TestListener&&) = delete;
  TestListener& operator= (TestListener const&) = delete;
  TestListener& operator= (TestListener&&) = delete;
  ~TestListener () override;

  void push_structured_event (
    CosNotification::StructuredEvent const& notification) override;

  void disconnect_structured_push_consumer () override;

  void offer_change (
    CosNotification::EventTypeSeq const& added,
    CosNotification::EventTypeSeq const& removed) override;

  PortableServer::POA_var root_poa {};
  PortableServer::ObjectId_var object_id {};
  CORBA::Object_var object_reference {};
  CosNotifyChannelAdmin::EventChannel_var event_channel {};
  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin {};
  CosNotifyComm::StructuredPushConsumer_var structured_push_consumer {};
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier {};
  CosNotifyChannelAdmin::ProxyID proxy_supplier_id {};
  CosNotifyChannelAdmin::AdminID consumer_admin_id {};

  std::vector<CORBA::Long> received_data;
};


#endif /* testListener_h */
