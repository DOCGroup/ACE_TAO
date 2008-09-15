//$Id$
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "Servant_Locator.h"

class Manager
{
  public:

  Manager (void);

  // Ctor

  int init (int argc, ACE_TCHAR *argv[]);

  // Initialize the ORB, POA etc.

  int init_register_name_service (void);
  // Initialize the Name service, tegister the combined IOR with it

  int activate_servant (void);
  // Activate the servant etc.

  int make_iors_register (void);

  int run (void);
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
