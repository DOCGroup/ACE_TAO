// $Id$

ACE_INLINE CORBA::Boolean
TAO_EC_ProxyPushConsumer::is_connected (void) const
{
  return !CORBA::is_nil (this->supplier_.in ());
}

ACE_INLINE RtecEventComm::PushSupplier_ptr
TAO_EC_ProxyPushConsumer::supplier (void) const
{
  return RtecEventComm::PushSupplier::_duplicate (this->supplier_);
}

ACE_INLINE const RtecEventChannelAdmin::SupplierQOS&
TAO_EC_ProxyPushConsumer::publications (void) const
{
  return this->qos_;
}
