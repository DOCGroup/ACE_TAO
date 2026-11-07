// -*- C++ -*-

//=============================================================================
/**
 *  @file     Servant_Locator.h
 *
 *   Defines a ServantLocator class, used with a POA having a
 *   NON_RETAIN policy
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
  explicit ServantLocator (CORBA::ORB_ptr orb);

  ServantLocator (ServantLocator const &) = delete;
  ServantLocator &operator= (ServantLocator const &) = delete;
  ServantLocator (ServantLocator &&) = delete;
  ServantLocator &operator= (ServantLocator &&) = delete;

  /// This method is invoked by a POA whenever it receives a request
  /// for test object that is not currently active.
  PortableServer::Servant preinvoke (PortableServer::ObjectId const &oid,
                                     PortableServer::POA_ptr adapter,
                                     char const *operation,
                                     PortableServer::ServantLocator::Cookie &the_cookie) override;

  /// This method is invoked whenever a test servant completes a
  /// request.
  void postinvoke (PortableServer::ObjectId const &oid,
                   PortableServer::POA_ptr adapter,
                   char const *operation,
                   PortableServer::ServantLocator::Cookie the_cookie,
                   PortableServer::Servant the_servant) override;

private:
  CORBA::ORB_var orb_;
};
