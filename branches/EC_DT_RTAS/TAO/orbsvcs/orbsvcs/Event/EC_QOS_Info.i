// $Id$

ACE_INLINE
TAO_EC_QOS_Info::TAO_EC_QOS_Info (void)
  :  rt_info (-1),
     preemption_priority (0),
     timer_id_ (-1)
{
}

ACE_INLINE
TAO_EC_QOS_Info::TAO_EC_QOS_Info (const TAO_EC_QOS_Info &rhs)
  :  rt_info (rhs.rt_info),
     preemption_priority (rhs.preemption_priority),
     timer_id_ (rhs.timer_id_)
{
}
