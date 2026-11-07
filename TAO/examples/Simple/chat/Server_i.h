/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Server_i.h
 *
 *  Definition of the Chat Server_i class.
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
  /// Constructor.
  Server_i ();

  /// Destructor.
  ~Server_i () = default;

  Server_i (Server_i const &) = delete;
  Server_i &operator= (Server_i const &) = delete;
  Server_i (Server_i &&) = delete;
  Server_i &operator= (Server_i &&) = delete;

  /// Initialize the server.
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Run the ORB.
  int run ();

private:
  /// Parses the command line arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Writes the server ior to a file, for the clients to pick up
  /// later.
  int write_IOR (char const *ior);

  /// The file name to save the ior to.
  ACE_TCHAR const *ior_file_name_ { ACE_TEXT ("chat.ior") };

  /// The tao orb manager object.
  TAO_ORB_Manager orb_manager_;

  /// The servant object registered with the orb.
  using Broadcaster_i_var = TAO_Intrusive_Ref_Count_Handle<Broadcaster_i>;
  Broadcaster_i_var broadcaster_i_;
};

#endif /* SERVER_I_H */
