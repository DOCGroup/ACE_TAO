// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/Time_Service
//
// = FILENAME
//    Clerk_i.h
//
// = AUTHOR
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#ifndef CLERK_I_H
#define CLERK_I_H

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "tao/TAO.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/Time/TAO_Time_Service_Clerk.h"

class Clerk_i
{
  // = TITLE
  //   CORBA Time Clerk implementation.
  //
  // = DESCRIPTION
  //   A CORBA server that initializes the TimeService clerk
  //   implementation and the ORB.
public:

  enum {
    // Pre-allocate slots for this many servers. The capacity grows on
    // demand.
    DEFAULT_SERVER_COUNT = 8
  };

  // = Initialization and termination methods.
  Clerk_i (void);
  // Constructor.

  ~Clerk_i (void);
  // Destructor.

  typedef ACE_Array_Base<CosTime::TimeService_var> IORS;
  // Set of available Time servers.

  int init (int argc,
	    char *argv[],
	    CORBA::Environment &env);
  // Initialize the Clerk state - parsing arguments and waiting.

  int run (CORBA::Environment &env);
  // Run the orb.

private:
  void insert_server (CosTime::TimeService_ptr server);
  // Insert a server in the internal data structure (servers_).

private:

  int read_ior (const char *filename);
  // Read the server IORs from a file instead of using a naming
  // service.

  int parse_args (void);
  // Parses the commandline arguments.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager.

  FILE *ior_output_file_;
  // File where the IOR of the Clerk object is stored.

  int init_naming_service (CORBA::Environment &env);
  // Initialises the name server and registers the <TimeService> Clerk
  // object with it.

  int create_clerk (void);
  // Create an instance of the clerk to use.

  int register_clerk (void);
  // Bind the clerk in the appropriate context in the Naming Service.

  int init_IR (void);
  // Initialise the Interface Repository and register the clerk
  // implementation with it.

  int get_first_IOR (void);
  // Get the reference to the server naming context and the first
  // server IOR.

  int next_n_IORs (CosNaming::BindingIterator_var iter,
		   CosNaming::NamingContext_var server_context);
  // Iterate over the given server context to get the rest of the
  // server IORs.

  TAO_Naming_Server my_name_server_;
  // An instance of the name server used for registering the
  // <TimeService Clerk> object. The same instance is used by the
  // Clerk to periodically iterate through the Server Naming Context
  // and get IORs of the available servers.

  TAO_Time_Service_Clerk *time_service_clerk_impl_;
  // Implementation of the <TimeService> Clerk object.

  CosTime::TimeService_var time_service_clerk_;
  // Reference of the time service clerk.

  CosNaming::NamingContext_var time_service_clerk_context_;
  // Naming context for the Naming Service.

  int timer_value_ ;
  // Clerk seeks time periodically after this interval from the
  // servers in the network.

  //  IR_Helper *ir_helper_;
  // Helper class for using the IR.

  IORS server_;
  // The unbounded set of server IORS.

  FILE *ior_fp_;
  // Handle to the file of server IORs.

  CORBA::ORB_var orb_;
  // My orb.

  int argc_;
  // Number of command line arguments.

  char **argv_;
  // The command line arguments.
};

#endif /* CLERK_I_H */
