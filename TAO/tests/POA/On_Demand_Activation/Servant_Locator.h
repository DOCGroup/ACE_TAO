// $Id$

//================================================================================
//
// = LIBRARY
//     TAO/tests/POA/On_Demand_Activation/Servant_Locator
//
// = FILENAME
//     Servant_Locator.h
//
// = DESCRIPTION
//     Defines a MyFooServantLocator class , used with a POA having
//     a NON_RETAIN policy
//
// = AUTHOR
//     Irfan Pyarali
//
//==================================================================================

#include "tao/corba.h"

class MyFooServantLocator : public POA_PortableServer::ServantLocator
{
  // = TITLE
  //   This class is used by a POA with USE_SERVANT_MANAGER and
  //   NON_RETAIN policy.
public:
  MyFooServantLocator (CORBA::ORB_ptr orb);
  // constructor

  virtual PortableServer::Servant preinvoke (const PortableServer::ObjectId &oid,
                                             PortableServer::POA_ptr adapter,
                                             const char *operation,
                                             PortableServer::ServantLocator::Cookie &the_cookie,
                                             CORBA::Environment &env);
  // This method is invoked by a POA whenever it receives a request
  // for MyFoo object that is not currently active.

  virtual void postinvoke (const PortableServer::ObjectId &oid,
                           PortableServer::POA_ptr adapter,
                           const char *operation,
                           PortableServer::ServantLocator::Cookie the_cookie,
                           PortableServer::Servant the_servant,
                           CORBA::Environment &env);
  // This method is invoked whenever a MyFooServant completes a
  // request.

private:
  int counter_;
  // Counter for number of invocations of this.

  CORBA::ORB_var orb_;
};
