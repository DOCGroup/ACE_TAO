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

#if !defined (_TRIANGLE_TEST_OBJECT_B_SERVER_H)
#define _TRIANGLE_TEST_OBJECT_B_SERVER_H

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "tao/TAO.h"
#include "Object_B_Impl.h"

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
            char **argv,
            CORBA::Environment& env);
  // Initialize the NestedUpCalls_Server state - parsing arguments and ...

  int run (CORBA::Environment& env);
  // Run the orb

private:
  int parse_args (void);
  // Parses the commandline arguments.

  FILE* ior_output_file_;
  // File to output the IOR of the object A.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager

  Object_B_Impl object_B_Impl_;
  // Implementation object of the NestedUpCalls reactor.

  int argc_;
  // Number of commandline arguments.

  char **argv_;
  // commandline arguments.
};

#endif /* _TRIANGLE_TEST_OBJECT_B_SERVER_H */
