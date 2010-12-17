// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE bool
TAO_Notify_ProxyConsumer::is_connected (void) const
{
  return (this->supplier_.get() != 0);
}

ACE_INLINE TAO_Notify_Supplier*
TAO_Notify_ProxyConsumer::supplier (void)
{
  return this->supplier_.get();
}

ACE_INLINE TAO_Notify_SupplierAdmin&
TAO_Notify_ProxyConsumer::supplier_admin (void)
{
  ACE_ASSERT( this->supplier_admin_.get() != 0 );
  return *this->supplier_admin_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
