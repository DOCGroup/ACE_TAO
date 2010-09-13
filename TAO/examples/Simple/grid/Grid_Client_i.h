// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/Simple/grid
//
// = FILENAME
//    Grid_Client_i.h
//
// = DESCRIPTION
//    This class implements the interface calls for RMI.
//
// = AUTHOR
//   Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef GRID_CLIENT_I_H
#define GRID_CLIENT_I_H

#include "../Simple_util.h"
#include "GridC.h"

class Grid_Client_i
{
  // = TITLE
  //   Grid_Client interface subclass.
  //
  // = DESCRIPTION
  //   This class implements the interface between the interface
  //   objects and the client .
public:
  // = Initialization and termination methods.
  Grid_Client_i (void);
  // Constructor

  ~Grid_Client_i (void);
  // Destructor

  int run (const char *, int, ACE_TCHAR *[]);
  // Execute the methods.

  int parse_args (int argc, ACE_TCHAR *argv[]);
  // Parses the command line arguments.

private:
  Client<Grid_Factory> client_;
  // Instantiate the client object.

  CORBA::Short height_;
  // Height of the grid.

  CORBA::Short width_;
  // Width of the grid.

  CORBA::Short setx_;
  // Cell identifier in which a value meeds to be stored.

  CORBA::Short sety_;
  // Cell identifier in which a value meeds to be stored.

  CORBA::Long value_;
  // The value that needs to be stored in the cell.
};

#endif /* TIME_CLIENT_I_H */
