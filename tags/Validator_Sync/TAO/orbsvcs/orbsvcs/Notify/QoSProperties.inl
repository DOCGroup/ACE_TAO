// $Id$

ACE_INLINE const TAO_NS_Property_ThreadPool&
TAO_NS_QoSProperties::thread_pool (void) const
{
  return this->thread_pool_;
}

ACE_INLINE const TAO_NS_Property_ThreadPoolLanes&
TAO_NS_QoSProperties::thread_pool_lane (void) const
{
  return this->thread_pool_lane_;
}

ACE_INLINE const TAO_NS_Property_Long&
TAO_NS_QoSProperties::maximum_batch_size (void) const
{
  return this->maximum_batch_size_;
}

ACE_INLINE const TAO_NS_Property_Time&
TAO_NS_QoSProperties::pacing_interval (void) const
{
  return this->pacing_interval_;
}
