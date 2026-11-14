// -*- C++ -*-

//=============================================================================
/**
 *  @file    airplane_server_i.h
 *
 *  Server that sets up the ORB and handles the registration and execution
 *  of the Paper Airplane Server.
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#if !defined (AIRPLANE_SERVER_I_H)
#define AIRPLANE_SERVER_I_H

#include "airplane_i.h"

#include "ace/OS_NS_stdio.h"

#include <memory>

/**
 * @class Airplane_Server_i
 *
 * @brief Paper Airplane Server Implementation Class
 *
 * Sets up everything necessary to get the Paper Airplane Server
 * running.
 */
class Airplane_Server_i
{
public:
  /// Default constructor
  Airplane_Server_i () = default;

  ~Airplane_Server_i () = default;

  Airplane_Server_i (Airplane_Server_i const &) = delete;
  Airplane_Server_i &operator= (Airplane_Server_i const &) = delete;
  Airplane_Server_i (Airplane_Server_i &&) = delete;
  Airplane_Server_i &operator= (Airplane_Server_i &&) = delete;

  /// Initialize the Server state - parsing arguments and waiting
  int init (int argc, ACE_TCHAR **argv);

  /// Run the orb
  int run ();

private:
  /// Parses the commandline arguments.
  int parse_args ();

  /// Number of command line arguments.
  int argc_ {};

  /// The command line arguments.
  ACE_TCHAR **argv_ {};

  /// The ORB.
  CORBA::ORB_var orb_ {};

  PortableServer::POA_var root_poa_ {};

  /// The POA for the Airplane Server.
  PortableServer::POA_var airplane_poa_ {};

  /// The POA manager.
  PortableServer::POAManager_var poa_manager_ {};

  /// The Paper Airplane Server Implementation.
  PortableServer::ServantBase_var server_impl_ {};

  /// File where the IOR of the server object is stored.
  using File_Handle = std::unique_ptr<FILE, int (*) (FILE *)>;
  File_Handle ior_output_file_ {nullptr, ACE_OS::fclose};

  /// File where the pid of the server is stored.
  File_Handle pid_output_file_ {nullptr, ACE_OS::fclose};

  /// The server name
  ACE_CString server_name_ {"airplane_server"};
};

#endif /* AIRPLANE_SERVER_I_H */
