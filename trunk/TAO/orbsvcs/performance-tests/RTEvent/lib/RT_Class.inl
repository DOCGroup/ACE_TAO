/**
 * @file RT_Class.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

ACE_INLINE int
RT_Class::prc_sched_class (void) const
{
  return this->prc_sched_class_;
}

ACE_INLINE int
RT_Class::thr_sched_class (void) const
{
  return this->thr_sched_class_;
}

ACE_INLINE int
RT_Class::priority_process (void) const
{
  return this->priority_process_;
}

ACE_INLINE int
RT_Class::priority_high (void) const
{
  return this->priority_high_;
}

ACE_INLINE int
RT_Class::priority_low (void) const
{
  return this->priority_low_;
}
