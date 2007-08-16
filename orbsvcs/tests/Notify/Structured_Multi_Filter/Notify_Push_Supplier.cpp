// $Id$
#include "Notify_Push_Supplier.h"

void
Notify_Push_Supplier::_connect (
                CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin,
                CosNotifyChannelAdmin::EventChannel_ptr notify_channel,
                bool useFilter)
{
  CosNotifyComm::StructuredPushSupplier_var objref =
    this->_this ();

  CosNotifyChannelAdmin::ProxyConsumer_var proxyconsumer =
    supplier_admin->obtain_notification_push_consumer (
      CosNotifyChannelAdmin::STRUCTURED_EVENT,
      proxy_id_);

  if (useFilter)
    {
      CosNotifyFilter::FilterFactory_var ffact =
        notify_channel->default_filter_factory ();

      CosNotifyFilter::Filter_var filter =
        ffact->create_filter ("TCL");

      ACE_ASSERT(! CORBA::is_nil (filter.in ()));

      CosNotifyFilter::ConstraintExpSeq constraint_list (1);
      constraint_list.length (1);

      constraint_list[0].event_types.length (0);
      constraint_list[0].constraint_expr = CORBA::string_dup ("group != 0");

      filter->add_constraints (constraint_list);

      proxyconsumer->add_filter (filter.in ());
    }

  this->proxy_ =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (
      proxyconsumer.in ());

  this->proxy_->connect_structured_push_supplier (objref.in ());

  // give ownership to POA
  this->_remove_ref ();
}
