// $Id$

// @METRICS@

#ifndef METRICS_UPCALL_MONITOR_T_I
#define METRICS_UPCALL_MONITOR_T_I

//# if defined (METRICS_USES_OP_CANCELLATION) 
//# undef METRICS_USES_OP_CANCELLATION
//# endif /* METRICS_USES_OP_CANCELLATION */

# if ! defined (METRICS_USES_OP_CANCELLATION) 
#   define METRICS_USES_OP_CANCELLATION 
# endif /* ! METRICS_USES_OP_CANCELLATION */


////////////////////////////////////////////
// class TAO_Metrics_UpcallMonitorAdapter //
////////////////////////////////////////////


// Constructor that takes a reference to the actual push consumer
// we're monitoring and an int that indicates whether (non-zero) or
// not (zero) we need to destroy the consumer in our destructor.

template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE
TAO_Metrics_UpcallMonitorAdapter<ACE_LOCK, ALLOCATOR>::
TAO_Metrics_UpcallMonitorAdapter (
  POA_Metrics::QoSMonitor & monitor,
  TAO_Metrics_LocalCache<ACE_LOCK, ALLOCATOR> &cache,
  TAO_Metrics_FrameManager<ACE_LOCK> &frame_manager,
  POA_RtecEventComm::PushConsumer & consumer,
  TAO_String_Manager entry_point,
  RtecScheduler::handle_t handle,
  RtecScheduler::Period_t period,
  RtecScheduler::Time wcet,
  RtecScheduler::Criticality_t criticality,
  int adapter_owns_consumer,
  int cache_register)
  : monitor_ (monitor),
    cache_ (cache),
    frame_manager_ (frame_manager),
    consumer_ (consumer),
    handle_ (handle),
    period_ (period),
    adapter_owns_consumer_ (adapter_owns_consumer)
{
//DCL:Added to remove Linux compile warning
      ACE_UNUSED_ARG(entry_point);

  // Ensure that the high res timer global scale factor
  // is set before any of its static methods are used
  ACE_High_Res_Timer::global_scale_factor ();

  // Store the RT_Info execution time.
  ORBSVCS_Time::TimeT_to_Time_Value(this->execution_time_,
                                    wcet);
  this->is_critical_ = 
    (criticality == RtecScheduler::HIGH_CRITICALITY
     || criticality == RtecScheduler::VERY_HIGH_CRITICALITY)
    ? 1 : 0;

  // Use the type-preserving argument rather than the base class
  // reference which is fine everywhere but here.
  if (cache_register)
    {
	  if (cache.metrics_enabled())
         cache.register_upcall_adapter (this->handle_,
                                        entry_point,
                                        this->is_critical_);
    }
}


// Get the current priod for the adapter.

template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE RtecScheduler::Period_t
TAO_Metrics_UpcallMonitorAdapter<ACE_LOCK, ALLOCATOR>::
get_period ()
{
  return this->period_;
}


// Set the current priod for the adapter.

template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE void
TAO_Metrics_UpcallMonitorAdapter<ACE_LOCK, ALLOCATOR>::
set_period (RtecScheduler::Period_t period)
{
  this->period_ = period;
}


