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

#include "orbsvcs/ImplRepoS.h"
#include "Repository.h"
#include "tao/TAO.h"

// Forward declarations.
class ImplRepo_i;
class IR_Forwarder;

// Typedefs.
typedef ImplRepo_i *ImplRepo_i_ptr;
typedef ImplRepo_i_ptr ImplRepo_i_ref;

class IR_Adapter_Activator : public POA_PortableServer::AdapterActivator
{
  // = TITLE
  //    Implementation Repository Adapter Activator
  //
  // = DESCRIPTION
  //    Part of the Default Servant/DSI combination that forwards
  //    arbitrary requests.  This allows for the setting up of child
  //    POAs with default servants.
public:
  IR_Adapter_Activator (IR_Forwarder *servant);
  // Constructor

  virtual CORBA::Boolean unknown_adapter (PortableServer::POA_ptr parent,
                                          const char *name,
                                          CORBA_Environment &_env = CORBA_Environment::default_environment ());
private:
  // @@ Darrell, please add comments for all of the methods in this file.

  IR_Forwarder *servant_;
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

  virtual CORBA::Object_ptr activate_object (CORBA::Object_ptr obj,
                                             CORBA::Environment &env);

  virtual Implementation_Repository::INET_Addr *activate_server (const char * server,
                                                                 CORBA::Environment &env);

  virtual void register_server (const char * server,
                                const Implementation_Repository::Process_Options &options,
                                CORBA::Environment &env);

  virtual void reregister_server (const char * server,
                                  const Implementation_Repository::Process_Options &options,
                                  CORBA::Environment &env);

  virtual void remove_server (const char * server,
                              CORBA::Environment &env);

  virtual Implementation_Repository::INET_Addr  
    *server_is_running (const char * server,
                        const Implementation_Repository::INET_Addr &addr,
                        CORBA::Object_ptr ping,
                        CORBA::Environment &env);

  virtual void server_is_shutting_down (const char * server,
                                        CORBA::Environment &env);

  // = Other methods

  int init (int argc, char **argv, CORBA::Environment& env);
  // Initialize the Server state - parsing arguments and waiting.

  int run (CORBA::Environment& env);
  // Runs the orb.

  CORBA::String get_forward_host (const char *server);
  // Returns the host of the server that needs to be forwarded to.

  CORBA::UShort get_forward_port (const char *server);
  // Returns the port of the server that needs to be forwarded to.

private:
  IR_Forwarder *forwarder_impl_;

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

  unsigned int debug_level_;
  // Debug level for the IR.  
  // 0 - Quiet
  // 1 - Trace messages
  // 2 - Detailed messages
};

class IR_Forwarder: public  PortableServer::DynamicImplementation
{
public:
  IR_Forwarder (CORBA::ORB_ptr orb_ptr,
                PortableServer::POA_ptr poa_ptr,
                ImplRepo_i *ir_impl);

  virtual void invoke (CORBA::ServerRequest_ptr request,
                       CORBA::Environment &env);
  // The invoke() method receives requests issued to any CORBA
  // object incarnated by the DSI servant and performs the
  // processing necessary to execute the request.

  CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &oid,
                                          PortableServer::POA_ptr poa,
                                          CORBA::Environment &env);

private:
  int forward (char *name, char *poa, char *obj, CORBA::Environment &env);

  class ImplRepo_i *ir_impl_;

  CORBA::ORB_var orb_var_;
  PortableServer::POA_var poa_var_;

};

#endif /* IMPLREPO_I_H */
