/* $Id$ */
#include "Counter.h"

ACE_RCSID(Event, Object_Counter, "Object_Counter,v 1.0 2004/02/19 10:24:13 storri Exp")

ACE_Object_Counter::ACE_Object_Counter ()
  : m_counter (1)
{
}

ACE_Queue_Counter::ACE_Queue_Counter ()
  : m_counter (1)
{
}

ACE_Object_Counter::object_id
ACE_Object_Counter::increment(void)
{
  ACE_Object_Counter::object_id oid;
  oid.id = m_counter++;
  oid.tid = ACE_OS::thr_self();
  oid.queue_id = 0;
  ACE_DEBUG ((LM_DEBUG, "Counter created (%t): %u:%u:%u\n", oid.id, oid.tid, oid.queue_id));
  return oid;
}

uint32_t
ACE_Queue_Counter::increment(void)
{
  return m_counter++;
}
