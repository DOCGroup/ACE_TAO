// $Id$

// Force CORBA::release(CORBA::Request...) prototype to be parsed
// before TAO_Pseudo_{Var,Out}_T templates.  All required
// non-dependent names must be parsed prior to the template in
// question when using compilers like g++ 3.4.x.
#include "orbsvcs/Log_Macros.h"
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
#include "orbsvcs/CosEvent/CEC_ProxyPushSupplier.inl"
#endif /* __ACE_INLINE__ */

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
    CosEventChannelAdmin::ProxyPushSupplier_ptr &activated_proxy)
{
  CosEventChannelAdmin::ProxyPushSupplier_var result;
  try
    {
      result = this->_this ();
    }
  catch (const CORBA::Exception&)
    {
      result = CosEventChannelAdmin::ProxyPushSupplier::_nil ();
    }
  activated_proxy = result._retn ();
}

void
TAO_CEC_ProxyPushSupplier::deactivate (void)
{
  try
    {
      PortableServer::POA_var poa =
        this->_default_POA ();
      PortableServer::ObjectId_var id =
        poa->servant_to_id (this);
      poa->deactivate_object (id.in ());
    }
  catch (const CORBA::Exception&)
    {
      // Exceptions here should not be propagated.  They usually
      // indicate that an object is being disconnected twice, or some
      // race condition, but not a fault that the user needs to know
      // about.
    }
}

void
TAO_CEC_ProxyPushSupplier::shutdown (void)
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

        typed_consumer = this->typed_consumer_._retn ();
      }

      this->deactivate ();

      if (CORBA::is_nil (typed_consumer.in ()))
        return;

      try
        {
          typed_consumer->disconnect_push_consumer ();
        }
      catch (const CORBA::Exception&)
        {
          // Ignore exceptions, we must isolate other clients from
          // problems on this one.
        }
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

    consumer = this->consumer_._retn ();
  }

  this->deactivate ();

  if (CORBA::is_nil (consumer.in ()))
    return;

  try
    {
      consumer->disconnect_push_consumer ();
    }
  catch (const CORBA::Exception&)
    {
      // Ignore exceptions, we must isolate other clients from
      // problems on this one.
    }
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
    } /* ! this->is_typed_ec */
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
}

typedef TAO_ESF_Proxy_RefCount_Guard<TAO_CEC_EventChannel,TAO_CEC_ProxyPushSupplier> Destroy_Guard;

void
TAO_CEC_ProxyPushSupplier::push (const CORBA::Any &event)
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
                                                  event);
    }
  }
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
typedef TAO_ESF_Proxy_RefCount_Guard<TAO_CEC_TypedEventChannel,TAO_CEC_ProxyPushSupplier> Destroy_Guard_Typed;

void
TAO_CEC_ProxyPushSupplier::invoke (const TAO_CEC_TypedEvent& typed_event)
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
                                                          typed_event);
    }
  }
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

