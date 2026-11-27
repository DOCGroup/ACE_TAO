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

  /// Initializes the ORB and POA.
  int init (int argc, ACE_TCHAR *argv[]);

  /// Activates the servant.
  int activate_servant ();

  int make_iors_register (Manager_Options const &options);

  /// Runs the ORB event loop.
  int run ();

private:
  /// The ORB.
  CORBA::ORB_var orb_;

  /// The new POA.
  PortableServer::POA_var new_poa_var_;

  /// Non-owning alias; servant_locator_var_ holds the servant reference.
  Servant_Locator *servant_locator_ {};

  /// The servant locator reference.
  PortableServer::ServantLocator_var servant_locator_var_;

  CORBA::Object_var new_manager_ior_;
};
#endif /*_MANAGER_H_ */
