/* -*- C++ -*- */
// $Id$ */

#include "Notify_StructuredProxyPushSupplier_i.h"
#include "Notify_ConsumerAdmin_i.h"

ACE_RCSID(Notify, Notify_StructuredProxyPushSupplier_i, "$Id$")

typedef ACE_Reverse_Lock<ACE_Lock> TAO_Notify_Unlock;

TAO_Notify_StructuredProxyPushSupplier_i::TAO_Notify_StructuredProxyPushSupplier_i (TAO_Notify_ConsumerAdmin_i* consumeradmin)
  :proxy_inherited (consumeradmin)
{
}

TAO_Notify_StructuredProxyPushSupplier_i::~TAO_Notify_StructuredProxyPushSupplier_i (void)
{
}

void
TAO_Notify_StructuredProxyPushSupplier_i::connect_structured_push_consumer (CosNotifyComm::StructuredPushConsumer_ptr push_consumer TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventChannelAdmin::AlreadyConnected,
                   CosEventChannelAdmin::TypeError
                   ))
{
  ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  if (CORBA::is_nil (push_consumer))
    ACE_THROW (CosEventChannelAdmin::TypeError ());
  else
    {
       if (this->is_connected_ == 1)
        ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());
      else
        {
          this->push_consumer_ =
            CosNotifyComm::StructuredPushConsumer::_duplicate (push_consumer);
        }

      this->is_connected_ = 1;
    }

  ACE_TRY
    {
      TAO_Notify_Unlock reverse_lock (*this->lock_);

      {
        ACE_GUARD_THROW_EX (TAO_Notify_Unlock, ace_mon, reverse_lock,
                            CORBA::INTERNAL ());
        ACE_CHECK;

        this->on_connected (TAO_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    }
  ACE_CATCHALL
    {
      this->push_consumer_ =
        CosNotifyComm::StructuredPushConsumer::_nil ();

      this->is_connected_ = 0;

      ACE_RE_THROW;
    }
 ACE_ENDTRY;
}

void
TAO_Notify_StructuredProxyPushSupplier_i::dispatch_event_i (TAO_Notify_Event &event TAO_ENV_ARG_DECL)
{
  ACE_TRY
    {
      event.do_push (this->push_consumer_.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
      ACE_DEBUG ((LM_DEBUG, "Exception dispatching structured event\n"));
      // misbehaving client,
      this->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);  // FUZZ: ignore check_for_ace_check
    }
  ACE_ENDTRY;
}

void
TAO_Notify_StructuredProxyPushSupplier_i::dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed TAO_ENV_ARG_DECL)
{
  ACE_TRY
    {
      this->push_consumer_->offer_change (added, removed TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "call to offer_change failed\n");
      // misbehaving client, ignore for now.
    }
  ACE_ENDTRY;
}

void
TAO_Notify_StructuredProxyPushSupplier_i::shutdown_i (TAO_ENV_SINGLE_ARG_DECL)
{
  this->on_disconnected (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // ask our parent to deactivate us.
  this->consumer_admin_->
    deactivate_proxy_pushsupplier (this TAO_ENV_ARG_PARAMETER);
}

void
TAO_Notify_StructuredProxyPushSupplier_i::disconnect_structured_push_supplier(TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // unregister with CA
  this->consumer_admin_->unregister_listener (this TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->shutdown_i (TAO_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_StructuredProxyPushSupplier_i::shutdown (TAO_ENV_SINGLE_ARG_DECL)
{
  // Tell the consumer that we're going away ...
  // @@ Later, lookup a "notify_on_disconnect" option.

  {
    ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

    if (this->is_connected_ == 0)
      return;
  }

  this->shutdown_i (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_TRY
    {
      this->push_consumer_->disconnect_structured_push_consumer (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
      // ignore
    }
  ACE_ENDTRY;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::StructuredProxyPushSupplier>;
template class TAO_Notify_Proxy<POA_CosNotifyChannelAdmin::StructuredProxyPushSupplier>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::StructuredProxyPushSupplier>

#pragma instantiate TAO_Notify_Proxy<POA_CosNotifyChannelAdmin::StructuredProxyPushSupplier>
#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
