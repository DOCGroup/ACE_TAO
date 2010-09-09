// $Id$

#include "orbsvcs/CosEvent/CEC_ProxyPullSupplier.h"
#include "orbsvcs/CosEvent/CEC_Dispatching.h"
#include "orbsvcs/CosEvent/CEC_EventChannel.h"

#include "ace/Reverse_Lock_T.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/CosEvent/CEC_ProxyPullSupplier.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

typedef ACE_Reverse_Lock<ACE_Lock> TAO_CEC_Unlock;

TAO_CEC_ProxyPullSupplier::TAO_CEC_ProxyPullSupplier
(TAO_CEC_EventChannel* ec, const ACE_Time_Value &timeout)
  : event_channel_ (ec),
    timeout_ (timeout),
    refcount_ (1),
    connected_ (0),
    wait_not_empty_ (queue_lock_)
{
  this->lock_ =
    this->event_channel_->create_supplier_lock ();

  this->default_POA_ =
    this->event_channel_->supplier_poa ();

  this->event_channel_->get_servant_retry_map ().bind (this, 0);
}

TAO_CEC_ProxyPullSupplier::~TAO_CEC_ProxyPullSupplier (void)
{
  this->event_channel_->get_servant_retry_map ().unbind (this);
  this->event_channel_->destroy_supplier_lock (this->lock_);
}

void
TAO_CEC_ProxyPullSupplier::activate (
    CosEventChannelAdmin::ProxyPullSupplier_ptr &activated_proxy)
{
  CosEventChannelAdmin::ProxyPullSupplier_var result;
  try
    {
      result = this->_this ();
    }
  catch (const CORBA::Exception&)
    {
      result = CosEventChannelAdmin::ProxyPullSupplier::_nil ();
    }
  activated_proxy = result._retn ();
}

void
TAO_CEC_ProxyPullSupplier::deactivate (void)
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
      // indicate that an object is beign disconnected twice, or some
      // race condition, but not a fault that the user needs to know
      // about.
    }
}

void
TAO_CEC_ProxyPullSupplier::shutdown (void)
{
  // Save the consumer we where connected to, we need to send a
  // disconnect message to it.
  CosEventComm::PullConsumer_var consumer;

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
      consumer->disconnect_pull_consumer ();
    }
  catch (const CORBA::Exception&)
    {
      // Ignore exceptions, we must isolate other clients from
      // problems on this one.
    }
}

CORBA::Boolean
TAO_CEC_ProxyPullSupplier::consumer_non_existent (
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
    if (CORBA::is_nil (this->nopolicy_consumer_.in ()))
      {
        return 0;
      }
    consumer = CORBA::Object::_duplicate (this->nopolicy_consumer_.in ());
  }

#if (TAO_HAS_MINIMUM_CORBA == 0)
  return consumer->_non_existent ();
#else
  return 0;
#endif /* TAO_HAS_MINIMUM_CORBA */
}

void
TAO_CEC_ProxyPullSupplier::push (const CORBA::Any &event)
{
  if (this->is_connected () == 0)
    return;

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->queue_lock_);

  // Ignore errors
  (void) this->queue_.enqueue_tail (event);

  this->wait_not_empty_.signal ();
}

CORBA::Any *
TAO_CEC_ProxyPullSupplier::pull (void)
{
  if (this->is_connected () == 0)
    throw CosEventComm::Disconnected ();

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, queue_lock_, 0);
  while (this->queue_.is_empty ())
    {
      this->wait_not_empty_.wait ();
    }
  CORBA::Any any;
  if (this->queue_.dequeue_head (any) != 0)
    {
      throw CORBA::INTERNAL ();
    }
  return new CORBA::Any (any);
}

