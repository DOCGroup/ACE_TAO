// $Id$

ACE_INLINE CORBA::Boolean
TAO_EC_ProxyPushSupplier::is_connected (void) const
{
  return !CORBA::is_nil (this->consumer_.in ());
}

ACE_INLINE CORBA::Boolean
TAO_EC_ProxyPushSupplier::is_suspended (void) const
{
  return this->suspended_;
}

ACE_INLINE RtecEventComm::PushConsumer_ptr
TAO_EC_ProxyPushSupplier::consumer (void) const
{
  return RtecEventComm::PushConsumer:_duplicate (this->consumer_);
}

ACE_INLINE const RtecEventChannelAdmin::ConsumerQOS&
TAO_EC_ProxyPushSupplier::subscriptions (void) const
{
  return this->qos_;
}
