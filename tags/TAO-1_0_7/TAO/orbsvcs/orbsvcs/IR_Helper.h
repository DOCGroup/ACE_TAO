// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs
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
#include "orbsvcs/ImplRepoS.h"

class TAO_ORBSVCS_Export IR_Helper
  // = TITLE
  //   Implementation Repository Helper Class
  //
  // = DESCRIPTION
  //   Contains basic IR operations required by a server.
  //   
{
public:
  IR_Helper (const char *server_name, PortableServer::POA_ptr poa, CORBA::ORB_ptr orb, int debug = 0);
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
  char *name_;
  
  ImplementationRepository::Address *ir_addr_;
  
  POA_ImplementationRepository::ServerObject *server_object_;  
  // Pointer to our implementation of the server object

  ImplementationRepository::ServerObject_ptr server_object_ptr_;
  // Object pointer to <server_object>
  
  ImplementationRepository::Administration *implrepo_;
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  int debug_; 
};

#endif /* IR_HELPER_H */
