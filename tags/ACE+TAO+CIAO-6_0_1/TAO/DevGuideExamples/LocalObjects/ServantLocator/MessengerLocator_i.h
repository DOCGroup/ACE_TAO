// $Id$

#ifndef MESSENGER_LOCATOR_I_H
#define MESSENGER_LOCATOR_I_H

#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/ServantLocatorC.h"

class Messenger_Locator_i : public PortableServer::ServantLocator,
          public CORBA::LocalObject
{
 public:
  Messenger_Locator_i();

  // Preinvoke function
  virtual PortableServer::Servant preinvoke (const PortableServer::ObjectId &oid,
                                             PortableServer::POA_ptr poa,
                                             const char * operation,
                                             void * & cookie);

  // Postinvoke function
  virtual void postinvoke (const PortableServer::ObjectId & oid,
                           PortableServer::POA_ptr poa,
                           const char * operation,
                           void * cookie,
                           PortableServer::Servant servant);
};

#endif
