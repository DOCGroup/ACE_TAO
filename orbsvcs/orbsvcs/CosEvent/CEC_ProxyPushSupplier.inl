// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE CORBA::Boolean
TAO_CEC_ProxyPushSupplier::is_connected_i (void) const
{
#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  if (this->is_typed_ec ())
  {
    return !CORBA::is_nil (this->typed_consumer_.in ());
  }
  else
  {
  return !CORBA::is_nil (this->consumer_.in ());
  }
#else
  return !CORBA::is_nil (this->consumer_.in ());
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
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

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  if (this->is_typed_ec ())
  {
    return this->typed_consumer_.in ();
  }
  else
  {
    return this->consumer_.in ();
  }
#else
  return this->consumer_.in ();
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */
}

ACE_INLINE void
TAO_CEC_ProxyPushSupplier::consumer_i (CosEventComm::PushConsumer_ptr consumer)
{
  this->consumer_ = consumer;
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
ACE_INLINE void
TAO_CEC_ProxyPushSupplier::consumer_i (CosTypedEventComm::TypedPushConsumer_ptr typed_consumer)
{
  this->typed_consumer_ = typed_consumer;
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

ACE_INLINE void
TAO_CEC_ProxyPushSupplier::consumer (CosEventComm::PushConsumer_ptr consumer)
{
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  this->consumer_i (consumer);
}

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
ACE_INLINE void
TAO_CEC_ProxyPushSupplier::consumer (CosTypedEventComm::TypedPushConsumer_ptr typed_consumer)
{
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  this->consumer_i (typed_consumer);
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
ACE_INLINE CORBA::Boolean
TAO_CEC_ProxyPushSupplier::is_typed_ec (void) const
{
  if (this->typed_event_channel_ != 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

TAO_END_VERSIONED_NAMESPACE_DECL
