
// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/examples/CosEC/Factory
//
// = FILENAME
//    FactoryDriver.h
//
// = DESCRIPTION
//    This class implements the Factory driver.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_FACTORYDRIVER_H
#define TAO_FACTORYDRIVER_H

#include "CosEventChannelFactory_i.h"
#include "orbsvcs/Naming/Naming_Utils.h"

// @@ Pradeep: I know this is just an example, but could you talk to
//    Vishal about making this a service that could be bootstraped
//    using the -ORBInitRef mechanisms?  That way we can run this
//    stuff without the naming service running.

class FactoryDriver
{
  // = TITLE
  //   Driver class for the CosEventChannel Factory.
  // = DESCRIPTION
  //   creates a CosEventChannel Factory and registers it with the
  //   naming service.
  //
 public:
  // = Initialization and termination code.
  FactoryDriver (const char* factory = "CosEC_Factory");
  // Constructor.

  ~FactoryDriver (void);
  // Destructor.

  int start (int argc, char *argv []);
  // Start the driver.

  int stop (void);
  //Stop the driver.

 protected:
  int parse_args (int argc, char *argv []);
  // Parse the command-line arguments and set options.

  const char* factoryName_;
  // The name of the factory registered with the naming service.

  const char* child_poa_name_;
  // The name of the Child POA.

  CORBA::ORB_var orb_;
  // The ORB that we use.

  TAO_CosEventChannelFactory_i *factory_servant_;
  // The factory servant.

  PortableServer::POA_var root_poa_;
  // Reference to the root poa.

  CosEventChannelFactory::ChannelFactory_var factory_;
  // The corba object after activation.

  TAO_Naming_Client naming_client_;
  // Use a naming client.

};
#endif /*  TAO_FACTORYDRIVER_H */
