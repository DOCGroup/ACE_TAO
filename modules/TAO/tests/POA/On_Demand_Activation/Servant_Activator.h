// -*- C++ -*-
// $Id$

// ================================================================
//
// = LIBRARY
//     TAO/tests/POA/On_Demand_Activation/Servant_Activator
//
// = FILENAME
//     Servant_Activator.h
//
// = DESCRIPTION
//     Defines a ServantActivator class.
//
// = AUTHOR
//     Irfan Pyarali
//
// ================================================================

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/ServantActivatorC.h"
#include "tao/LocalObject.h"

#include "tao/ORB.h"

class ServantActivator :
  public virtual PortableServer::ServantActivator,
  public virtual ::CORBA::LocalObject
{
public:
  ServantActivator (CORBA::ORB_ptr orb);

  PortableServer::Servant incarnate (const PortableServer::ObjectId &oid,
                                     PortableServer::POA_ptr poa);
  // This method is invoked by a POA with USE_SERVANT_MANAGER and
  // RETAIN policies, whenever it receives a request for a test object
  // that is not currently active.

  void etherealize (const PortableServer::ObjectId &oid,
                    PortableServer::POA_ptr adapter,
                    PortableServer::Servant servant,
                    CORBA::Boolean cleanup_in_progress,
                    CORBA::Boolean remaining_activations);
  // This method is invoked whenever a test object is deactivated.

private:
  CORBA::ORB_var orb_;
  // A reference to the ORB.
};
