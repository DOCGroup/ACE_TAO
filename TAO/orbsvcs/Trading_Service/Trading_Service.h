// $Id$

// ========================================================================
//
// = BINARY
//    trader
//
// = FILENAME
//    Trading_Service.h
//
// = AUTHOR
//    Seth Widoff <sbw1@cs.wustl.edu>
//
// =======================================================================

#ifndef _TRADING_SERVICE_H
#define _TRADING_SERVICE_H

#include "tao/TAO.h"
#include "ace/Auto_Ptr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/IOR_Multicast.h"
#include "orbsvcs/Trader/Trader.h"
#include "orbsvcs/Trader/Service_Type_Repository.h"

class Trading_Service;

class Trading_Shutdown : public ACE_Event_Handler
{
public:

  Trading_Shutdown (Trading_Service& trader);

  virtual int handle_signal (int, siginfo_t*, ucontext_t*);

private:

  Trading_Service& trader_;
  ACE_Sig_Handler shutdown_;
};


class Trading_Service
// = TITLE
//   A class that initializes a Trading Service instance.
{
public:

  Trading_Service (void);
  // Default constructor.

  ~Trading_Service (void);
  // Destructor

  int init (int argc, char* argv[]);
  // Initialize the Trading Service with arguments.

  int run (void);
  // Run the Trading Service.

  int shutdown (void);

private:

  int init_multicast_server (void);
  // Enable the Trading Service to answer multicast requests for its IOR.

  int bootstrap_to_federation (void);
  // Bootstrap to another trader, and attach to its trader network.

  int parse_args (int& argc, char *argv[]);
  // parses the arguments.

  TAO_ORB_Manager orb_manager_;
  // The ORB manager.

  TAO_Service_Type_Repository type_repos_;
  // Service Type Repository used by the trading service.

  auto_ptr<TAO_Trader_Factory::TAO_TRADER> trader_;
  // Pointer to the linked trader.

  CORBA::String_var name_;
  // Name of this trading service: "hostname:pid".

  CORBA::String_var ior_;
  // IOR of the trader kept around for handiness purposes.

  CORBA::Boolean federate_;
  // Flag indicating whether this trader should join the federation.

  FILE *ior_output_file_;
  // File to output the Naming Service IOR.
  
  CORBA::Boolean dumpior_;
  // Flag indicating whether to dump the ior to standard output.
  
  CORBA::Boolean bootstrapper_;
  // Flag inidicating whether we're the trader others are bootstrapping to.
  
  TAO_IOR_Multicast ior_multicast_;
  // Event handler that responds to resolve_initial_references requests.
};


#endif /* _TRADING_SERVICE_H */
