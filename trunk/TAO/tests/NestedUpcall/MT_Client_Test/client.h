// -*- c++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/NestedUpCalls/MT_Client_Test
//
// = FILENAME
//    client.h
//
// = DESCRIPTION
//    Test for multithreaded client using one global ORB (and Reactor)
//
// = AUTHORS
//    Michael Kircher
//
// ============================================================================

#ifndef MT_CLIENT_H
#define MT_CLIENT_H

#include "ace/Get_Opt.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Task.h"
#include "ace/Thread_Manager.h"
#include "tao/corba.h"
#include "MT_Client_TestC.h"

class MT_Client
{
  // = TITLE
  //    A simple client talking to one server
  //
  // = DESCRIPTION
  //    It is connecting to a server which holds the Object_A
  //    or using the "-m" switch to a server holding the MT_Object

public:
  MT_Client (void);
  ~MT_Client (void);
  // = Constructor and destructor.

  int run (void);
  // Execute client example code.

  int init (int argc,
            char **argv,
            int client_number);
  // Initialize the client communication endpoint with server.

private:
  int read_ior (char *filename);
  // reading the IOR of object A in

  int parse_args (void);
  // Parses the arguments passed on the command line.

  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  char *object_key_;
  // Key of the object

  int shutdown_;
  // Flag to tell server to shutdown.

  MT_Object_var mT_Object_var_;
  // pointer to the mt Object

  CORBA::ORB_var orb_var_;
  // Remember our orb.

  int client_number_;
  // This is used to choose the server...

  unsigned long iterations_;
  // number of loops

};

class MT_Client_Task : public ACE_Task<ACE_SYNCH>
{
  // = TITLE
  //    Wrapper for the MT_Client to be an extra thread
  //
  // = DESCRIPTION
  //    No big purpose, just wrapping
public:
  MT_Client_Task (int argc, char **argv, int client_number);

  virtual int svc (void);

private:
  MT_Client mT_Client_;
  int argc_;
  char **argv_;

  int client_number_;
  // To determine if we use the first or the second server.
};

#endif /* MT_CLIENT_H */
