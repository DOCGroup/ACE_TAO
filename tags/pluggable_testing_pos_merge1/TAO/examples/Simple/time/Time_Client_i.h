// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Simple/time
//
// = FILENAME
//    Time_Client_i.h
//
// = DESCRIPTION
//    This class implements the  interface calls for RMI.
//
// = AUTHOR
//    
//   Balachandran Natarajan <bala@cs.wustl.edu>
//    
// ============================================================================

#ifndef TIME_CLIENT_I_H
#define TIME_CLIENT_I_H

#include "../Simple_util.h"
#include "TimeC.h"

class Time_Client_i
{
  // = TITLE
  //   Time_Client interface subclass.
  //
  // = DESCRIPTION
  //   This class implements the interface between the interface
  //   objects and the client .
public:
  // = Initialization and termination methods.
  Time_Client_i (void);
  // Constructor

  virtual ~Time_Client_i (void);
  // Destructor

  virtual int run (char *,int, char** );
  // Execute the methods

private:
  Client<Time, Time_var> client;
  // Instantiate the client object.
};


#endif /* TIME_CLIENT_I_H */
