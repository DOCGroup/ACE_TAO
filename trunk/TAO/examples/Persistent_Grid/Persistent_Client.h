// -*- C++ -*-

//=============================================================================
/**
 *  @file    Persistent_Client_i.h
 *
 *  $Id$
 *
 *  This class implements the interface calls for RMI.
 *
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef PERSISTENT_CLIENT_I_H
#define PERSISTENT_CLIENT_I_H

#include "Simple_util.h"
#include "GridC.h"

/**
 * @class Persistent_Client_i
 *
 * @brief Grid_Client interface subclass.
 *
 * This class implements the interface between the interface
 * objects and the client .
 */
class Persistent_Client_i
{
public:
  // = Initialization and termination methods.
  /// Constructor
  Persistent_Client_i (void);

  /// Destructor
  virtual ~Persistent_Client_i (void);

  /// Execute the methods.
  virtual int run (const char *, int, ACE_TCHAR *[]);

  /// Parses the command line arguments.
  virtual int parse_args (int argc, ACE_TCHAR *argv[]);

private:
  /// Instantiate the client object.
  Client<Grid_Factory, Grid_Factory_var> client;

  /// Height of the grid
  CORBA::Short height_;

  /// Width of the grid.
  CORBA::Short width_;

  /// Flag for the deletion of memory
  CORBA::Short remove_;
};

#endif /* PERSISTENT_CLIENT_I_H */
