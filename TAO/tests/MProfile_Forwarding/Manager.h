#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "Servant_Locator.h"
#include "ace/SString.h"

struct Manager_Options
{
  ACE_TString first_ior;
  ACE_TString second_ior;
  ACE_TString third_ior;
  ACE_TString ior_output_file;
};

class Manager
{
public:
  Manager () = default;
  Manager (Manager const &) = delete;
  Manager (Manager &&) = delete;
  Manager &operator= (Manager const &) = delete;
  Manager &operator= (Manager &&) = delete;

  int init (int argc, ACE_TCHAR *argv[]);

  // Initialize the ORB, POA etc.

  int activate_servant ();
  // Activate the servant etc.

  int make_iors_register (Manager_Options const &options);

  int run ();
  // Run the  ORB event loop..
 private:
  CORBA::ORB_var orb_;
  // Our ORB

  PortableServer::POA_var new_poa_var_;
  // The new poa that is created..

  Servant_Locator *servant_locator_ {};
  // Non-owning alias; servant_locator_var_ holds the servant reference.

  PortableServer::ServantLocator_var servant_locator_var_;
  // Our servant locator var

  CORBA::Object_var new_manager_ior_;
};
#endif /*_MANAGER_H_ */
