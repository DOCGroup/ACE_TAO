// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Simple
// 
// = FILENAME
//    simple_object_impl.h
//
// = DESCRIPTION
//    This class implements the Time interface.
//
// = AUTHOR
//    Darrell Brunsch <brunsch@cs.wustl.edu>
// 
// ============================================================================

#if !defined (TIME_IMPL_H)
#define	TIME_IMPL_H

#include "TimeS.h"

// Forward declarations.
class Time_Impl;

// Typedefs.
typedef Time_Impl *Time_Impl_ptr;
typedef Time_Impl_ptr Time_Impl_ref;

class Time_Impl: public POA_Time
{
  // = TITLE
  //    Simple Time Object Implementation
  //
  // = DESCRIPTION
  //    Implementation of a simple object that has two methods, one that 
  //    return the current time/date on the server and the other that
  //    shuts down the server.  
public:
  // = Initialization and termination methods.
  Time_Impl (const char *obj_name = 0);
  // Constructor

  ~Time_Impl (void);
  // Destructor

  virtual CORBA::Long time (CORBA::Environment &env);
  // Return the current time/date on the server.

  virtual void shutdown (CORBA::Environment &env);
  // Shutdown the server.
};

#endif /* TIME_IMPL_H */
