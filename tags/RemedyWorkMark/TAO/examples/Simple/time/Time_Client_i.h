// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/Simple/time
//
// = FILENAME
//    Time_Client_i.h
//
// = DESCRIPTION
//    This class implements the client calls to the Time example.
//
// = AUTHOR
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
  //   objects and the client.
public:
  // = Initialization and termination methods.
  Time_Client_i (void);
  // Constructor

  ~Time_Client_i (void);
  // Destructor

  int run (const char *, int, ACE_TCHAR**);
  // Execute the methods

private:
  Client<Time> client_;
  // Instantiate the client object.
};

#endif /* TIME_CLIENT_I_H */
