// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/Time_Service
//
// = FILENAME
//    TAO_TIO.h
//
// = DESCRIPTION
//    This class implements the CosTime::TIO IDL interface.
//
// = AUTHOR
//    Vishal Kachroo  <vishal@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_TIO_H
#define	TAO_TIO_H

#include "tao/TAO.h"
#include "orbsvcs/TimeServiceS.h"
#include "TAO_UTO.h"

class TAO_ORBSVCS_Export TAO_TIO : public POA_CosTime::TIO
{
  // = TITLE
  //    Time Interval Object Implementation.
  //
  // = DESCRIPTION
  //    The TIO represents a time interval and has operations to
  //    compare itself with a UTO or another TIO. It also has an
  //    operation to create a UTO from the value of it's time
  //    interval.
public:
  // = Initialization and termination methods.
  TAO_TIO (TimeBase::TimeT lower,
	   TimeBase::TimeT upper);
  // Constructor.

  ~TAO_TIO (void);
  // Destructor.

  virtual TimeBase::IntervalT  time_interval (CORBA::Environment &env);
  // This is the get method for the attribute time interval.

  virtual CosTime::OverlapType spans (CosTime::UTO_ptr time,
				      CosTime::TIO_out overlap,
				      CORBA::Environment &env);
  // This operation returns a value of type OverlapType depending on
  // how the interval in the object and the time range represented by
  // the parameter UTO overlap.  If OverlapType is not OTNoOverlap,
  // then the out parameter overlap contains the overlap interval,
  // otherwise the out parameter contains the gap between the two
  // intervals.

  virtual CosTime::OverlapType overlaps (CosTime::TIO_ptr interval,
					 CosTime::TIO_out overlap,
					 CORBA::Environment &env);
  // This operation returns a value of type OverlapType depending on
  // how the interval in the object and interval in the parameter TIO
  // overlap. If OverlapType is not OTNoOverlap, then the out
  // parameter overlap contains the overlap interval, otherwise the
  // out parameter contains the gap between the two intervals.

  virtual CosTime::UTO_ptr time (CORBA::Environment &env);
  // Returns a UTO in which the inaccuracy interval is equal to the
  // time interval in the TIO and time value is the midpoint of the
  // interval.

private:
  TimeBase::IntervalT attr_time_interval;
  // This attribute returns an IntervalT structure with the values of
  // its fields filled in with the corresponding values from the TIO.
};

#endif /* TAO_TIO_H */
