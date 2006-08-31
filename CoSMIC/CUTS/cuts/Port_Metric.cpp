// $Id$

#include "cuts/Port_Metric.h"

#if !defined (__CUTS_INLINE__)
#include "cuts/Port_Metric.inl"
#endif

#include "cuts/Time_Metric.h"
#include "cuts/System_Metrics_Visitor.h"
#include "ace/Auto_Ptr.h"

//
// CUTS_Port_Metric
//
CUTS_Port_Metric::CUTS_Port_Metric (void)
{

}

//
// ~CUTS_Port_Metric
//
CUTS_Port_Metric::~CUTS_Port_Metric (void)
{
  CUTS_Endpoint_Metric_Map::iterator iter;

  for (iter = this->endpoints_.begin ();
       iter != this->endpoints_.end ();
       iter ++)
  {
    delete iter->second;
  }
}

//
// insert_endpoint
//
CUTS_Time_Metric * CUTS_Port_Metric::insert_endpoint (const char * endpoint)
{
  CUTS_Time_Metric * metric = 0;
  ACE_NEW_RETURN (metric, CUTS_Time_Metric (), 0);
  ACE_Auto_Ptr <CUTS_Time_Metric> auto_clean (metric);

  std::pair <CUTS_Endpoint_Metric_Map::iterator, bool> result;

  ACE_WRITE_GUARD_RETURN (ACE_RW_Thread_Mutex,
                          guard,
                          this->lock_,
                          0);

  result = this->endpoints_.insert (
    CUTS_Endpoint_Metric_Map::value_type (endpoint, metric));

  if (!result.second)
  {
    metric = result.first->second;
  }
  else
  {
    auto_clean.release ();
  }

  return metric;
}

//
// remove_endpoint
//
void CUTS_Port_Metric::remove_endpoint (const char * endpoint)
{
  ACE_WRITE_GUARD (ACE_RW_Thread_Mutex, guard, this->lock_);
  this->endpoints_.erase (endpoint);
}

//
// time_metric
//
CUTS_Time_Metric * CUTS_Port_Metric::endpoint (const char * endpoint)
{
  CUTS_Time_Metric * metric = 0;
  CUTS_Endpoint_Metric_Map::iterator result;

  do {
    ACE_READ_GUARD_RETURN (
      ACE_RW_Thread_Mutex,
      guard,
      this->lock_,
      0);

    // Locate the endpoint in the mapping.
    result = this->endpoints_.find (endpoint);
  } while (false);

  if (result != this->endpoints_.end ())
  {
    // Save the located endpoint.
    metric = result->second;
  }
  else
  {
    // Since we could not locate it, we have to create a new one.
    metric = insert_endpoint (endpoint);
  }

  return metric;
}

//
// accept
//
void CUTS_Port_Metric::accept (CUTS_System_Metrics_Visitor & visitor)
{
  visitor.visit_port_metrics (*this);
}
