// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/tests/Simple/chat
//
// = FILENAME
//    client_i.h
//
// = DESCRIPTION
//    Definition of the Chat Client class, Client_i.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ===========================================================

#if !defined (CLIENT_I_H)
#define CLIENT_I_H

#include "Receiver_i.h"
#include "BroadcasterC.h"
#include "ace/Read_Buffer.h"
#include "tao/TAO.h"

class Client_i : public ACE_Event_Handler
{
  // = TITLE
  //    Chat Client class.
  //
  // = DESCRIPTION
  //    Connects to the Chat server and registers the Receiver_i
  //    object with the chat server.  It also takes in user chat
  //    messages and sends them to the server.
public:
  // = Initialization and termination methods.
  Client_i (char *ior_file_name,
	    char* nickname);
  // Constructor.

  ~Client_i (void);
  // Destructor.

  int init (int argc, char *argv[]);
  // Initialize the client communication with the server.

  int run (void);
  // Start the ORB object.

  virtual int handle_input (ACE_HANDLE);
  // Handle the user input.

 private:
  int read_ior (const char *filename);
  // Function to read the server ior from a file.

  char *ior_;
  // IOR of the obj ref of the server.

  const char* ior_file_name_;
  // The filename that stores the ior of the server

  const char* nickname_;
  // Nickname of the user chatting.

  Broadcaster_var server_;
  // Server object ptr.

  CORBA::ORB_var orb_;
  // Our orb.

  Receiver_i receiver_i_;
  // The receiver object.

  Receiver_var receiver_var_;
  // Pointer to the receiver object registered with the ORB.
};

#endif /* CLIENT_I_H */
