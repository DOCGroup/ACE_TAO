// -*- C++ -*-

//=============================================================================
/**
 *  @file    nestea_server_i.h
 *
 *  Server that sets up the ORB and handles the registration and execution
 *  of the Nestea Bookshelf Server.
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================


#if !defined (NESTEA_SERVER_I_H)
#define NESTEA_SERVER_I_H

#include "nestea_i.h"

/**
 * @class Nestea_Server_i
 *
 * @brief Nestea Bookshelf Server Implementation Class
 *
 * Sets up everything necessary to get the Nestea Bookshelf Server
 * running.
 */
class Nestea_Server_i
{
public:
  /// Default constructor
  Nestea_Server_i (const char *filename = "nestea.dat");

  /// Destructor
  ~Nestea_Server_i ();

  /// Initialize the Server state - parsing arguments and waiting
  int init (int argc, ACE_TCHAR **argv);

  /// Run the orb
  int run ();

private:
  /// Parses the commandline arguments.
  int parse_args ();

  /// Number of command line arguments.
  int argc_;

  /// The command line arguments.
  ACE_TCHAR **argv_;

  CORBA::ORB_var orb_;

  PortableServer::POA_var root_poa_;

  /// The POA for the Nestea Server.
  PortableServer::POA_var nestea_poa_;

  /// The POA manager.
  PortableServer::POAManager_var poa_manager_;

  /// The Nestea Bookshelf Server Implementation.
  Nestea_i *server_impl_;

  /// File where the IOR of the server object is stored.
  FILE *ior_output_file_;
};

#endif /* NESTEA_SERVER_I_H */
