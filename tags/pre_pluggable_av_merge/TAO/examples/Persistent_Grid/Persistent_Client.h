// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/Persistent_Client
//
// = FILENAME
//    Persistent_Client_i.h
//
// = DESCRIPTION
//    This class implements the interface calls for RMI.
//
// = AUTHOR
//   Balachandran Natarajan <bala@cs.wustl.edu>
//    
// ============================================================================

#ifndef PERSISTENT_CLIENT_I_H
#define PERSISTENT_CLIENT_I_H

#include "Simple_util.h"
#include "GridC.h"

class Persistent_Client_i
{
  // = TITLE
  //   Grid_Client interface subclass.
  //
  // = DESCRIPTION
  //   This class implements the interface between the interface
  //   objects and the client .
public:
  // = Initialization and termination methods.
  Persistent_Client_i (void);
  // Constructor

  virtual ~Persistent_Client_i (void);
  // Destructor

  virtual int run (const char *, int, char *[]);
  // Execute the methods.

  virtual int parse_args (int argc, char *argv[]);
  // Parses the command line arguments.

private:
  Client<Grid_Factory, Grid_Factory_var> client;
  // Instantiate the client object.

  CORBA::Short height_;
  // Height of the grid

  CORBA::Short width_;
  // Width of the grid.
  
  CORBA::Short remove_;
  // Flag for the deletion of memory
};

#endif /* PERSISTENT_CLIENT_I_H */
