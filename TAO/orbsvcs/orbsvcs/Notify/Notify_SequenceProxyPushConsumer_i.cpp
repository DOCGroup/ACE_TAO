/* -*- C++ -*- */
// $Id$

#include "Notify_SequenceProxyPushConsumer_i.h"
#include "Notify_Event_Manager.h"
#include "Notify_SupplierAdmin_i.h"

ACE_RCSID(Notify, Notify_SequenceProxyPushConsumer_i, "$Id$")

TAO_Notify_SequenceProxyPushConsumer_i::TAO_Notify_SequenceProxyPushConsumer_i (TAO_Notify_SupplierAdmin_i* supplieradmin, TAO_Notify_Resource_Manager* resource_manager)
  : sequence_proxy_inherited(supplieradmin, resource_manager)
{
}

TAO_Notify_SequenceProxyPushConsumer_i::~TAO_Notify_SequenceProxyPushConsumer_i (void)
{
}

void
TAO_Notify_SequenceProxyPushConsumer_i::cleanup_i (CORBA::Environment& ACE_TRY_ENV)
{
  sequence_proxy_inherited::cleanup_i (ACE_TRY_ENV);

  this->is_destroyed_ = 1;
}

void
TAO_Notify_SequenceProxyPushConsumer_i::connect_sequence_push_supplier (CosNotifyComm::SequencePushSupplier_ptr push_supplier, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventChannelAdmin::AlreadyConnected
                   ))
{
  // @@ Pradeep: here is another example on code that is not thread
  // safe, i know you are post-poning the thread safety issues, but it
  // is not that easy!

  if (this->is_connected_ == 1)
    ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());
  else
    this->push_supplier_ =
      CosNotifyComm::SequencePushSupplier::_duplicate (push_supplier);

  this->event_manager_->register_for_subscription_updates (this, ACE_TRY_ENV);
  ACE_CHECK;

  this->is_connected_ = 1;
}

void
TAO_Notify_SequenceProxyPushConsumer_i::dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed, CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      this->push_supplier_->subscription_change (added, removed,
                                                        ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
      // misbehaving client, ignore for now.
    }
  ACE_ENDTRY;
}

void
TAO_Notify_SequenceProxyPushConsumer_i::push_structured_events (const CosNotification::EventBatch & notifications, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
{
  if (this->is_connected_ == 0)
    ACE_THROW (CosEventComm::Disconnected ());

  for (CORBA::ULong i = 0; i < notifications.length (); ++i)
    {
      TAO_Notify_StructuredEvent notify_event (notifications[i]);

      CORBA::Boolean bval =
        this->check_filters_i (notify_event, ACE_TRY_ENV);
      ACE_CHECK;

      if (bval == 1)
        this->event_manager_->push (notify_event, ACE_TRY_ENV);
    }
}

void
TAO_Notify_SequenceProxyPushConsumer_i::disconnect_sequence_push_consumer (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->is_destroyed_ = 1;

  // ask our parent to deaactivate us.
  this->myadmin_->
    deactivate_proxy_pushconsumer (this, ACE_TRY_ENV);

  this->cleanup_i (ACE_TRY_ENV);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Notify_ProxyConsumer<POA_CosNotifyChannelAdmin::SequenceProxyPushConsumer>;
template class TAO_Notify_Proxy<POA_CosNotifyChannelAdmin::SequenceProxyPushConsumer>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Notify_ProxyConsumer<POA_CosNotifyChannelAdmin::SequenceProxyPushConsumer>
#pragma instantiate TAO_Notify_Proxy<POA_CosNotifyChannelAdmin::SequenceProxyPushConsumer>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
