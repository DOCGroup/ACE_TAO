// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/Time_Service
//
// = FILENAME
//    TAO_UTO.h
//
// = DESCRIPTION
//    This class implements the CosTime::UTO IDL interface.
//
// = AUTHOR
//    Vishal Kachroo  <vishal@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_UTO_H
#define	TAO_UTO_H

#include "orbsvcs/TimeServiceS.h"

class TAO_UTO : public POA_CosTime::UTO
{
  // = TITLE
  //    Universal Time Object Implementation.
  //
  // = DESCRIPTION
  //    This is an encapsulation of the time. It provides the following
  //    operations on basic time.
  //
  //      - Construction of a UTO from piece parts, and extraction of the
  //        piece parts from a UTO. The piece parts are the readonly
  //        attributes :
  //                      time
  //                      inaccuracy
  //                      time displacement factor
  //                      structure with all the above.
  //
  //      - Comparison of time.
  //
  //      - Conversion from relative to absolute time, and conversion to
  //        an interval (TIO).
public:
  // = Initialization and termination methods.
  TAO_UTO (TimeBase::TimeT time,
	   TimeBase::InaccuracyT inaccuracy,
	   TimeBase::TdfT tdf);
  // Constructor.

  ~TAO_UTO (void);
  // Destructor.

  virtual TimeBase::TimeT time (CORBA::Environment &env);
  // For the readonly attribute <time>.

  virtual TimeBase::InaccuracyT  inaccuracy (CORBA::Environment &env);
  // For the readonly attribute <inaccuracy>.

  virtual TimeBase::TdfT tdf (CORBA::Environment &env);
  // For the readonly attribute <tdf>, which is the "time displacement
  // factor".

  virtual TimeBase::UtcT utc_time (CORBA::Environment &env);
  // For the readonly attribute <utc_time>.

  CosTime::UTO_ptr absolute_time (CORBA::Environment &env);
  // Absolute time = Relative time + Base time.
  // ?? Find out more about the Base Time, UTC and
  //    Distributed Time Sync. Algos. [3

  CosTime::TimeComparison compare_time (CosTime::ComparisonType comparison_type,
					CosTime::UTO_ptr uto,
					CORBA::Environment &env);
  // Compares the time contained in the object with the time in the
  // supplied uto according to the supplied comparison type.


  CosTime::TIO_ptr time_to_interval (CosTime::UTO_ptr,
				     CORBA::Environment &env);
  // Returns a TIO representing the time interval between the time in
  // the object and the time in the UTO passed as a parameter. The
  // interval returned is the interval between the mid-points of the
  // two UTOs. Inaccuracies are ignored.  Note the result of this
  // operation is meaningless if the base times of UTOs are different.

  CosTime::TIO_ptr interval (CORBA::Environment &env);
  // Returns a TIO object representing the error interval around the
  // time value in the UTO.

private:
  TimeBase::UtcT attr_utc_time_;
  // The readonly attribute structure having the time, inaccuracy and
  // displacement.  The get methods for other readonly attributes
  // (time, inaccuracy, tdf) defined in the IDL use the members of
  // this structure and hence need not have separate member variables
  // for them.
};

#endif /* TAO_UTO_H */
