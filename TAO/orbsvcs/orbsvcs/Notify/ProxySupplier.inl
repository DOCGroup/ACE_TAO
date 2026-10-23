// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE bool
TAO_Notify_ProxySupplier::is_connected () const
{
  return ( this->consumer_.get() != nullptr );
}

ACE_INLINE TAO_Notify_Consumer*
TAO_Notify_ProxySupplier::consumer ()
{
  return this->consumer_.get();
}

ACE_INLINE TAO_Notify_ConsumerAdmin&
TAO_Notify_ProxySupplier::consumer_admin ()
{
  ACE_ASSERT( this->consumer_admin_.get() != nullptr );
  return *this->consumer_admin_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
