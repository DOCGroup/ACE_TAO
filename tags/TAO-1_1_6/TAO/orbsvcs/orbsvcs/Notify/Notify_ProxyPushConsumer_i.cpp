// $Id$

#include "Notify_ProxyPushConsumer_i.h"
#include "Notify_Event_Manager.h"
#include "Notify_SupplierAdmin_i.h"

ACE_RCSID(Notify, Notify_ProxyPushConsumer_i, "$Id$")

// Implementation skeleton constructor
TAO_Notify_ProxyPushConsumer_i::TAO_Notify_ProxyPushConsumer_i (TAO_Notify_SupplierAdmin_i* supplieradmin, TAO_Notify_Resource_Manager* resource_manager)
  : proxy_inherited (supplieradmin, resource_manager),
    notify_style_supplier_ (0)
{
}

// Implementation skeleton destructor
TAO_Notify_ProxyPushConsumer_i::~TAO_Notify_ProxyPushConsumer_i (void)
{
}

void
TAO_Notify_ProxyPushConsumer_i::cleanup_i (CORBA::Environment& ACE_TRY_ENV)
{
  proxy_inherited::cleanup_i (ACE_TRY_ENV);
  this->cosec_push_supplier_ = CosEventComm::PushSupplier::_nil ();
  this->notify_push_supplier_ = CosNotifyComm::PushSupplier::_nil ();
}

void
TAO_Notify_ProxyPushConsumer_i::connect_any_push_supplier (CosEventComm::PushSupplier_ptr push_supplier, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventChannelAdmin::AlreadyConnected
                   ))
{
  if (this->is_connected_ == 1)
    ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());
  else
    {
      this->cosec_push_supplier_ =
        CosEventComm::PushSupplier::_duplicate (push_supplier);

      this->notify_push_supplier_ =
        CosNotifyComm::PushSupplier::_narrow (push_supplier, ACE_TRY_ENV);
      ACE_CHECK;

      if (!CORBA::is_nil (this->notify_push_supplier_.in ()))
        {
          this->cosec_push_supplier_ = CosNotifyComm::PushSupplier::_nil ();
          this->notify_style_supplier_ = 1;
        }

      this->event_manager_->register_for_subscription_updates (this,
                                                               ACE_TRY_ENV);

      this->is_connected_ = 1;
    }
}

void
TAO_Notify_ProxyPushConsumer_i::dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed, CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      this->notify_push_supplier_->subscription_change (added, removed,
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
TAO_Notify_ProxyPushConsumer_i::push (const CORBA::Any & data, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
{
  if (this->is_connected_ == 0)
    ACE_THROW (CosEventComm::Disconnected ());

  TAO_Notify_Any notify_event (data);

  CORBA::Boolean bval = this->check_filters_i (notify_event, ACE_TRY_ENV);
  ACE_CHECK;

  if (bval == 1)
    this->event_manager_->push (notify_event, ACE_TRY_ENV);
}

void TAO_Notify_ProxyPushConsumer_i::disconnect_push_consumer (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  this->is_destroyed_ = 1;

  // ask our parent to deactivate us.
  this->myadmin_->
    deactivate_proxy_pushconsumer (this, ACE_TRY_ENV);

  this->cleanup_i (ACE_TRY_ENV);
}

// = TAO_Notify_CosEC_ProxyPushConsumer_i

TAO_Notify_CosEC_ProxyPushConsumer_i::TAO_Notify_CosEC_ProxyPushConsumer_i (TAO_Notify_SupplierAdmin_i* supplieradmin, TAO_Notify_Resource_Manager* resource_manager)
  :notify_proxy_ (supplieradmin, resource_manager)
{
  // No-Op.
}

TAO_Notify_CosEC_ProxyPushConsumer_i::~TAO_Notify_CosEC_ProxyPushConsumer_i (void)
{
  // No-Op.
}

void
TAO_Notify_CosEC_ProxyPushConsumer_i::init (CORBA::Environment &ACE_TRY_ENV)
{
  // Pass the default id.
  this->notify_proxy_.init (0, ACE_TRY_ENV);
}

void
TAO_Notify_CosEC_ProxyPushConsumer_i::push (const CORBA::Any &data, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->notify_proxy_.push (data, ACE_TRY_ENV);
}

void
TAO_Notify_CosEC_ProxyPushConsumer_i::disconnect_push_consumer (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->notify_proxy_.disconnect_push_consumer (ACE_TRY_ENV);
}

void
TAO_Notify_CosEC_ProxyPushConsumer_i::connect_push_supplier(CosEventComm::PushSupplier_ptr push_supplier, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosEventChannelAdmin::AlreadyConnected))
{
  this->notify_proxy_.connect_any_push_supplier (push_supplier, ACE_TRY_ENV);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Notify_ProxyConsumer<POA_CosNotifyChannelAdmin::ProxyPushConsumer>;
template class TAO_Notify_Proxy<POA_CosNotifyChannelAdmin::ProxyPushConsumer>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_Notify_ProxyConsumer<POA_CosNotifyChannelAdmin::ProxyPushConsumer>
#pragma instantiate TAO_Notify_Proxy<POA_CosNotifyChannelAdmin::ProxyPushConsumer>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
