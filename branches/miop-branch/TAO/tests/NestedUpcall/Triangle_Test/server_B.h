// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/Triangle_Test
//
// = FILENAME
//    server_B.h
//
// = DESCRIPTION
//      This class implements a simple server for the
//      Nested Upcalls - Triangle test
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#ifndef _TRIANGLE_TEST_OBJECT_B_SERVER_H
#define _TRIANGLE_TEST_OBJECT_B_SERVER_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Log_Msg.h"
#include "tao/PortableServer/ORB_Manager.h"
#include "Object_B_i.h"

class Object_B_Server
{
  // = TITLE
  //   This is the server for the object A in the test.
  //
  // = DESCRIPTION
  //   See the README file for more information.

public:

  Object_B_Server (void);
  // Default constructor

  ~Object_B_Server (void);
  // Destructor

  int init (int argc,
            char **argv
            TAO_ENV_ARG_DECL);
  // Initialize the NestedUpCalls_Server state - parsing arguments and ...

  int run (TAO_ENV_SINGLE_ARG_DECL);
  // Run the orb

private:
  int parse_args (void);
  // Parses the commandline arguments.

  FILE* ior_output_file_;
  // File to output the IOR of the object A.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager

  Object_B_i object_B_i_;
  // Implementation object of the NestedUpCalls reactor.

  int argc_;
  // Number of commandline arguments.

  char **argv_;
  // commandline arguments.
};

#endif /* _TRIANGLE_TEST_OBJECT_B_SERVER_H */
