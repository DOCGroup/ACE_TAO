/* -*- C++ -*- */
// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/tests/Simple/chat
//
// = FILENAME
//    Client_i.h
//
// = DESCRIPTION
//    Definition of the Chat Client class, Client_i.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ===========================================================

#ifndef CLIENT_I_H
#define CLIENT_I_H

#include "Receiver_i.h"
#include "BroadcasterC.h"
#include "ace/Read_Buffer.h"
#include "ace/Event_Handler.h"
#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Utils/ORB_Manager.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"

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
  Client_i (void);
  // Constructor.

  int init (int argc, ACE_TCHAR *argv[]);
  // Initialize the client communication with the server.

  int run (void);
  // Start the ORB object.

  virtual int handle_input (ACE_HANDLE);
  // Handle the user input.

private:
  int parse_args (int argc, ACE_TCHAR *argv[]);
  // Parse the command line arguments.
  // Returns 0 on success, -1 on error.

  int read_ior (const ACE_TCHAR *filename);
  // Function to read the server ior from a file.

  ACE_CString ior_;
  // IOR of the obj ref of the server.

  const ACE_TCHAR* ior_file_name_;
  // The filename that stores the ior of the server

  ACE_CString nickname_;
  // Nickname of the user chatting.

  TAO_ORB_Manager orb_manager_;
  // Our orb.

  Broadcaster_var server_;
  // Server object ptr.

  typedef TAO_Intrusive_Ref_Count_Handle<Receiver_i> Receiver_i_var;
  Receiver_i_var receiver_i_;
  // The receiver object.

  Receiver_var receiver_var_;
  // Pointer to the receiver object registered with the ORB.
};

#define QUIT_STRING "quit"
// The string that the user must type to quit the chat.

#endif /* CLIENT_I_H */
