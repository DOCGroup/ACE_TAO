// $Id$

//================================================================================
//
// = LIBRARY
//     TAO/tests/POA/On_Demand_Loading
//
// = FILENAME
//     Servant_Locator.h
//
// = DESCRIPTION
//     Defines a ServantLocator_i class , used with a POA having
//     a NON_RETAIN policy. 
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
//==================================================================================

#ifndef SERVANT_LOCATOR_H
#define SERVANT_LOCATOR_H

#include "tao/corba.h"
#include "ace/DLL.h"
#include "Servant_Manager.h"

class ServantLocator_i : public POA_PortableServer::ServantLocator
{
  // = TITLE
  //   This class is used by a POA with USE_SERVANT_MANAGER and
  //   NON_RETAIN policy.
  // 
  // = DESCRIPTION
  //   @@ Kirthika, please explain what this class DOES.
public:
  ServantLocator_i (CORBA::ORB_ptr orb);
  // Constructor.

  virtual PortableServer::Servant preinvoke (const PortableServer::ObjectId &oid,
                                             PortableServer::POA_ptr adapter,
                                             const char *operation,
                                             PortableServer::ServantLocator::Cookie &the_cookie,
                                             CORBA::Environment &env);
  // This method is invoked by a POA whenever it receives a request
  // for MyFoo object that is not currently active.
  // @@ Kirthika, please explain briefly what THIS method does
  // whenever it is invoked, i.e., explain the use of the
  // ServantManager stuff...

  virtual void postinvoke (const PortableServer::ObjectId &oid,
                           PortableServer::POA_ptr adapter,
                           const char *operation,
                           PortableServer::ServantLocator::Cookie the_cookie,
                           PortableServer::Servant the_servant,
                           CORBA::Environment &env);
  // This method is invoked whenever a MyFooServant completes a
  // request.
  // @@ Kirthika, please explain briefly what THIS method does
  // whenever it is invoked, i.e., explain the use of the
  // ServantManager stuff...

  PortableServer::ObjectId_var create_dll_object_id (const char *dllname, 
                                                     const char *factory_function);
  // Returns an ObjectId when given an dll name and the factory method
  // to be invoked in the dll.
private:
  ServantManager_i servant_manager_;
  // The <ServantManager_i> object that provides utility methods.
  // @@ Kirthika, briefly summarize what these methods accomplish (in
  // general).

  int counter_;
  // Counter for number of invocations of this.
  // @@ Kirthika, please explain what this is needed for.
};

#endif /* SERVANT_LOCATOR_H */
