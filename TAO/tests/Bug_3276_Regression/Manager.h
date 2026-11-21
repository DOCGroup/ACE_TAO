#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "Servant_Locator.h"

class Manager
{
public:
  Manager () = default;

  ~Manager ();

  Manager (Manager const &) = delete;
  Manager (Manager &&) = delete;
  Manager &operator= (Manager const &) = delete;
  Manager &operator= (Manager &&) = delete;

  /// Initializes the ORB, POA, and related objects.
  int init (int argc, ACE_TCHAR * argv[]);

  /// Activates the servant.
  int activate_servant ();

  /// Runs the ORB event loop.
  int run ();

  /// Returns the ORB.
  CORBA::ORB_ptr orb () const;

  /// Returns the server object.
  CORBA::Object_ptr server () const;

private:
  /// ORB used by this manager.
  CORBA::ORB_var orb_ {};

  /// Newly created POA.
  PortableServer::POA_var new_poa_var_ {};

  /// Servant locator used by this manager.
  PortableServer::ServantLocator_var servant_locator_ {};

  /// Server object reference.
  CORBA::Object_var server_ {};
};

#endif /*_MANAGER_H_ */
