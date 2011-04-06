// -*- C++ -*-

//=============================================================================
/**
 *  @file    Client_i.h
 *
 *  $Id$
 *
 *  This class implements a simple CORBA client that accesses a Time Service
 *  server.
 *
 *
 *  @author Vishal Kachroo <vishal@cs.wustl.edu>
 */
//=============================================================================


#include "orbsvcs/TimeServiceC.h"
#include "orbsvcs/Naming/Naming_Client.h"

/**
 * @class Client_i
 *
 * @brief Client implementation to access the Time Service server.
 *
 * Class wrapper for a client that gets the IOR of the clerk
 * running on its machine and uses it to get the globally
 * synchronized time
 */
class Client_i
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  Client_i (void);

  /// Destructor.
  ~Client_i (void);

  /// Execute client example code.
  int run (void);

  /// Initialize the client communication endpoint with server.
  int init (int argc, ACE_TCHAR *argv[]);

  /**
   * This test throws an exception that the operation
   * <secure_universal_time> has not been currently implemented in the
   * given server.
   */
  void test_for_secure_universal_time (void);

  /// Tests if the server returns a valid time.
  void test_for_universal_time (void);

  /// Tests if the server returns a new UTO filled with the given time.
  void test_for_new_universal_time (void);

  /// Tests if the returned UTO has the same value as the given UTC.
  void test_for_uto_from_utc (void);

  /// Tests if a new TIO is created according to the given bounds.
  void test_for_new_interval (void);

private:
  /// Function to read the server IOR from a file.
  int read_ior (ACE_TCHAR *filename);

  /// Parses the arguments passed on the command line.
  int parse_args (void);

  /// To initialize the naming service and get a clerk IOR.
   int obtain_initial_references (void);

  /// # of arguments on the command line.
  int argc_;

  /// arguments from command line.
  ACE_TCHAR **argv_;

  /// IOR of the obj ref of the server.
  char *ior_;

  /// Number of times to invoke the <time> operation.
  u_int loop_count_;

  /// Flag for server shutdown.
  int shutdown_;

  /// Time Service Server object ptr.
  CosTime::TimeService_var clerk_;

  /// An instance of the name client used for resolving the Time
  /// Service object.
  TAO_Naming_Client my_name_client_;

  /// Remember our orb.
  CORBA::ORB_var orb_;

};
