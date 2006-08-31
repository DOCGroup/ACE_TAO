// $Id$

//
// transit_time
//
CUTS_INLINE
const CUTS_Time_Measurement &CUTS_Port_Measurement::transit_time (void) const
{
  return this->transit_time_;
}

CUTS_INLINE
void CUTS_Port_Measurement::transit_time (const ACE_Time_Value &tv)
{
  this->transit_time_ += tv;
}

//
// process_time
//
CUTS_INLINE
const CUTS_Time_Measurement &CUTS_Port_Measurement::process_time (void) const
{
  return this->process_time_;
}

CUTS_INLINE
void CUTS_Port_Measurement::process_time (const ACE_Time_Value &tv)
{
  this->process_time_ += tv;
}

//
// worker_measurements
//
CUTS_INLINE
const CUTS_Port_Measurement::Worker_Map &
CUTS_Port_Measurement::worker_measurements (void) const
{
  return this->workers_;
}

CUTS_INLINE
CUTS_Port_Measurement::Worker_Map &
CUTS_Port_Measurement::worker_measurements (void)
{
  return this->workers_;
}

//
// record_entry
//
CUTS_INLINE
void CUTS_Port_Measurement::record_entry (size_t reps, long worker, long opid,
                                          const ACE_Time_Value & tm)
{
  this->workers_[worker].record_operation (reps, opid, tm);
}

//
// record_exit_time
//
CUTS_INLINE
void CUTS_Port_Measurement::record_exit_point_time (const std::string & uid,
                                                    const ACE_Time_Value & tv)
{
  this->exit_points_[uid] += tv;
}

//
// exit_points
//
CUTS_INLINE
CUTS_Port_Measurement::Exit_Points &
CUTS_Port_Measurement::exit_points (void)
{
  return this->exit_points_;
}

CUTS_INLINE
const CUTS_Port_Measurement::Exit_Points &
CUTS_Port_Measurement::exit_points (void) const
{
  return this->exit_points_;
}
