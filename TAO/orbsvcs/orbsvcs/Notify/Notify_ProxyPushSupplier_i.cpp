//$Id$

#include "Notify_ProxyPushSupplier_i.h"
#include "Notify_Event_Manager.h"
#include "Notify_ConsumerAdmin_i.h"

ACE_RCSID(Notify, Notify_ProxyPushSupplier_i, "$Id$")

typedef ACE_Reverse_Lock<ACE_Lock> TAO_Notify_Unlock;

TAO_Notify_ProxyPushSupplier_i::TAO_Notify_ProxyPushSupplier_i (TAO_Notify_ConsumerAdmin_i* consumer_admin)
  :proxy_inherited (consumer_admin),
   notify_style_consumer_ (0)
{
}

// Implementation skeleton destructor
TAO_Notify_ProxyPushSupplier_i::~TAO_Notify_ProxyPushSupplier_i (void)
{
}

void
TAO_Notify_ProxyPushSupplier_i::dispatch_event_i (TAO_Notify_Event &event TAO_ENV_ARG_DECL)
{
  ACE_TRY
    {
      event.do_push (this->cosec_push_consumer_.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
      ACE_DEBUG ((LM_DEBUG, "Exception dispatching any event\n"));
      // misbehaving client,
      this->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER);
    }
  ACE_ENDTRY;
}

void
TAO_Notify_ProxyPushSupplier_i::dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed TAO_ENV_ARG_DECL)
{
  {
    ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);
    if (this->notify_style_consumer_ == 0)
      return; // Our consumer doesn't support offer_change.
  }

  ACE_TRY
    {
      this->notify_push_consumer_->offer_change (added, removed TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
      // misbehaving client, ignore for now.
    }
  ACE_ENDTRY;
}

void
TAO_Notify_ProxyPushSupplier_i::connect_any_push_consumer (CosEventComm::PushConsumer_ptr push_consumer TAO_ENV_ARG_DECL)
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
  else if (this->is_connected_ == 1)
    ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());
  else
    {
      this->cosec_push_consumer_ =
        CosEventComm::PushConsumer::_duplicate (push_consumer);

      this->notify_push_consumer_ =
        CosNotifyComm::PushConsumer::_narrow (push_consumer TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (!CORBA::is_nil (this->notify_push_consumer_.in ()))
        {
          CosNotifyComm::PushConsumer::
            _duplicate (this->notify_push_consumer_.in ());
          this->notify_style_consumer_ = 1;
        }

      this->is_connected_ = 1;
    } // else

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
     this->cosec_push_consumer_ = CosNotifyComm::PushConsumer::_nil ();
     this->notify_push_consumer_ = CosNotifyComm::PushConsumer::_nil ();

     this->is_connected_ = 0;
     ACE_RE_THROW;
   }
 ACE_ENDTRY;
}

void
TAO_Notify_ProxyPushSupplier_i::shutdown_i (TAO_ENV_SINGLE_ARG_DECL)
{
  this->on_disconnected (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // ask our parent to deactivate us.
  this->consumer_admin_->
    deactivate_proxy_pushsupplier (this TAO_ENV_ARG_PARAMETER);
}

void
TAO_Notify_ProxyPushSupplier_i::disconnect_push_supplier (TAO_ENV_SINGLE_ARG_DECL)
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
TAO_Notify_ProxyPushSupplier_i::shutdown (TAO_ENV_SINGLE_ARG_DECL)
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
      this->cosec_push_consumer_->disconnect_push_consumer (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
      // ignore
    }
  ACE_ENDTRY;
}

// = TAO_Notify_CosEC_ProxyPushSupplier_i

TAO_Notify_CosEC_ProxyPushSupplier_i::TAO_Notify_CosEC_ProxyPushSupplier_i (TAO_Notify_ConsumerAdmin_i* consumer_admin)
  :notify_proxy_ (consumer_admin)
{
  // No-Op.
}

TAO_Notify_CosEC_ProxyPushSupplier_i::~TAO_Notify_CosEC_ProxyPushSupplier_i (void)
{
}

void
TAO_Notify_CosEC_ProxyPushSupplier_i::init (TAO_ENV_SINGLE_ARG_DECL)
{
  this->notify_proxy_.init (0 TAO_ENV_ARG_PARAMETER);
}

void
TAO_Notify_CosEC_ProxyPushSupplier_i::disconnect_push_supplier (TAO_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->notify_proxy_.disconnect_push_supplier (TAO_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_CosEC_ProxyPushSupplier_i::connect_push_consumer(CosEventComm::PushConsumer_ptr push_consumer TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosEventChannelAdmin::AlreadyConnected,
                     CosEventChannelAdmin::TypeError))
{
  this->notify_proxy_.connect_any_push_consumer (push_consumer TAO_ENV_ARG_PARAMETER);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::ProxyPushSupplier>;
template class TAO_Notify_Proxy<POA_CosNotifyChannelAdmin::ProxyPushSupplier>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::ProxyPushSupplier>
#pragma instantiate TAO_Notify_Proxy<POA_CosNotifyChannelAdmin::ProxyPushSupplier>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Unbounded_Queue<TAO_Notify_Event*>;
template class ACE_Node<TAO_Notify_Event*>;
template class ACE_Unbounded_Queue_Iterator <TAO_Notify_Event*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Unbounded_Queue<TAO_Notify_Event*>
#pragma instantiate ACE_Node<TAO_Notify_Event*>
#pragma instantiate ACE_Unbounded_Queue_Iterator <TAO_Notify_Event*>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
