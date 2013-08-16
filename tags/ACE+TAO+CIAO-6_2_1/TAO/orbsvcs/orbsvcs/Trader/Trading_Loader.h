// -*- C++ -*-

// $Id$

// ===========================================================================
// FILENAME
//   Trading_Loader.h
//
// DESCRIPTION
//   This class loads the Trading Service dynamically
//   either from svc.conf file or <string_to_object> call.
//
// AUTHORS
//   Priyanka Gontla <pgontla@ece.uci.edu>
//
// ==========================================================================

#ifndef TAO_TRADING_LOADER_H
#define TAO_TRADING_LOADER_H

#include "orbsvcs/Trader/Trader.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/IOR_Multicast.h"
#include "orbsvcs/Trader/Service_Type_Repository.h"

#include "tao/Object_Loader.h"
#include "tao/Utils/ORB_Manager.h"
#include "ace/Auto_Ptr.h"

class TAO_Trading_Serv_Export TAO_Trading_Loader : public TAO_Object_Loader
{
public:
  /// Constructor
  TAO_Trading_Loader (void);

  /// Destructor
  ~TAO_Trading_Loader (void);

  /// Called by the Service Configurator framework to initialize the
  /// Event Service. Defined in <ace/Service_Config.h>
  virtual int init (int argc, ACE_TCHAR *argv[]);

  /// Called by the Service Configurator framework to remove the
  /// Event Service. Defined in <ace/Service_Config.h>
  virtual int fini (void);

  /// Run the Trading Service
  int run (void);

  /// This function call initializes the Trading Service given a reference to the
  /// ORB and the command line parameters.
  CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                   int argc,
                                   ACE_TCHAR *argv[]);

protected:

  /// Enable the Trading Service to answer multicast requests for its
  /// IOR.
  int init_multicast_server (void);

  /// Bootstrap to another trader, and attach to its trader network.
  int bootstrap_to_federation (void);

  /// Parses the command line arguments
  int parse_args (int &argc, ACE_TCHAR *argv []);

  /// The ORB manager.
  TAO_ORB_Manager orb_manager_;

  /// Pointer to the linked trader.
  auto_ptr<TAO_Trader_Factory::TAO_TRADER> trader_;

  /// Service Type Repository used by the trading service.
  TAO_Service_Type_Repository type_repos_;

  /// IOR of the trader kept around for handiness purposes.
  CORBA::String_var ior_;

  /// Flag indicating whether this trader should join the federation.
  CORBA::Boolean federate_;

  /// File to output the Naming Service IOR.
  FILE *ior_output_file_;

  /// Name of this trading service: "hostname:pid".
  CORBA::String_var name_;

  /// Event handler that responds to resolve_initial_references
  /// requests.
  TAO_IOR_Multicast ior_multicast_;

  /// Flag inidicating whether we're the trader others are bootstrapping to.
  CORBA::Boolean bootstrapper_;

private:
  // Disallow copying and assignment.
  TAO_Trading_Loader (const TAO_Trading_Loader &);
  TAO_Trading_Loader &operator= (const TAO_Trading_Loader &);
};

ACE_FACTORY_DECLARE (TAO_Trading_Serv, TAO_Trading_Loader)

#endif /* TAO_TRADING_LOADER_H */
