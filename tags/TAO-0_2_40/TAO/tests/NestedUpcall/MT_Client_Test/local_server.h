// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/MT_Client_Test
//
// = FILENAME
//    local_server.h
//
// = DESCRIPTION
//     This class implements a simple server for the
//     Nested Upcalls - MT Client test
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#ifndef MT_CLIENT_TEST_MT_SERVER_H
#define MT_CLIENT_TEST_MT_SERVER_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Log_Msg.h"
#include "tao/TAO.h"
#include "MT_Object_i.h"

class MT_Server
{
  // = TITLE
  //   This is the server for the mt_object
  //
  // = DESCRIPTION
  //   See the README file for more information.

public:

  MT_Server ();
  // Default constructor

  ~MT_Server (void);
  // Destructor

  int init (int argc,
            char **argv,
            TAO_ORB_Manager* orb_manager_ptr);
  // Initialize the MT_Server state - parsing arguments and ...

  int run (void);
  // Run the orb

  int run_ORB_briefly (void);
  // Just run it briefly

private:
  int read_ior (char *filename);
  // reading the IOR of mt object in

  int parse_args (void);
  // Parses the commandline arguments.

  char *object_key_;
  // Key of the object

  FILE* ior_output_file_;
  // File to output the IOR of the object A.

  TAO_ORB_Manager* orb_manager_ptr_;
  // The ORB manager

  MT_Object_i mT_Object_i_;
  // Implementation object

  MT_Object_var mT_Object_var_;
  // reference for the distant MT Object

  int argc_;
  // Number of commandline arguments.

  char **argv_;
  // commandline arguments.

  unsigned int iterations_;
  // number of times the server should call to the disted MT OBject
};


class MT_Server_Task : public ACE_Task<ACE_SYNCH>
{
  // = TITLE
  //    Wrapper for the MT_Server to be an extra thread
  //
  // = DESCRIPTION
  //    No big purpose, just wrapping
public:
  MT_Server_Task (ACE_Thread_Manager* thr_mgr_ptr,
                  int argc,
                  char **argv,
                  TAO_ORB_Manager* orb_manager_ptr);

  virtual int svc (void);

private:
  MT_Server mT_Server_;
  int argc_;
  char **argv_;
  TAO_ORB_Manager* orb_manager_ptr_;
};


#endif /* MT_CLIENT_TEST_MT_SERVER_H */
