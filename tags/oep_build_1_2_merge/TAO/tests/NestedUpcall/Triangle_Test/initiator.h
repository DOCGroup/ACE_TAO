// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/Triangle_Test
//
// = FILENAME
//    initiator.h
//
// = DESCRIPTION
//      This class implements a simple server for the
//      Nested Upcalls - Triangle test
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#ifndef _TRIANGLE_TEST_INITITATOR_SERVER_H
#define _TRIANGLE_TEST_INITITATOR_SERVER_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Log_Msg.h"
#include "tao/PortableServer/ORB_Manager.h"
#include "Initiator_i.h"

class Initiator_Server
{
  // = TITLE
  //   This is the server for the Initiator in the test.
  //
  // = DESCRIPTION
  //   See the README file for more information.

public:

  Initiator_Server (void);
  // Default constructor

  ~Initiator_Server (void);
  // Destructor

  int read_ior (char *filename, unsigned int A_B);
  // read in the IOR's for the two objects A and B

  int init (int argc,
            char **argv
            ACE_ENV_ARG_DECL);
  // Initialize the Initiator_Server state - parsing arguments and ...

  int run (ACE_ENV_SINGLE_ARG_DECL);
  // Run the orb

private:
  int parse_args (void);
  // Parses the commandline arguments.

  char * object_A_key_;
  // The IOR of object A

  char * object_B_key_;
  // The IOR of object B

  Object_A_var object_A_var_;
  // reference to object A

  Object_B_var object_B_var_;
  // reference to object B

  TAO_ORB_Manager orb_manager_;
  // The ORB manager

  Initiator_i *initiator_i_ptr_;
  // Implementation object of the Initiator

  int argc_;
  // Number of commandline arguments.

  char **argv_;
  // commandline arguments.

  CORBA::String_var str_;
  // IOR of my servant.
};

#endif /* _TRIANGLE_TEST_INITITATOR_SERVER_H */
