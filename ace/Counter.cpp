/* $Id$ */
#include "Counter.h"

#if ! defined (__ACE_INLINE__)
#include "Counter.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, Object_Counter, "Object_Counter,v 1.0 2004/02/19 10:24:13 storri Exp")

ACE_Object_Counter::ACE_Object_Counter ()
  : m_counter (0)
{
}

ACE_Queue_Counter::ACE_Queue_Counter ()
  : m_counter (0)
{
}

ACE_Object_Counter::object_id
ACE_Object_Counter::increment(void)
{
  ACE_Object_Counter::object_id oid;
  oid.id = m_counter++;
  oid.tid = ACE_OS::thr_self();
  oid.queue_id = 0;
  ACE_DEBUG((LM_DEBUG,"(%t) Counter created: %d:%d:%d\n", oid.id, oid.tid, oid.queue_id));
  return oid;
}

uint32_t
ACE_Queue_Counter::increment(void)
{
  return m_counter++;
}
