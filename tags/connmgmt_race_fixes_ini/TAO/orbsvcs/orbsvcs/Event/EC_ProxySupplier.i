// $Id$

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
