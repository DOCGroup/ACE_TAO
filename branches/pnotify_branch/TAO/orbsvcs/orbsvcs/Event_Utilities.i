/* -*- C++ -*- */
// $Id$


ACE_INLINE int
ACE_ConsumerQOS_Factory::insert (RtecEventComm::EventSourceID source,
                                 RtecEventComm::EventType type,
                                 RtecBase::handle_t rt_info)
{
  RtecEventChannelAdmin::Dependency dependency;
  if (this->event_initializer_ != 0)
    (*this->event_initializer_) (dependency.event);
  dependency.event.header.source = source;
  dependency.event.header.type = type;
  //dependency.event.header.creation_time = 0;
  //dependency.event.header.ec_recv_time = 0;
  //dependency.event.header.ec_send_time = 0;
  dependency.rt_info = rt_info;
  return this->insert (dependency);
}

ACE_INLINE int
ACE_ConsumerQOS_Factory::insert_bitmasked_value (CORBA::ULong source_mask,
                                                 CORBA::ULong type_mask,
                                                 CORBA::ULong source_value,
                                                 CORBA::ULong type_value)

{
  RtecEventChannelAdmin::Dependency dependency;
  if (this->event_initializer_ != 0)
    (*this->event_initializer_) (dependency.event);
  dependency.event.header.type = ACE_ES_MASKED_TYPE_DESIGNATOR;
  dependency.event.header.source = 0;
  //dependency.event.header.creation_time = 0;
  //dependency.event.header.ec_recv_time = 0;
  //dependency.event.header.ec_send_time = 0;
  dependency.rt_info = 0;
  this->insert (dependency);

  dependency.event.header.source = source_mask;
  dependency.event.header.type = type_mask;
  this->insert (dependency);

  dependency.event.header.source = source_value;
  dependency.event.header.type = type_value;
  return this->insert (dependency);
}

ACE_INLINE int
ACE_ConsumerQOS_Factory::insert_null_terminator (void)
{
  RtecEventChannelAdmin::Dependency dependency;
  if (this->event_initializer_ != 0)
    (*this->event_initializer_) (dependency.event);
  dependency.event.header.type = ACE_ES_NULL_DESIGNATOR;
  dependency.event.header.source = 0;
  return this->insert (dependency);
}

ACE_INLINE int
ACE_ConsumerQOS_Factory::insert_type (RtecEventComm::EventType type,
                                      RtecBase::handle_t rt_info)
{
  RtecEventChannelAdmin::Dependency dependency;
  if (this->event_initializer_ != 0)
    (*this->event_initializer_) (dependency.event);
  dependency.event.header.source = ACE_ES_EVENT_SOURCE_ANY;
  dependency.event.header.type = type;
  //dependency.event.header.creation_time = 0;
  //dependency.event.header.ec_recv_time = 0;
  //dependency.event.header.ec_send_time = 0;
  dependency.rt_info = rt_info;
  return this->insert (dependency);
}

ACE_INLINE int
ACE_ConsumerQOS_Factory::insert_source (RtecEventComm::EventSourceID source,
                                        RtecBase::handle_t rt_info)
{
  RtecEventChannelAdmin::Dependency dependency;
  if (this->event_initializer_ != 0)
    (*this->event_initializer_) (dependency.event);
  dependency.event.header.source = source;
  dependency.event.header.type = ACE_ES_EVENT_ANY;
  //dependency.event.header.creation_time = 0;
  //dependency.event.header.ec_recv_time = 0;
  //dependency.event.header.ec_send_time = 0;
  dependency.rt_info = rt_info;
  return this->insert (dependency);
}

ACE_INLINE int
ACE_ConsumerQOS_Factory::insert_time (RtecEventComm::EventType type,
                                      RtecEventComm::Time interval,
                                      RtecBase::handle_t rt_info)
{
  RtecEventChannelAdmin::Dependency dependency;
  if (this->event_initializer_ != 0)
    (*this->event_initializer_) (dependency.event);
  dependency.event.header.source = ACE_ES_EVENT_SOURCE_ANY;
  dependency.event.header.type = type;
  dependency.event.header.creation_time = interval;
  //dependency.event.header.ec_recv_time = 0;
  //dependency.event.header.ec_send_time = 0;
  dependency.rt_info = rt_info;
  return this->insert (dependency);
}

ACE_INLINE int
ACE_ConsumerQOS_Factory::insert_act (RtecEventComm::EventData act)
{
  RtecEventChannelAdmin::Dependency dependency;
  if (this->event_initializer_ != 0)
    (*this->event_initializer_) (dependency.event);
  dependency.event.header.source = 0;
  dependency.event.header.type = ACE_ES_EVENT_ACT;
  //dependency.event.header.creation_time = 0;
  //dependency.event.header.ec_recv_time = 0;
  //dependency.event.header.ec_send_time = 0;
  dependency.event.data = act;
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
