// $Id$

ACE_INLINE
TAO_EC_Priority_Scheduling::TAO_EC_Priority_Scheduling (
       RtecScheduler::Scheduler_ptr scheduler)
  :  scheduler_ (RtecScheduler::Scheduler::_duplicate (scheduler))
{
}
