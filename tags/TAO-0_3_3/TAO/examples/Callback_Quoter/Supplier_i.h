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
#ifndef SUPPLIER_I_H
#define SUPPLIER_I_H

#include "orbsvcs/Naming/Naming_Utils.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Reactor.h"
#include "ace/Read_Buffer.h"
#include "ace/OS.h"
#include "NotifierC.h"
#include "Supplier_Timer_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Supplier_Timer_Handler;
class Supplier
{
  // = TITLE
  //     Market feed  daemon implementation.
  //
  // = DESCRIPTION
  //    This class feeds stock information to the Callback Quoter 
  //    notifier. 
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

  int send_market_status (const char *stock_name,
                          long value);
  // Sends the stock name and its value.

  Supplier_Timer_Handler *supplier_timer_handler_;
  // The timer handler used to send the market status to the notifier
  // periodically.

private:
  int read_ior (char *filename);
  // Function to read the Notifier IOR from a file.

  int parse_args (void);
  // Parses the arguments passed on the command line.

  int via_naming_service(void);
  // This method initialises the naming service and registers the
  // object with the POA.
  
   ACE_Reactor *reactor_used (void) const;
  // returns the TAO instance of the singleton Reactor.

  int read_file (char *filename);
  // This method used for getting stock information from a file.

  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  char *ior_;
  // IOR of the obj ref of the Notifier.
  
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

  FILE  *f_ptr_;
  // The pointer for accessing the input stream.

  int loop_count_;
  // Iteration count.
  
  long  period_value_;
  // Time period between two succesive market feeds to the Notifier.
};

#endif /*SUPPLIER_I_H */
