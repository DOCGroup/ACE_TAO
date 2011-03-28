// -*- C++ -*-
//=============================================================================
/**
 *  @file    MarketFeed.h
 *
 *  $Id$
 *
 *  This class implements a simple CORBA server that keeps
 *   on sending stock values to the Notifier.
 *
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================

#ifndef SUPPLIER_I_H
#define SUPPLIER_I_H

#include "orbsvcs/Naming/Naming_Client.h"
#include "orbsvcs/CosNamingC.h"
#include "ace/Reactor.h"
#include "ace/Read_Buffer.h"
#include "NotifierC.h"
#include "Supplier_Timer_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Supplier
 *
 * @brief Market feed  daemon implementation.
 *
 * This class feeds stock information to the Callback Quoter
 * notifier.
 */
class Supplier_Timer_Handler;
class Supplier
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  Supplier (void);

  /// Destructor.
  ~Supplier (void);

  /// Execute  the daemon.
  int run (void);

  /// Initialize the client communication endpoint with Notifier.
  int init (int argc, ACE_TCHAR *argv[]);

  /// Sends the stock name and its value.
  int send_market_status (const char *stock_name,
                          long value);

  /// The timer handler used to send the market status to the notifier
  /// periodically.
  Supplier_Timer_Handler *supplier_timer_handler_;

private:
  /// Remember our orb.
  CORBA::ORB_var orb_;

  /// Function to read the Notifier IOR from a file.
  int read_ior (ACE_TCHAR *filename);

  /// Parses the arguments passed on the command line.
  int parse_args (void);

  /// This method initialises the naming service and registers the
  /// object with the POA.
  int via_naming_service(void);

  /// returns the TAO instance of the singleton Reactor.
   ACE_Reactor *reactor_used (void) const;

  /// This method used for getting stock information from a file.
  int read_file (ACE_TCHAR *filename);

  /// # of arguments on the command line.
  int argc_;

  /// arguments from command line.
  ACE_TCHAR **argv_;

  /// IOR of the obj ref of the Notifier.
  ACE_TCHAR *ior_;

  /// An instance of the name client used for resolving the factory
  /// objects.
  TAO_Naming_Client naming_services_client_;

  /// This variable denotes whether the naming service
  /// is used or not.
  int use_naming_service_;

  /// Notifier object reference.
  Notifier_var notifier_;

  /// The pointer for accessing the input stream.
  FILE  *f_ptr_;

  /// Iteration count.
  int loop_count_;

  /// Time period between two succesive market feeds to the Notifier.
  long  period_value_;
};

#endif /*SUPPLIER_I_H */
