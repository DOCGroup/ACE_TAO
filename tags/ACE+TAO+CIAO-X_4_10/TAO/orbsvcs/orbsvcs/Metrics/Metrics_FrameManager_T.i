// $Id$

#ifndef METRICS_FRAME_MANAGER_T_I
#define METRICS_FRAME_MANAGER_T_I


////////////////////////////////////
// Class TAO_Metrics_FrameManager //
////////////////////////////////////


// Default constructor.

template <class ACE_LOCK>
ACE_INLINE
TAO_Metrics_FrameManager<ACE_LOCK>::
TAO_Metrics_FrameManager ()
  : period_count_ (0)
{
	  // Ensure that the high res timer global scale factor
	  // is set before any of its static methods are used
	  ACE_High_Res_Timer::global_scale_factor ();
}

// Destructor.  Note: this is *not* thread safe.  If we're destroying
// the frame manager then there had better not be any threads in the
// manager anyway.  If a use case requires thread safety at
// destruction, then some kind of external reader-writer locking and
// barrier synchronization scheme will be needed to make this work
// efficiently.

template <class ACE_LOCK>
ACE_INLINE
TAO_Metrics_FrameManager<ACE_LOCK>::
~TAO_Metrics_FrameManager ()
{
  // Iterate through and delete the data structures in the map
  METRICS_FRAME_DATA_MAP_ITERATOR iter (frame_data_map_);
  while (iter.done () == 0)
    {
      delete (*iter).int_id_;
      ++iter;
    }
}


// Binds the period to the internal frame map, with a new frame data
// structure.  Returns 0 if a new entry is bound successfully, returns
// 1 if an attempt is made to bind an existing entry, and returns -1
// if failures occur.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
register_period (RtecScheduler::Period_t p,
                 CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL,
                     Metrics::SYNCHRONIZATION))
{
  // Treat a zero period as a special case: no need to register, but
  // no harm if someone tries.
  if (p == 0)
    {
      return 0;
    }

  // Synchronize read/write access to the map while the new period's
  // frame is being installed.
  ACE_Write_Guard<ACE_LOCK> mon (this->lock_);
  if (mon.locked () == 0)
    {
      ACE_THROW_RETURN (Metrics::SYNCHRONIZATION (), -1);
    }

  int result = 0;
  TAO_Metrics_Frame_Manager_Data<ACE_LOCK> * data = 0;
  if (frame_data_map_.find (p) != 0)
    {
      ACE_NEW_RETURN (data, TAO_Metrics_Frame_Manager_Data<ACE_LOCK> (p), -1);
      result = frame_data_map_.trybind (p, data);
      if (result == 0)
        {
          // All is well: count the new period.
          ++period_count_;
        }
      else
        {
		  delete data;

          // We failed to bind: throw an exception.
          ACE_THROW_RETURN (Metrics::INTERNAL (), -1);
        }
    }

  return result;
}

// Updates time frames, based on the passed time value.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
update_all_frames (const ACE_Time_Value& tv)
{
  // Synchronize read/write access to the map while the new frames are
  // being computed.
  ACE_Write_Guard<ACE_LOCK> mon (this->lock_);
  if (mon.locked () == 0)
    {
      return -1;
    }

  METRICS_FRAME_DATA_MAP_ITERATOR iter (frame_data_map_);
  while (iter.done () == 0)
    {
      update_data (*((*iter).int_id_), tv);
      ++iter;
    }
  return 0;
}


// Updates time frames, based on the passed time value.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
update_all_frames_with_time (Metrics::Time mt,
                             CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::SYNCHRONIZATION))
{
  // Reentrant method that calls locked method.
  ACE_Time_Value tv;
  ORBSVCS_Time::TimeT_to_Time_Value(tv, mt);
  if (this->update_all_frames (tv) < 0)
    {
      ACE_THROW_RETURN (Metrics::SYNCHRONIZATION (), -1);
    }

  return 0;
}

