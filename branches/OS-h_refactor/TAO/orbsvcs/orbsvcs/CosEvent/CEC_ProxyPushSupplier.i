// $Id$

ACE_INLINE CORBA::Boolean
TAO_CEC_ProxyPushSupplier::is_connected_i (void) const
{
  return !CORBA::is_nil (this->consumer_.in ());
}

ACE_INLINE CORBA::Boolean
TAO_CEC_ProxyPushSupplier::is_connected (void) const
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);

  return this->is_connected_i ();
}

ACE_INLINE CosEventComm::PushConsumer_ptr
TAO_CEC_ProxyPushSupplier::consumer (void) const
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);

  return this->consumer_.in ();
}

ACE_INLINE void
TAO_CEC_ProxyPushSupplier::consumer_i (CosEventComm::PushConsumer_ptr consumer)
{
  this->consumer_ = consumer;
}

ACE_INLINE void
TAO_CEC_ProxyPushSupplier::consumer (CosEventComm::PushConsumer_ptr consumer)
{
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  this->consumer_i (consumer);
}
