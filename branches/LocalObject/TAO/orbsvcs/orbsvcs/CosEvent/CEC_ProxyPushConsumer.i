// $Id$

ACE_INLINE CORBA::Boolean
TAO_CEC_ProxyPushConsumer::is_connected_i (void) const
{
  return !CORBA::is_nil (this->supplier_.in ());
}

ACE_INLINE CORBA::Boolean
TAO_CEC_ProxyPushConsumer::is_connected (void) const
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);

  return this->is_connected_i ();
}

ACE_INLINE CosEventComm::PushSupplier_ptr
TAO_CEC_ProxyPushConsumer::supplier (void) const
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
  return CosEventComm::PushSupplier::_duplicate (this->supplier_.in ());
}

ACE_INLINE void
TAO_CEC_ProxyPushConsumer::supplier_i (CosEventComm::PushSupplier_ptr supplier)
{
  this->supplier_ = supplier;
}

ACE_INLINE void
TAO_CEC_ProxyPushConsumer::supplier (CosEventComm::PushSupplier_ptr supplier)
{
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  this->supplier_i (supplier);
}