// Updates time frames, based on the current time as of the call.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
update_all_frames (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::SYNCHRONIZATION))
{
  // Synchronize read/write access to the map while the new frames are
  // being computed.
  ACE_Write_Guard<ACE_LOCK> mon (this->lock_);
  if (mon.locked () == 0)
    {
      ACE_THROW_RETURN (Metrics::SYNCHRONIZATION (), -1);
    }

  METRICS_FRAME_DATA_MAP_ITERATOR iter (frame_data_map_);

  // Grab the common time stamp as late as possible, i.e., after all
  // locks are held.
  ACE_Time_Value tv;
  ACE_hrtime_t hrtime_now = ACE_OS::gethrtime ();
  ACE_High_Res_Timer::hrtime_to_tv (tv, hrtime_now);

  while (iter.done () == 0)
    {
      update_data (*((*iter).int_id_), tv);
      ++iter;
    }
  return 0;
}


// Updates the passed period's time frame, based on the passed time
// value.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
update_frame (RtecScheduler::Period_t p, const ACE_Time_Value& tv)
{
  // Treat a zero period as a special case.
  if (p == 0)
    {
      return 0;
    }

  TAO_Metrics_Frame_Manager_Data<ACE_LOCK> * data;

  // Synchronize write access to the map during pointer lookup only
  ACE_Read_Guard<ACE_LOCK> mon (this->lock_);
  if (mon.locked () == 0)
    {
      return -1;
    }

  if (frame_data_map_.find (p, data) == 0 && data != 0)
    {
      mon.release ();

      // Synchronize read/write access to the specific data structure
      // instance during update
      ACE_Write_Guard<ACE_LOCK> data_mon (data->lock_);
      if (data_mon.locked () == 0)
        {
          return -1;
        }
      return update_data (*data, tv);
    }

  return -2;
}

// Updates the passed period's time frame, based on the passed time
// value.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
update_frame_with_time (RtecScheduler::Period_t p, Metrics::Time mt,
                        CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL,
                     Metrics::SYNCHRONIZATION))
{
  // Treat a zero period as a special case.
  if (p == 0)
    {
      return 0;
    }

  // Reentrant method that calls locked method.
  ACE_Time_Value tv;
  ORBSVCS_Time::TimeT_to_Time_Value(tv, mt);
  switch (this->update_frame (p, tv))
    {
      case -1: ACE_THROW_RETURN (Metrics::SYNCHRONIZATION (), -1);
      case -2: ACE_THROW_RETURN (Metrics::INTERNAL (), -1);
      default: return 0;      
    }
}


// Updates the passed period's time frame, based on the current time
// as of the call.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
update_frame (RtecScheduler::Period_t p, CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL,
                     Metrics::SYNCHRONIZATION))
{
  // Treat a zero period as a special case.
  if (p == 0)
    {
      return 0;
    }

  TAO_Metrics_Frame_Manager_Data<ACE_LOCK> * data;

  // Synchronize write access to the map during pointer lookup only
  ACE_Read_Guard<ACE_LOCK> mon (this->lock_);
  if (mon.locked () == 0)
    {
      ACE_THROW_RETURN (Metrics::SYNCHRONIZATION (), -1);
    }

  if (frame_data_map_.find (p, data) == 0 && data != 0)
    {
      mon.release ();

      // Synchronize read/write access to the specific data structure instance during
      // update
      ACE_Write_Guard<ACE_LOCK> data_mon (data->lock_);
      if (data_mon.locked () == 0)
        {
          ACE_THROW_RETURN (Metrics::SYNCHRONIZATION (), -1);
        }

      // Grab the time stamp as late as possible, i.e., after all locks
      // are held.
      ACE_Time_Value tv;
      ACE_hrtime_t hrtime_now = ACE_OS::gethrtime ();
      ACE_High_Res_Timer::hrtime_to_tv (tv, hrtime_now);

      return update_data (*data, tv);
    }

  ACE_THROW_RETURN (Metrics::INTERNAL (), -1);
}


// Resets time frames, based on the passed time value.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
reset_all_frames (const ACE_Time_Value& tv)
{
  // Synchronize read/write access to the map during the reset
  ACE_Write_Guard<ACE_LOCK> mon (this->lock_);
  if (mon.locked () == 0)
    {
      return -1;
    }

  METRICS_FRAME_DATA_MAP_ITERATOR iter (frame_data_map_);
  while (iter.done () == 0)
    {
      reset_data (*((*iter).int_id_), tv);
      ++iter;
    }
  return 0;
}


