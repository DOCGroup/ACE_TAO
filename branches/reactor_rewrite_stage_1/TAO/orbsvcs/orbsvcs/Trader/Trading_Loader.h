// $Id$

// ===========================================================================================
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
// ==========================================================================================

#ifndef TAO_TRADING_LOADER_H
#define TAO_TRADING_LOADER_H

#include "tao/Object_Loader.h"
#include "tao/PortableServer/ORB_Manager.h"
#include "ace/Auto_Ptr.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/IOR_Multicast.h"
#include "orbsvcs/Trader/Trader.h"
#include "orbsvcs/Trader/Service_Type_Repository.h"

class TAO_Trading_Export TAO_Trading_Loader : public TAO_Object_Loader
{
public:

  TAO_Trading_Loader (void);
  // Constructor

  ~TAO_Trading_Loader (void);
  // Destructor

  virtual int init (int argc, char *argv[]);
  // Called by the Service Configurator framework to initialize the
  // Event Service. Defined in <ace/Service_Config.h>

  virtual int fini (void);
  // Called by the Service Configurator framework to remove the
  // Event Service. Defined in <ace/Service_Config.h>

  int run (TAO_ENV_SINGLE_ARG_DECL);
  // Run the Trading Service

  CORBA::Object_ptr create_object (CORBA::ORB_ptr orb,
                                   int argc, char *argv[]
                                   TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This function call initializes the Trading Service given a reference to the
  // ORB and the command line parameters.

 protected:

  int init_multicast_server (void);
  // Enable the Trading Service to answer multicast requests for its
  // IOR.

  int bootstrap_to_federation (TAO_ENV_SINGLE_ARG_DECL);
  // Bootstrap to another trader, and attach to its trader network.

  int parse_args (int &argc, char *argv []);
  // Parses the command line arguments

  TAO_ORB_Manager orb_manager_;
  // The ORB manager.

  auto_ptr<TAO_Trader_Factory::TAO_TRADER> trader_;
  // Pointer to the linked trader.

  TAO_Service_Type_Repository type_repos_;
  // Service Type Repository used by the trading service.

  CORBA::String_var ior_;
  // IOR of the trader kept around for handiness purposes.

  CORBA::Boolean federate_;
  // Flag indicating whether this trader should join the federation.

  FILE *ior_output_file_;
  // File to output the Naming Service IOR.

  CORBA::String_var name_;
  // Name of this trading service: "hostname:pid".

  TAO_IOR_Multicast ior_multicast_;
  // Event handler that responds to resolve_initial_references
  // requests.

  CORBA::Boolean bootstrapper_;
  // Flag inidicating whether we're the trader others are bootstrapping to.

  CORBA::Boolean dumpior_;
  // Flag indication whether to dump the ior to standard output

 private:
   ACE_UNIMPLEMENTED_FUNC (TAO_Trading_Loader (const TAO_Trading_Loader &))
   ACE_UNIMPLEMENTED_FUNC (TAO_Trading_Loader &operator= (const TAO_Trading_Loader &))

};

ACE_FACTORY_DECLARE (TAO_Trading, TAO_Trading_Loader)

#endif /* TAO_TRADING_LOADER_H */
