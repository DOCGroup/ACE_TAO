/* $Id$ */
#include "Counter.h"

ACE_RCSID(Event, Object_Counter, "Object_Counter,v 1.0 2004/02/19 10:24:13 storri Exp")

Object_ID::Object_ID ()
  : id (0),
    tid (0),
    queue_id (0),
    type (0),
    task_id (0),
    guid (0)
{
}

ACE_Object_Counter::ACE_Object_Counter ()
  : m_counter (1)
{
}

ACE_Queue_Counter::ACE_Queue_Counter ()
  : m_counter (1)
{
}

Object_ID
ACE_Object_Counter::increment(void)
{
  Object_ID oid;
  oid.id = m_counter++;
  oid.tid = ACE_OS::thr_self();
  oid.pid = ACE_OS::getpid();
  return oid;
}

uint32_t
ACE_Queue_Counter::increment(void)
{
  return m_counter++;
}
