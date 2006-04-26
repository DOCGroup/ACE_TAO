// ============================================================================
//
// $Id$
//
// ============================================================================


// Construct a helper class instance from values for
// the fields of the IDL struct it wraps.

ACE_INLINE
ACE_RT_Info::ACE_RT_Info (const char* entry_point_,
                          RtecScheduler::Time worst_time_,
                          RtecScheduler::Time typical_time_,
                          RtecScheduler::Time cached_time_,
                          RtecScheduler::Period_t period_,
                          RtecScheduler::Importance_t importance_,
                          RtecScheduler::Quantum_t quantum_,
                          CORBA::Long threads_)
{
    // Cannot use the initialization list, as these are members of the wrapped base
    // class.  This wrapper class must assign them in the constructor body.
    this->entry_point = entry_point_;
    this->worst_case_execution_time = worst_time_;
    this->typical_execution_time = typical_time_;
    this->cached_execution_time = cached_time_;
    this->period = period_;
    this->importance = importance_;
    this->quantum = quantum_;
    this->threads = threads_;
}


// Construct a helper class instance from the IDL struct it wraps.

ACE_INLINE
ACE_RT_Info::ACE_RT_Info (const RtecScheduler::RT_Info& rt_info)
  :  RtecScheduler::RT_Info (rt_info)
{
}
