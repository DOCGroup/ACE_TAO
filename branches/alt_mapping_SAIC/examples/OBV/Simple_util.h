

//=============================================================================
/**
 *  @file    Simple_util.h
 *
 *  $Id$
 *
 *  The classe define the templates for the client and server.
 *
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_UTIL_H
#define TAO_UTIL_H

#include "tao/Utils/ORB_Manager.h"

#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

/**
 * @class Server
 *
 * @brief A set of useful class Templates for using the TAO CORBA
 * implementation.
 *
 * A template server definition. This template can be used by
 * single server/client projects for defintion of their
 * server/clients.  See the directories time, bank, echo for
 * further details of implemenatation.
 */
template <class Servant>
class Server
{
public:
  // = Initialization and termination methods.

  /// Constructor.
  Server (void);

  /// Destructor.
  ~Server (void);

  /// Initialize the Server state - parsing arguments and waiting.
  /// interface_name is the name used to register the Servant.
  int init (const char *servant_name,
            int argc,
            ACE_TCHAR *argv[]);

  /// After calling <init>, this method will register the server with
  /// the TAO Naming Service using the servant_name passed to <init>.
  int register_name (void);

  /// Run the orb.
  int run (void);

protected:
  /// Servant class
  Servant servant_;

  /// name of the servant to be used for TAO Naming Service
  const char *name;

  /// Parses the commandline arguments.
  int parse_args (void);

  /// The ORB manager - a helper class for accessing the POA and
  /// registering objects.
  TAO_ORB_Manager orb_manager_;

  // TAO_Naming_Server namingServer;
  // helper class for getting access to Naming Service.

  /// File where the IOR of the server object is stored.
  FILE *ior_output_file_;

  /// Number of command line arguments.
  int argc_;

  /// The command line arguments.
  ACE_TCHAR **argv_;
};

/**
 * @class Client
 *
 * @brief Template Client class
 *
 * A template client implementation for a single server/client
 * model. The example usage of these usage can be found in the
 * sub-directories below
 */
template <class InterfaceObj, class Var>
class Client
{
public:

  // = Initialization and termination methods.
  /// Constructor.
  Client (void);

  /// Destructor.
  ~Client (void);

  /// Initialize the client communication endpoint with server.
  int init (const char *name,int argc, ACE_TCHAR *argv[]);

  /// Return the interface object pointer.
  InterfaceObj *operator-> () { return server_.in ();};

  /// Returns the shutdown flag.
  int shutdown (void );

  /// Fills in the shutdwon flag.
  void shutdown (int);

  /// Initialize naming service
  int obtain_initial_references (void);

protected:
  /// Function to read the server IOR from a file.
  int read_ior (ACE_TCHAR *filename);

  /// Parses the arguments passed on the command line.
  int parse_args (void);

//  TAO_Naming_Client namingClient;
  // helper class for getting access to Naming Service.

  /// # of arguments on the command line.
  int argc_;

  /// arguments from command line.
  ACE_TCHAR **argv_;

  /// IOR of the obj ref of the server.
  char *ior_;

  /// Name to be usred for the naming service
  char *name_;

  /// Remember our orb.
  CORBA::ORB_var orb_;

  /// Server object
  Var server_;

  /// Flag to use the naming service
  int naming_;

  /// Flag for shutting down the server
  int shutdown_;

};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Simple_util.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */
#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Simple_util.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_UTIL_H */
