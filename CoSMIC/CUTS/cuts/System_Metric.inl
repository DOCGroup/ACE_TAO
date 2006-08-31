// $Id$

//
// lock
//
CUTS_INLINE
ACE_RW_Thread_Mutex & CUTS_System_Metric::lock (void)
{
  return this->lock_;
}

//
// component_metrics
//
CUTS_INLINE
const CUTS_Component_Metric_Map &
CUTS_System_Metric::component_metrics (void) const
{
  return this->component_metrics_;
}

//
// timestamp
//
CUTS_INLINE
const ACE_Time_Value & CUTS_System_Metric::timestamp (void) const
{
  return this->timestamp_;
}

//
// timestamp
//
CUTS_INLINE
void CUTS_System_Metric::timestamp (const ACE_Time_Value & timestamp)
{
  this->timestamp_ = timestamp;
}
