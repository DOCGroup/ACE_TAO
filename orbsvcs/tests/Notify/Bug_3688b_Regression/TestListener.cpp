// $Id$
#include "TestListener.h"

TestListener::TestListener(
  CORBA::ORB * p_orb,
  std::string const & rc_channelIor,
  std::string const & rc_filterExpression
)
{
  CORBA::Object_var v_poa =
   p_orb->resolve_initial_references("RootPOA");

  mv_rootPOA = PortableServer::POA::_narrow (v_poa.in ());

  PortableServer::ServantBase * p_servant = this;
  mv_oid = mv_rootPOA->activate_object(p_servant);

  mv_this = mv_rootPOA->id_to_reference(mv_oid.in());

  CORBA::Object_var v_obj = p_orb->string_to_object(rc_channelIor.c_str());

  mv_eventChannel = CosNotifyChannelAdmin::EventChannel::_narrow(v_obj.in());

  mv_consumerAdmin = mv_eventChannel->new_for_consumers(
    CosNotifyChannelAdmin::AND_OP,
    m_consumerAdminId
  );

  CosNotifyFilter::FilterFactory_var v_filterFactory =
    mv_eventChannel->default_filter_factory();

  // setup a filter at the consumer admin
  CosNotifyFilter::Filter_var v_filter =
//          v_filterFactory->create_filter("ETCL");
    v_filterFactory->create_filter("EXTENDED_TCL");
//          v_filterFactory->create_filter("TCL");

  CosNotifyFilter::ConstraintExpSeq constraintSeq(1);
  constraintSeq.length(1);

  // Set up constraint expression for new filter
  constraintSeq[0].event_types.length(1);
  constraintSeq[0].event_types[0].domain_name = "Test";
  constraintSeq[0].event_types[0].type_name = "Data";
  constraintSeq[0].constraint_expr = rc_filterExpression.c_str();

  v_filter->add_constraints (constraintSeq);

  mv_consumerAdmin->add_filter(v_filter.in());

  v_obj = mv_rootPOA->id_to_reference(mv_oid.in());
  mv_structuredPushConsumer = CosNotifyComm::StructuredPushConsumer::_narrow(v_obj.in());

  CosNotifyChannelAdmin::ProxySupplier_var v_proxySupplier =
    mv_consumerAdmin->obtain_notification_push_supplier(
      CosNotifyChannelAdmin::STRUCTURED_EVENT,
      m_proxySupplierId
    );

  mv_proxySupplier =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow(
      v_proxySupplier.in()
    );

  mv_proxySupplier->connect_structured_push_consumer(mv_structuredPushConsumer.in());
}


TestListener::~TestListener()
{
  if(!CORBA::is_nil(mv_proxySupplier.in()))
  {
    // calls to disconnect_structured_push_supplier implicitely destroy
    // the proxy, so we must set the proxy reference as nil.
    mv_proxySupplier->disconnect_structured_push_supplier();
    mv_proxySupplier = CosNotifyChannelAdmin::StructuredProxyPushSupplier_var();
  }

  if(!CORBA::is_nil(mv_structuredPushConsumer.in()) && !CORBA::is_nil(mv_rootPOA.in()))
  {
    mv_rootPOA->deactivate_object(mv_oid.in());
  }
}


void TestListener::push_structured_event (
  const CosNotification::StructuredEvent & notification
)
{
  CORBA::Long data;
  notification.remainder_of_body >>= data;

  m_receivedData.push_back(data);
}


void TestListener::disconnect_structured_push_consumer ( )
{
}


void TestListener::offer_change(
  const CosNotification::EventTypeSeq & ,
  const CosNotification::EventTypeSeq &
)
{
}
