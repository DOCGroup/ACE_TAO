//=============================================================================
/**
 *  @file     test_i.h
 *
 *   Defines test_i class for the test interface
 *
 *  @author  Irfan Pyarali and Michael Kircher
 */
//=============================================================================

#include "testS.h"

class ServantActivator;

class test_i :
  public virtual POA_test
{
public:
  test_i (CORBA::ORB_ptr orb_ptr,
          PortableServer::POA_ptr poa,
          ServantActivator &activator,
          CORBA::Long value);

  test_i (test_i const &) = delete;
  test_i &operator= (test_i const &) = delete;
  test_i (test_i &&) = delete;
  test_i &operator= (test_i &&) = delete;

  CORBA::Long doit () override;

  // Setup forwarding
  void forward () override;

  void shutdown () override;

  /// Returns the default POA for this servant.
  PortableServer::POA_ptr _default_POA () override;

protected:
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  ServantActivator &activator_;
  CORBA::Long value_;
};

