// -*- C++ -*-

//=============================================================================
/**
 *  @file    TAO_Time_Service_Clerk.h
 *
 *  $Id$
 *
 *  This class implements the CosTime::TimeService IDL interface.
 *
 *
 *  @author Vishal Kachroo  <vishal@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_TIME_SERVICE_CLERK_H
#define TAO_TIME_SERVICE_CLERK_H
#include "ace/pre.h"

#include "ace/Reactor.h"
#include "orbsvcs/Naming/Naming_Utils.h"

#include "orbsvcs/TimeServiceS.h"
#include "Timer_Helper.h"
#include "time_export.h"

/**
 * @class TAO_Time_Service_Clerk
 *
 * @brief TimeService Object Implementation.
 *
 * The Object implementation implements methods to retrieve
 * GLOBAL time as well as secure GLOBAL time. The times are
 * retrieved as UTOs. The object also allows creation of a TIO
 * for a given time interval.  In general, the TimeService clerk
 * manages the UTOs and the TIOs. The notion of time returned
 * here is the globally synchronized time.
 */
class TAO_Time_Export TAO_Time_Service_Clerk : public POA_CosTime::TimeService
{
public:

  /// Helper class to help in the updation of time.
  friend class Timer_Helper;

  /// Unbounded set of IORs.
  typedef ACE_Array_Base<CosTime::TimeService_var> IORS;

  // = Initialization and termination methods.
  /// Constructor.
  TAO_Time_Service_Clerk (int timer_value,
                          int timer_value_usecs,
                          const IORS& server);

  /// Destructor.
  ~TAO_Time_Service_Clerk (void);

  /// This operation returns the global time and an estimate of
  /// inaccuracy in a UTO.
  virtual CosTime::UTO_ptr universal_time (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosTime::TimeUnavailable));

  /**
   * This operation returns the global time in a UTO only if the time
   * can be guaranteed to have been obtained securely. Currently this
   * operation is not implemented and throws a CORBA::NO_IMPLEMENT
   * exception, if called.
   */
  virtual CosTime::UTO_ptr secure_universal_time (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosTime::TimeUnavailable));

  /// This creates a new UTO based on the given parameters.
  virtual CosTime::UTO_ptr new_universal_time (TimeBase::TimeT time,
                                               TimeBase::InaccuracyT inaccuracy,
                                               TimeBase::TdfT tdf
                                               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// This creates a new UTO given a time in the UtcT form.
  virtual CosTime::UTO_ptr uto_from_utc (const TimeBase::UtcT &utc
                                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// This creates a new TIO with the given parameters.
  virtual CosTime::TIO_ptr new_interval (TimeBase::TimeT lower,
                                         TimeBase::TimeT upper
                                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Return the globally synchronized time.
  virtual CORBA::ULongLong get_time (void);

  /// This method is called by the driver program to set the Naming
  /// Server instance.
  void name_server (TAO_Naming_Server &server);

  /// Returns the time displacement factor.
  CORBA::Short time_displacement_factor (void);

  /// Set the TDF.
  void time_displacement_factor (CORBA::Short);

  /// Get method for inaccuracy.
  TimeBase::InaccuracyT inaccuracy (void);

  /// Set method for inaccuracy.
  void inaccuracy (TimeBase::InaccuracyT inaccuracy);

  /// Clerk's notion of time.
  CORBA::ULongLong time_;

private:

  /// Time displacement factor in minutes.
  CORBA::Short time_displacement_factor_;

  /// Inaccuracy in the time.
  TimeBase::InaccuracyT inaccuracy_;

  /// Set of server Time Server IORs.
  IORS server_;

  /// Time when last global synchronization was done.
  CORBA::ULongLong update_timestamp_;

  /**
   * This is a friend class that inherits from the Event Handler
   * class.  The handle_timeout method of this class is scheduled for
   * periodic invocation by the reactor. This method, in turn, updates
   * the clerks notion of time. Using this class obviates the need for
   * the clerk to multiple inherit from the servant base as well as
   * the event handler.
   */
  Timer_Helper helper_;
};

#include "ace/post.h"
#endif /* TIME_SERVICE_CLERK_H */