// Resets time frames, based on the passed time value.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
reset_all_frames_with_time (Metrics::Time mt,
                            CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::SYNCHRONIZATION))
{
  // Reentrant method that calls locked method.
  ACE_Time_Value tv;
  ORBSVCS_Time::TimeT_to_Time_Value(tv, mt);
  if (this->reset_all_frames (tv) < 0)
    {
      ACE_THROW_RETURN (Metrics::SYNCHRONIZATION (), -1);
    }

  return 0;
}

// Resets time frames, based on the current time as of the call.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
reset_all_frames (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::SYNCHRONIZATION))
{
  // Synchronize read/write access to the map during the reset
  ACE_Write_Guard<ACE_LOCK> mon (this->lock_);
  if (mon.locked () == 0)
    {
      ACE_THROW_RETURN (Metrics::SYNCHRONIZATION (), -1);
    }

  METRICS_FRAME_DATA_MAP_ITERATOR iter (frame_data_map_);

  // Grab the time stamp as late as possible, i.e., after all locks
  // are held.
  ACE_Time_Value tv;
  ACE_hrtime_t hrtime_now = ACE_OS::gethrtime ();
  ACE_High_Res_Timer::hrtime_to_tv (tv, hrtime_now);

  while (iter.done () == 0)
    {
      reset_data (* ((*iter).int_id_), tv);
      ++iter;
    }
  return 0;
}


// Resets the passed period's start-of-frame, based on the passed time
// value.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
reset_frame (RtecScheduler::Period_t p, const ACE_Time_Value& tv)
{
  // Treat a zero period as a special case.
  if (p == 0)
    {
      return 0;
    }

  TAO_Metrics_Frame_Manager_Data<ACE_LOCK> * data;

  // Synchronize write access to the map during lookup only
  ACE_Read_Guard<ACE_LOCK> mon (this->lock_);
  if (mon.locked () == 0)
    {
      return -1;
    }

  if (frame_data_map_.find (p, data) == 0 && data != 0)
    {
      mon.release ();

      // Synchronize read/write access to the individual data
      // structure instance during reset
      ACE_Write_Guard<ACE_LOCK> data_mon (data->lock_);
      if (data_mon.locked () == 0)
        {
          return -1;
        }

      return reset_data (*data, tv);
    }

  return -2;
}

// Resets the passed period's start-of-frame, based on the passed time
// value.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
reset_frame_with_time (RtecScheduler::Period_t p, Metrics::Time mt,
                       CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL,
                     Metrics::SYNCHRONIZATION))
{
  // Treat a zero period as a special case.
  if (p == 0)
    {
      return 0;
    }

  // Reentrant method that calls locked method.
  ACE_Time_Value tv;
  ORBSVCS_Time::TimeT_to_Time_Value(tv, mt);
  switch (this->reset_frame (p, tv))
    {
      case -1: ACE_THROW_RETURN (Metrics::SYNCHRONIZATION (), -1);
      case -2: ACE_THROW_RETURN (Metrics::INTERNAL (), -1);
      default: return 0;      
    }
}


// Resets the passed period's start-of-frame, based on the current
// time as of the call.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
reset_frame (RtecScheduler::Period_t p, CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL,
                     Metrics::SYNCHRONIZATION))
{
  // Treat a zero period as a special case.
  if (p == 0)
    {
      return 0;
    }

  TAO_Metrics_Frame_Manager_Data<ACE_LOCK> * data;

  // Synchronize write access to the map during lookup only
  ACE_Read_Guard<ACE_LOCK> mon (this->lock_);
  if (mon.locked () == 0)
    {
      ACE_THROW_RETURN (Metrics::SYNCHRONIZATION (), -1);
    }

  if (frame_data_map_.find (p, data) == 0 && data != 0)
    {
      mon.release ();

      // Synchronize read/write access to the individual data
      // structure instance during reset
      ACE_Write_Guard<ACE_LOCK> data_mon (data->lock_);
      if (data_mon.locked () == 0)
        {
          ACE_THROW_RETURN (Metrics::SYNCHRONIZATION (), -1);
        }

      // Grab the time stamp as late as possible, i.e., after all locks
      // are held.
    ACE_Time_Value tv;
    ACE_hrtime_t hrtime_now = ACE_OS::gethrtime ();
    ACE_High_Res_Timer::hrtime_to_tv (tv, hrtime_now);

    if (data == 0)
      {
        ACE_THROW_RETURN (Metrics::INTERNAL (), -1);
      }

    return reset_data (*data, tv);
  }

  ACE_THROW_RETURN (Metrics::INTERNAL (), -1);
}


