// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/Callback_Quoter
//
// = FILENAME
//    MarketFeed.h
//
// = DESCRIPTION
//    This class implements a simple CORBA client that keeps
//     on sending stock values to the server.
//
// = AUTHORS
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/CosNamingC.h"

#include <string.h>
#include "SupplierC.h"

class MarketFeed_i
{
  // = TITLE
  //     Market feed  daemon implementation.
  //
  // = DESCRIPTION
  //     Class wrapper for a daemon which keeps sending
  //     current stock values to the Callback Quoter server.

public:
  // = Initialization and termination methods.
  MarketFeed_i (void);
  // Constructor.

  ~MarketFeed_i (void);
  // Destructor.

  int run (void);
  // Execute  the daemon.

  int init (int argc, char *argv[]);
  // Initialize the client communication endpoint with server.

private:
  int read_ior (char *filename);
  // Function to read the server IOR from a file.

  int parse_args (void);
  // Parses the arguments passed on the command line.

  int via_naming_service(void);
  // This method initialises the naming service
  // and registers the object with the POA.

  int send_market_status (const char *stock_name,
			   long value);
  // sends the stock name and its value.

  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  char *ior_;
  // IOR of the obj ref of the server.

  u_int feed_time;
  //  time period between two succesive market feeds to
  // the server.

  CORBA::Environment env_;
  // Environment variable.

  TAO_Naming_Client naming_services_client_;
  // An instance of the name client used for resolving the factory
  // objects.

  int use_naming_service_;
  // This variable denotes whether the naming service
  // is used or not.

  Supplier_var server_;
  // Server object reference.

  CORBA::ORB_var orb_;
  // Remember our orb.

  int loop_count_;
  // Iteration count
};
