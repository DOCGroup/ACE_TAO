// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/MT_Client_Test
//
// = FILENAME
//    server_A.h
//
// = DESCRIPTION
//      This class implements a simple server for the
//      Nested Upcalls - MT Client test
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#ifndef MT_CLIENT_TEST_MT_OBJECT_SERVER_H
#define MT_CLIENT_TEST_MT_OBJECT_SERVER_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Log_Msg.h"
#include "tao/TAO.h"
#include "MT_Object_i.h"

class MT_Object_Server
{
  // = TITLE
  //   This is the server for the object A in the test.
  //
  // = DESCRIPTION
  //   See the README file for more information.

public:

  MT_Object_Server (void);
  // Default constructor

  ~MT_Object_Server (void);
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

  MT_Object_i mT_Object_i_;
  // Implementation object MT_OBject

  int argc_;
  // Number of commandline arguments.

  char **argv_;
  // commandline arguments.

  u_char use_mt_object_;
};

#endif /* MT_CLIENT_TEST_MT_OBJECT_SERVER_H */