// Gets the start time for the period's current frame.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
get_start_time (RtecScheduler::Period_t p, ACE_Time_Value &start)
{
  // Treat a zero period as a special case.
  if (p == 0)
    {
      start = ACE_Time_Value::zero;
      return 0;
    }

  TAO_Metrics_Frame_Manager_Data<ACE_LOCK> * data;

  // Synchronize write access to the map during lookup only
  ACE_Read_Guard<ACE_LOCK> mon (this->lock_);
  if (mon.locked () == 0)
    {
      return -1;
    }

  if (frame_data_map_.find (p, data) == 0 && data != 0)
    {
      mon.release ();

      // Synchronize write access to the individual data structure instance
      ACE_Read_Guard<ACE_LOCK> data_mon (data->lock_);
      if (data_mon.locked () == 0)
        {
          return -1;
        }

      start.set (data->start_.sec (), data->start_.usec ());
      return 0;
    }

  return -2;
}

// Gets the start time for the period's current frame.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
get_start_time (RtecScheduler::Period_t p, Metrics::Time &start,
                CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL,
                     Metrics::SYNCHRONIZATION))
{
  // Treat a zero period as a special case.
  if (p == 0)
    {
      start = 0;
      return 0;
    }

  // Reentrant method that calls locked method.
  ACE_Time_Value tv;
  switch (this->get_start_time (p, tv))
    {
      case -1: ACE_THROW_RETURN (Metrics::SYNCHRONIZATION (), -1);
      case -2: ACE_THROW_RETURN (Metrics::INTERNAL (), -1);
      default: ORBSVCS_Time::Time_Value_to_TimeT (start, tv);
               return 0;      
    }
}


// Gets the end time for the period's current frame.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
get_end_time (RtecScheduler::Period_t p, ACE_Time_Value &end)
{
  // Treat a zero period as a special case.
  if (p == 0)
    {
      end = ACE_Time_Value::zero;
      return 0;
    }

  TAO_Metrics_Frame_Manager_Data<ACE_LOCK> * data = 0;

  // Synchronize write access to the map during lookup only
  ACE_Read_Guard<ACE_LOCK> mon (this->lock_);
  if (mon.locked () == 0)
    {
      return -1;
    }

  if (frame_data_map_.find (p, data) == 0 && data != 0)
    {
      mon.release ();

      // Synchronize write access to the individual data structure instance
      ACE_Read_Guard<ACE_LOCK> data_mon (data->lock_);
      if (data_mon.locked () == 0)
        {
          return -1;
        }

      end.set (data->end_.sec (), data->end_.usec ());
      return 0;
    }

  return -2;
}


// Gets the end time for the period's current frame.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
get_end_time (RtecScheduler::Period_t p, Metrics::Time &end,
              CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL,
                     Metrics::SYNCHRONIZATION))
{
  // Treat a zero period as a special case.
  if (p == 0)
    {
      end = 0;
      return 0;
    }

  // Reentrant method that calls locked method.
  ACE_Time_Value tv;
  switch (this->get_end_time (p, tv))
    {
      case -1: ACE_THROW_RETURN (Metrics::SYNCHRONIZATION (), -1);
      case -2: ACE_THROW_RETURN (Metrics::INTERNAL (), -1);
      default: ORBSVCS_Time::Time_Value_to_TimeT (end, tv);
               return 0;      
    }
}

