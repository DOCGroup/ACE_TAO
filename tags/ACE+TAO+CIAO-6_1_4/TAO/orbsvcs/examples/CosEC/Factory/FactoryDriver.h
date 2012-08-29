
// -*- C++ -*-

//=============================================================================
/**
 *  @file    FactoryDriver.h
 *
 *  $Id$
 *
 *  This class implements the Factory driver.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_FACTORYDRIVER_H
#define TAO_FACTORYDRIVER_H

#include "CosEventChannelFactory_i.h"
#include "orbsvcs/Naming/Naming_Client.h"

// @@ Pradeep: I know this is just an example, but could you talk to
//    Vishal about making this a service that could be bootstraped
//    using the -ORBInitRef mechanisms?  That way we can run this
//    stuff without the naming service running.

 /**
  * @class FactoryDriver
  *
  * @brief Driver class for the CosEventChannel Factory.
  *
  * creates a CosEventChannel Factory and registers it with the
  * naming service.
  */
class FactoryDriver
{
 public:
  // = Initialization and termination code.
  /// Constructor.
  FactoryDriver (const ACE_TCHAR* factory = ACE_TEXT("CosEC_Factory"));

  /// Destructor.
  ~FactoryDriver (void);

  /// Start the driver.
  int start (int argc, ACE_TCHAR *argv []);

  ///Stop the driver.
  int stop (void);

 protected:
  /// Parse the command-line arguments and set options.
  int parse_args (int argc, ACE_TCHAR *argv []);

  /// The name of the factory registered with the naming service.
  const ACE_TCHAR *factoryName_;

  /// The name of the Child POA.
  const char* child_poa_name_;

  /// The ORB that we use.
  CORBA::ORB_var orb_;

  /// The factory servant.
  TAO_CosEventChannelFactory_i *factory_servant_;

  /// Reference to the root poa.
  PortableServer::POA_var root_poa_;

  /// The corba object after activation.
  CosEventChannelFactory::ChannelFactory_var factory_;

  /// Use a naming client.
  TAO_Naming_Client naming_client_;

};
#endif /*  TAO_FACTORYDRIVER_H */