void
TAO_CEC_ProxyPushSupplier::push_nocopy (CORBA::Any &event)
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
                                                         event);
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
      CosEventComm::PushConsumer_ptr push_consumer)
{
  // Nil PushConsumers are illegal
  if (CORBA::is_nil (push_consumer))
    throw CORBA::BAD_PARAM ();

  {

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
    // Check if we have a typed event channel
    if (this->is_typed_ec () )
      {
        if (TAO_debug_level >= 10)
          {
            ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("***** connect_push_consumer, ")
                        ACE_TEXT ("Event channel is typed *****\n")));
          }

        // Temporary for the TypedPushConsumer and it's Typed interface,
        // declared and obtained before the Guard to avoid deadlock during the
        // _is_a (during _narrow) and get_typed_consumer invocations.
        // They are eventually assigned onto this object inside the Guard.
        CosTypedEventComm::TypedPushConsumer_var local_typed_consumer =
          CosTypedEventComm::TypedPushConsumer::_narrow (push_consumer);

        // Obtain the typed object interface from the consumer
        CORBA::Object_var local_typed_consumer_obj =
          CORBA::Object::_duplicate (local_typed_consumer->get_typed_consumer () );

        {
          ACE_GUARD_THROW_EX (
              ACE_Lock, ace_mon, *this->lock_,
              CORBA::INTERNAL ());
          // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

          if (this->is_connected_i ())
            {
              if (this->typed_event_channel_->consumer_reconnect () == 0)
                throw CosEventChannelAdmin::AlreadyConnected ();

              // Re-connections are allowed....
              this->cleanup_i ();

              this->typed_consumer_ = apply_policy (local_typed_consumer.in () );

              TAO_CEC_Unlock reverse_lock (*this->lock_);

              {
                ACE_GUARD_THROW_EX (
                    TAO_CEC_Unlock, ace_mon, reverse_lock,
                    CORBA::INTERNAL ());
                // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

                this->typed_event_channel_->reconnected (this);
              }
              return;

            }

          this->typed_consumer_ = apply_policy (local_typed_consumer.in () );

          // Store the typed object interface from the consumer
          this->typed_consumer_obj_ =
            apply_policy_obj (local_typed_consumer_obj.in () );
        }

        // Notify the event channel...
        this->typed_event_channel_->connected (this);

      } /* this->is_typed_ec */
    else
      {
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

        if (TAO_debug_level >= 10)
          {
            ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("***** connect_push_consumer, ")
                        ACE_TEXT ("Event channel is un-typed *****\n")));
          }

        {

    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    if (this->is_connected_i ())
      {
        if (this->event_channel_->consumer_reconnect () == 0)
          throw CosEventChannelAdmin::AlreadyConnected ();

        // Re-connections are allowed....
        this->cleanup_i ();

        this->consumer_ = apply_policy (push_consumer);

        TAO_CEC_Unlock reverse_lock (*this->lock_);

        {
          ACE_GUARD_THROW_EX (
              TAO_CEC_Unlock, ace_mon, reverse_lock,
              CORBA::INTERNAL ());
          // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

          this->event_channel_->reconnected (this);
        }
        return;
      }

    this->consumer_ = apply_policy (push_consumer);
  }

  // Notify the event channel...
  this->event_channel_->connected (this);

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
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
      if (this->typed_event_channel_)
        {
          policy_list[0] = this->typed_event_channel_->
            create_roundtrip_timeout_policy (this->timeout_);
        }
      else
        {
#endif
          policy_list[0] = this->event_channel_->
            create_roundtrip_timeout_policy (this->timeout_);
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
        }
#endif
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
TAO_CEC_ProxyPushSupplier::disconnect_push_supplier ()
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

    if (this->is_connected_i () == 0)
      throw CORBA::BAD_INV_ORDER ();

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
      this->typed_event_channel_->disconnected (this);
    }
  else
    {
      this->event_channel_->disconnected (this);
    }
#else
  this->event_channel_->disconnected (this);
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  // Disconnect callbacks
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  if (this->is_typed_ec () )
    {
      if (this->typed_event_channel_->disconnect_callbacks ())
        {
          try
            {
              typed_consumer->disconnect_push_consumer ();
            }
          catch (const CORBA::Exception& ex)
            {
              // Ignore exceptions, we must isolate other clients from
              // problems on this one.
              ex._tao_print_exception (
                "ProxySupplier::disconnect_push_supplier");
            }
        }
    } /* this->is_typed_ec */
  else
    {
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  if (this->event_channel_->disconnect_callbacks ())
    {
      try
        {
          consumer->disconnect_push_consumer ();
        }
      catch (const CORBA::Exception& ex)
        {
          // Ignore exceptions, we must isolate other clients from
          // problems on this one.
          ex._tao_print_exception ("ProxySupplier::disconnect_push_supplier");
        }
    }

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
    } /* ! this->is_typed_ec */
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
}

void
TAO_CEC_ProxyPushSupplier::push_to_consumer (const CORBA::Any& event)
{
  CosEventComm::PushConsumer_var consumer;
  {
    ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    if (this->is_connected_i () == 0)
      return; // ACE_THROW (CosEventComm::Disconnected ());????

    consumer =
      CosEventComm::PushConsumer::_duplicate (this->consumer_.in ());
  }

  TAO_CEC_ConsumerControl *control =
               this->event_channel_->consumer_control ();
  try
    {
      consumer->push (event);

      // Inform the control that we were able to push something
      control->successful_transmission(this);
    }
  catch (const CORBA::OBJECT_NOT_EXIST&)
    {
      control->consumer_not_exist (this);
    }
  catch (CORBA::SystemException& sysex)
    {
      control->system_exception (this,
                                 sysex);
    }
  catch (const CORBA::Exception&)
    {
      // Shouldn't happen, but does not hurt
    }
}

