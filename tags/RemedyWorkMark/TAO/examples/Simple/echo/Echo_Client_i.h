// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Simple/echo
//
// = FILENAME
//    Echo_Client_i.h
//
// = DESCRIPTION
//    This class implements the  interface calls for RMI.
//
// = AUTHOR
//
//   Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef ECHO_CLIENT_I_H
#define ECHO_CLIENT_I_H

#include "../Simple_util.h"
#include "EchoC.h"

class Echo_Client_i
{
  // = TITLE
  //   Echo_Client interface subclass.
  //
  // = DESCRIPTION
  //   This class implements the interface between the interface
  //   objects and the client .
public:
  // = Initialization and termination methods.
  Echo_Client_i (void);
  // Constructor

  ~Echo_Client_i (void);
  // Destructor

  int run (const char *, int, ACE_TCHAR **);
  // Execute the methods

private:
  Client<Echo> client_;
  // Instantiate the client object.
};

#endif /* TIME_CLIENT_I_H */
