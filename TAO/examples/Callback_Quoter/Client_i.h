/* -*- C++ -*- */
// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/examples/Callback_Quoter
//
// = FILENAME
//    Client_i.h
//
// = DESCRIPTION
//    Definition of the Callback_Qouter Consumer Client class, Client_i.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#if !defined (CLIENT_I_H)
#define CLIENT_I_H

#include "Consumer_i.h"
#include "SupplierC.h"
#include "ace/Read_Buffer.h"
#include "tao/TAO.h"
#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/CosNamingC.h"

class Client_i 
{
  // = TITLE
  //    Callback Quoter Consumer Client class.
  //
  // = DESCRIPTION
  //    Connects to the Callback Quoter server and registers the Consumer_i
  //    object with the it and receives the stock status
  //   from the Supplier.

public:
  // = Initialization and termination methods.
  Client_i (void);
  // Constructor.

  ~Client_i (void);
  // Destructor.

  int init (int argc, char *argv[]);
  // Initialize the client communication with the server.

  int run (void);
  // Start the ORB object.

 private:

  int read_ior (char *filename);
  // Function to read the server IOR from a file.

  int parse_args (void);
  // Parse the command line arguments.
  // Returns 0 on success, -1 on error.

  int via_naming_service (void);
  // This method initialises the naming service
  // and registers the object with the POA.

  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  char *ior_;
  // IOR of the obj ref of the server.

  int shutdown_;
  // Flag for server shutdown.

  CORBA::Environment env_;
  // Environment variable.

  TAO_Naming_Client naming_services_client_;
  // An instance of the name client used for resolving the factory
  // objects.

  int use_naming_service_;
  // This variable denotes whether the naming service
  // is used or not.

  char *stock_name_;

  int threshold_value_;

  Supplier_var server_;
  // Server object ptr.

  CORBA::ORB_var orb_;
  // Our orb.

  Consumer_i consumer_i_;
  // The consumer object.

  Callback_Quoter::Consumer_var consumer_var_;
  // Pointer to the consumerobject registered with the ORB.
};


#endif /* CLIENT_I_H */
