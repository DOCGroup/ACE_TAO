// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Simple/time
// 
// = FILENAME
//    Time_i.h
//
// = DESCRIPTION
//    This class implements the Time interface.
//
// = AUTHOR
//    Darrell Brunsch <brunsch@cs.wustl.edu>
// 
// ============================================================================

#if !defined (TIME_I_H)
#define	TIME_I_H

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
  Time_i (CORBA::ORB_ptr orb);
  // Constructor

  ~Time_i (void);
  // Destructor

  virtual CORBA::Long time (CORBA::Environment &env);
  // Return the current time/date on the server.

  virtual void shutdown (CORBA::Environment &env);
  // Shutdown the server.

private:
  CORBA::ORB_var orb_;
  // ORB pointer
};

#endif /* TIME_I_H */
