// -*- C++ -*-

//=============================================================================
/**
 *  @file    airplane_client_i.h
 *
 *  $Id$
 *
 *  This class implements a simple CORBA client which returns a random
 *  paper airplane from the paper airplane server.
 *
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================


#if !defined (AIRPLANE_CLIENT_I_H)
#define AIRPLANE_CLIENT_I_H

#include "AirplaneC.h"
#include "tao/corba.h"

/**
 * @class Airplane_Client_i
 *
 * @brief Paper Airplane Client Implementation
 *
 * Class wrapper for a client which gets the server IOR and then makes
 * a couple of calls to the server.
 */
class Airplane_Client_i
{
public:
  // = Constructor and destructor.
  Airplane_Client_i (void);
  ~Airplane_Client_i (void);

  /// Execute client example code.
  int run ();

  /// Initialize the client communication endpoint with server.
  int init (int argc, char **argv);

private:
  /// Parses the arguments passed on the command line.
  int parse_args (void);

  /// Ask the Paper Airplane Server for <count> planes.
  void get_planes (size_t count);

  /// # of arguments on the command line.
  int argc_;

  /// arguments from command line.
  char **argv_;

  /// Key of the obj ref of the server.
  char *server_key_;

  /// Number of airplanes to query for.
  size_t loop_count_;

  /// Server object ptr.
  Paper_Airplane_Server_ptr server_;

  /// Remember our orb.
  CORBA::ORB_var orb_;
};

#endif /* AIRPLANE_CLIENT_I_H */
