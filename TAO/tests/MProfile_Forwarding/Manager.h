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
            char *argv[], 
            CORBA::Environment & ACE_TRY_ENV);

  // Initialize the ORB, POA etc.

  int init_register_name_service (CORBA::Environment &ACE_TRY_ENV);
  // Initialize the Name service, tegister the combined IOR with it

  int activate_servant (CORBA::Environment &ACE_TRY_ENV);
  // Activate the servant etc.

  int make_iors_register (CORBA::Environment &);

  int run (CORBA::Environment &ACE_TRY_ENV);
  // Run the  ORB event loop..
 private:
  CORBA::ORB_var orb_;
  // Our ORB 

  PortableServer::POA_var new_poa_var_;
  // The new poa that is created..
  
  Servant_Locator *servant_locator_;
  // Our servant locator

  CORBA::Object_var new_manager_ior_;
};
#endif /*_MANAGER_H_ */
