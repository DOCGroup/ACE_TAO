// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/Simple/time
//
// = FILENAME
//    Time_i.h
//
// = DESCRIPTION
//    This class implements the Time IDL interface.
//
// = AUTHOR
//    Darrell Brunsch <brunsch@cs.wustl.edu>
//
// ============================================================================

#ifndef TIME_I_H
#define TIME_I_H

#include "TimeS.h"

class Time_i: public POA_Time
{
  // = TITLE
  //    Time Object Implementation
  //
  // = DESCRIPTION
  //    Implementation of a simple object that has two methods, one that
  //    return the current time/date on the server and the other that
  //    shuts down the server.
public:
  // = Initialization and termination methods.
  Time_i (void);
  // Constructor

  Time_i (Time_i &);
  // Copy Constructor added because g++ generates a
  // bad one if not defined

  ~Time_i (void);
  // Destructor

  virtual CORBA::Long current_time (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return the current time/date on the server

  virtual void shutdown (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Shutdown the server.

  void orb (CORBA::ORB_ptr o);
  // Set the ORB pointer.

private:
  CORBA::ORB_var orb_;
  // ORB pointer.

  ACE_UNIMPLEMENTED_FUNC (void operator= (const Time_i &))
  // Keeping g++2.7.2
};

#endif /* TIME_I_H */
