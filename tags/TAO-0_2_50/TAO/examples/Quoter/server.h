// $Id$
// -*- C++ -*-

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Quoter
//
// = FILENAME
//    quoter_server.h
//
// = DESCRIPTION
//    Implements the Quoter_Server class which handles the initialization of
//    the quoter implementations.
//
// = AUTHORS
//    Darrell Brunsch (brunsch@cs.wustl.edu)
//
// ============================================================================

#ifndef QUOTER_SERVER_H
#define QUOTER_SERVER_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Log_Msg.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Utils.h"
#include "tao/TAO.h"
#include "Quoter_i.h"
#include "QuoterC.h"

class Quoter_Server
{
  // =TITLE
  //   Defines a Quoter Server class that implements the functionality
  //   of a server process as an object.
  //
  // =DESCRIPTION
  //   The interface is quite simple. A server program has to call
  //   init to initialize the quoter_server's state and then call run
  //   to run the orb.
public:

  Quoter_Server (void);
  // Default constructor

  ~Quoter_Server (void);
  // Destructor

  int init (int argc, char *argv[], CORBA::Environment& env);
  // Initialize the Quoter_Server state - parsing arguments and ...

  int run (CORBA::Environment& env);
  // Run the orb

private:
  int parse_args (void);
  // Parses the commandline arguments.

  int init_naming_service (CORBA::Environment &env);
  // Initializes the name server and registers cubit factory with the
  // name server.

  int num_of_objs_;
  // Number of quoter objects we export.

  CosNaming::NamingContext_var namingContext_var_;
  // Naming Service context

  Quoter_Factory_i  *quoter_Factory_i_ptr_;
  // Instantiate the Quoter Factory

  TAO_ORB_Manager orb_manager_;
  // The ORB manager.

  int argc_;
  // Number of commandline arguments.

  char **argv_;
  // commandline arguments.
};

#endif /* QUOTER_SERVER_H */
