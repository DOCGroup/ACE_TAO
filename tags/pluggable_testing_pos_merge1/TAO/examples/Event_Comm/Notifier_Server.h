/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    EventComm
//
// = FILENAME
//    Notifier_Server.h
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef _NOTIFIER_SERVER_H
#define _NOTIFIER_SERVER_H

#include "Event_Comm_i.h"
#include "tao/TAO.h"
#include "orbsvcs/CosNamingS.h"
#include "orbsvcs/Naming/Naming_Utils.h"

class Notifier_Server
{
  // = TITLE
  //    The Notifier Server that handles CORBA operations from
  //    Consumers and Suppliers.
public:
  // = Initialization and termination methods.
  Notifier_Server (void);
  // Constructor.

  ~Notifier_Server (void);
  // Destructor.

 int init (int argc, char *argv[], CORBA::Environment &env);
  // Initialize the Server state.

  int run (CORBA::Environment &env);
  // Run the orb.

  int close (void);
  // Close the naming service.

  ACE_Reactor *reactor(void);
  // Reactor accessor.

private:
  int init_naming_service (CORBA::Environment &env);
  // Initialises the name server and registers the Notifier server
  // object name with the name server.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager.

  Notifier_i servant_;
  // Servant for the Notifier interface.

  CosNaming::NamingContext_var naming_context_;
  // Naming context for the naming service.

  TAO_Naming_Server naming_server_;
  // An instance of the name server, wherein the naming context
  // containing the factory of objects will be registered.
};

#define NOTIFIER_BIND_NAME "Notifier"

#endif /* _NOTIFIER_SERVER_H */
