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
#include "tao/Utils/ORB_Manager.h"
#include "orbsvcs/CosNamingS.h"
#include "orbsvcs/Naming/Naming_Client.h"

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

 int init (int argc, char *argv[]);
  // Initialize the Server state.

  int run (void);
  // Run the orb.

  int close (void);
  // Close the naming service.

  ACE_Reactor *reactor(void);
  // Reactor accessor.

private:
  int init_naming_service (void);
  // Initialises the name server and registers the Notifier server
  // object name with the name server.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager.

  Notifier_i servant_;
  // Servant for the Notifier interface.

  CosNaming::NamingContext_var naming_context_;
  // Naming context for the naming service.

  TAO_Naming_Client naming_server_;
  // helper class for getting access to Naming Service.
};

#define NOTIFIER_BIND_NAME "Notifier"

#endif /* _NOTIFIER_SERVER_H */
