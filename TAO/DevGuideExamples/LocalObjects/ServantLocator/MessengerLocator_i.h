#ifndef MESSENGER_LOCATOR_I_H
#define MESSENGER_LOCATOR_I_H

#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/ServantLocatorC.h"

class Messenger_Locator_i : public PortableServer::ServantLocator,
          public CORBA::LocalObject
{
 public:
  Messenger_Locator_i () = default;
  ~Messenger_Locator_i () override = default;

  Messenger_Locator_i (Messenger_Locator_i const &) = delete;
  Messenger_Locator_i (Messenger_Locator_i &&) = delete;
  Messenger_Locator_i &operator= (Messenger_Locator_i const &) = delete;
  Messenger_Locator_i &operator= (Messenger_Locator_i &&) = delete;

  // Preinvoke function
  PortableServer::Servant preinvoke (PortableServer::ObjectId const &oid,
                                     PortableServer::POA_ptr poa,
                                     char const *operation,
                                     void *&cookie) override;

  // Postinvoke function
  void postinvoke (PortableServer::ObjectId const &oid,
                   PortableServer::POA_ptr poa,
                   char const *operation,
                   void *cookie,
                   PortableServer::Servant servant) override;
};

#endif
