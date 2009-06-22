// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO_Notify_EventChannel *
TAO_Notify_Admin::event_channel () const
{
  return this->ec_.get();
}


ACE_INLINE TAO_Notify_FilterAdmin&
TAO_Notify_Admin::filter_admin (void)
{
  return this->filter_admin_;
}

ACE_INLINE void
TAO_Notify_Admin::filter_operator (CosNotifyChannelAdmin::InterFilterGroupOperator filter_operator)
{
  this->filter_operator_ = filter_operator;
}

ACE_INLINE CosNotifyChannelAdmin::InterFilterGroupOperator
TAO_Notify_Admin::filter_operator (void)
{
  return this->filter_operator_;
}

ACE_INLINE
void
TAO_Notify_Admin::set_default (bool is_default)
{
  this->is_default_ = is_default;
}

ACE_INLINE
bool
TAO_Notify_Admin::is_default (void) const
{
  return this->is_default_;
}

ACE_INLINE TAO_Notify_Admin::TAO_Notify_Proxy_Container&
TAO_Notify_Admin::proxy_container()
{
  ACE_ASSERT( proxy_container_.get() != 0 );
  return *proxy_container_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
