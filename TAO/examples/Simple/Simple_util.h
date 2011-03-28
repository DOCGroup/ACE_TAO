
//=============================================================================
/**
 *  @file    Simple_Util.h
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
#include "orbsvcs/Naming/Naming_Client.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"

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
  /// the TAO Naming Service using the name passed to this method.
  int register_name (const char *name);

  /// Run the orb.
  int run (void);

  /// Ignore this method if you are not testing the InterOperable
  /// Naming Service.
  int test_for_ins (const char *ior);

private:
  /// Parses the commandline arguments.
  int parse_args (void);

  /// A holder of the servant that does ref counting for him.
  typedef TAO_Intrusive_Ref_Count_Handle<Servant> Servant_var;
  Servant_var servant_;

  /// The ORB manager - a helper class for accessing the POA and
  /// registering objects.
  TAO_ORB_Manager orb_manager_;

  /// helper class for getting access to Naming Service.
  TAO_Naming_Client naming_client_;

  /// File where the IOR of the server object is stored.
  ACE_TCHAR *ior_output_file_;

  /// Flag to indicate whether naming service could be used
  int naming_;

  /// Used test the INS.
  ACE_TCHAR *ins_;

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
template <class ServerInterface>
class Client
{
public:

  // = Initialization and termination methods.
  /// Constructor.
  Client (void);

  /// Destructor.
  ~Client (void);

  /// Initialize the client communication endpoint with server.
  int init (const char *name, int argc, ACE_TCHAR *argv[]);

  /// Return the interface object pointer.
  ServerInterface *operator-> () { return server_.in (); }

  /// Returns the shutdown flag.
  int do_shutdown (void);

  /// Fills in the shutdwon flag.
  void do_shutdown (int);

  /// Initialize naming service
  int obtain_initial_references (const char *name);

  CORBA::ORB_ptr orb (void)
    {
      return CORBA::ORB::_duplicate (this->orb_.in ());
    }

private:
  /// Function to read the server IOR from a file.
  int read_ior (ACE_TCHAR *filename);

  /// Parses the arguments passed on the command line.
  int parse_args (void);

  /// Remember our orb.
  CORBA::ORB_var orb_;

  /// helper class for getting access to Naming Service.
  TAO_Naming_Client naming_client_;

  /// # of arguments on the command line.
  int argc_;

  /// arguments from command line.
  ACE_TCHAR **argv_;

  /// IOR of the obj ref of the server.
  ACE_CString ior_;

  /// Server object
  typedef TAO_Intrusive_Ref_Count_Handle<ServerInterface> ServerInterface_var;
  ServerInterface_var server_;

  /// Flag to use the naming service
  int naming_;

  /// Flag for shutting down the server
  int do_shutdown_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Simple_util.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */
#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Simple_util.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_UTIL_H */
