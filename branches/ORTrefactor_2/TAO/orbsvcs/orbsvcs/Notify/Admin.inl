// $Id$

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
