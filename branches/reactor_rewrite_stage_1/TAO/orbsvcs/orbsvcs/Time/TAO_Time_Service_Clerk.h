// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/Time_Service
//
// = FILENAME
//    TAO_Time_Service_Clerk.h
//
// = DESCRIPTION
//    This class implements the CosTime::TimeService IDL interface.
//
// = AUTHOR
//    Vishal Kachroo  <vishal@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_TIME_SERVICE_CLERK_H
#define TAO_TIME_SERVICE_CLERK_H
#include "ace/pre.h"

#include "ace/Reactor.h"
#include "orbsvcs/Naming/Naming_Utils.h"

#include "orbsvcs/TimeServiceS.h"
#include "Timer_Helper.h"
#include "time_export.h"

class TAO_Time_Export TAO_Time_Service_Clerk : public POA_CosTime::TimeService
{
  // = TITLE
  //    TimeService Object Implementation.
  //
  // = DESCRIPTION
  //    The Object implementation implements methods to retrieve
  //    GLOBAL time as well as secure GLOBAL time. The times are
  //    retrieved as UTOs. The object also allows creation of a TIO
  //    for a given time interval.  In general, the TimeService clerk
  //    manages the UTOs and the TIOs. The notion of time returned
  //    here is the globally synchronized time.
public:

  friend class Timer_Helper;
  // Helper class to help in the updation of time.

  typedef ACE_Array_Base<CosTime::TimeService_var> IORS;
  // Unbounded set of IORs.

  // = Initialization and termination methods.
  TAO_Time_Service_Clerk (int timer_value,
                          int timer_value_usecs,
                          const IORS& server);
  // Constructor.

  ~TAO_Time_Service_Clerk (void);
  // Destructor.

  virtual CosTime::UTO_ptr universal_time (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosTime::TimeUnavailable));
  // This operation returns the global time and an estimate of
  // inaccuracy in a UTO.

  virtual CosTime::UTO_ptr secure_universal_time (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosTime::TimeUnavailable));
  // This operation returns the global time in a UTO only if the time
  // can be guaranteed to have been obtained securely. Currently this
  // operation is not implemented and throws a CORBA::NO_IMPLEMENT
  // exception, if called.

  virtual CosTime::UTO_ptr new_universal_time (TimeBase::TimeT time,
                                               TimeBase::InaccuracyT inaccuracy,
                                               TimeBase::TdfT tdf
                                               TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This creates a new UTO based on the given parameters.

  virtual CosTime::UTO_ptr uto_from_utc (const TimeBase::UtcT &utc
                                         TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This creates a new UTO given a time in the UtcT form.

  virtual CosTime::TIO_ptr new_interval (TimeBase::TimeT lower,
                                         TimeBase::TimeT upper
                                         TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This creates a new TIO with the given parameters.

  virtual CORBA::ULongLong get_time (void);
  // Return the globally synchronized time.

  void name_server (TAO_Naming_Server &server);
  // This method is called by the driver program to set the Naming
  // Server instance.

  CORBA::Short time_displacement_factor (void);
  // Returns the time displacement factor.

  void time_displacement_factor (CORBA::Short);
  // Set the TDF.

  TimeBase::InaccuracyT inaccuracy (void);
  // GET method for inaccuracy.

  void inaccuracy (TimeBase::InaccuracyT inaccuracy);
  // SET method for inaccuracy.

  CORBA::ULongLong time_;
  // Clerk's notion of time.

private:

  CORBA::Short time_displacement_factor_;
  // Time displacement factor in minutes.

  TimeBase::InaccuracyT inaccuracy_;
  // Inaccuracy in the time.

  IORS server_;
  // Set of server Time Server IORs.

  CORBA::ULongLong update_timestamp_;
  // Time when last global synchronization was done.

  Timer_Helper helper_;
  // This is a friend class that inherits from the Event Handler
  // class.  The handle_timeout method of this class is scheduled for
  // periodic invocation by the reactor. This method, in turn, updates
  // the clerks notion of time. Using this class obviates the need for
  // the clerk to multiple inherit from the servant base as well as
  // the event handler.
};

#include "ace/post.h"
#endif /* TIME_SERVICE_CLERK_H */
