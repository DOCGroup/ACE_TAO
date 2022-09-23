// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
const TAO_Notify_Property_Short&
TAO_Notify_QoSProperties::event_reliability() const
{
  return this->event_reliability_;
}

ACE_INLINE
const TAO_Notify_Property_Short&
TAO_Notify_QoSProperties::connection_reliability() const
{
  return this->connection_reliability_;
}

ACE_INLINE
const TAO_Notify_Property_Short&
TAO_Notify_QoSProperties::priority() const
{
  return this->priority_;
}

ACE_INLINE
const TAO_Notify_Property_Time&
TAO_Notify_QoSProperties::timeout() const
{
  return this->timeout_;
}

ACE_INLINE
const TAO_Notify_Property_Boolean&
TAO_Notify_QoSProperties::stop_time_supported() const
{
  return this->stop_time_supported_;
}

ACE_INLINE const TAO_Notify_Property_Long&
TAO_Notify_QoSProperties::maximum_batch_size () const
{
  return this->maximum_batch_size_;
}

ACE_INLINE const TAO_Notify_Property_Time&
TAO_Notify_QoSProperties::pacing_interval () const
{
  return this->pacing_interval_;
}

ACE_INLINE
const TAO_Notify_Property_Long&
TAO_Notify_QoSProperties::max_events_per_consumer () const
{
  return this->max_events_per_consumer_;
}

ACE_INLINE
const TAO_Notify_Property_Short&
TAO_Notify_QoSProperties::discard_policy() const
{
  return this->discard_policy_;
}

ACE_INLINE
const TAO_Notify_Property_Short&
TAO_Notify_QoSProperties::order_policy() const
{
  return this->order_policy_;
}

ACE_INLINE const TAO_Notify_Property_ThreadPool&
TAO_Notify_QoSProperties::thread_pool () const
{
  return this->thread_pool_;
}

ACE_INLINE const TAO_Notify_Property_ThreadPoolLanes&
TAO_Notify_QoSProperties::thread_pool_lane () const
{
  return this->thread_pool_lane_;
}

ACE_INLINE
const TAO_Notify_Property_Time&
TAO_Notify_QoSProperties::blocking_policy() const
{
  return this->blocking_policy_;
}

///////////////////////////////////////////////////////////////////////////////

ACE_INLINE
TAO_Notify_Property_Short&
TAO_Notify_QoSProperties::event_reliability()
{
  return this->event_reliability_;
}

ACE_INLINE
TAO_Notify_Property_Short&
TAO_Notify_QoSProperties::connection_reliability()
{
  return this->connection_reliability_;
}

ACE_INLINE
TAO_Notify_Property_Short&
TAO_Notify_QoSProperties::priority()
{
  return this->priority_;
}

ACE_INLINE
TAO_Notify_Property_Time&
TAO_Notify_QoSProperties::timeout()
{
  return this->timeout_;
}

ACE_INLINE
TAO_Notify_Property_Boolean&
TAO_Notify_QoSProperties::stop_time_supported()
{
  return this->stop_time_supported_;
}

ACE_INLINE TAO_Notify_Property_Long&
TAO_Notify_QoSProperties::maximum_batch_size ()
{
  return this->maximum_batch_size_;
}

ACE_INLINE TAO_Notify_Property_Time&
TAO_Notify_QoSProperties::pacing_interval ()
{
  return this->pacing_interval_;
}
ACE_INLINE
TAO_Notify_Property_Long&
TAO_Notify_QoSProperties::max_events_per_consumer ()
{
  return this->max_events_per_consumer_;
}

ACE_INLINE
TAO_Notify_Property_Short&
TAO_Notify_QoSProperties::discard_policy()
{
  return this->discard_policy_;
}

ACE_INLINE
TAO_Notify_Property_Short&
TAO_Notify_QoSProperties::order_policy()
{
  return this->order_policy_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
