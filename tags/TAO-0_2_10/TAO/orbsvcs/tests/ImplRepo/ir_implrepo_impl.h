// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/ImplRepo
// 
// = FILENAME
//    ir_implrepo_impl.h
//
// = DESCRIPTION
//    This class implements the Simple Object object.
//
// = AUTHOR
//    Darrell Brunsch <brunsch@cs.wustl.edu>
// 
// ============================================================================

#if !defined (IR_IMPLREPO_IMPL_H)
#define	IR_IMPLREPO_IMPL_H

#include "Impl_RepoS.h"
#include "tao/TAO.h"

// Forward declarations.
class IR_iRepo_i;
class IR_Simple_i;

// Typedefs.
typedef IR_iRepo_i *IR_iRepo_i_ptr;
typedef IR_iRepo_i_ptr IR_iRepo_i_ref;
typedef IR_Simple_i *IR_Simple_i_ptr;
typedef IR_Simple_i_ptr IR_Simple_i_ref;

class IR_iRepo_i : public POA_iementation_Repository
{
  // = TITLE
  //    Implementation Repository 
  //
  // = DESCRIPTION
  //    This provides the interface to communicate directly with the
  //    Implementation Repository.
public:
  // = Constructor and destructor
  IR_iRepo_i (void);
  ~IR_iRepo_i (void);

  // = Interface methods
//  virtual void register_server (const char *server,
//                                const Implementation_Repository::Process_Options &options,
//                                CORBA::Environment &_tao_environment);

  virtual void server_is_running (const char *server,
                                  const Implementation_Repository::INET_Addr &addr,
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

  int parse_args (void);
  // Parses the commandline arguments.

  int read_ior (char *filename);
  // Reads the IOR of the real server from the file

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
                  IR_iRepo_i *ir_impl);

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

  class IR_iRepo_i *ir_impl_;

  CORBA::ORB_var orb_var_;
  PortableServer::POA_var poa_var_;
  CORBA::Object_var forward_to_var_;
};


#endif /* IR_IMPLREPO_IMPL_H */
