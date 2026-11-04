
//=============================================================================
/**
 *  @file     Servant_Locator.h
 *
 *   Defines a MyFooServantLocator class , used with a POA having
 *   a NON_RETAIN policy
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#ifndef _SERVANT_LOCATOR_H_
#define _SERVANT_LOCATOR_H_

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/ServantLocatorC.h"

/**
 * @class Servant_Locator
 *
 * @brief This class is used by a POA with USE_SERVANT_MANAGER and
 * NON_RETAIN policy.
 */
class Servant_Locator : public PortableServer::ServantLocator
{
public:
  Servant_Locator () = default;
  Servant_Locator (Servant_Locator const &) = delete;
  Servant_Locator (Servant_Locator &&) = delete;
  Servant_Locator &operator= (Servant_Locator const &) = delete;
  Servant_Locator &operator= (Servant_Locator &&) = delete;
  ~Servant_Locator () override = default;

  /// This method is invoked by a POA whenever it receives a request
  /// for MyFoo object that is not currently active.
  PortableServer::Servant preinvoke (const PortableServer::ObjectId &oid,
                                     PortableServer::POA_ptr adapter,
                                     const char *operation,
                                     PortableServer::ServantLocator::Cookie &the_cookie) override;

  /// This method is invoked whenever a MyFooServant completes a
  /// request.
  void postinvoke (const PortableServer::ObjectId &oid,
                   PortableServer::POA_ptr adapter,
                   const char *operation,
                   PortableServer::ServantLocator::Cookie the_cookie,
                   PortableServer::Servant the_servant) override;

  /// To be removed..
  void set (CORBA::Object_ptr obj);

 private:
  /// To be removed
  CORBA::Object_var objref_;
};

#endif /* _SERVANT_LOCATOR_H */
