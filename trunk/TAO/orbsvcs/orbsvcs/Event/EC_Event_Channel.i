// $Id$

ACE_INLINE TAO_EC_Dispatching*
TAO_EC_Event_Channel::dispatching (void) const
{
  this->dispatching_;
}

ACE_INLINE TAO_EC_Filter_Builder*
TAO_EC_Event_Channel::filter_builder (void) const
{
  this->filter_builder_;
}

ACE_INLINE TAO_EC_ConsumerAdmin*
TAO_EC_Event_Channel::consumer_admin (void) const
{
  this->consumer_admin_;
}

ACE_INLINE TAO_EC_SupplierAdmin*
TAO_EC_Event_Channel::supplier_admin (void) const
{
  this->supplier_admin_;
}

ACE_INLINE TAO_EC_Timer_Module*
TAO_EC_Event_Channel::timer_module (void) const
{
  this->timer_module_;
}


