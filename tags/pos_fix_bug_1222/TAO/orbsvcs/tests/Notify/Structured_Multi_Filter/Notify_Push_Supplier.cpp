// $Id$
#include "Notify_Push_Supplier.h"

void
Notify_Push_Supplier::connect (
                CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin,
                CosNotifyChannelAdmin::EventChannel_ptr notify_channel,
                CORBA::Boolean isFilter
                ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosNotifyComm::StructuredPushSupplier_var objref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxyConsumer_var proxyconsumer =
    supplier_admin->obtain_notification_push_consumer (
      CosNotifyChannelAdmin::STRUCTURED_EVENT,
      proxy_consumer_id_
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  //add filters
  if (isFilter)
    {
      CosNotifyFilter::FilterFactory_var ffact =
        notify_channel->default_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      CosNotifyFilter::Filter_var filter =
        ffact->create_filter ("TCL" ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (CORBA::is_nil (filter.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      " (%P|%t) Consumer unable to initialize filter.\n"));
          exit (1);
        }

      CosNotifyFilter::ConstraintExpSeq constraint_list (1);
      constraint_list.length (1);

      constraint_list[0].event_types.length (0);
      constraint_list[0].constraint_expr = CORBA::string_dup ("String != 'One Hundred'");

      filter->add_constraints (constraint_list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // apply filter
      proxyconsumer->add_filter (filter.in ());
    }

  this->proxy_consumer_ =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (
      proxyconsumer.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->proxy_consumer_->connect_structured_push_supplier (objref.in ()
                                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // give ownership to POA
  this->_remove_ref ();
}


