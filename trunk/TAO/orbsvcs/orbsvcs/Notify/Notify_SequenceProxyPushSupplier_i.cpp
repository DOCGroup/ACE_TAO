/* -*- C++ -*- */
// $Id$

#include "Notify_SequenceProxyPushSupplier_i.h"
#include "Notify_ConsumerAdmin_i.h"
#include "Notify_StructuredEvents.h"
#include "Notify_Factory.h"
#include "Notify_Channel_Objects_Factory.h"

#include "tao/ORB_Core.h"
#include "tao/PortableServer/Object_Adapter.h"

ACE_RCSID(Notify, Notify_SequenceProxyPushSupplier_i, "$Id$")

typedef ACE_Reverse_Lock<ACE_Lock> TAO_Notify_Unlock;

TAO_Notify_SequenceProxyPushSupplier_i::TAO_Notify_SequenceProxyPushSupplier_i (TAO_Notify_ConsumerAdmin_i* consumer_admin)
  : proxy_inherited (consumer_admin),
    from_timeout_ (0),
    reset_timeout_flag_ (0),
    batch_lock_ (0)
{
}

TAO_Notify_SequenceProxyPushSupplier_i::~TAO_Notify_SequenceProxyPushSupplier_i (void)
{
  delete batch_lock_;
}

void
TAO_Notify_SequenceProxyPushSupplier_i::connect_sequence_push_consumer (CosNotifyComm::SequencePushConsumer_ptr push_consumer ACE_ENV_ARG_DECL)
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
      this->push_consumer_ =
        CosNotifyComm::SequencePushConsumer::_duplicate (push_consumer);

      this->is_connected_ = 1;
    }

  ACE_TRY
    {
      TAO_Notify_Unlock reverse_lock (*this->lock_);

      {
        ACE_GUARD_THROW_EX (TAO_Notify_Unlock, ace_mon, reverse_lock,
                            CORBA::INTERNAL ());
        ACE_CHECK;

        this->on_connected (ACE_ENV_SINGLE_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
    }
  ACE_CATCHALL
    {
     this->push_consumer_ =
       CosNotifyComm::SequencePushConsumer::_nil ();
     ACE_RE_THROW;
    }
 ACE_ENDTRY;
}

void
TAO_Notify_SequenceProxyPushSupplier_i::dispatch_event_i (TAO_Notify_Event& events
                                                          ACE_ENV_ARG_DECL)
{
  // If we are in this method then we are a SequenceProxyPushSupplier
  // connected up to a SequenceProxyPushConsumer and the only thing
  // that events can actually be is a TAO_Notify_StructuredEvents object.

  ACE_TRY
    {
      // We only have a batch_lock_ if
      // the PacingInterval policy was used.
      if (this->batch_lock_ != 0)
        {
          // I don't use ACE_GUARD here, because I don't want
          // to block if I can't get this lock.  The timeout handler
          // in the same thread would cause a deadlock.
          ACE_Guard<ACE_Lock> ace_mon (*this->batch_lock_, 0);
          if (ace_mon.locked () == 0) return;

          // Only set this if we have successfully obtained the lock.
          // This way, if from_timeout_ is ever set to 1 it will
          // only be set back to 0 if do_push () is called when timeout_
          // is set to 1 (causing the cache to be flushed).
          this->reset_timeout_flag_ = 1;
          events.do_push (this->push_consumer_.in (),
                          this->qos_admin (),
                          this->event_cache_,
                          this->from_timeout_
                          ACE_ENV_ARG_PARAMETER);
        }
      else
        {
          events.do_push (this->push_consumer_.in (),
                          this->qos_admin (),
                          this->event_cache_,
                          this->from_timeout_
                          ACE_ENV_ARG_PARAMETER);
        }
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
      ACE_DEBUG ((LM_DEBUG, "Exception dispatching structured event\n"));
      // misbehaving client, ignore for now.
    }
  ACE_ENDTRY;
}

void
TAO_Notify_SequenceProxyPushSupplier_i::dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      this->push_consumer_->offer_change (added, removed ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
      // misbehaving client, ignore for now.
    }
  ACE_ENDTRY;
}

void
TAO_Notify_SequenceProxyPushSupplier_i::disconnect_sequence_push_supplier(ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->on_disconnected (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // ask our parent to deactivate us.
  this->consumer_admin_->
    deactivate_proxy_pushsupplier (this ACE_ENV_ARG_PARAMETER);
}


void
TAO_Notify_SequenceProxyPushSupplier_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  // Tell the consumer that we're going away ...
  // @@ Later, lookup a "notify_on_disconnect" option.

  {
    ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

    if (this->is_connected_ == 0)
      return;
  }

  this->disconnect_sequence_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_TRY
    {
      this->push_consumer_->disconnect_sequence_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
      // ignore
    }
  ACE_ENDTRY;
}


void
TAO_Notify_SequenceProxyPushSupplier_i::setup_qos_policies (
                                          ACE_ENV_SINGLE_ARG_DECL)
{
# if defined (ACE_CONFIG_WIN32_H)
  ACE_Time_Value interval (
      ACE_static_cast (long, this->qos_admin ().pacing_interval ()));
# else
  ACE_Time_Value interval (this->qos_admin ().pacing_interval () / 1);
# endif /* ACE_CONFIG_WIN32_H */

  if (interval != ACE_Time_Value::zero)
    {
      // Only create the batch_lock_ if we are going to have a timer
      TAO_Notify_CO_Factory* cof =
        TAO_Notify_Factory::get_channel_objects_factory ();

      this->batch_lock_ = cof->create_proxy_supplier_lock (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      // Get back to the reactor to schedule the timer
      TAO_POA_Current_Impl *poa_current_impl =
        ACE_static_cast (TAO_POA_Current_Impl *,
                         TAO_TSS_RESOURCES::instance ()->poa_current_impl_);
      if (poa_current_impl != 0)
        {
          ACE_Reactor* reactor = poa_current_impl->orb_core ().reactor ();
          reactor->schedule_timer (this, 0, interval, interval);
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) Unable to schedule "
                      "timer for the pacing interval\n"));
        }
    }
}


int
TAO_Notify_SequenceProxyPushSupplier_i::handle_timeout (
                                  const ACE_Time_Value& /*current_time*/,
                                  const void* /*act*/)
{

  ACE_TRY_NEW_ENV
    {
      // We don't need to bother if there is nothing in the cache
      if (this->event_cache_.length () > 0)
        {
          // We need a TAO_Notify_StructuredEvents when we call
          // dispatch_event ().  It doesn't matter that there's nothing
          // in it.
          CosNotification::EventBatch notifications;
          TAO_Notify_StructuredEvents dummy_events (notifications);

          // Set the flag saying that we are comming from the timeout.
          // The worst that could happen if this is set while
          // dispatch_event_i was called by the event processor is that
          // the event cache will be flushed a split second before it
          // normally would.
          this->from_timeout_ = 1;
          this->dispatch_event (dummy_events ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHALL
    {
      ACE_DEBUG ((LM_DEBUG, "Exception dispatching handle_timeout\n"));
      // We must eat the exception in this case
    }
  ACE_ENDTRY;

  if (this->reset_timeout_flag_)
    {
      this->from_timeout_ = 0;
      this->reset_timeout_flag_ = 0;
    }
  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier>;
template class TAO_Notify_Proxy<POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier>
#pragma instantiate TAO_Notify_Proxy<POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
