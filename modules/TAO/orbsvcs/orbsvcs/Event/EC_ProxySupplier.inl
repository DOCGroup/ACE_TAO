// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE void
TAO_EC_ProxyPushSupplier::suspend_connection_i (void)
{
  this->suspended_ = 1;
}

ACE_INLINE void
TAO_EC_ProxyPushSupplier::suspend_connection_locked (void)
{
  ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            CORBA::INTERNAL ());
  // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

  this->suspend_connection_i ();
}

ACE_INLINE void
TAO_EC_ProxyPushSupplier::resume_connection_i (void)
{
  this->suspended_ = 0;
}

ACE_INLINE void
TAO_EC_ProxyPushSupplier::resume_connection_locked (void)
{
  ACE_GUARD_THROW_EX (
            ACE_Lock, ace_mon, *this->lock_,
            CORBA::INTERNAL ());
  // @@ RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());

  this->resume_connection_i ();
}

ACE_INLINE CORBA::Boolean
TAO_EC_ProxyPushSupplier::is_connected_i (void) const
{
  return !CORBA::is_nil (this->consumer_.in ());
}

ACE_INLINE CORBA::Boolean
TAO_EC_ProxyPushSupplier::is_connected (void) const
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);

  return this->is_connected_i ();
}

ACE_INLINE CORBA::Boolean
TAO_EC_ProxyPushSupplier::is_suspended (void) const
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);

  return this->suspended_;
}

ACE_INLINE RtecEventComm::PushConsumer_ptr
TAO_EC_ProxyPushSupplier::consumer (void) const
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
  return RtecEventComm::PushConsumer::_duplicate (this->consumer_.in ());
}

ACE_INLINE void
TAO_EC_ProxyPushSupplier::consumer_i (RtecEventComm::PushConsumer_ptr consumer)
{
  this->consumer_ = consumer;
}

ACE_INLINE void
TAO_EC_ProxyPushSupplier::consumer (RtecEventComm::PushConsumer_ptr consumer)
{
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  this->consumer_i (consumer);
}

ACE_INLINE const RtecEventChannelAdmin::ConsumerQOS&
TAO_EC_ProxyPushSupplier::subscriptions (void) const
{
  // @@ TODO There should be a better way to signal errors here.
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, this->qos_);

  return this->qos_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
