/* -*- C++ -*- */
// $Id$

// ===========================================================
//
//
// = LIBRARY
//    TAO/examples/Callback_Quoter
//
// = FILENAME
//    Consumer_Handler.h
//
// = DESCRIPTION
//    Definition of the Callback_Qouter Consumer Client class, Consumer_Handler.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ===========================================================

#ifndef CONSUMER_HANDLER_H
#define CONSUMER_HANDLER_H

#include "ConsumerC.h"
#include "NotifierC.h"
#include "Consumer_i.h"
#include "ace/Read_Buffer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/ORB_Manager.h"
#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/CosNamingC.h"

#include "Consumer_Input_Handler.h"
#include "Consumer_Signal_Handler.h"

class Consumer_Input_Handler;
class Consumer_Signal_Handler;

class Consumer_Handler
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
  Consumer_Handler (void);
  // Constructor.

  ~Consumer_Handler (void);
  // Destructor.

  int init (int argc, char *argv[]);
  // Initialize the client communication with the server.

  int run (void);
  // Start the ORB object.

  const char *stock_name_;
  // the name of the stock the consumer is interested in.

  int threshold_value_;
  // the desired price of the stock.

  Notifier_var server_;
  // Server object ptr.

  Consumer_i *consumer_servant_;
  // The consumer object.

  Callback_Quoter::Consumer_var consumer_var_;
  // Pointer to the consumer object registered with the ORB.

  ACE_Reactor* reactor_used (void) const;
  // This method gives the reactor pointer.

  int registered_;
  // Flag which notes whether the consumer has got registered with the
  // Notifier-server.

  int unregistered_;
  // Flag which notes whether the consumer has got unregistered from
  // the Notifier-server.

private:

  CORBA::ORB_var orb_;
  // Our orb.

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

  TAO_Naming_Client naming_services_client_;
  // An instance of the name client used for resolving the factory
  // objects.

  int use_naming_service_;
  // This variable denotes whether the naming service
  // is used or not.

 Consumer_Input_Handler *consumer_input_handler_;
  // Reference to the input_event_handler.

 Consumer_Signal_Handler *consumer_signal_handler_;
  // Reference to the signal_event_handler.

  int interactive_;
  // Is the example interactive?
};

#endif /* CONSUMER_HANDLER_H */
