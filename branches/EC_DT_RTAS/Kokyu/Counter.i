/* $Id$ */
inline Object_Counter::object_id
Object_Counter::increment(void)
{
  Object_Counter::object_id oid;
  oid.id = m_counter++;
  oid.tid = ACE_OS::thr_self();
  return oid;
}

