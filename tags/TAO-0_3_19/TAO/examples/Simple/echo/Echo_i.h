// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Simple/echo
//
// = FILENAME
//    Echo_i.h
//
// = DESCRIPTION
//    This class implements the Echo IDL interface.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef ECHO_I_H
#define ECHO_I_H

#include "EchoS.h"

class Echo_i : public POA_Echo
{
  // = TITLE
  //    Echo Object Implementation
  //
  // = DESCRIPTION
  //    The object implementation  performs teh following functions:
  //   -- To return the string which needs to be displayed
  //      from the server.
  //   -- shuts down the server
public:
  // = Initialization and termination methods.
  Echo_i (void);
  // Constructor.

  ~Echo_i (void);
  // Destructor.

  virtual Echo::List *echo_list (const char *mesg,
                                 CORBA::Environment &env);
  // Return the mesg string back from the server.

  virtual char *echo_string (const char *mesg,
                             CORBA::Environment &env);
  // Return the mesg string back from the server.

 virtual void shutdown (CORBA::Environment &env);
  // Shutdown the server.

  void orb (CORBA::ORB_ptr o);
  // Set the ORB pointer.

private:
  CORBA::ORB_var orb_;
  // ORB pointer.
};

#endif /* ECHO_I_H */
