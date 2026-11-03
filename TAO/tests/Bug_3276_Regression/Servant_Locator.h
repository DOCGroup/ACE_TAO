#ifndef _SERVANT_LOCATOR_H_
#define _SERVANT_LOCATOR_H_

#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/ServantLocatorC.h"
#include "tao/LocalObject.h"

class Servant_Locator
  : public PortableServer::ServantLocator
  , public virtual ::CORBA::LocalObject
{
public:
  Servant_Locator () = default;
  Servant_Locator (Servant_Locator const &) = delete;
  Servant_Locator &operator= (Servant_Locator const &) = delete;
  Servant_Locator (Servant_Locator &&) = delete;
  Servant_Locator &operator= (Servant_Locator &&) = delete;

  PortableServer::Servant preinvoke (PortableServer::ObjectId const &oid,
                                     PortableServer::POA_ptr adapter,
                                     char const *operation,
                                     PortableServer::ServantLocator::Cookie &the_cookie) override;
  // This method is invoked by a POA whenever it receives a request.

  void postinvoke (PortableServer::ObjectId const &oid,
                   PortableServer::POA_ptr adapter,
                   char const *operation,
                   PortableServer::ServantLocator::Cookie the_cookie,
                   PortableServer::Servant the_servant) override;
  // This method is invoked whenever a servant completes a
  // request.
};

#endif /* _SERVANT_LOCATOR_H_ */
