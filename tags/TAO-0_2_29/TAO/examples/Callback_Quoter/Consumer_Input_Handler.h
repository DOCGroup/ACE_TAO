/* -*- C++ -*- */
// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/examples/Callback_Quoter
//
// = FILENAME
//    Consumer_Input_Handler.h
//
// = DESCRIPTION
//    Definition of the Callback_Qouter Consumer Client class, Consumer_Input_Handler.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#if !defined (CONSUMER_INPUT_HANDLER_H)
#define CONSUMER_INPUT_HANDLER_H

#include "ConsumerC.h"
#include "NotifierC.h"
#include "Consumer_i.h"
#include "ace/Read_Buffer.h"
#include "tao/TAO.h"
#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/CosNamingC.h"

class Consumer_Input_Handler : public ACE_Event_Handler
{
  // = TITLE
  //    Callback Quoter Consumer Client class.
  //
  // = DESCRIPTION
  //    Connects to the Callback Quoter server and
  //    registers the Consumer object with the it
  //    and receives the stock status from the Notifier.

public:
  // = Initialization and termination methods.
  Consumer_Input_Handler (void);
  // Constructor.

  ~Consumer_Input_Handler (void);
  // Destructor.

  int init (int argc, char *argv[]);
  // Initialize the client communication with the server.

  int run (void);
  // Start the ORB object.

  virtual int handle_input (ACE_HANDLE);
  // Handle the user input.

private:
  int read_ior (char *filename);
  // Function to read the server IOR from a file.

  int parse_args (void);
  // Parse the command line arguments.  Returns 0 on success, -1 on
  // error.

  int via_naming_service (void);
  // This method initialises the naming service and registers the
  // object with the POA.

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
  // the name of the stock the consumer is interested in.

  int threshold_value_;
  // the desired price of the stock.

  Notifier_var server_;
  // Server object ptr.

  CORBA::ORB_var orb_;
  // Our orb.

  Consumer_i *consumer_servant_;
  // The consumer object.

  Callback_Quoter::Consumer_var consumer_var_;
  // Pointer to the consumerobject registered with the ORB.

  int registered_;
  // Flag which notes whether the consumer has got registered with the
  // Notifier-server.

  int unregistered_;
  // Flag which notes whether the consumer has got unregistered from
  // the Notifier-server.
};

#define REGISTER_STRING "register"
// The string that the user must type to register the consumer with
// the Notifier_server.

#define UNREGISTER_STRING "unregister"
// The string that the user must type to unregister the consumer with
// the Notifier_server.

#define EXIT_STRING "quit"
// The string the user must type to quit the consumer client
// application.

#endif /* CONSUMER_INPUT_HANDLER_H */
