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
//     Defines a MyFooServantActivator class.
//
// = AUTHOR
//     Irfan Pyarali
//
// ================================================================

#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"

class MyFooServantActivator : public PortableServer::ServantActivator
{
public:
  MyFooServantActivator (CORBA::ORB_ptr orb);

  virtual PortableServer::Servant incarnate (const PortableServer::ObjectId &oid,
                                             PortableServer::POA_ptr poa
                                             TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableServer::ForwardRequest));
  // This method is invoked by a POA with USE_SERVANT_MANAGER and
  // RETAIN policies , whenever it receives a request for a MyFoo
  // object that is not currently active.

  virtual void etherealize (const PortableServer::ObjectId &oid,
                            PortableServer::POA_ptr adapter,
                            PortableServer::Servant servant,
                            CORBA::Boolean cleanup_in_progress,
                            CORBA::Boolean remaining_activations
                            TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // This method is invoked whenever a MyFooServant for a MyFoo object
  // is deactivated.

private:
  CORBA::ORB_var orb_;
  // A reference to the ORB.
};
