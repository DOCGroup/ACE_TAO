// $Id$

ACE_INLINE CORBA::Boolean
TAO_EC_ProxyPushConsumer::is_connected_i (void) const
{
  return this->connected_;
}

ACE_INLINE CORBA::Boolean
TAO_EC_ProxyPushConsumer::is_connected (void) const
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);

  return this->is_connected_i ();
}

ACE_INLINE RtecEventComm::PushSupplier_ptr
TAO_EC_ProxyPushConsumer::supplier (void) const
{
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);
  return RtecEventComm::PushSupplier::_duplicate (this->supplier_.in ());
}

ACE_INLINE void
TAO_EC_ProxyPushConsumer::supplier_i (RtecEventComm::PushSupplier_ptr supplier)
{
  this->supplier_ = supplier;
}

ACE_INLINE void
TAO_EC_ProxyPushConsumer::supplier (RtecEventComm::PushSupplier_ptr supplier)
{
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  this->supplier_i (supplier);
}

ACE_INLINE const RtecEventChannelAdmin::SupplierQOS&
TAO_EC_ProxyPushConsumer::publications (void) const
{
  // @@ TODO There should some way to signal errors here.
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, this->qos_);

  return this->qos_;
}

ACE_INLINE const RtecEventChannelAdmin::SupplierQOS&
TAO_EC_ProxyPushConsumer::publications_i (void) const
{
  return this->qos_;
}

ACE_INLINE TAO_EC_Supplier_Filter *
TAO_EC_ProxyPushConsumer::filter_i (void) const
{
  return this->filter_;
}

// ****************************************************************

ACE_INLINE int
TAO_EC_ProxyPushConsumer_Guard::locked (void) const
{
  return this->locked_;
}
