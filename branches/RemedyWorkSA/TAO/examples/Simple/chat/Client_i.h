/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Client_i.h
 *
 *  $Id$
 *
 *  Definition of the Chat Client class, Client_i.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


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

/**
 * @class Client_i
 *
 * @brief Chat Client class.
 *
 * Connects to the Chat server and registers the Receiver_i
 * object with the chat server.  It also takes in user chat
 * messages and sends them to the server.
 */
class Client_i : public ACE_Event_Handler
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  Client_i (void);

  /// Initialize the client communication with the server.
  int init (int argc, ACE_TCHAR *argv[]);

  /// Start the ORB object.
  int run (void);

  /// Handle the user input.
  virtual int handle_input (ACE_HANDLE);

private:
  /// Parse the command line arguments.
  /// Returns 0 on success, -1 on error.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Function to read the server ior from a file.
  int read_ior (const ACE_TCHAR *filename);

  /// IOR of the obj ref of the server.
  ACE_CString ior_;

  /// The filename that stores the ior of the server
  const ACE_TCHAR* ior_file_name_;

  /// Nickname of the user chatting.
  ACE_CString nickname_;

  /// Our orb.
  TAO_ORB_Manager orb_manager_;

  /// Server object ptr.
  Broadcaster_var server_;

  /// The receiver object.
  typedef TAO_Intrusive_Ref_Count_Handle<Receiver_i> Receiver_i_var;
  Receiver_i_var receiver_i_;

  /// Pointer to the receiver object registered with the ORB.
  Receiver_var receiver_var_;
};

#define QUIT_STRING "quit"
// The string that the user must type to quit the chat.

#endif /* CLIENT_I_H */
