// $Id$

#include "Notify_ProxyPushConsumer_i.h"
#include "Notify_Event_Manager.h"
#include "Notify_SupplierAdmin_i.h"

ACE_RCSID(Notify, Notify_ProxyPushConsumer_i, "$Id$")

typedef ACE_Reverse_Lock<ACE_Lock> TAO_Notify_Unlock;

// Implementation skeleton constructor
TAO_Notify_ProxyPushConsumer_i::TAO_Notify_ProxyPushConsumer_i (TAO_Notify_SupplierAdmin_i* supplier_admin)
  : proxy_inherited (supplier_admin),
    notify_style_supplier_ (0)
{
}

// Implementation skeleton destructor
TAO_Notify_ProxyPushConsumer_i::~TAO_Notify_ProxyPushConsumer_i (void)
{
}

void
TAO_Notify_ProxyPushConsumer_i::connect_any_push_supplier (CosEventComm::PushSupplier_ptr push_supplier TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventChannelAdmin::AlreadyConnected
                   ))
{
  ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  if (this->is_connected_ == 1)
    ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());
  else
    {
      this->cosec_push_supplier_ =
        CosEventComm::PushSupplier::_duplicate (push_supplier);

      this->notify_push_supplier_ =
        CosNotifyComm::PushSupplier::_narrow (push_supplier TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (!CORBA::is_nil (this->notify_push_supplier_.in ()))
        {
          this->cosec_push_supplier_ = CosNotifyComm::PushSupplier::_nil ();
          this->notify_style_supplier_ = 1;
        }

      this->is_connected_ = 1;
    }

  ACE_TRY
    {
      TAO_Notify_Unlock reverse_lock (*this->lock_);

      {
        ACE_GUARD_THROW_EX (TAO_Notify_Unlock, ace_mon, reverse_lock,
                            CORBA::INTERNAL ());
        ACE_TRY_CHECK;

        this->on_connected (TAO_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    }
  ACE_CATCHALL
    {
      this->cosec_push_supplier_ =
        CosEventComm::PushSupplier::_nil ();

      this->notify_push_supplier_ =
        CosNotifyComm::PushSupplier::_nil ();

      this->is_connected_ = 0;

      ACE_RE_THROW;
    }
  ACE_ENDTRY;
}

void
TAO_Notify_ProxyPushConsumer_i::dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed TAO_ENV_ARG_DECL)
{
  {
    ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK;

    if (this->notify_style_supplier_ == 0)
      return; // Our supplier doesn't support subscription_change.
  }

  ACE_TRY
    {
      this->notify_push_supplier_->subscription_change (added, removed
                                                        TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
      // misbehaving client, ignore for now.
    }
  ACE_ENDTRY;
}

void
TAO_Notify_ProxyPushConsumer_i::push (const CORBA::Any & data TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
{
  {
    ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK;

    if (this->is_connected_ == 0)
      ACE_THROW (CosEventComm::Disconnected ());
  }

  CORBA::Any * data_copy;
  ACE_NEW_THROW_EX (data_copy, CORBA::Any (data), CORBA::NO_MEMORY ());

  TAO_Notify_Any* notify_event = new TAO_Notify_Any(data_copy);

  this->event_manager_->process_event (notify_event, this TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  notify_event->_decr_refcnt ();
}

void
TAO_Notify_ProxyPushConsumer_i::disconnect_push_consumer (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  // ask our parent to deactivate us.
  this->supplier_admin_->
    deactivate_proxy_pushconsumer (this TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->on_disconnected (TAO_ENV_SINGLE_ARG_PARAMETER);
}

// = TAO_Notify_CosEC_ProxyPushConsumer_i

TAO_Notify_CosEC_ProxyPushConsumer_i::TAO_Notify_CosEC_ProxyPushConsumer_i (TAO_Notify_SupplierAdmin_i* supplieradmin)
  :notify_proxy_ (supplieradmin)
{
  // No-Op.
}

TAO_Notify_CosEC_ProxyPushConsumer_i::~TAO_Notify_CosEC_ProxyPushConsumer_i (void)
{
  // No-Op.
}

void
TAO_Notify_CosEC_ProxyPushConsumer_i::init (TAO_ENV_SINGLE_ARG_DECL)
{
  // Pass the default id.
  this->notify_proxy_.init (0 TAO_ENV_ARG_PARAMETER);
}

void
TAO_Notify_CosEC_ProxyPushConsumer_i::push (const CORBA::Any &data TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->notify_proxy_.push (data TAO_ENV_ARG_PARAMETER);
}

void
TAO_Notify_CosEC_ProxyPushConsumer_i::disconnect_push_consumer (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->notify_proxy_.disconnect_push_consumer (TAO_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_CosEC_ProxyPushConsumer_i::connect_push_supplier(CosEventComm::PushSupplier_ptr push_supplier TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosEventChannelAdmin::AlreadyConnected))
{
  this->notify_proxy_.connect_any_push_supplier (push_supplier TAO_ENV_ARG_PARAMETER);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Notify_ProxyConsumer<POA_CosNotifyChannelAdmin::ProxyPushConsumer>;
template class TAO_Notify_Proxy<POA_CosNotifyChannelAdmin::ProxyPushConsumer>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_Notify_ProxyConsumer<POA_CosNotifyChannelAdmin::ProxyPushConsumer>
#pragma instantiate TAO_Notify_Proxy<POA_CosNotifyChannelAdmin::ProxyPushConsumer>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
