//$Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Simple
//
// = FILENAME
//    Simple_Util.h
//
// = DESCRIPTION
//    The classe define the templates for the client and server.
//
// = AUTHOR
//   Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_UTIL_H
#define TAO_UTIL_H

#include "tao/Utils/ORB_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"

template <class Servant>
class Server
{
  // = TITLE
  //   A set of useful class Templates for using the TAO CORBA
  //   implementation.
  //
  // = DESCRIPTION
  //   A template server definition. This template can be used by
  //   single server/client projects for defintion of their
  //   server/clients.  See the directories time, bank, echo for
  //   further details of implemenatation.
public:
  // = Initialization and termination methods.

  Server (void);
  // Constructor.

  ~Server (void);
  // Destructor.

  int init (const char *servant_name,
            int argc,
            ACE_TCHAR *argv[]);
  // Initialize the Server state - parsing arguments and waiting.
  // interface_name is the name used to register the Servant.

  int run (void);
  // Run the orb.

  int test_for_ins (const char *ior);
  // Ignore this method if you are not testing the InterOperable
  // Naming Service.

private:
  int parse_args (void);
  // Parses the commandline arguments.

  typedef TAO_Intrusive_Ref_Count_Handle<Servant> Servant_var;
  Servant_var servant_;
  // A holder of the servant that does ref counting for him.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager - a helper class for accessing the POA and
  // registering objects.

  ACE_TCHAR *ior_output_file_;
  // File where the IOR of the server object is stored.

  int naming_;
  // Flag to indicate whether naming service could be used

  ACE_TCHAR *ins_;
  // Used test the INS.

  int argc_;
  // Number of command line arguments.

  ACE_TCHAR **argv_;
  // The command line arguments.
};

template <class ServerInterface>
class Client
{
  // = TITLE
  //   Template Client class
  //
  // = DESCRIPTION
  //   A template client implementation for a single server/client
  //   model. The example usage of these usage can be found in the
  //   sub-directories below
public:

  // = Initialization and termination methods.
  Client (void);
  // Constructor.

  ~Client (void);
  // Destructor.

  int init (const char *name, int argc, ACE_TCHAR *argv[]);
  // Initialize the client communication endpoint with server.

  ServerInterface *operator-> () { return server_.in (); }
  // Return the interface object pointer.

  int do_shutdown (void);
  // Returns the shutdown flag.

  void do_shutdown (int);
  // Fills in the shutdwon flag.

  int obtain_initial_references (const char *name);
  // Initialize naming service

  CORBA::ORB_ptr orb (void)
    {
      return CORBA::ORB::_duplicate (this->orb_.in ());
    }

private:
  int read_ior (ACE_TCHAR *filename);
  // Function to read the server IOR from a file.

  int parse_args (void);
  // Parses the arguments passed on the command line.

  CORBA::ORB_var orb_;
  // Remember our orb.

  int argc_;
  // # of arguments on the command line.

  ACE_TCHAR **argv_;
  // arguments from command line.

  ACE_CString ior_;
  // IOR of the obj ref of the server.

  typedef TAO_Intrusive_Ref_Count_Handle<ServerInterface> ServerInterface_var;
  ServerInterface_var server_;
  // Server object

  int naming_;
  // Flag to use the naming service

  int do_shutdown_;
  // Flag for shutting down the server
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Simple_util.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */
#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Simple_util.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_UTIL_H */
