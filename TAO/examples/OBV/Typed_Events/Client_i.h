// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/OBV/Typed_Events
//
// = FILENAME
//    Client_i.h
//
// = DESCRIPTION
//    This class is a template for a CORBA client.
//    run () sends some valuetypes to a server and
//    gets back a list of valuetypes.
//
// = AUTHOR
//
//   Torsten Kuepper
//   based on the echo example
//   from code from Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef ECHO_CLIENT_I_H
#define ECHO_CLIENT_I_H

#include "../Simple_util.h"
#include "Event_TypesC.h"

double random_number (double min, double max);

class Checkpoint_Client_i
{
  // = TITLE
  //   Checkpoint_Client interface subclass.
  //
  // = DESCRIPTION
  //   This class implements the interface between the interface
  //   objects and the client .
public:
  // = Initialization and termination methods.
  Checkpoint_Client_i (void);
  // Constructor

  virtual  ~Checkpoint_Client_i (void);
  // Destructor

  virtual int run (char *,int, char** );
  // Execute the methods

private:
  Client<Checkpoint, Checkpoint_var> checkpoint;
  // Instantiate the checkpoint object.
};


#endif /* TIME_CLIENT_I_H */
