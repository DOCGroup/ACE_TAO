// $Id$

ACE_INLINE CORBA::Boolean
TAO_EC_ProxyPushConsumer::is_connected_i (void) const
{
  return !CORBA::is_nil (this->supplier_.in ());
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

ACE_INLINE const RtecEventChannelAdmin::SupplierQOS&
TAO_EC_ProxyPushConsumer::publications (void) const
{
  // @@ TODO There should be a better way to signal errors here.
  static RtecEventChannelAdmin::SupplierQOS empty_qos;
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, empty_qos);

  return this->qos_;
}
