// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/tests/Simple/chat
//
// = FILENAME
//    Chat_i.h
//
// = DESCRIPTION
//    Definition of the Chat Client class, Chat_i.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ===========================================================

#if !defined (CHAT_I_H)
#define CHAT_I_H

#include "Receiver_i.h"
#include "BroadcasterC.h"
#include "ace/Read_Buffer.h"
#include "tao/TAO.h"

class Chat_i : public ACE_Event_Handler
{
  // = TITLE
  //    Chat Client class.
  //
  // = DESCRIPTION
  //    Connects to the Chat server and registers the Receiver_i object
  //    with the chat server.
  //    It also takes in user chat messages and sends them to the server.
  //

public:
  // = Initialization and termination methods.

  Chat_i (char *ior_file_name, char* nick_);
  // Constructor.

  ~Chat_i (void);
  // Destructor.

  int init (int argc, char *argv[]);
  // Initialize the client communication with the server

  int run (void);
  // start the orb  object.

  virtual int handle_input (ACE_HANDLE);
  // handle the user input

 private:
  int read_ior (const char *filename);
  // function to read the server ior from a file.

  char *ior_;
  // IOR of the obj ref of the server.

  const char* ior_file_name_;
  // The filename that stores the ior of the server

  const char* nick_;
  // nickname of the user chatting

  Broadcaster_var server_;
  // Server object ptr.

  CORBA::ORB_var orb_;
  // our orb

  Receiver_i receiver_i_;
  // The receiver object.

  Receiver_var receiver_var_;
  // pointer to the receiver object registered with the orb.
};

#endif /* CHAT_I_H */
