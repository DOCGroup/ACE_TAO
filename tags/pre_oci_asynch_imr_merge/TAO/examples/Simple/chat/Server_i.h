/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Server_i.h
 *
 *  $Id$
 *
 *  Definition of the Chat Server_i class.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef SERVER_I_H
#define SERVER_I_H

#include "Broadcaster_i.h"
#include "tao/Utils/ORB_Manager.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"

/**
 * @class Server_i
 *
 * @brief The class defines the server for the chat. It sets up the Orb
 * manager and registers the Broadcaster servant object.
 */
class Server_i
{

public:
  // = Initialization and termination methods.
  /// Constructor.
  Server_i (void);

  /// Destructor.
  ~Server_i (void);

  /// Initialize the server.
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Run the ORB.
  int run (void);

private:
  /// Parses the command line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Writes the server ior to a file, for the clients to pick up
  /// later.
  int write_IOR (const char *ior);

  /// The file name to save the ior to.
  const ACE_TCHAR *ior_file_name_;

  /// The tao orb manager object.
  TAO_ORB_Manager orb_manager_;

  /// The servant object registered with the orb.
  typedef TAO_Intrusive_Ref_Count_Handle<Broadcaster_i> Broadcaster_i_var;
  Broadcaster_i_var broadcaster_i_;
};

#endif /* SERVER_I_H */
