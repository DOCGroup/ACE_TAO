/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/ImplRepo_Service
//
// = FILENAME
//    ImplRepo_i.h
//
// = DESCRIPTION
//    This class implements the Implementation Repository.
//
// = AUTHOR
//    Darrell Brunsch <brunsch@cs.wustl.edu>
//
// ============================================================================

#if !defined (IMPLREPO_I_H)
#define IMPLREPO_I_H

#include "ImplRepoS.h"
#include "Repository.h"
#include "tao/TAO.h"

// Forward declarations.
class ImplRepo_i;
class IR_Simple_i;

// Typedefs.
typedef ImplRepo_i *ImplRepo_i_ptr;
typedef ImplRepo_i_ptr ImplRepo_i_ref;
typedef IR_Simple_i *IR_Simple_i_ptr;
typedef IR_Simple_i_ptr IR_Simple_i_ref;

class IR_Adapter_Activator : public POA_PortableServer::AdapterActivator
{
  // = TITLE
  //    Implementation Repository Adapter Activator
  //
  // = DESCRIPTION
  //    Part of the Default Servant/DSI combination that forwards
  //    arbitrary requests.  This allows for the setting up of child POAs
  //    with default servants.
public:
  // Constructor
  IR_Adapter_Activator (IR_Simple_i_ptr servant);

  virtual CORBA::Boolean unknown_adapter (PortableServer::POA_ptr parent,
                                          const char *name,
                                          CORBA_Environment &_env = CORBA_Environment::default_environment ());
private:
  IR_Simple_i_ptr servant_;
};

class ImplRepo_i : public POA_Implementation_Repository
{
  // = TITLE
  //    Implementation Repository
  //
  // = DESCRIPTION
  //    This provides the interface to communicate directly with the
  //    Implementation Repository.
public:
  // = Constructor and destructor
  ImplRepo_i (void);
  ~ImplRepo_i (void);

  // = Interface methods
  virtual void register_server (const char *server,
                                const Implementation_Repository::Process_Options &options,
                                CORBA::Environment &env);

  virtual void server_is_running (const char *server,
                                  const Implementation_Repository::INET_Addr &addr,
                                  Ping_Object_ptr ping,
                                  CORBA::Environment &_tao_environment);

  // = Other methods

  int init (int argc, char **argv, CORBA::Environment& env);
  // Initialize the Server state - parsing arguments and waiting

  int run (CORBA::Environment& env);
  // Run the orb

  void start (const char *server);
  // Starts the program registered as <server>

private:
  IR_Simple_i *server_impl_;

  IR_Adapter_Activator *activator_;
  // Used for the forwarding of any type of POA.

  Repository repository_;
  // Repository containing information about each server.

  int parse_args (void);
  // Parses the commandline arguments.

  int read_ior (char *filename);
  // Reads the IOR of the real server from the file.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager.

  FILE *ior_output_file_;
  // File where the IOR of the server object is stored.

  char *server_key_;
  // Key of the obj ref of the server.

  char *server_input_file_;
  // Copy of the filename for the server output file.

  int argc_;
  // Number of command line arguments.

  char **argv_;
  // The command line arguments.
};

class IR_Simple_i: public POA_simple_object
{
  // = TITLE
  //    Simple Object Implementation
  //
  // = DESCRIPTION
  //    Implementation of a simple object that has two methods, one that
  //    returns the cube of a long, another that shuts down the server.
public:
  //  = Constructor and Destructor
  IR_Simple_i (CORBA::ORB_ptr orb_ptr,
                  PortableServer::POA_ptr poa_ptr,
                  ImplRepo_i *ir_impl);

  ~IR_Simple_i (void);

  // = Interface methods
  virtual CORBA::Long simple_method (CORBA::Long l,
                                     CORBA::Environment &env);
  // Just cubes the long parameter

  virtual void shutdown (CORBA::Environment &env);
  // Shutdown routine.

  // = Other methods
  void forward_to (CORBA::Object_ptr forward_to_ptr);
  // Sets the ior of the forward to ptr)

private:
  int forward (CORBA::Environment &env);

  class ImplRepo_i *ir_impl_;

  CORBA::ORB_var orb_var_;
  PortableServer::POA_var poa_var_;
  CORBA::Object_ptr forward_to_ptr_;
};


#endif /* IMPLREPO_I_H */
