// -*- C++ -*-

//=============================================================================
/**
 *  @file    Server_i.h
 *
 *  $Id$
 *
 *  @author Vishal Kachroo <vishal@cs.wustl.edu>
 */
//=============================================================================


#ifndef SERVER_I_H
#define SERVER_I_H

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "tao/Utils/ORB_Manager.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/Time/TAO_Time_Service_Server.h"

/**
 * @class Server_i
 *
 * @brief CORBA Server implementation.
 *
 * A CORBA server that initializes the <TimeService Server>
 * servant implementation and the ORB.
 */
class Server_i
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  Server_i (void);

  /// Destructor.
  ~Server_i (void);

  /// Initialize the Server state.
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Run the orb.
  int run (void);

  /// Initialises the name client.
  int init_naming_service ();

  /// Create the time server object.
  int create_server (void);

  /// Register the time server object with the Naming Service.
  int register_server (void);

  /// Parse the commandline arguments.
  int parse_args (int argc,
                  ACE_TCHAR* argv[]);

private:

  /// My orb.
  CORBA::ORB_var orb_;

  /// The ORB manager.
  TAO_ORB_Manager orb_manager_;

  /// File where the IOR of the Clerk object is stored.
  FILE *ior_output_file_;

  /// An instance of the name server used for registering the
  /// <TimeService Server> object.
  TAO_Naming_Client naming_client_;

  /// Implementation of the <TimeService> Server object.
  TAO_Time_Service_Server *time_service_server_impl_;

  /// Reference of the time server.
  CosTime::TimeService_var time_service_server_;

  /// Naming context for the Naming Service.
  CosNaming::NamingContext_var time_service_server_context_;
};

#endif /* SERVER_I_H */
