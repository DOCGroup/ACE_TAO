// $Id$

ACE_INLINE CORBA::Boolean
TAO_CEC_ProxyPullSupplier::is_connected_i (void) const
{
  return this->connected_;
}

ACE_INLINE CORBA::Boolean
TAO_CEC_ProxyPullSupplier::is_connected (void) const
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);

  return this->is_connected_i ();
}

ACE_INLINE CosEventComm::PullConsumer_ptr
TAO_CEC_ProxyPullSupplier::consumer (void) const
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);

  return this->consumer_.in ();
}

ACE_INLINE void
TAO_CEC_ProxyPullSupplier::consumer_i (CosEventComm::PullConsumer_ptr consumer)
{
  this->consumer_ = consumer;
}

ACE_INLINE void
TAO_CEC_ProxyPullSupplier::consumer (CosEventComm::PullConsumer_ptr consumer)
{
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  this->consumer_i (consumer);
}
