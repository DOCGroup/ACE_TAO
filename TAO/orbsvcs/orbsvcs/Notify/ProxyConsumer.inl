// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE bool
TAO_Notify_ProxyConsumer::is_connected () const
{
  return (this->supplier_.get() != nullptr);
}

ACE_INLINE TAO_Notify_Supplier*
TAO_Notify_ProxyConsumer::supplier ()
{
  return this->supplier_.get();
}

ACE_INLINE TAO_Notify_SupplierAdmin&
TAO_Notify_ProxyConsumer::supplier_admin ()
{
  ACE_ASSERT( this->supplier_admin_.get() != nullptr );
  return *this->supplier_admin_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
