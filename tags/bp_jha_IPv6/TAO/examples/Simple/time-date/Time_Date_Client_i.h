// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/Simple/time-date
//
// = FILENAME
//    Time_Date_Client_i.h
//
// = DESCRIPTION
//    This class implements the interface calls for RMI.
//
// = AUTHOR
//   Douglas C. Schmidt <schmidt@cs.wustl.edu>
//    
// ============================================================================

#ifndef TIME_DATE_CLIENT_I_H
#define TIME_DATE_CLIENT_I_H

#include "../Simple_util.h"
#include "Time_DateC.h"

class Time_Date_Client_i 
{
  // = TITLE
  //   Time_Date_Client interface adapter.
  //
  // = DESCRIPTION
  //   This class implements the interface between the interface
  //   objects and the client.
public:
  // = Initialization and termination methods.
  Time_Date_Client_i (void);
  // Constructor

  virtual ~Time_Date_Client_i (void);
  // Destructor

  virtual int run (const char *, int, char *[]);
  // Execute the methods.

  virtual int parse_args (int argc, char *argv[]);
  // Parses the command line arguments.

private:
  Client<Time_Date, Time_Date_var> client_;
  // Instantiate the client object.
};

#endif /* TIME_CLIENT_I_H */
