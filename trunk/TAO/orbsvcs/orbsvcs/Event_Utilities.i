/* -*- C++ -*- */
// $Id$


ACE_INLINE int
ACE_ConsumerQOS_Factory::insert (RtecEventComm::EventSourceID source,
				 RtecEventComm::EventType type,
				 RtecScheduler::handle_t rt_info)
{
  RtecEventChannelAdmin::Dependency dependency;
  dependency.event_.source_ = source;
  dependency.event_.type_ = type;
  //dependency.event_.creation_time_ = 0;
  //dependency.event_.ec_recv_time_ = 0;
  //dependency.event_.ec_send_time_ = 0;
  dependency.rt_info = rt_info;
  return this->insert (dependency);
}

ACE_INLINE int
ACE_ConsumerQOS_Factory::insert_type (RtecEventComm::EventType type,
				      RtecScheduler::handle_t rt_info)
{
  RtecEventChannelAdmin::Dependency dependency;
  dependency.event_.source_ = 0;
  dependency.event_.type_ = type;
  //dependency.event_.creation_time_ = 0;
  //dependency.event_.ec_recv_time_ = 0;
  //dependency.event_.ec_send_time_ = 0;
  dependency.rt_info = rt_info;
  return this->insert (dependency);
}

ACE_INLINE int
ACE_ConsumerQOS_Factory::insert_source (RtecEventComm::EventSourceID source,
					RtecScheduler::handle_t rt_info)
{
  RtecEventChannelAdmin::Dependency dependency;
  dependency.event_.source_ = source;
  dependency.event_.type_ = ACE_ES_EVENT_ANY;
  //dependency.event_.creation_time_ = 0;
  //dependency.event_.ec_recv_time_ = 0;
  //dependency.event_.ec_send_time_ = 0;
  dependency.rt_info = rt_info;
  return this->insert (dependency);
}

ACE_INLINE int
ACE_ConsumerQOS_Factory::insert_time (RtecEventComm::EventType type,
				      RtecEventComm::Time interval,
				      RtecScheduler::handle_t rt_info)
{
  RtecEventChannelAdmin::Dependency dependency;
  dependency.event_.source_ = 0;
  dependency.event_.type_ = type;
  dependency.event_.creation_time_ = interval;
  //dependency.event_.ec_recv_time_ = 0;
  //dependency.event_.ec_send_time_ = 0;
  dependency.rt_info = rt_info;
  return this->insert (dependency);
}

ACE_INLINE int
ACE_ConsumerQOS_Factory::insert_act (RtecEventComm::EventData act)
{
  RtecEventChannelAdmin::Dependency dependency;
  dependency.event_.source_ = 0;
  dependency.event_.type_ = ACE_ES_EVENT_ACT;
  //dependency.event_.creation_time_ = 0;
  //dependency.event_.ec_recv_time_ = 0;
  //dependency.event_.ec_send_time_ = 0;
  dependency.event_.data_ = act;
  return this->insert (dependency);
}

ACE_INLINE const RtecEventChannelAdmin::ConsumerQOS& 
ACE_ConsumerQOS_Factory::get_ConsumerQOS (void)
{
  return qos_;
}

ACE_INLINE
ACE_ConsumerQOS_Factory::operator const RtecEventChannelAdmin::ConsumerQOS& (void)
{
  return qos_;
}

// ************************************************************

ACE_INLINE const RtecEventChannelAdmin::SupplierQOS& 
ACE_SupplierQOS_Factory::get_SupplierQOS (void)
{
  return qos_;
}

ACE_INLINE
ACE_SupplierQOS_Factory::operator const RtecEventChannelAdmin::SupplierQOS& (void)
{
  return qos_;
}