CORBA::Any *
TAO_CEC_ProxyPullSupplier::try_pull (CORBA::Boolean_out has_event)
{
  has_event = 0;
  if (this->is_connected () == 0)
    throw CosEventComm::Disconnected ();

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, queue_lock_, 0);
  CORBA::Any any;
  if (this->queue_.is_empty ())
    {
      any <<= CORBA::Long(0);
      return new CORBA::Any (any);
    }

  if (this->queue_.dequeue_head (any) != 0)
    {
      throw CORBA::INTERNAL ();
    }
  has_event = 1;
  return new CORBA::Any (any);
}

void
TAO_CEC_ProxyPullSupplier::cleanup_i (void)
{
  this->consumer_ =
    CosEventComm::PullConsumer::_nil ();
  this->connected_ = 0;
}

CORBA::ULong
TAO_CEC_ProxyPullSupplier::_incr_refcnt (void)
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
  return this->refcount_++;
}

CORBA::ULong
TAO_CEC_ProxyPullSupplier::_decr_refcnt (void)
{
  {
    ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
    this->refcount_--;
    if (this->refcount_ != 0)
      return this->refcount_;
  }

  // Notify the event channel
  this->event_channel_->destroy_proxy (this);
  return 0;
}

void
TAO_CEC_ProxyPullSupplier::connect_pull_consumer (
      CosEventComm::PullConsumer_ptr pull_consumer)
{
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

        this->consumer_ = apply_policy (pull_consumer);
        this->connected_ = 1;

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

    this->consumer_ = apply_policy (pull_consumer);
    this->connected_ = 1;
  }

  // Notify the event channel...
  this->event_channel_->connected (this);
}

CosEventComm::PullConsumer_ptr
TAO_CEC_ProxyPullSupplier::apply_policy (CosEventComm::PullConsumer_ptr pre)
{
  if (CORBA::is_nil (pre)) return pre;
  this->nopolicy_consumer_ = CosEventComm::PullConsumer::_duplicate (pre);
#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  CosEventComm::PullConsumer_var post =
    CosEventComm::PullConsumer::_duplicate (pre);
  if (this->timeout_ > ACE_Time_Value::zero)
    {
      CORBA::PolicyList policy_list;
      policy_list.length (1);
      policy_list[0] = this->event_channel_->
        create_roundtrip_timeout_policy (this->timeout_);

      CORBA::Object_var post_obj = pre->_set_policy_overrides
        (policy_list, CORBA::ADD_OVERRIDE);
      post = CosEventComm::PullConsumer::_narrow(post_obj.in ());

      policy_list[0]->destroy ();
      policy_list.length (0);
    }
  return post._retn ();
#else
  return CosEventComm::PullConsumer::_duplicate (pre);
#endif /* TAO_HAS_CORBA_MESSAGING */
}

void
TAO_CEC_ProxyPullSupplier::disconnect_pull_supplier ()
{
  CosEventComm::PullConsumer_var consumer;

  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        CORBA::INTERNAL ());
    // @@ CosEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

    if (this->is_connected_i () == 0)
      throw CORBA::BAD_INV_ORDER ();

    consumer = this->consumer_._retn ();

    this->cleanup_i ();
  }

  // Notify the event channel....
  this->event_channel_->disconnected (this);

  if (CORBA::is_nil (consumer.in ()))
    return;

  if (this->event_channel_->disconnect_callbacks ())
    {
      try
        {
          consumer->disconnect_pull_consumer ();
        }
      catch (const CORBA::Exception& ex)
        {
          // Ignore exceptions, we must isolate other clients from
          // problems on this one.
          ex._tao_print_exception ("ProxySupplier::disconnect_pull_supplier");
        }
    }
}

PortableServer::POA_ptr
TAO_CEC_ProxyPullSupplier::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->default_POA_.in ());
}

void
TAO_CEC_ProxyPullSupplier::_add_ref (void)
{
  this->_incr_refcnt ();
}

void
TAO_CEC_ProxyPullSupplier::_remove_ref (void)
{
  this->_decr_refcnt ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
