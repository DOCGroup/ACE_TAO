// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/performance-tests/Pluggable
//
// = FILENAME
//    PP_Test_Server.h
//
// = DESCRIPTION
//      This class implements simple oneway and twoway no-op requests
//      to time latency and overhead of the pluggable protocol functionality.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef _PP_TEST_SERVER_H
#define _PP_TEST_SERVER_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/ORB_Manager.h"
#include "PP_Test_i.h"

class PP_Test_Server
{
  // = TITLE
  //   Defines a server class that implements the functionality
  //   of a server process as an object.
  //
  // = DESCRIPTION
  //   The interface is quite simple. A server program has to call
  //   init to initialize the server's state and then call run
  //   to run the orb.
public:
  // = Initialization and termination methods.
  PP_Test_Server (void);
  // Default constructor

  ~PP_Test_Server (void);
  // Destructor

  int init (int argc,
            char **argv
            ACE_ENV_ARG_DECL);
  // Initialize the server state - parsing arguments, etc.

  int run (ACE_ENV_SINGLE_ARG_DECL);
  // Run the orb

private:
  int parse_args (void);
  // Parses the commandline arguments.

  FILE* ior_output_file_;
  // File to output the cubit factory IOR.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager.

  Pluggable_Test_Factory_i *factory_impl_;
  // Implementation object of the Pluggable Test factory.

  CORBA::String_var factory_id_;
  // ID of the factory.

  int argc_;
  // Number of commandline arguments.

  char **argv_;
  // commandline arguments.
};

#endif /* _PP_TEST_SERVER_H */
