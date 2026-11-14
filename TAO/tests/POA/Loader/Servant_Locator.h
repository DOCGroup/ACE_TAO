//=============================================================================
/**
 *  @file     Servant_Locator.h
 *
 *   Defines a ServantLocator class , used with a POA having
 *   a NON_RETAIN policy.
 *
 *  @author  Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================

#ifndef SERVANT_LOCATOR_H
#define SERVANT_LOCATOR_H

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/ServantLocatorC.h"
#include "tao/LocalObject.h"

#include "tao/ORB.h"

#include "ace/DLL.h"
#include "ace/Log_Msg.h"

/**
 * This class is used by a POA with USE_SERVANT_MANAGER and NON_RETAIN
 * policy. It defines the Servant Locator interface of the Servant
 * Manager and is invoked when the POA has a servant_retention policy
 * of NON_RETAIN type.
 */
class ServantLocator :
  public virtual PortableServer::ServantLocator,
  public virtual ::CORBA::LocalObject
{
public:
  /**
   * This typedef is used to typecast the void* obtained when finding
   * a symbol in the dll. Invoking the function pointer obtained would
   * get a servant.
   */
  using SERVANT_FACTORY = PortableServer::Servant (*) (PortableServer::ObjectId const &oid,
                                                       PortableServer::POA_ptr poa,
                                                       CORBA::ORB_ptr orb);

  /**
   * This typedef is used to obtain the garbage_collection_function symbol
   * in the dll. Invoking the function pointer obtained would then destroy
   * the servant.
   */
  using SERVANT_GARBAGE_COLLECTOR = void (*) (PortableServer::ObjectId const &oid,
                                              PortableServer::POA_ptr,
                                              PortableServer::Servant servant);

  ServantLocator (CORBA::ORB_ptr orb,
                  ACE_TCHAR const *dllname,
                  ACE_TCHAR const *factory_function,
                  ACE_TCHAR const *garbage_collection_function);

  ServantLocator (ServantLocator const &) = delete;
  ServantLocator &operator= (ServantLocator const &) = delete;
  ServantLocator (ServantLocator &&) = delete;
  ServantLocator &operator= (ServantLocator &&) = delete;

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
  PortableServer::Servant preinvoke (PortableServer::ObjectId const &oid,
                                     PortableServer::POA_ptr adapter,
                                     char const *operation,
                                     PortableServer::ServantLocator::Cookie &the_cookie) override;

  /**
   * This method is invoked whenever a test servant completes a
   * request. As the Servant Loactor interface is used when the POA
   * doesnt maintain the Active Object Map, its necessary to get rid
   * of the servant after the client request has been processed. The
   * appropriate servant is destroyed by verifying the cookie.Again
   * this method is invoked per client request.
   */
  void postinvoke (PortableServer::ObjectId const &oid,
                           PortableServer::POA_ptr adapter,
                           char const *operation,
                           PortableServer::ServantLocator::Cookie the_cookie,
                           PortableServer::Servant the_servant) override;

private:
  /// The ACE_DLL object which performs the task of loading the dll
  /// and accessing it.
  ACE_DLL dll_;

  /// The function pointer of factory_function type.
  SERVANT_FACTORY servant_supplier_ {};

  /// The function pointer of garbage_collection_function type.
  SERVANT_GARBAGE_COLLECTOR servant_garbage_collector_ {};

  /// A reference to the ORB.
  CORBA::ORB_var orb_;
};

#endif /* SERVANT_LOCATOR_H */
