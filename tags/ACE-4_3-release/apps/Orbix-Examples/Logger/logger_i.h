/* -*- C++ -*- */
// $Id$


#include "ace/Profile_Timer.h"
#define EXCEPTIONS
#include "logger.hh"

class logger_i 
#if defined (USE_BOA_IMPL)
  : virtual public loggerBOAImpl
#endif /* USE_BOA_IMPL */
  // = TITLE
  // Implementation of the logger interface.
  //
  // = DESCRIPTION
  // Uses either the BOAImpl or the DEF_TIE approach,
  // depending on the #ifdef
{
public:
  logger_i (int verbose = 0);
  // Select non-verbose logging by default.

  virtual void log (const logger::Log_Record &log_rec, CORBA::Environment &IT_env);
  // Implement the log method.

  virtual void verbose (char verbose, CORBA::Environment &IT_env);
  // Enable/disable verbosity.

  virtual char verbose (CORBA::Environment &IT_env);
  // Report current verbosity level.

private:
  unsigned char verbose_value_;
  // Indicate if we are using verbose logging or not.

  unsigned char verbose_message_;
  // Indicate if we outputting the messages (turn off if you
  // want to conduct timing tests that just measure throughput).
};

class profile_logger_i :
#if defined (USE_BOA_IMPL)
  public virtual profile_loggerBOAImpl,
  public virtual Logger_i
#else /* USE_TIE */
  public logger_i 
#endif /* USE_BOA_IMPL */
  // = TITLE
  // Implementation of the profiler logger interface.
  //
  // = DESCRIPTION
  // Uses the BOAImpl approach.
{
public:
  virtual void start_timer (CORBA::Environment &env);
  // Activate the timer.

  virtual void stop_timer (profile_logger::Elapsed_Time &et,
			   CORBA::Environment &env);
  // Deactivate the timer and return the elapsed time.

private:
  ACE_Profile_Timer pt_;
  // Object that keeps track of the user and system execution time.
};

#if !defined (USE_BOA_IMPL)
// Indicate that the C++ classes logger_i and profile_logger_i implement
// the IDL interface logger and profile_logger, respectively:

DEF_TIE_logger (logger_i)
DEF_TIE_profile_logger (profile_logger_i)

#endif /* USE_BOA_IMPL */
