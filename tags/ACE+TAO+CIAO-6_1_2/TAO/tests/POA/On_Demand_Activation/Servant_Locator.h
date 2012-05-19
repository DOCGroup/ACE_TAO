// -*- C++ -*-

//=============================================================================
/**
 *  @file     Servant_Locator.h
 *
 *  $Id$
 *
 *   Defines a ServantLocator class, used with a POA having a
 *   NON_RETAIN policy
 *
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/ServantLocatorC.h"
#include "tao/LocalObject.h"

#include "tao/ORB.h"

class ServantLocator :
  public virtual PortableServer::ServantLocator,
  public virtual ::CORBA::LocalObject

{
  // = TITLE
  //   This class is used by a POA with USE_SERVANT_MANAGER and
  //   NON_RETAIN policy.
public:
  /// constructor
  ServantLocator (CORBA::ORB_ptr orb);

  /// This method is invoked by a POA whenever it receives a request
  /// for test object that is not currently active.
  virtual PortableServer::Servant preinvoke (const PortableServer::ObjectId &oid,
                                             PortableServer::POA_ptr adapter,
                                             const char *operation,
                                             PortableServer::ServantLocator::Cookie &the_cookie);

  /// This method is invoked whenever a test servant completes a
  /// request.
  virtual void postinvoke (const PortableServer::ObjectId &oid,
                           PortableServer::POA_ptr adapter,
                           const char *operation,
                           PortableServer::ServantLocator::Cookie the_cookie,
                           PortableServer::Servant the_servant);

private:
  /// Counter for number of invocations of this.
  int counter_;

  CORBA::ORB_var orb_;
};
