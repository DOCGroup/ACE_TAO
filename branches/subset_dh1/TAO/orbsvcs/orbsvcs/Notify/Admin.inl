// $Id$

ACE_INLINE
TAO_Notify_EventChannel *
TAO_Notify_Admin::event_channel () const
{
  return this->ec_;
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
