// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/Time_Service
//
// = FILENAME
//    Server_i.h
//
// = AUTHOR
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#ifndef SERVER_I_H
#define SERVER_I_H

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "tao/Utils/ORB_Manager.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/Time/TAO_Time_Service_Server.h"

class Server_i
{
  // = TITLE
  //   CORBA Server implementation.
  //
  // = DESCRIPTION
  //    A CORBA server that initializes the <TimeService Server>
  //    servant implementation and the ORB.
public:
  // = Initialization and termination methods.
  Server_i (void);
  // Constructor.

  ~Server_i (void);
  // Destructor.

  int init (int argc,
            ACE_TCHAR *argv[]);
  // Initialize the Server state.

  int run (void);
  // Run the orb.

  int init_naming_service ();
  // Initialises the name client.

  int create_server (void);
  // Create the time server object.

  int register_server (void);
  // Register the time server object with the Naming Service.

  int parse_args (int argc,
                  ACE_TCHAR* argv[]);
  // Parse the commandline arguments.

private:

  CORBA::ORB_var orb_;
  // My orb.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager.

  FILE *ior_output_file_;
  // File where the IOR of the Clerk object is stored.

  TAO_Naming_Client naming_client_;
  // An instance of the name server used for registering the
  // <TimeService Server> object.

  TAO_Time_Service_Server *time_service_server_impl_;
  // Implementation of the <TimeService> Server object.

  CosTime::TimeService_var time_service_server_;
  // Reference of the time server.

  CosNaming::NamingContext_var time_service_server_context_;
  // Naming context for the Naming Service.
};

#endif /* SERVER_I_H */
