/* -*- C++ -*- */
// $Id$

#include "Notify_SequenceProxyPushConsumer_i.h"
#include "Notify_Event_Manager.h"
#include "Notify_SupplierAdmin_i.h"

ACE_RCSID(Notify, Notify_SequenceProxyPushConsumer_i, "$Id$")

typedef ACE_Reverse_Lock<ACE_Lock> TAO_Notify_Unlock;

TAO_Notify_SequenceProxyPushConsumer_i::TAO_Notify_SequenceProxyPushConsumer_i (TAO_Notify_SupplierAdmin_i* supplier_admin)
  : proxy_inherited (supplier_admin)
{
}

TAO_Notify_SequenceProxyPushConsumer_i::~TAO_Notify_SequenceProxyPushConsumer_i (void)
{
}

void
TAO_Notify_SequenceProxyPushConsumer_i::connect_sequence_push_supplier (CosNotifyComm::SequencePushSupplier_ptr push_supplier TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventChannelAdmin::AlreadyConnected
                   ))
{
  // @@ Pradeep: here is another example on code that is not thread
  // safe, i know you are post-poning the thread safety issues, but it
  // is not that easy!

  ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  if (this->is_connected_ == 1)
    ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());
  else
    this->push_supplier_ =
      CosNotifyComm::SequencePushSupplier::_duplicate (push_supplier);

  this->is_connected_ = 1;

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
      this->push_supplier_ = CosNotifyComm::SequencePushSupplier::_nil ();
      this->is_connected_ = 0;

      ACE_RE_THROW;
    }
  ACE_ENDTRY;
}

void
TAO_Notify_SequenceProxyPushConsumer_i::dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed TAO_ENV_ARG_DECL)
{
  ACE_TRY
    {
      this->push_supplier_->subscription_change (added, removed
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
TAO_Notify_SequenceProxyPushConsumer_i::push_structured_events (const CosNotification::EventBatch & notifications TAO_ENV_ARG_DECL)
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

  CosNotification::StructuredEvent *notification_copy;
  for (CORBA::ULong i = 0; i < notifications.length (); ++i)
    {
      ACE_NEW_THROW_EX (notification_copy,
                        CosNotification::StructuredEvent (notifications[i]),
                        CORBA::NO_MEMORY ());

      TAO_Notify_StructuredEvent* notify_event =
        new TAO_Notify_StructuredEvent(notification_copy);

      this->event_manager_->process_event (notify_event, this TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      notify_event->_decr_refcnt ();
    }
}

void
TAO_Notify_SequenceProxyPushConsumer_i::disconnect_sequence_push_consumer (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // ask our parent to deaactivate us.
  this->supplier_admin_->
    deactivate_proxy_pushconsumer (this TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->on_disconnected (TAO_ENV_SINGLE_ARG_PARAMETER);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Notify_ProxyConsumer<POA_CosNotifyChannelAdmin::SequenceProxyPushConsumer>;
template class TAO_Notify_Proxy<POA_CosNotifyChannelAdmin::SequenceProxyPushConsumer>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Notify_ProxyConsumer<POA_CosNotifyChannelAdmin::SequenceProxyPushConsumer>
#pragma instantiate TAO_Notify_Proxy<POA_CosNotifyChannelAdmin::SequenceProxyPushConsumer>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
