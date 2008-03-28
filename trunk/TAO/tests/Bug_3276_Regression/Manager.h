//$Id$

#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "ace/Auto_Ptr.h"
#include "Servant_Locator.h"

class Manager
{
public:
  Manager (void);
  // Ctor

  ~Manager (void);

  int init (int argc, char *argv[]);
  // Initialize the ORB, POA etc.

  int activate_servant (void);
  // Activate the servant etc.

  int run (void);
  // Run the ORB's event loop.

  CORBA::ORB_ptr orb (void);
  // ORB's accessor.

  CORBA::Object_ptr server (void);
  // Server's accessor.

private:
  CORBA::ORB_var orb_;
  // Our ORB

  PortableServer::POA_var new_poa_var_;
  // The new poa that is created.

  ACE_Auto_Ptr<Servant_Locator> servant_locator_;
  // Our servant locator

  CORBA::Object_var server_;
  // 
};

#endif /*_MANAGER_H_ */
