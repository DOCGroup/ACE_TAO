// $Id$
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
    CORBA::ORB * p_orb,
    std::string const & rc_channelIor,
    std::string const & rc_filterExpression = ""
  );

  ~TestListener();

  virtual void push_structured_event (
    const CosNotification::StructuredEvent & notification
  );

  virtual void disconnect_structured_push_consumer ( );

  virtual void offer_change(
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
  );

  PortableServer::POA_var mv_rootPOA;
  PortableServer::ObjectId_var mv_oid;
  CORBA::Object_var mv_this;
  CosNotifyChannelAdmin::EventChannel_var mv_eventChannel;
  CosNotifyChannelAdmin::ConsumerAdmin_var mv_consumerAdmin;
  CosNotifyComm::StructuredPushConsumer_var mv_structuredPushConsumer;
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var mv_proxySupplier;
  CosNotifyChannelAdmin::ProxyID m_proxySupplierId;
  CosNotifyChannelAdmin::AdminID m_consumerAdminId;

  std::vector<CORBA::Long> m_receivedData;
};


#endif /* testListener_h */
