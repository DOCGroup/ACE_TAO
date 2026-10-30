
//=============================================================================
/**
 *  @file    Simple_util.h
 *
 *  The classe define the templates for the client and server.
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_UTIL_H
#define TAO_UTIL_H

#include "tao/Utils/ORB_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"

#include <optional>
#include <string>

/**
 * @class Server
 *
 * @brief A set of useful class Templates for using the TAO CORBA
 * implementation.
 *
 * A template server definition. This template can be used by
 * single server/client projects for definition of their
 * server/clients.  See the directories time, bank, echo for
 * further details of implemenatation.
 */
template <class Servant>
class Server
{
public:
  /// Constructor.
  Server () = default;

  /// Destructor.
  ~Server () = default;

  Server (Server const&) = delete;
  Server (Server&&) = delete;
  Server& operator= (Server const&) = delete;
  Server& operator= (Server&&) = delete;

  /// Initialize the Server state - parsing arguments and waiting.
  /// interface_name is the name used to register the Servant.
  int init (char const *servant_name,
            int argc,
            ACE_TCHAR *argv[]);

  // int register_name ();
  // After calling <init>, this method will register the server with
  // the TAO Naming Service using the servant_name passed to <init>.

  /// Run the orb.
  int run ();

 protected:
  /// Servant class
  Servant servant_;

  /// Parses the commandline arguments.
  int parse_args ();

  /// The ORB manager - a helper class for accessing the POA and
  /// registering objects.
  TAO_ORB_Manager orb_manager_;

  /// File where the IOR of the server object is stored.
  FILE *ior_output_file_ {};

  /// Memory pool name that stores the state
  ACE_TCHAR const *mem_pool_name_ {};

  /// Number of command line arguments.
  int argc_ {};

  /// The command line arguments.
  ACE_TCHAR **argv_ {};
};


// Client Class starts here

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
  /// Constructor.
  Client () = default;

  /// Destructor.
  ~Client () = default;

  Client (Client const&) = delete;
  Client (Client&&) = delete;
  Client& operator= (Client const&) = delete;
  Client& operator= (Client&&) = delete;

  /// Initialize the client communication endpoint with server.
  int init (char const *name, int argc, ACE_TCHAR *argv[]);

  /// Return the interface object pointer.
  InterfaceObj *operator-> () { return server_.in (); }

  /// Returns the shutdown flag.
  int shutdown ();

  /// Fills in the shutdwon flag.
  void shutdown (int);

  /// Initialize naming service
  int obtain_initial_references ();

protected:
  /// Function to read the server IOR from a file.
  int read_ior (ACE_TCHAR const *filename);

  /// Parses the arguments passed on the command line.
  int parse_args ();

  /// Remember our orb.
  CORBA::ORB_var orb_;

  /// # of arguments on the command line.
  int argc_ {};

  /// arguments from command line.
  ACE_TCHAR **argv_ {};

  /// IOR of the obj ref of the server.
  std::optional<std::string> ior_ {};

  /// Flag to use the naming service
  bool naming_ {};

  /// Flag for shutting down the server
  bool shutdown_ {};

  /// Server object
  Var server_;
};

#include "Simple_util.cpp"

#endif /* TAO_UTIL_H */
