// $Id$

//=================================================================================
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
//==================================================================================

#include "tao/corba.h"

class MyFooServantActivator : public POA_PortableServer::ServantActivator
{
public:
  MyFooServantActivator (CORBA::ORB_ptr orb);

  virtual PortableServer::Servant incarnate (const PortableServer::ObjectId &oid,
                                             PortableServer::POA_ptr poa,
                                             CORBA::Environment &env);
  // This method is invoked by a POA with USE_SERVANT_MANAGER and
  // RETAIN policies , whenever it receives a request for a MyFoo
  // object that is not currently active.

  virtual void etherealize (const PortableServer::ObjectId &oid,
                            PortableServer::POA_ptr adapter,
                            PortableServer::Servant servant,
                            CORBA::Boolean cleanup_in_progress,
                            CORBA::Boolean remaining_activations,
                            CORBA::Environment &env);
  // This method is invoked whenever a MyFooServant for a MyFoo object
  // is deactivated.

private:
  CORBA::ORB_var orb_;
  // A reference to the ORB.
};
