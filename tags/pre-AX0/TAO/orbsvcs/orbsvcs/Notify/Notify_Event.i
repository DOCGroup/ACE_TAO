// $Id$

#include "Notify_Event.h"

ACE_INLINE CORBA::Short
TAO_Notify_Event::event_reliability (void)
{
  return this->event_reliability_;
}

ACE_INLINE void
TAO_Notify_Event::event_reliability (CORBA::Short event_reliability)
{
  this->event_reliability_ = event_reliability;
}

ACE_INLINE CORBA::Short
TAO_Notify_Event::priority (void)
{
  return this->priority_;
}

ACE_INLINE void
TAO_Notify_Event::priority (CORBA::Short priority)
{
  this->priority_ = priority;
}

ACE_INLINE TimeBase::UtcT
TAO_Notify_Event::start_time (void)
{
  return this->start_time_;
}

ACE_INLINE void
TAO_Notify_Event::start_time (TimeBase::UtcT start_time)
{
  this->start_time_ = start_time;
}

ACE_INLINE TimeBase::UtcT
TAO_Notify_Event::stop_time (void)
{
  return this->stop_time_;
}

ACE_INLINE void
TAO_Notify_Event::stop_time (TimeBase::UtcT stop_time)
{
  this->stop_time_ = stop_time;
}

ACE_INLINE TimeBase::TimeT
TAO_Notify_Event::timeout (void)
{
  return this->timeout_;
}

ACE_INLINE void
TAO_Notify_Event::timeout (TimeBase::TimeT timeout)
{
  this->timeout_ = timeout;
}
