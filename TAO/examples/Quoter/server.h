// -*- C++ -*-

//=============================================================================
/**
 *  @file    quoter_server.h
 *
 *  $Id$
 *
 *  Implements the Quoter_Server class which handles the initialization of
 *  the quoter implementations.
 *
 *
 *  @author Darrell Brunsch (brunsch@cs.wustl.edu)
 */
//=============================================================================


#ifndef QUOTER_SERVER_H
#define QUOTER_SERVER_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Log_Msg.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Server.h"
#include "tao/Utils/ORB_Manager.h"
#include "Quoter_i.h"
#include "QuoterC.h"

/**
 * @class Quoter_Server
 *
 * Defines a Quoter Server class that implements the functionality
 * of a server process as an object.
 * =DESCRIPTION
 * The interface is quite simple. A server program has to call
 * init to initialize the quoter_server's state and then call run
 * to run the orb.
 */
class Quoter_Server
{
public:

  /// Default constructor
  Quoter_Server (void);

  /// Destructor
  ~Quoter_Server (void);

  /// Initialize the Quoter_Server state - parsing arguments and ...
  int init (int argc, ACE_TCHAR *argv[]);

  /// Run the server.
  int run (void);

private:
  /// Parses the commandline arguments.
  int parse_args (void);

  /// Initializes the name server and registers cubit factory with the
  /// name server.
  int init_naming_service (void);

  /// Number of quoter objects we export.
  int num_of_objs_;

  /// Naming Service context
  CosNaming::NamingContext_var namingContext_var_;

  /// Instantiate the Quoter Factory
  Quoter_Factory_i  *quoter_Factory_i_ptr_;

  /// The ORB manager.
  TAO_ORB_Manager orb_manager_;

  /// Number of commandline arguments.
  int argc_;

  /// commandline arguments.
  ACE_TCHAR **argv_;

  /// debug level (0 = quiet, 1 = default, informative, 2+ = noisy);
  int debug_level_;
};

#endif /* QUOTER_SERVER_H */
