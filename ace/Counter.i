/* $Id$ */

ACE_Object_Counter::object_id
ACE_Object_Counter::increment(void)
{
  ACE_Object_Counter::object_id oid;
  oid.id = m_counter++;
  oid.tid = ACE_OS::thr_self();
  oid.queue_id = 0;
  ACE_DEBUG((LM_INFO,"Counter created: %d:%d:%d", oid.id, oid.tid, oid.queue_id));
  return oid;
}

ACE_INLINE
uint32_t
ACE_Queue_Counter::increment(void)
{
  return m_counter++;
}
