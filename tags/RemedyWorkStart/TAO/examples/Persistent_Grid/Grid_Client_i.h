// -*- C++ -*-

//=============================================================================
/**
 *  @file    Grid_Client_i.h
 *
 *  $Id$
 *
 *  This class implements the interface calls for RMI.
 *
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef GRID_CLIENT_I_H
#define GRID_CLIENT_I_H

#include "Simple_util.h"
#include "GridC.h"

/**
 * @class Grid_Client_i
 *
 * @brief Grid_Client interface subclass.
 *
 * This class implements the interface between the interface
 * objects and the client .
 */
class Grid_Client_i
{
public:
  // = Initialization and termination methods.
  /// Constructor
  Grid_Client_i (void);

  /// Destructor
  virtual ~Grid_Client_i (void);

  /// Execute the methods.
  virtual int run (const char *, int, ACE_TCHAR *[]);

  /// Parses the command line arguments.
  virtual int parse_args (int argc, ACE_TCHAR *argv[]);

private:
  /// Instantiate the client object.
  Client<Grid_Factory, Grid_Factory_var> client;

  /// Height of the grid.
  CORBA::Short height_;

  /// Width of the grid.
  CORBA::Short width_;

  /// Cell identifier in which a value meeds to be stored.
  CORBA::Short setx_;

  /// Cell identifier in which a value meeds to be stored.
  CORBA::Short sety_;

  /// The value that needs to be stored in the cell.
  CORBA::Long value_;

  /// A flag for testing persistence
  CORBA::Short repeat_;
};

#endif /* TIME_CLIENT_I_H */