// Gets the locally unique identifier for the period's current frame.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
get_frame_id (RtecScheduler::Period_t p,
              CORBA::ULong &id,
              CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL,
                     Metrics::SYNCHRONIZATION))
{
  // Treat a zero period as a special case.
  if (p == 0)
    {
      id = 0;
      return 0;
    }

  TAO_Metrics_Frame_Manager_Data<ACE_LOCK> * data;

  // Synchronize write access to the map during lookup only
  ACE_Read_Guard<ACE_LOCK> mon (this->lock_);
  if (mon.locked () == 0)
    {
      ACE_THROW_RETURN (Metrics::SYNCHRONIZATION (), -1);
    }

  if (frame_data_map_.find (p, data) == 0 && data != 0)
    {
      mon.release ();

      // Synchronize write access to the individual data structure instance
      ACE_Read_Guard<ACE_LOCK> data_mon (data->lock_);
      if (data_mon.locked () == 0)
        {
          ACE_THROW_RETURN (Metrics::SYNCHRONIZATION (), -1);
        }

      id = data->frame_id_;
      return 0;
    }

  ACE_THROW_RETURN (Metrics::INTERNAL (), -1);
}


// Gets the start time, end time, and id period's current frame.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
get_frame_data (RtecScheduler::Period_t p, ACE_Time_Value &start,
                ACE_Time_Value &end, CORBA::ULong &id)
{
  // Treat a zero period as a special case.
  if (p == 0)
    {
      start = ACE_Time_Value::zero;
      end = ACE_Time_Value::zero;
      id = 0;
      return 0;
    }

  TAO_Metrics_Frame_Manager_Data<ACE_LOCK> * data;

  // Synchronize write access to the map during lookup only
  ACE_Read_Guard<ACE_LOCK> mon (this->lock_);
  if (mon.locked () == 0)
    {
      return -1;
    }

  if (frame_data_map_.find (p, data) == 0 && data != 0)
    {
      mon.release ();

      // Synchronize write access to the individual data structure instance
      ACE_Read_Guard<ACE_LOCK> data_mon (data->lock_);
      if (data_mon.locked () == 0)
        {
          return -1;
        }

      start.set (data->start_.sec (), data->start_.usec ());
      end.set (data->end_.sec (), data->end_.usec ());
      id = data->frame_id_;
      return 0;
    }

  return -2;
}



// Gets the start time, end time, and id period's current frame.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
get_frame_data (RtecScheduler::Period_t p, Metrics::Time &start,
                Metrics::Time &end, CORBA::ULong &id,
                 CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Metrics::INTERNAL,
                     Metrics::SYNCHRONIZATION))
{
  // Treat a zero period as a special case.
  if (p == 0)
    {
      start = 0;
      end = 0;
      id = 0;
      return 0;
    }

  // Reentrant method that calls locked method.
  ACE_Time_Value start_tv, end_tv;
  switch (this->get_frame_data (p, start_tv, end_tv, id))
    {
      case -1: ACE_THROW_RETURN (Metrics::SYNCHRONIZATION (), -1);
      case -2: ACE_THROW_RETURN (Metrics::INTERNAL (), -1);
      default: ORBSVCS_Time::Time_Value_to_TimeT (start, start_tv);
               ORBSVCS_Time::Time_Value_to_TimeT (end, end_tv);
               return 0;
    }
}


// Updates the passed frame data structure's start and end times,
// based on the passed time value.  This is a protected internal
// method, and should only be called with locks already held.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
update_data (TAO_Metrics_Frame_Manager_Data<ACE_LOCK> & data, const ACE_Time_Value& tv)
{
  if (data.end_ == ACE_Time_Value::zero)
    {
      return reset_data (data, tv);
    }

  // Advance to the frame that encloses the passed time value.
  while (data.end_ < tv)
    {
      data.end_ += data.period_;
      ++data.frame_id_;
    }

  data.start_.set (data.end_.sec (), data.end_.usec ());
  data.start_ -= data.period_;
  return 0;
}


// Resets the passed frame data structure's start and end times, to
// the passed time value and one period later, respectively.  This is
// a protected internal method, and should only be called with locks
// already held.

template <class ACE_LOCK>
ACE_INLINE CORBA::Short
TAO_Metrics_FrameManager<ACE_LOCK>::
reset_data (TAO_Metrics_Frame_Manager_Data<ACE_LOCK> & data, const ACE_Time_Value& tv)
{
  data.start_.set (tv.sec (), tv.usec ());
  data.end_.set (tv.sec () + data.period_.sec (),
                 tv.usec () + data.period_.usec ());
  return 0;
}


#endif /* METRICS_FRAME_MANAGER_T_I */

