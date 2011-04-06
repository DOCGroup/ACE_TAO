// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file     Servant_Locator.h
 *
 *  $Id$
 *
 *   Defines a ServantLocator_i class , used with a POA having
 *   a NON_RETAIN policy.
 *
 *
 *  @author  Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef SERVANT_LOCATOR_H
#define SERVANT_LOCATOR_H

#include "ace/DLL.h"

#include "Servant_Manager.h"
#include "tao/PortableServer/ServantLocatorC.h"
#include "tao/LocalObject.h"

class ServantLocator_i :
  public virtual PortableServer::ServantLocator,
  public virtual ::CORBA::LocalObject

{
  // = TITLE
  //   This class is used by a POA with USE_SERVANT_MANAGER and
  //   NON_RETAIN policy.
  //
  // = DESCRIPTION
  //   This class defines the Servant Locator interface of the Servant
  //   Manager. It is invoked when the POA has an USE_SERVANT_MANAGER
  //   policy and a servant_retention policy of NON_RETAIN type.
public:
  /// Constructor.
  ServantLocator_i (CORBA::ORB_ptr orb);

  /**
   * This method is invoked by a POA whenever it receives a request
   * for test object that is not currently active.  When the POA is
   * created using the NON_RETAIN policy the Active Object Map is not
   * maintained, in other words, an association between the ObjectId
   * and the servant is not maintained. Hence every client request the
   * servant has to be loaded. Note the operation argument. This
   * argument specifies the operation to be invoked on the
   * servant. The cookie helps in marking the servant. This marking is
   * useful while destroying the servant.
   */
  virtual PortableServer::Servant preinvoke (const PortableServer::ObjectId &oid,
                                             PortableServer::POA_ptr adapter,
                                             const char *operation,
                                             PortableServer::ServantLocator::Cookie &the_cookie);

  /**
   * This method is invoked whenever a test servant completes a
   * request. As the Servant Loactor interface is used when the POA
   * doesnt maintain the Active Object Map, its necessary to get rid
   * of the servant after the client request has been processed. The
   * appropriate servant is destroyed by verifying the cookie.Again
   * this method is invoked per client request.
   */
  virtual void postinvoke (const PortableServer::ObjectId &oid,
                           PortableServer::POA_ptr adapter,
                           const char *operation,
                           PortableServer::ServantLocator::Cookie the_cookie,
                           PortableServer::Servant the_servant);

  /// Returns an ObjectId when given an dll name and the factory method
  /// to be invoked in the dll.
  PortableServer::ObjectId_var create_dll_object_id (const char *dllname,
                                                     const char *factory_function);
private:
  /**
   * The <ServantManager_i> object that provides utility methods.  The
   * methods include obtaining the servant using an ACE_DLL object,
   * destroying the servant and extracting the dllname and factory
   * function from the ObjectId.
   */
  ServantManager_i servant_manager_;
};

#endif /* SERVANT_LOCATOR_H */
