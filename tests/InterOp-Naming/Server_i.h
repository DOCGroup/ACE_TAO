
//=============================================================================
/**
 *  @file   Server_i.h
 *
 *  $Id$
 *
 *  The class is the driver for the INS test server.
 *
 *
 *  @author Vishal Kachroo <vishal@cs.wustl.edu>
 */
//=============================================================================


#ifndef SERVER_I_H
#define SERVER_I_H

#include "tao/Utils/ORB_Manager.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "INS_i.h"

class Server_i
{

public:
  // = Initialization and termination methods.

  /// Constructor.
  Server_i (void);

  /// Destructor.
  ~Server_i (void);

  /// Initialize the Server state - parsing arguments and waiting.
  int init (int argc,
            ACE_TCHAR *argv[]);

  /// Add an IOR to the ORB-IOR table.
  int add_IOR_to_table (CORBA::String_var ior);

  /// Run the orb.
  int run (void);

protected:

  /// Servant class
  INS_i servant_;

  /// Parses the commandline arguments.
  int parse_args (void);

  /// The ORB manager - a helper class for accessing the POA and
  /// registering objects.
  TAO_ORB_Manager orb_manager_;

  /// File where the IOR of the server object is stored.
  FILE *ior_output_file_;

  /// Used test the INS.
  char * ins_;

  /// Number of command line arguments.
  int argc_;

  /// The command line arguments.
  ACE_TCHAR **argv_;
};

#endif /* SERVER_I_H */
