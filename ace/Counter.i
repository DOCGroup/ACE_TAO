/* $Id$ */

ACE_INLINE
ACE_Object_Counter::object_id
ACE_Object_Counter::increment(void)
{
  ACE_Object_Counter::object_id oid;
  oid.id = m_counter++;
  oid.tid = ACE_OS::thr_self();
  return oid;
}

ACE_INLINE
uint32_t
ACE_Queue_Counter::increment(void)
{
  return m_counter++;
}
