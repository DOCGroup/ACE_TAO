// $Id$

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
  virtual PortableServer::Servant preinvoke (const PortableServer::ObjectId &oid,
                                             PortableServer::POA_ptr adapter,
                                             const char *operation,
                                             PortableServer::ServantLocator::Cookie &the_cookie);
  // This method is invoked by a POA whenever it receives a request.

  virtual void postinvoke (const PortableServer::ObjectId &oid,
                           PortableServer::POA_ptr adapter,
                           const char *operation,
                           PortableServer::ServantLocator::Cookie the_cookie,
                           PortableServer::Servant the_servant);
  // This method is invoked whenever a servant completes a
  // request.
};

#endif /* _SERVANT_LOCATOR_H_ */
