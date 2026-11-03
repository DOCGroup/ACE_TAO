// -*- C++ -*-
//=============================================================================
/**
 *  @file    Supplier_i.h
 *
 *  This class implements a simple CORBA server that keeps
 *   on sending stock values to the Notifier.
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
#include "ace/OS_NS_stdio.h"
#include "NotifierC.h"
#include "Supplier_Timer_Handler.h"

#include <cstdio>
#include <memory>
#include <string>

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
  /// Constructor.
  Supplier () = default;

  Supplier (Supplier const &) = delete;
  Supplier &operator= (Supplier const &) = delete;
  Supplier (Supplier &&) = delete;
  Supplier &operator= (Supplier &&) = delete;

  /// Destructor.
  ~Supplier ();

  /// Execute  the daemon.
  int run ();

  /// Initialize the client communication endpoint with Notifier.
  int init (int argc, ACE_TCHAR *argv[]);

  /// Sends the stock name and its value.
  int send_market_status (char const *stock_name,
                          long value);

private:
  /// The timer handler used to send the market status to the notifier
  /// periodically.
  /// Timer handler is owned here and passed as a borrowed reactor callback.
  std::unique_ptr<Supplier_Timer_Handler> supplier_timer_handler_;

  /// Remember our orb.
  CORBA::ORB_var orb_ {};

  /// Function to read the Notifier IOR from a file.
  int read_ior (ACE_TCHAR const *filename);

  /// Parses the arguments passed on the command line.
  int parse_args ();

  /// This method initialises the naming service and registers the
  /// object with the POA.
  int via_naming_service();

  /// Returns the TAO instance of the singleton Reactor.
  ACE_Reactor *reactor_used () const;

  /// This method used for getting stock information from a file.
  int read_file (ACE_TCHAR const *filename);

  /// # of arguments on the command line.
  int argc_ {};

  /// Pointer-array copy made before ORB_init modifies argv.
  std::unique_ptr<ACE_TCHAR *[]> argv_;

  /// IOR of the obj ref of the Notifier.
  std::basic_string<ACE_TCHAR> ior_;

  /// An instance of the name client used for resolving the factory
  /// objects.
  TAO_Naming_Client naming_services_client_;

  /// This variable denotes whether the naming service
  /// is used or not.
  bool use_naming_service_ {true};

  /// Notifier object reference.
  Notifier_var notifier_ {};

  /// The pointer for accessing the input stream.
  using File_Handle = std::unique_ptr<FILE, int (*) (FILE *)>;
  File_Handle f_ptr_ {nullptr, &ACE_OS::fclose};

  /// Identifier used to cancel the reactor timer before destroying its handler.
  long timer_id_ {-1};

  /// Iteration count.
  int loop_count_ {10};

  /// Time period between two succesive market feeds to the Notifier.
  long period_value_ {1};
};

#endif /*SUPPLIER_I_H */
