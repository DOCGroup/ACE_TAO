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
//    This class implements a simple CORBA server that keeps
//     on sending stock values to the Notifier.
//
// = AUTHORS
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/CosNamingC.h"
#include "NotifierC.h"

class Supplier
{
  // = TITLE
  //     Market feed  daemon implementation.
  //
  // = DESCRIPTION
  //     Class wrapper for a daemon which keeps sending
  //     current stock values to the Callback Quoter server.
public:
  // = Initialization and termination methods.
  Supplier (void);
  // Constructor.

  ~Supplier (void);
  // Destructor.

  int run (void);
  // Execute  the daemon.

  int init (int argc, char *argv[]);
  // Initialize the client communication endpoint with Notifier.

private:
  int read_ior (char *filename);
  // Function to read the Notifier IOR from a file.

  int parse_args (void);
  // Parses the arguments passed on the command line.

  int via_naming_service(void);
  // This method initialises the naming service and registers the
  // object with the POA.

  int send_market_status (const char *stock_name,
                          long value);
  // Sends the stock name and its value.

  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  char *ior_;
  // IOR of the obj ref of the Notifier.

  u_int feed_time;
  // Time period between two succesive market feeds to the Notifier.

  CORBA::Environment env_;
  // Environment variable.

  TAO_Naming_Client naming_services_client_;
  // An instance of the name client used for resolving the factory
  // objects.

  int use_naming_service_;
  // This variable denotes whether the naming service
  // is used or not.

  Notifier_var notifier_;
  // Notifier object reference.

  CORBA::ORB_var orb_;
  // Remember our orb.

  int loop_count_;
  // Iteration count.
};
