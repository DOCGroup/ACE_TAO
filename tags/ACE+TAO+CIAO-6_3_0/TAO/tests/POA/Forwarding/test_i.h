//=============================================================================
/**
 *  @file     test_i.h
 *
 *  $Id$
 *
 *   Defines test_i class for the test interface
 *
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

  virtual CORBA::Long doit (void);

  // Setup forwarding
  virtual void forward (void);

  virtual void shutdown (void);

  /// Returns the default POA for this servant.
  virtual PortableServer::POA_ptr _default_POA (void);

protected:
  CORBA::ORB_var orb_;
  PortableServer::POA_var poa_;
  ServantActivator &activator_;
  CORBA::Long value_;
};

