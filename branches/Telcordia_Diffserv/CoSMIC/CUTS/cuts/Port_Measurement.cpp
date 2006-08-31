// $Id$

#include "cuts/Port_Measurement.h"

#if !defined (__CUTS_INLINE__)
#include "cuts/Port_Measurement.inl"
#endif

#include <algorithm>

//
// Reset_Exit_Times
//
struct Reset_Exit_Times
{
  void operator () (CUTS_Port_Measurement::Exit_Points::value_type & item)
  {
    item.second.reset ();
  }
};

//
// CUTS_Port_Measurement
//
CUTS_Port_Measurement::CUTS_Port_Measurement (void)
{

}

//
// ~CUTS_Port_Measurement
//
CUTS_Port_Measurement::~CUTS_Port_Measurement (void)
{

}

//
// reset
//
void CUTS_Port_Measurement::reset (void)
{
  // Reset the <process_time_> and <transmit_time_> values
  this->process_time_.reset ();
  this->transit_time_.reset ();

  // Reset all the exit times via this port.
  std::for_each (
    this->exit_points_.begin (),
    this->exit_points_.end (),
    Reset_Exit_Times ());
}
