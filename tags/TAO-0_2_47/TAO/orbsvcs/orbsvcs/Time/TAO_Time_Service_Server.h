// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/Time_Service
//
// = FILENAME
//    TAO_Time_Service_Server.h
//
// = DESCRIPTION
//    This class implements the CosTime::TimeService IDL interface.
//
// = AUTHOR
//    Vishal Kachroo  <vishal@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_TIME_SERVICE_SERVER_H
#define	TAO_TIME_SERVICE_SERVER_H

#include "orbsvcs/TimeServiceS.h"

class TAO_ORBSVCS_Export TAO_Time_Service_Server : public POA_CosTime::TimeService
{
  // = TITLE
  //    TimeService Object Implementation.
  //
  // = DESCRIPTION
  //    The Object implementation implements methods to retrieve
  //    current time as well as secure current time. The times are
  //    retrieved as UTOs. The object also allows creation of a TIO
  //    for a given time interval.  In general, the TimeService
  //    manages the UTOs and the TIOs. The notion of time returned
  //    here is the local time of the system.

public:
  // = Initialization and termination methods.
  TAO_Time_Service_Server (int shutdown);
  // Constructor.

  ~TAO_Time_Service_Server (void);
  // Destructor.

  virtual CosTime::UTO_ptr universal_time (CORBA::Environment &env);
  // This operation returns the current system time and an estimate of
  // inaccuracy in a UTO.

  virtual CosTime::UTO_ptr secure_universal_time (CORBA::Environment &env);
  // This operation returns the current time in a UTO only if the time
  // can be guaranteed to have been obtained securely. Currently this operation
  // is not implemented and throws a CORBA::NO_IMPLEMENT exception, if called.

  virtual CosTime::UTO_ptr new_universal_time (TimeBase::TimeT time,
					       TimeBase::InaccuracyT inaccuracy,
					       TimeBase::TdfT tdf,
					       CORBA::Environment &env);
  // This creates a new UTO based on the given parameters.

  virtual CosTime::UTO_ptr uto_from_utc (const TimeBase::UtcT &utc,
					 CORBA::Environment &env);
  // This creates a new UTO given a time in the UtcT form.

  virtual CosTime::TIO_ptr new_interval (TimeBase::TimeT lower,
					 TimeBase::TimeT upper,
					 CORBA::Environment &env);
  // This creates a new TIO with the given parameters.

  int shutdown_;
  // Used if the server is used with IR. If set the orb is shutdown after
  // every method invocation.
};

#endif /* TAO_TIME_SERVICE_SERVER_H */
