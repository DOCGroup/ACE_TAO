// $Id$

//
// port_metrics
//
CUTS_INLINE
const CUTS_Port_Metric_Map &
CUTS_Component_Metric::port_metrics (void) const
{
  return this->port_metrics_;
}

//
// lock
//
CUTS_INLINE
ACE_RW_Thread_Mutex & CUTS_Component_Metric::lock (void)
{
  return this->lock_;
}

//
// timestamp
//
CUTS_INLINE
const ACE_Time_Value & CUTS_Component_Metric::timestamp (void) const
{
  return this->timestamp_;
}

//
// timestamp
//
CUTS_INLINE
void CUTS_Component_Metric::timestamp (const ACE_Time_Value & timestamp)
{
  this->timestamp_ = timestamp;
}
