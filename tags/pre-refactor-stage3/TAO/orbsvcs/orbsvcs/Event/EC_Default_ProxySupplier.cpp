// $Id$

#include "EC_Default_ProxySupplier.h"
#include "EC_Event_Channel_Base.h"
#include "EC_Filter_Builder.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Default_ProxySupplier.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_ProxySupplier, "$Id$")

typedef ACE_Reverse_Lock<ACE_Lock> TAO_EC_Unlock;

TAO_EC_Default_ProxyPushSupplier::TAO_EC_Default_ProxyPushSupplier (
      TAO_EC_Event_Channel_Base* ec,
      int validate_connection)
  : TAO_EC_ProxyPushSupplier (ec, validate_connection)
{
}

TAO_EC_Default_ProxyPushSupplier::~TAO_EC_Default_ProxyPushSupplier (void)
{
}

void
TAO_EC_Default_ProxyPushSupplier::connect_push_consumer (
      RtecEventComm::PushConsumer_ptr push_consumer,
      const RtecEventChannelAdmin::ConsumerQOS& qos
      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     RtecEventChannelAdmin::AlreadyConnected,
                     RtecEventChannelAdmin::TypeError))
{
  // Nil PushConsumers are illegal
  if (CORBA::is_nil (push_consumer))
    ACE_THROW (CORBA::BAD_PARAM ());

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    if (this->is_connected_i ())
      {
        if (this->event_channel_->consumer_reconnect () == 0)
          ACE_THROW (RtecEventChannelAdmin::AlreadyConnected ());

        // Re-connections are allowed....
        this->cleanup_i ();

        this->consumer_ =
          RtecEventComm::PushConsumer::_duplicate (push_consumer);
        this->qos_ = qos;
        this->child_ =
          this->event_channel_->filter_builder ()->build (this,
                                                          this->qos_
                                                          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        this->adopt_child (this->child_);

        TAO_EC_Unlock reverse_lock (*this->lock_);

        {
          ACE_GUARD_THROW_EX (
              TAO_EC_Unlock, ace_mon, reverse_lock,
              CORBA::INTERNAL ());
          // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
          ACE_CHECK;

          this->event_channel_->reconnected (this ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

        // A separate thread could have connected simultaneously,
        // this is probably an application error, handle it as
        // gracefully as possible
        if (this->is_connected_i ())
          return; // @@ Should we throw
      }

      if ( consumer_validate_connection_ == 1 )
      {
        // Validate connection during connect.
        CORBA::PolicyList_var unused;
        int status = push_consumer->_validate_connection (unused
                                                          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
#if TAO_EC_ENABLE_DEBUG_MESSAGES
        ACE_DEBUG ((LM_DEBUG, "Validated connection to PushConsumer on connect. Status[%d]\n", status));
#else
        ACE_UNUSED_ARG(status);
#endif /* TAO_EC_ENABLED_DEBUG_MESSAGES */
      }

      this->consumer_ =
        RtecEventComm::PushConsumer::_duplicate (push_consumer);
      this->qos_ = qos;

#if TAO_EC_ENABLE_DEBUG_MESSAGES
    ACE_DEBUG ((EC_FORMAT (DEBUG,
                           "Building filters for consumer <%x>."),
                this));
#endif /* TAO_EC_ENABLED_DEBUG_MESSAGES */
    this->child_ =
      this->event_channel_->filter_builder ()->build (this,
                                                      this->qos_
                                                      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    this->adopt_child (this->child_);
  }

  // Notify the event channel...
  this->event_channel_->connected (this ACE_ENV_ARG_PARAMETER);
}

void
TAO_EC_Default_ProxyPushSupplier::disconnect_push_supplier (
      ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  RtecEventComm::PushConsumer_var consumer;
  int connected = 0;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    connected = this->is_connected_i ();
    consumer = this->consumer_._retn ();

    if (connected)
      this->cleanup_i ();
  }

  // Notify the event channel....
  this->event_channel_->disconnected (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (!connected)
    {
      return;
    }

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
}

void
TAO_EC_Default_ProxyPushSupplier::suspend_connection (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->suspend_connection_locked (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_EC_Default_ProxyPushSupplier::resume_connection (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->resume_connection_locked (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

PortableServer::POA_ptr
TAO_EC_Default_ProxyPushSupplier::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_EC_Default_ProxyPushSupplier::_add_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_incr_refcnt ();
}

void
TAO_EC_Default_ProxyPushSupplier::_remove_ref (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  this->_decr_refcnt ();
}

void
TAO_EC_Default_ProxyPushSupplier::activate (
   RtecEventChannelAdmin::ProxyPushSupplier_ptr &proxy
   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  proxy = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

PortableServer::ObjectId
TAO_EC_Default_ProxyPushSupplier::object_id (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableServer::ObjectId_var result =
    this->default_POA_->servant_to_id (this ACE_ENV_ARG_PARAMETER);

  return result.in ();
}