// Do a monitored push to the actual PushConsumer for which we're a proxy
template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE void
TAO_Metrics_UpcallMonitorAdapter<ACE_LOCK, ALLOCATOR>::
push (const RtecEventComm::EventSet & data,
      CORBA::Environment &ACE_TRY_ENV)
{
  // Save stamped deadline.
  ACE_Time_Value deadline_tv;

  // If we get an error, just set the deadline to zero, and possibly
  // complain.
  if (this->frame_manager_.get_end_time (this->period_, deadline_tv) < 0)
    {
      deadline_tv = ACE_Time_Value::zero;
# if defined (METRICS_MONITOR_ERROR_OUTPUT_ENABLED)
      ACE_ERROR ((LM_ERROR,
                  "TAO_Metrics_UpcallMonitorAdapter::push"
                  " get_end_time failed .\n"));
# endif /* METRICS_MONITOR_ERROR_OUTPUT_ENABLED */
    }

# if defined (METRICS_USES_OP_CANCELLATION)

  // Take current time stamp and add expected execution to get
  // projected completion.  Done this way for efficiency, to minimize
  // temporaries and their copies and constructor and destructor calls.
  ACE_Time_Value projected_tv; 
  ACE_hrtime_t hrtime_now = ACE_OS::gethrtime ();
  ACE_High_Res_Timer::hrtime_to_tv (projected_tv,
                                    hrtime_now);
  projected_tv += this->execution_time_;

  // Operation cancellation: uses WCET to determine whether or not
  // deadline will be met. A critical operation (is_critical_) is
  // dispatched unconditionally.  A zero deadline indicates an
  // operation is not deadline-bound and thus is not subject to
  // cancellation.
  if (this->is_critical_ 
      || deadline_tv >= projected_tv
      || deadline_tv == ACE_Time_Value::zero)
    {
# endif /* METRICS_USES_OP_CANCELLATION */

      // Record the start of the upcall in the cache.
#if 0
      if ( this->handle_ != TAO_Metrics_Utils::timeoutEvent ())
#endif /* 0 */
        if (cache_.metrics_enabled())
           cache_.report_upcall_start (this->handle_);

      // Push to the actual consumer.
      consumer_.push (data, ACE_TRY_ENV);

      // Record the end of the upcall in the cache, and report whether
      // the deadline was made or missed to the monitor.

#if 0
      if ( this->handle_ != TAO_Metrics_Utils::timeoutEvent ())
	{
#else /* 1 */
      if (1)
	{
#endif /* 0 */
        if (cache_.metrics_enabled())
        {
          int result = cache_.report_upcall_stop (this->handle_, deadline_tv);
          if (result == 0)
          {
              monitor_.report_made_deadline (this->handle_, ACE_TRY_ENV);
              ACE_CHECK;
          }
          else if (result == 1)
          {
              monitor_.report_missed_deadline (this->handle_, ACE_TRY_ENV);
              ACE_CHECK;
          }
# if defined (METRICS_MONITOR_ERROR_OUTPUT_ENABLED)
          else
	       {
              ACE_ERROR ((LM_ERROR,
                          "TAO_Metrics_UpcallMonitorAdapter::push"
                          " upcall stop reporting failed .\n"));
          }

# endif /* METRICS_MONITOR_ERROR_OUTPUT_ENABLED */
        }
	}
# if defined (METRICS_USES_OP_CANCELLATION)
    }
  else
    {
#  if defined (METRICS_MONITOR_DEBUG_OUTPUT_ENABLED)
      ACE_DEBUG ((LM_INFO,
                  "SRT Operation cancelled .\n"));
#  endif /* METRICS_MONITOR_DEBUG_OUTPUT_ENABLED */

      if (cache_.metrics_enabled())
      {
         // The operation push was cancelled: report the cancellation to
         // the cache and the missed deadline to the monitor.
         cache_.report_upcall_cancellation (this->handle_);
         monitor_.report_missed_deadline (this->handle_, ACE_TRY_ENV);
         ACE_CHECK;
      }
    }

# endif /* METRICS_USES_OP_CANCELLATION */
}

// Delegates to the actual PushConsumer for which we're a proxy.

template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE void
TAO_Metrics_UpcallMonitorAdapter<ACE_LOCK, ALLOCATOR>::
disconnect_push_consumer (CORBA::Environment &ACE_TRY_ENV)
{
  consumer_.disconnect_push_consumer (ACE_TRY_ENV);
}


template <class ACE_LOCK, class ALLOCATOR>
ACE_INLINE
TAO_Metrics_ReportingUpcallMonitorAdapter<ACE_LOCK, ALLOCATOR>::
TAO_Metrics_ReportingUpcallMonitorAdapter (
  POA_Metrics::QoSMonitor & monitor,
  TAO_Metrics_ReportingLocalCache<ACE_LOCK, ALLOCATOR> &cache,
  TAO_Metrics_FrameManager<ACE_LOCK> &frame_manager,
  POA_RtecEventComm::PushConsumer & consumer,
  TAO_String_Manager entry_point,
  RtecScheduler::handle_t handle,
  RtecScheduler::Period_t period,
  RtecScheduler::Time wcet,
  RtecScheduler::Criticality_t criticality,
  int adapter_owns_consumer,
  Metrics::QoSLogger_ptr logger)
  : TAO_Metrics_UpcallMonitorAdapter<ACE_LOCK, ALLOCATOR>
      (monitor,
       cache,
       frame_manager,
       consumer,
       entry_point,
       handle,
       period,
       wcet,
       criticality,
       adapter_owns_consumer,
       0 /* don't register */),
    logger_ (logger)
{
      // Use the type-preserving argument rather than the base class
      // reference which is fine everywhere but here.
      cache.register_upcall_adapter (this->handle_,
                                     entry_point,
                                     this->is_critical_,
                                     this->logger_);
}

#endif /* METRICS_UPCALL_MONITOR_T_I */
