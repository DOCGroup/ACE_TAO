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
//     Defines a MyFooServantLocator class , used with a POA having
//     a NON_RETAIN policy. 
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
//==================================================================================

#include "tao/corba.h"
#include "ace/DLL.h"

class MyFooServantLocator : public POA_PortableServer::ServantLocator
{
  // = TITLE
  //   This class is used by a POA with USE_SERVANT_MANAGER and
  //   NON_RETAIN policy.
public:

  typedef PortableServer::Servant (*Servant_Creator_Prototype) (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa, CORBA::Long value);
  // This typedef is used to typecast the void* obtained on finding a 
  // symbol in the library.

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

  PortableServer::ObjectId_var create_objectId (const char *libname, const char *factory_method);
  // Returns an ObjectId when given an library name and the factory method to be invoked in the library.

private:

  PortableServer::Servant invoke_servant (const char *str,
                                          PortableServer::POA_ptr poa,
                                          long value);
  // Gets the servant on the preinvoke call by loading the appropriate library
  // and getting the servant object.

  void parse_string (const char* s);
  // Parse the string to obtain the library name and the symbol which
  // will get us the servant pointer.

  int counter_;
  // Counter for number of invocations of this.

  CORBA::ORB_var orb_;
 // A reference to the ORB.

  CORBA::String_var dllname_;
  // The name of the library containing the servant.

  CORBA::String_var create_symbol_;
  // The symbol which on getting invoked will give us the servant
  // pointer.

  ACE_DLL dll_;
  // The library object.
};
