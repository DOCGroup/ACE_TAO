/* -*- C++ -*- */
// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/tests/Simple/chat
//
// = FILENAME
//    Server_i.h
//
// = DESCRIPTION
//    Definition of the Chat Server_i class.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ===========================================================

#ifndef SERVER_I_H
#define SERVER_I_H

#include "Broadcaster_i.h"
#include "tao/Utils/ORB_Manager.h"

class Server_i
{
  // = TITLE
  //    The class defines the server for the chat. It sets up the Orb
  //    manager and registers the Broadcaster servant object.

public:
  // = Initialization and termination methods.
 Server_i (void);
 // Constructor.

 ~Server_i (void);
 // Destructor.

 int init (int argc,
            char *argv[]);
 // Initialize the server.

 int run (void);
 // Run the ORB.

private:
  int parse_args (int argc, ACE_TCHAR *argv[]);
  // Parses the command line arguments.

  int write_IOR (const char *ior);
  // Writes the server ior to a file, for the clients to pick up
  // later.

  const char *ior_file_name_;
  // The file name to save the ior to.

  TAO_ORB_Manager orb_manager_;
  // The tao orb manager object.

  Broadcaster_i broadcaster_i_;
  // The servant object registered with the orb.
};

#endif /* SERVER_I_H */
