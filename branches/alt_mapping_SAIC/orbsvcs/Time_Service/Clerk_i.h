// -*- C++ -*-

//=============================================================================
/**
 *  @file    Clerk_i.h
 *
 *  $Id$
 *
 *  @author Vishal Kachroo <vishal@cs.wustl.edu>
 */
//=============================================================================


#ifndef CLERK_I_H
#define CLERK_I_H

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "tao/Utils/ORB_Manager.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/Time/TAO_Time_Service_Clerk.h"

/**
 * @class Clerk_i
 *
 * @brief CORBA Time Clerk implementation.
 *
 * A CORBA server that initializes the TimeService clerk
 * implementation and the ORB.
 */
class Clerk_i
{
public:

  enum {
    // Pre-allocate slots for this many servers. The capacity grows on
    // demand.
    DEFAULT_SERVER_COUNT = 8
  };

  // = Initialization and termination methods.
  /// Constructor.
  Clerk_i (void);

  /// Destructor.
  ~Clerk_i (void);

  /// Set of available Time servers.
  typedef ACE_Array_Base<CosTime::TimeService_var> IORS;

  /// Initialize the Clerk state - parsing arguments and waiting.
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Run the orb.
  int run (void);

private:
  /// Insert a server in the internal data structure (servers_).
  void insert_server (CosTime::TimeService_ptr server);

private:

  /// Read the server IORs from a file instead of using a naming
  /// service.
  int read_ior (const ACE_TCHAR* filename);

  /// Parses the commandline arguments.
  int parse_args (int argc,
                  ACE_TCHAR* argv[]);

  /// The ORB manager.
  TAO_ORB_Manager orb_manager_;

  /// File where the IOR of the Clerk object is stored.
  FILE *ior_output_file_;

  /// Initialises the name client and registers the <TimeService> Clerk
  /// object with it.
  int init_naming_service ();

  /// Create an instance of the clerk to use.
  int create_clerk (void);

  /// Bind the clerk in the appropriate context in the Naming Service.
  int register_clerk (void);

  /// Initialise the Interface Repository and register the clerk
  /// implementation with it.
  int init_IR (void);

  /// Get the reference to the server naming context and the first
  /// server IOR.
  int get_first_IOR (void);

  /// Iterate over the given server context to get the rest of the
  /// server IORs.
  int next_n_IORs (CosNaming::BindingIterator_var iter,
                   CosNaming::NamingContext_var server_context);

  /**
   * An instance of the name server used for registering the
   * <TimeService Clerk> object. The same instance is used by the
   * Clerk to periodically iterate through the Server Naming Context
   * and get IORs of the available servers.
   */
  TAO_Naming_Client naming_client_;

  /// Implementation of the <TimeService> Clerk object.
  TAO_Time_Service_Clerk *time_service_clerk_impl_;

  /// Reference of the time service clerk.
  CosTime::TimeService_var time_service_clerk_;

  /// Naming context for the Naming Service.
  CosNaming::NamingContext_var time_service_clerk_context_;

  /// My orb.
  CORBA::ORB_var orb_;

  /// Clerk seeks time periodically after this interval in secs. from the
  /// servers in the network.
  int timer_value_ ;

  /// Continues the precision of timer_value_ in microseconds.
  int timer_value_usecs_;

  //  IR_Helper *ir_helper_;
  // Helper class for using the IR.

  /// The unbounded set of server IORS.
  IORS server_;

  /// Read IORs from a file.
  int ior_fp_;
};

#endif /* CLERK_I_H */
