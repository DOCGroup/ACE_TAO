// ============================================================================
//
// $Id$
//
// ============================================================================

ACE_INLINE
ACE_RT_Info::ACE_RT_Info (const char* entry_point_,
			  RtecScheduler::Time worst_time_,
			  RtecScheduler::Time typical_time_,
			  RtecScheduler::Time cached_time_,
			  RtecScheduler::Period period_,
			  RtecScheduler::Importance importance_,
			  RtecScheduler::Quantum quantum_,
			  CORBA::Long threads_)
{
  this->entry_point = entry_point_;
  this->worst_case_execution_time = worst_time_;
  this->typical_execution_time = typical_time_;
  this->cached_execution_time = cached_time_;
  this->period = period_;
  this->importance = importance_;
  this->quantum = quantum_;
  this->threads = threads_;
}

ACE_INLINE
ACE_RT_Info::ACE_RT_Info (const RtecScheduler::RT_Info& rt_info)
  :  RtecScheduler::RT_Info (rt_info)
{
}

