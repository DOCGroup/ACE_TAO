//$Id$
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "Servant_Locator.h"

class Manager
{
  public:

  Manager (void);

  // Ctor

  int init (int argc,
            char *argv[]
            TAO_ENV_ARG_DECL);

  // Initialize the ORB, POA etc.

  int init_register_name_service (TAO_ENV_SINGLE_ARG_DECL);
  // Initialize the Name service, tegister the combined IOR with it

  int activate_servant (TAO_ENV_SINGLE_ARG_DECL);
  // Activate the servant etc.

  int make_iors_register (TAO_ENV_SINGLE_ARG_DECL_NOT_USED);

  int run (TAO_ENV_SINGLE_ARG_DECL);
  // Run the  ORB event loop..
 private:
  CORBA::ORB_var orb_;
  // Our ORB

  PortableServer::POA_var new_poa_var_;
  // The new poa that is created..

  Servant_Locator *servant_locator_;
  // Our servant locator

  PortableServer::ServantLocator_var servant_locator_var_;
  // Our servant locator var

  CORBA::Object_var new_manager_ior_;
};
#endif /*_MANAGER_H_ */
