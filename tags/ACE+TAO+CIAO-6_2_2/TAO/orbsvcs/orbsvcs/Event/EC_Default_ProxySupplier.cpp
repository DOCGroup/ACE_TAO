// $Id$

#include "orbsvcs/Event/EC_Default_ProxySupplier.h"
#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "orbsvcs/Event/EC_Filter_Builder.h"

#include "ace/Reverse_Lock_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
      const RtecEventChannelAdmin::ConsumerQOS& qos)
{
  // Nil PushConsumers are illegal
  if (CORBA::is_nil (push_consumer))
    throw CORBA::BAD_PARAM ();

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    if (this->is_connected_i ())
      {
        if (this->event_channel_->consumer_reconnect () == 0)
          throw RtecEventChannelAdmin::AlreadyConnected ();

        // Re-connections are allowed....
        this->cleanup_i ();

        this->consumer_ =
          RtecEventComm::PushConsumer::_duplicate (push_consumer);
        this->qos_ = qos;
        this->child_ =
          this->event_channel_->filter_builder ()->build (this,
                                                          this->qos_);

        this->adopt_child (this->child_);

        TAO_EC_Unlock reverse_lock (*this->lock_);

        {
          ACE_GUARD_THROW_EX (
              TAO_EC_Unlock, ace_mon, reverse_lock,
              CORBA::INTERNAL ());
          // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

          this->event_channel_->reconnected (this);
        }

        // A separate thread could have connected simultaneously,
        // this is probably an application error, handle it as
        // gracefully as possible
        if (this->is_connected_i ())
          return; // @@ Should we throw
      }

#if (TAO_HAS_CORBA_MESSAGING == 1)
      if ( consumer_validate_connection_ == 1 )
      {
        // Validate connection during connect.
        CORBA::PolicyList_var unused;
        int status = push_consumer->_validate_connection (unused);
#if TAO_EC_ENABLE_DEBUG_MESSAGES
        ORBSVCS_DEBUG ((LM_DEBUG, "Validated connection to PushConsumer on connect. Status[%d]\n", status));
#else
        ACE_UNUSED_ARG(status);
#endif /* TAO_EC_ENABLED_DEBUG_MESSAGES */
      }
#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

      this->consumer_ =
        RtecEventComm::PushConsumer::_duplicate (push_consumer);
      this->qos_ = qos;

#if TAO_EC_ENABLE_DEBUG_MESSAGES
    ORBSVCS_DEBUG ((LM_DEBUG,
                "Building filters for consumer <%x>.\n",
                this));
#endif /* TAO_EC_ENABLED_DEBUG_MESSAGES */
    this->child_ =
      this->event_channel_->filter_builder ()->build (this,
                                                      this->qos_);

    this->adopt_child (this->child_);
  }

  // Notify the event channel...
  this->event_channel_->connected (this);
}

void
TAO_EC_Default_ProxyPushSupplier::disconnect_push_supplier ()
{
  RtecEventComm::PushConsumer_var consumer;
  int connected = 0;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    connected = this->is_connected_i ();
    consumer = this->consumer_._retn ();

    if (connected)
      this->cleanup_i ();
  }

  // Notify the event channel....
  this->event_channel_->disconnected (this);

  if (!connected)
    {
      return;
    }

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
}

void
TAO_EC_Default_ProxyPushSupplier::suspend_connection (void)
{
  this->suspend_connection_locked ();
}

void
TAO_EC_Default_ProxyPushSupplier::resume_connection (void)
{
  this->resume_connection_locked ();
}

PortableServer::POA_ptr
TAO_EC_Default_ProxyPushSupplier::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_EC_Default_ProxyPushSupplier::_add_ref (void)
{
  this->_incr_refcnt ();
}

void
TAO_EC_Default_ProxyPushSupplier::_remove_ref (void)
{
  this->_decr_refcnt ();
}

void
TAO_EC_Default_ProxyPushSupplier::activate (
   RtecEventChannelAdmin::ProxyPushSupplier_ptr &proxy)
{
  proxy = this->_this ();
}

PortableServer::ObjectId
TAO_EC_Default_ProxyPushSupplier::object_id (void)
{
  PortableServer::ObjectId_var result =
    this->default_POA_->servant_to_id (this);

  return result.in ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