void
TAO_CEC_ProxyPushSupplier::reactive_push_to_consumer (
    const CORBA::Any& event)
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

  try
    {
      consumer->push (event);

      // Inform the control that we were able to push something
      control->successful_transmission(this);
    }
  catch (const CORBA::OBJECT_NOT_EXIST& not_used)
    {
       if (TAO_debug_level >= 4)
        {
          not_used._tao_print_exception (
            "during TAO_CEC_ProxyPushSupplier::reactive_push_to_consumer");
        }
     control->consumer_not_exist (this);
    }
  catch (CORBA::SystemException& sysex)
    {
      if (TAO_debug_level >= 4)
        {
          sysex._tao_print_exception (
            "during TAO_CEC_ProxyPushSupplier::reactive_push_to_consumer");
        }

      control->system_exception (this,
                                 sysex);
    }
  catch (const CORBA::Exception&)
    {
      // Shouldn't happen, but does not hurt
    }
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
void
TAO_CEC_ProxyPushSupplier::invoke_to_consumer (const TAO_CEC_TypedEvent &typed_event)
{
  CORBA::Object_var typed_consumer_obj;

  // The DII target request object
  CORBA::Request_var target_request;

  {
    ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    if (this->is_connected_i () == 0)
      return; // ACE_THROW (CosEventComm::Disconnected ());????

    typed_consumer_obj =
      CORBA::Object::_duplicate (this->typed_consumer_obj_.in ());
  }

  TAO_CEC_ConsumerControl *control =
               this->typed_event_channel_->consumer_control ();

  // Create the DII request
  try
    {
      typed_consumer_obj_->_create_request (0, // ctx
                                            typed_event.operation_,
                                            typed_event.list_,
                                            0, // result
                                            0, // exception_list,
                                            0, // context_list,
                                            target_request.inout(),
                                            0);

      // Call the DII invoke for the operation on the target object
      target_request->invoke ();

      // Inform the control that we were able to invoke something
      control->successful_transmission(this);
    }
  catch (const CORBA::OBJECT_NOT_EXIST& not_used)
    {
      if (TAO_debug_level >= 4)
        {
          not_used._tao_print_exception (
            "during TAO_CEC_ProxyPushSupplier::invoke_to_consumer");
        }
      control->consumer_not_exist (this);
    }
  catch (CORBA::SystemException& sysex)
    {
      if (TAO_debug_level >= 4)
        {
          sysex._tao_print_exception (
            "during TAO_CEC_ProxyPushSupplier::invoke_to_consumer");
        }
      control->system_exception (this,
                                 sysex);
    }
  catch (const CORBA::Exception& ex)
    {
      // Shouldn't happen, but does not hurt
      if (TAO_debug_level >= 4)
        {
          ex._tao_print_exception (
            "ex raised during TAO_CEC_ProxyPushSupplier::invoke_to_consumer");
        }
    }
}

void
TAO_CEC_ProxyPushSupplier::reactive_invoke_to_consumer (
    const TAO_CEC_TypedEvent& typed_event)
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
  try
    {
      typed_consumer_obj_->_create_request (0, // ctx
                                            typed_event.operation_,
                                            typed_event.list_,
                                            0, // result
                                            0, // exception_list,
                                            0, // context_list,
                                            target_request.inout(),
                                            0);

      // Call the DII invoke for the operation on the target object
      target_request->invoke ();

      // Inform the control that we were able to invoke something
      control->successful_transmission(this);
    }
  catch (const CORBA::OBJECT_NOT_EXIST& not_used)
    {
      if (TAO_debug_level >= 4)
        {
          not_used._tao_print_exception (
            "during TAO_CEC_ProxyPushSupplier::reactive_invoke_to_consumer");
        }
      control->consumer_not_exist (this);
    }
  catch (CORBA::SystemException& sysex)
    {
      if (TAO_debug_level >= 4)
        {
          sysex._tao_print_exception (
            "during TAO_CEC_ProxyPushSupplier::reactive_invoke_to_consumer");
        }
      control->system_exception (this,
                                 sysex);
    }
  catch (const CORBA::Exception& ex)
    {
      if (TAO_debug_level >= 4)
        {
          ex._tao_print_exception (
            "ex raised during TAO_CEC_ProxyPushSupplier::reactive_invoke_to_consumer");
        }
    }
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

CORBA::Boolean
TAO_CEC_ProxyPushSupplier::consumer_non_existent (
      CORBA::Boolean_out disconnected)
{
  CORBA::Object_var consumer;
  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());

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
  return consumer->_non_existent ();
#else
  return 0;
#endif /* TAO_HAS_MINIMUM_CORBA */
}

PortableServer::POA_ptr
TAO_CEC_ProxyPushSupplier::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_CEC_ProxyPushSupplier::_add_ref (void)
{
  this->_incr_refcnt ();
}

void
TAO_CEC_ProxyPushSupplier::_remove_ref (void)
{
  this->_decr_refcnt ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
