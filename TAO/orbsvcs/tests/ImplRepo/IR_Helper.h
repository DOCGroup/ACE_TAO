// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/ImplRepo
//
// = FILENAME
//    IR_Helper.h
//
// = DESCRIPTION
//    Class which contains basic operations needed by a server to use the IR.
//
// = AUTHORS
//    Darrell Brunsch <brunsch@cs.wustl.edu>
//
// ============================================================================

#if !defined (IR_HELPER_H)
#define IR_HELPER_H

#include "tao/TAO.h"
#include "orbsvcs/ImplRepoC.h"

class POA_Ping_Object;

class IR_Helper
  // = TITLE
  //   Implementation Repository Helper Class
  //
  // = DESCRIPTION
  //   Contains basic IR operations required by a server.
  //   
{
public:
  IR_Helper (char *server_name, PortableServer::POA_ptr poa, CORBA::ORB_ptr orb, int debug = 0);
  // Constructor
  ~IR_Helper ();
  // Destructor

  void notify_startup (CORBA_Environment &_env = CORBA_Environment::default_environment ());
  // Notify the IR that the server is  now running.

  void notify_shutdown (CORBA_Environment &_env = CORBA_Environment::default_environment ());
  // Notify the IR that the server has been shut down.
  
  int register_server (const char *comm_line, 
                       const char *environment = "", 
                       const char *working_dir = "",
                       CORBA_Environment &_env = CORBA_Environment::default_environment ());
  // Tell the IR how to start yourself up.

  void change_object (CORBA::Object_ptr obj, 
                      CORBA_Environment &_env = CORBA_Environment::default_environment ());
  // Convert an object into a IR controlled object.
private:
  int read_ir_ior (CORBA_Environment &_env = CORBA_Environment::default_environment ());

  char *name_;
  char *ir_key_;

  
  Implementation_Repository::INET_Addr *ir_addr_;
  
  POA_Ping_Object *ping_;  
  CORBA::Object_ptr ping_ptr_;
  
  Implementation_Repository *implrepo_;
  PortableServer::POA_var poa_;
  CORBA::ORB_var orb_;
  int debug_; 
};

#endif /* IR_HELPER_H */
