// $Id$

// Force CORBA::release(CORBA::Request...) prototype to be parsed
// before TAO_Pseudo_{Var,Out}_T templates.  All required
// non-dependent names must be parsed prior to the template in
// question when using compilers like g++ 3.4.x.
#include "tao/DynamicInterface/DII_CORBA_methods.h"

#include "orbsvcs/CosEvent/CEC_ProxyPushSupplier.h"
#include "orbsvcs/CosEvent/CEC_Dispatching.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"
#include "orbsvcs/CosEvent/CEC_ConsumerControl.h"
#include "orbsvcs/ESF/ESF_RefCount_Guard.h"
#include "orbsvcs/ESF/ESF_Proxy_RefCount_Guard.h"

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
#include "orbsvcs/CosEvent/CEC_TypedEvent.h"
#include "orbsvcs/CosEvent/CEC_TypedEventChannel.h"
#include "tao/DynamicInterface/Request.h"
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

#include "tao/debug.h"
#include "ace/Reverse_Lock_T.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_ProxyPushSupplier.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (CosEvent,
           CEC_ProxyPushSupplier,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

typedef ACE_Reverse_Lock<ACE_Lock> TAO_CEC_Unlock;

// TAO_CEC_ProxyPushSupplier Constructure (Un-typed EC)
TAO_CEC_ProxyPushSupplier::
TAO_CEC_ProxyPushSupplier (TAO_CEC_EventChannel* ec,
                           const ACE_Time_Value &timeout)
  : event_channel_ (ec),
    timeout_ (timeout),
    refcount_ (1)
{
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  typed_event_channel_ = 0;
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  this->lock_ =
    this->event_channel_->create_supplier_lock ();

  this->default_POA_ =
    this->event_channel_->supplier_poa ();

  this->event_channel_->get_servant_retry_map ().bind (this, 0);
}

// TAO_CEC_ProxyPushSupplier Constructure (Typed EC)
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
TAO_CEC_ProxyPushSupplier::
TAO_CEC_ProxyPushSupplier (TAO_CEC_TypedEventChannel* ec,
                           const ACE_Time_Value &timeout)
  : timeout_ (timeout),
    typed_event_channel_ (ec),
    refcount_ (1)
{
  event_channel_ = 0;

  this->lock_ =
    this->typed_event_channel_->create_supplier_lock ();

  this->default_POA_ =
    this->typed_event_channel_->typed_supplier_poa ();

  this->typed_event_channel_->get_servant_retry_map ().bind (this, 0);
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

TAO_CEC_ProxyPushSupplier::~TAO_CEC_ProxyPushSupplier (void)
{
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  if (this->is_typed_ec () )
    {
      this->typed_event_channel_->get_servant_retry_map ().unbind (this);
      this->typed_event_channel_->destroy_supplier_lock (this->lock_);
    }
  else
    {
      this->event_channel_->get_servant_retry_map ().unbind (this);
      this->event_channel_->destroy_supplier_lock (this->lock_);
    }
#else
  this->event_channel_->get_servant_retry_map ().unbind (this);
  this->event_channel_->destroy_supplier_lock (this->lock_);
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
}

void
TAO_CEC_ProxyPushSupplier::activate (
    CosEventChannelAdmin::ProxyPushSupplier_ptr &activated_proxy
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosEventChannelAdmin::ProxyPushSupplier_var result;
  ACE_TRY
    {
      result = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      result = CosEventChannelAdmin::ProxyPushSupplier::_nil ();
    }
  ACE_ENDTRY;
  activated_proxy = result._retn ();
}

void
TAO_CEC_ProxyPushSupplier::deactivate (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_TRY
    {
      PortableServer::POA_var poa =
        this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      PortableServer::ObjectId_var id =
        poa->servant_to_id (this ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poa->deactivate_object (id.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Exceptions here should not be propagated.  They usually
      // indicate that an object is beign disconnected twice, or some
      // race condition, but not a fault that the user needs to know
      // about.
    }
  ACE_ENDTRY;
}

void
TAO_CEC_ProxyPushSupplier::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  if (this->is_typed_ec () )
    {
      // Save the consumer we where connected to, we need to send a
      // disconnect message to it.
      CosTypedEventComm::TypedPushConsumer_var typed_consumer;
      {
        ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            CORBA::INTERNAL ());
        // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
        ACE_CHECK;

        typed_consumer = this->typed_consumer_._retn ();
      }

      this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (CORBA::is_nil (typed_consumer.in ()))
        return;

      ACE_TRY_EX (typed)
        {
          typed_consumer->disconnect_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (typed);
        }
      ACE_CATCHANY
        {
          // Ignore exceptions, we must isolate other clients from
          // problems on this one.
        }
      ACE_ENDTRY;
    } /* this->is_typed_ec */
  else
    {
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  // Save the consumer we where connected to, we need to send a
  // disconnect message to it.
  CosEventComm::PushConsumer_var consumer;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    consumer = this->consumer_._retn ();
  }

  this->deactivate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (consumer.in ()))
    return;

  ACE_TRY
    {
      consumer->disconnect_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Ignore exceptions, we must isolate other clients from
      // problems on this one.
    }
  ACE_ENDTRY;
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
    } /* ! this->is_typed_ec */
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
}

