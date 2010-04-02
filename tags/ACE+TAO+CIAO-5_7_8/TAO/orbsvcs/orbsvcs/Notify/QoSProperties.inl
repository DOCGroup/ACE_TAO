// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
const TAO_Notify_Property_Short&
TAO_Notify_QoSProperties::event_reliability(void) const
{
  return this->event_reliability_;
}

ACE_INLINE
const TAO_Notify_Property_Short&
TAO_Notify_QoSProperties::connection_reliability(void) const
{
  return this->connection_reliability_;
}

ACE_INLINE
const TAO_Notify_Property_Short&
TAO_Notify_QoSProperties::priority(void) const
{
  return this->priority_;
}

ACE_INLINE
const TAO_Notify_Property_Time&
TAO_Notify_QoSProperties::timeout(void) const
{
  return this->timeout_;
}

ACE_INLINE
const TAO_Notify_Property_Boolean&
TAO_Notify_QoSProperties::stop_time_supported(void) const
{
  return this->stop_time_supported_;
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

ACE_INLINE
const TAO_Notify_Property_Long&
TAO_Notify_QoSProperties::max_events_per_consumer (void) const
{
  return this->max_events_per_consumer_;
}

ACE_INLINE
const TAO_Notify_Property_Short&
TAO_Notify_QoSProperties::discard_policy(void) const
{
  return this->discard_policy_;
}

ACE_INLINE
const TAO_Notify_Property_Short&
TAO_Notify_QoSProperties::order_policy(void) const
{
  return this->order_policy_;
}

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

ACE_INLINE
const TAO_Notify_Property_Time&
TAO_Notify_QoSProperties::blocking_policy(void) const
{
  return this->blocking_policy_;
}

///////////////////////////////////////////////////////////////////////////////

ACE_INLINE
TAO_Notify_Property_Short&
TAO_Notify_QoSProperties::event_reliability(void)
{
  return this->event_reliability_;
}

ACE_INLINE
TAO_Notify_Property_Short&
TAO_Notify_QoSProperties::connection_reliability(void)
{
  return this->connection_reliability_;
}

ACE_INLINE
TAO_Notify_Property_Short&
TAO_Notify_QoSProperties::priority(void)
{
  return this->priority_;
}

ACE_INLINE
TAO_Notify_Property_Time&
TAO_Notify_QoSProperties::timeout(void)
{
  return this->timeout_;
}

ACE_INLINE
TAO_Notify_Property_Boolean&
TAO_Notify_QoSProperties::stop_time_supported(void)
{
  return this->stop_time_supported_;
}

ACE_INLINE TAO_Notify_Property_Long&
TAO_Notify_QoSProperties::maximum_batch_size (void)
{
  return this->maximum_batch_size_;
}

ACE_INLINE TAO_Notify_Property_Time&
TAO_Notify_QoSProperties::pacing_interval (void)
{
  return this->pacing_interval_;
}
ACE_INLINE
TAO_Notify_Property_Long&
TAO_Notify_QoSProperties::max_events_per_consumer (void)
{
  return this->max_events_per_consumer_;
}

ACE_INLINE
TAO_Notify_Property_Short&
TAO_Notify_QoSProperties::discard_policy(void)
{
  return this->discard_policy_;
}

ACE_INLINE
TAO_Notify_Property_Short&
TAO_Notify_QoSProperties::order_policy(void)
{
  return this->order_policy_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
