/* $Id$ */
inline EC_Event_Counter::event_id
EC_Event_Counter::increment(void)
{
  EC_Event_Counter::event_id eid;
  eid.id = m_counter++;
  eid.tid = ACE_OS::thr_self();
  return eid;
}