typedef TAO_ESF_Proxy_RefCount_Guard<TAO_CEC_EventChannel,TAO_CEC_ProxyPushSupplier> Destroy_Guard;

void
TAO_CEC_ProxyPushSupplier::push (const CORBA::Any &event
                                 ACE_ENV_ARG_DECL)
{
  Destroy_Guard auto_destroy (this->refcount_,
                              this->event_channel_,
                              this);

  {
    ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

    if (this->is_connected_i () == 0)
      return;

    TAO_ESF_RefCount_Guard<CORBA::ULong> cnt_mon (this->refcount_);

    {
      TAO_CEC_Unlock reverse_lock (*this->lock_);

      ACE_GUARD (TAO_CEC_Unlock, ace_mon, reverse_lock);
      this->event_channel_->dispatching ()->push (this,
                                                  event
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  }
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
typedef TAO_ESF_Proxy_RefCount_Guard<TAO_CEC_TypedEventChannel,TAO_CEC_ProxyPushSupplier> Destroy_Guard_Typed;

void
TAO_CEC_ProxyPushSupplier::invoke (const TAO_CEC_TypedEvent& typed_event
                                   ACE_ENV_ARG_DECL)
{
  Destroy_Guard_Typed auto_destroy (this->refcount_,
                                    this->typed_event_channel_,
                                    this);
  {

    ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

    if (this->is_connected_i () == 0)
      return;

    TAO_ESF_RefCount_Guard<CORBA::ULong> cnt_mon (this->refcount_);

    {
      TAO_CEC_Unlock reverse_lock (*this->lock_);

      ACE_GUARD (TAO_CEC_Unlock, ace_mon, reverse_lock);
      this->typed_event_channel_->dispatching ()->invoke (this,
                                                          typed_event
                                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  }
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

void
TAO_CEC_ProxyPushSupplier::push_nocopy (CORBA::Any &event
                                        ACE_ENV_ARG_DECL)
{
  Destroy_Guard auto_destroy (this->refcount_,
                              this->event_channel_,
                              this);

  {
    ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

    if (this->is_connected_i () == 0)
      return;

    TAO_ESF_RefCount_Guard<CORBA::ULong> cnt_mon (this->refcount_);

    {
      TAO_CEC_Unlock reverse_lock (*this->lock_);

      ACE_GUARD (TAO_CEC_Unlock, ace_mon, reverse_lock);
      this->event_channel_->dispatching ()->push_nocopy (this,
                                                         event
                                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  }
}

void
TAO_CEC_ProxyPushSupplier::cleanup_i (void)
{
  this->consumer_ =
    CosEventComm::PushConsumer::_nil ();
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  this->typed_consumer_ =
    CosTypedEventComm::TypedPushConsumer::_nil ();
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
}

CORBA::ULong
TAO_CEC_ProxyPushSupplier::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
  return this->refcount_++;
}

CORBA::ULong
TAO_CEC_ProxyPushSupplier::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
    --this->refcount_;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  // Notify the event channel
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  if (this->is_typed_ec () )
    {
      this->typed_event_channel_->destroy_proxy (this);
    }
  else
    {
      this->event_channel_->destroy_proxy (this);
    }
#else
  this->event_channel_->destroy_proxy (this);
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
  return 0;
}

void
TAO_CEC_ProxyPushSupplier::connect_push_consumer (
      CosEventComm::PushConsumer_ptr push_consumer
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosEventChannelAdmin::AlreadyConnected,
                     CosEventChannelAdmin::TypeError))
{
  // Nil PushConsumers are illegal
  if (CORBA::is_nil (push_consumer))
    ACE_THROW (CORBA::BAD_PARAM ());

  {

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
    // Check if we have a typed event channel
    if (this->is_typed_ec () )
      {
        if (TAO_debug_level >= 10)
          {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("***** connect_push_consumer, ")
                        ACE_TEXT ("Event channel is typed *****\n")));
          }

        // Temporary for the TypedPushConsumer and it's Typed interface,
        // declared and obtained before the Guard to avoid deadlock during the
        // _is_a (during _narrow) and get_typed_consumer invocations.
        // They are eventually assigned onto this object inside the Guard.
        CosTypedEventComm::TypedPushConsumer_var local_typed_consumer =
          CosTypedEventComm::TypedPushConsumer::_narrow (push_consumer
                                                         ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        // Obtain the typed object interface from the consumer
        CORBA::Object_var local_typed_consumer_obj =
          CORBA::Object::_duplicate (local_typed_consumer->get_typed_consumer (
                                             ACE_ENV_SINGLE_ARG_PARAMETER) );
        ACE_CHECK;

        {
          ACE_GUARD_THROW_EX (
              ACE_Lock, ace_mon, *this->lock_,
              CORBA::INTERNAL ());
          // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
          ACE_CHECK;

          if (this->is_connected_i ())
            {
              if (this->typed_event_channel_->consumer_reconnect () == 0)
                ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());

              // Re-connections are allowed....
              this->cleanup_i ();

              this->typed_consumer_ = apply_policy (local_typed_consumer.in () );
              ACE_CHECK;

              TAO_CEC_Unlock reverse_lock (*this->lock_);

              {
                ACE_GUARD_THROW_EX (
                    TAO_CEC_Unlock, ace_mon, reverse_lock,
                    CORBA::INTERNAL ());
                // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
                ACE_CHECK;

                this->typed_event_channel_->reconnected (this ACE_ENV_ARG_PARAMETER);
                ACE_CHECK;
              }
              return;

            }

          this->typed_consumer_ = apply_policy (local_typed_consumer.in () );
          ACE_CHECK;

          // Store the typed object interface from the consumer
          this->typed_consumer_obj_ =
            apply_policy_obj (local_typed_consumer_obj.in () );
          ACE_CHECK;
        }

        // Notify the event channel...
        this->typed_event_channel_->connected (this ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

      } /* this->is_typed_ec */
    else
      {
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

        if (TAO_debug_level >= 10)
          {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("***** connect_push_consumer, ")
                        ACE_TEXT ("Event channel is un-typed *****\n")));
          }

        {

    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i ())
      {
        if (this->event_channel_->consumer_reconnect () == 0)
          ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());

        // Re-connections are allowed....
        this->cleanup_i ();

        this->consumer_ = apply_policy (push_consumer);

        TAO_CEC_Unlock reverse_lock (*this->lock_);

        {
          ACE_GUARD_THROW_EX (
              TAO_CEC_Unlock, ace_mon, reverse_lock,
              CORBA::INTERNAL ());
          // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
          ACE_CHECK;

          this->event_channel_->reconnected (this ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
        return;
      }

    this->consumer_ = apply_policy (push_consumer);
  }

  // Notify the event channel...
  this->event_channel_->connected (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
      } /* ! this->is_typed_ec */
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
  }
}

CORBA::Object_ptr
TAO_CEC_ProxyPushSupplier::apply_policy_obj (CORBA::Object_ptr pre)
{
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  CORBA::Object_var post = CORBA::Object::_duplicate (pre);
  if (this->timeout_ > ACE_Time_Value::zero)
    {
      CORBA::PolicyList policy_list;
      policy_list.length (1);
      if (this->typed_event_channel_)
        {
          policy_list[0] = this->typed_event_channel_->
            create_roundtrip_timeout_policy (this->timeout_);
        }
      else
        {
          policy_list[0] = this->event_channel_->
            create_roundtrip_timeout_policy (this->timeout_);
        }
      post = pre->_set_policy_overrides (policy_list, CORBA::ADD_OVERRIDE);

      policy_list[0]->destroy ();
      policy_list.length (0);
    }
  return post._retn ();
#else
  return CORBA::Object::_duplicate (pre);
#endif /* TAO_HAS_CORBA_MESSAGING */
}

CosEventComm::PushConsumer_ptr
TAO_CEC_ProxyPushSupplier::apply_policy (CosEventComm::PushConsumer_ptr pre)
{
  this->nopolicy_consumer_ = CosEventComm::PushConsumer::_duplicate (pre);
  CORBA::Object_var post_obj = apply_policy_obj (pre);
  CosEventComm::PushConsumer_var post =
    CosEventComm::PushConsumer::_narrow (post_obj.in ());
  return post._retn ();
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
CosTypedEventComm::TypedPushConsumer_ptr
TAO_CEC_ProxyPushSupplier::apply_policy
  (CosTypedEventComm::TypedPushConsumer_ptr pre)
{
  this->nopolicy_typed_consumer_ =
    CosTypedEventComm::TypedPushConsumer::_duplicate (pre);
  CORBA::Object_var post_obj = apply_policy_obj (pre);
  CosTypedEventComm::TypedPushConsumer_var post =
    CosTypedEventComm::TypedPushConsumer::_narrow (post_obj.in ());
  return post._retn ();
}
#endif

void
TAO_CEC_ProxyPushSupplier::disconnect_push_supplier (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosEventComm::PushConsumer_var consumer;
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  CosTypedEventComm::TypedPushConsumer_var typed_consumer;
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i () == 0)
      ACE_THROW (CORBA::BAD_INV_ORDER ());

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
    if (this->is_typed_ec () )
      {
        typed_consumer = this->typed_consumer_._retn ();
      }
    else
      {
        consumer = this->consumer_._retn ();
      }
#else
    consumer = this->consumer_._retn ();
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

    this->cleanup_i ();
  }

  // Notify the event channel....
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  if (this->is_typed_ec () )
    {
      this->typed_event_channel_->disconnected (this ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      this->event_channel_->disconnected (this ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
#else
  this->event_channel_->disconnected (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  // Disconnect callbacks
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  if (this->is_typed_ec () )
    {
      if (this->typed_event_channel_->disconnect_callbacks ())
        {
          ACE_TRY_EX (typed)
            {
              typed_consumer->disconnect_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK_EX (typed);
            }
          ACE_CATCHANY
            {
              // Ignore exceptions, we must isolate other clients from
              // problems on this one.
              ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                   "ProxySupplier::disconnect_push_supplier");
            }
          ACE_ENDTRY;
        }
    } /* this->is_typed_ec */
  else
    {
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  if (this->event_channel_->disconnect_callbacks ())
    {
      ACE_TRY
        {
          consumer->disconnect_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          // Ignore exceptions, we must isolate other clients from
          // problems on this one.
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "ProxySupplier::disconnect_push_supplier");
        }
      ACE_ENDTRY;
    }

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
    } /* ! this->is_typed_ec */
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
}

void
TAO_CEC_ProxyPushSupplier::push_to_consumer (const CORBA::Any& event
                                             ACE_ENV_ARG_DECL)
{
  CosEventComm::PushConsumer_var consumer;
  {
    ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i () == 0)
      return; // ACE_THROW (CosEventComm::Disconnected ());????

    consumer =
      CosEventComm::PushConsumer::_duplicate (this->consumer_.in ());
  }

  TAO_CEC_ConsumerControl *control =
               this->event_channel_->consumer_control ();
  ACE_TRY
    {
      consumer->push (event ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Inform the control that we were able to push something
      control->successful_transmission(this);
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, not_used)
    {
      control->consumer_not_exist (this ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      control->system_exception (this,
                                 sysex
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Shouldn't happen, but does not hurt
    }
  ACE_ENDTRY;
}

void
TAO_CEC_ProxyPushSupplier::reactive_push_to_consumer (
    const CORBA::Any& event
    ACE_ENV_ARG_DECL)
{
  CosEventComm::PushConsumer_var consumer;
  {
    ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);
    if (this->is_connected_i () == 0)
      return; // TAO_THROW (CosEventComm::Disconnected ());????

    consumer =
      CosEventComm::PushConsumer::_duplicate (this->consumer_.in ());
  }

  TAO_CEC_ConsumerControl *control =
                  this->event_channel_->consumer_control ();

  ACE_TRY
    {
      consumer->push (event ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Inform the control that we were able to push something
      control->successful_transmission(this);
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, not_used)
    {
       if (TAO_debug_level >= 4)
        {
          ACE_PRINT_EXCEPTION (not_used, "during TAO_CEC_ProxyPushSupplier::reactive_push_to_consumer");
        }
     control->consumer_not_exist (this ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      if (TAO_debug_level >= 4)
        {
          ACE_PRINT_EXCEPTION (sysex, "during TAO_CEC_ProxyPushSupplier::reactive_push_to_consumer");
        }

      control->system_exception (this,
                                 sysex
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Shouldn't happen, but does not hurt
    }
  ACE_ENDTRY;
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
void
TAO_CEC_ProxyPushSupplier::invoke_to_consumer (const TAO_CEC_TypedEvent &typed_event
                                               ACE_ENV_ARG_DECL)
{
  CORBA::Object_var typed_consumer_obj;

  // The DII target request object
  CORBA::Request_var target_request;

  {
    ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i () == 0)
      return; // ACE_THROW (CosEventComm::Disconnected ());????

    typed_consumer_obj =
      CORBA::Object::_duplicate (this->typed_consumer_obj_.in ());
  }

  TAO_CEC_ConsumerControl *control =
               this->typed_event_channel_->consumer_control ();

  // Create the DII request
  ACE_TRY
    {
      typed_consumer_obj_->_create_request (0, // ctx
                                            typed_event.operation_,
                                            typed_event.list_,
                                            0, // result
                                            0, // exception_list,
                                            0, // context_list,
                                            target_request.inout(),
                                            0
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Call the DII invoke for the operation on the target object
      target_request->invoke (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Inform the control that we were able to invoke something
      control->successful_transmission(this);
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, not_used)
    {
      if (TAO_debug_level >= 4)
        {
          ACE_PRINT_EXCEPTION (not_used, "during TAO_CEC_ProxyPushSupplier::invoke_to_consumer");
        }
      control->consumer_not_exist (this ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      if (TAO_debug_level >= 4)
        {
          ACE_PRINT_EXCEPTION (sysex, "during TAO_CEC_ProxyPushSupplier::invoke_to_consumer");
        }
      control->system_exception (this,
                                 sysex
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Shouldn't happen, but does not hurt
      if (TAO_debug_level >= 4)
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "ACE_ANY_EXCEPTION raised during TAO_CEC_ProxyPushSupplier::invoke_to_consumer");
        }
    }
  ACE_ENDTRY;
}

void
TAO_CEC_ProxyPushSupplier::reactive_invoke_to_consumer (
    const TAO_CEC_TypedEvent& typed_event
    ACE_ENV_ARG_DECL)
{
  CORBA::Object_var typed_consumer_obj;

  // The DII target request object
  CORBA::Request_var target_request;

  {
    ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);
    if (this->is_connected_i () == 0)
    {
      return; // TAO_THROW (CosEventComm::Disconnected ());????
    }
    if (CORBA::is_nil(this->typed_consumer_obj_.in()))
    {
      return; // TAO_THROW ...
    }

    typed_consumer_obj =
      CORBA::Object::_duplicate (this->typed_consumer_obj_.in ());
  }

  TAO_CEC_ConsumerControl *control =
                  this->typed_event_channel_->consumer_control ();

  // Create the DII request
  ACE_TRY
    {
      typed_consumer_obj_->_create_request (0, // ctx
                                            typed_event.operation_,
                                            typed_event.list_,
                                            0, // result
                                            0, // exception_list,
                                            0, // context_list,
                                            target_request.inout(),
                                            0
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Call the DII invoke for the operation on the target object
      target_request->invoke (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Inform the control that we were able to invoke something
      control->successful_transmission(this);
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, not_used)
    {
      if (TAO_debug_level >= 4)
        {
          ACE_PRINT_EXCEPTION (not_used, "during TAO_CEC_ProxyPushSupplier::reactive_invoke_to_consumer");
        }
      control->consumer_not_exist (this ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      if (TAO_debug_level >= 4)
        {
          ACE_PRINT_EXCEPTION (sysex, "during TAO_CEC_ProxyPushSupplier::reactive_invoke_to_consumer");
        }
      control->system_exception (this,
                                 sysex
                                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level >= 4)
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "ACE_ANY_EXCEPTION raised during TAO_CEC_ProxyPushSupplier::reactive_invoke_to_consumer");
        }
    }
  ACE_ENDTRY;
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

CORBA::Boolean
TAO_CEC_ProxyPushSupplier::consumer_non_existent (
      CORBA::Boolean_out disconnected
      ACE_ENV_ARG_DECL)
{
  CORBA::Object_var consumer;
  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    ACE_CHECK_RETURN (0);

    disconnected = 0;
    if (this->is_connected_i () == 0)
      {
        disconnected = 1;
        return 0;
      }

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
    if (this->is_typed_ec () )
      {
        if (CORBA::is_nil (this->nopolicy_typed_consumer_.in ()))
          {
            return 0;
          }
        consumer = CORBA::Object::_duplicate
          (this->nopolicy_typed_consumer_.in ());
      }
    else
      {
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
    if (CORBA::is_nil (this->nopolicy_consumer_.in ()))
      {
        return 0;
      }
    consumer = CORBA::Object::_duplicate (this->nopolicy_consumer_.in ());
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
      } /* ! this->is_typed_ec */
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
  }

#if (TAO_HAS_MINIMUM_CORBA == 0)
  return consumer->_non_existent (ACE_ENV_SINGLE_ARG_PARAMETER);
#else
  return 0;
#endif /* TAO_HAS_MINIMUM_CORBA */
}

PortableServer::POA_ptr
TAO_CEC_ProxyPushSupplier::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_CEC_ProxyPushSupplier::_add_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_incr_refcnt ();
}

void
TAO_CEC_ProxyPushSupplier::_remove_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_decr_refcnt ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
