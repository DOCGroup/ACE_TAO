// $Id$

ACE_INLINE const TAO_Notify_Property_ThreadPool&
TAO_Notify_QoSProperties::thread_pool (void) const
{
  return this->thread_pool_;
}

ACE_INLINE const TAO_Notify_Property_ThreadPoolLanes&
TAO_Notify_QoSProperties::thread_pool_lane (void) const
{
  return this->thread_pool_lane_;
}

ACE_INLINE const TAO_Notify_Property_Long&
TAO_Notify_QoSProperties::maximum_batch_size (void) const
{
  return this->maximum_batch_size_;
}

ACE_INLINE const TAO_Notify_Property_Time&
TAO_Notify_QoSProperties::pacing_interval (void) const
{
  return this->pacing_interval_;
}
