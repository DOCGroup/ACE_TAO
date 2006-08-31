// $Id$

//
// lock
//
CUTS_INLINE
ACE_RW_Thread_Mutex & CUTS_Port_Metric::lock (void)
{
  return this->lock_;
}

//
// transit_time
//
CUTS_INLINE
CUTS_Time_Metric & CUTS_Port_Metric::transit_time (void)
{
  return this->transit_time_;
}

//
// endpoints
//
CUTS_INLINE
const CUTS_Endpoint_Metric_Map &
CUTS_Port_Metric::endpoints (void) const
{
  return this->endpoints_;
}

//
// timestamp
//
CUTS_INLINE
void CUTS_Port_Metric::timestamp (const ACE_Time_Value & timestamp)
{
  this->timestamp_ = timestamp;
}

//
// timestamp
//
CUTS_INLINE
const ACE_Time_Value & CUTS_Port_Metric::timestamp (void) const
{
  return this->timestamp_;
}
