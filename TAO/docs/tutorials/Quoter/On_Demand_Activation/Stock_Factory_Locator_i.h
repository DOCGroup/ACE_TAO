
#ifndef STOCK_FACTORY_LOCATOR_I_H
#define STOCK_FACTORY_LOCATOR_I_H

#include "tao/ORB.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/ServantLocatorC.h"

class Quoter_Stock_Factory_Locator_i : public PortableServer::ServantLocator
{
 public:
  explicit Quoter_Stock_Factory_Locator_i (CORBA::ORB_ptr orb);

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

  Quoter_Stock_Factory_Locator_i (Quoter_Stock_Factory_Locator_i const &) = delete;
  Quoter_Stock_Factory_Locator_i (Quoter_Stock_Factory_Locator_i &&) = delete;
  Quoter_Stock_Factory_Locator_i &operator= (Quoter_Stock_Factory_Locator_i const &) = delete;
  Quoter_Stock_Factory_Locator_i &operator= (Quoter_Stock_Factory_Locator_i &&) = delete;

 private:
  CORBA::ORB_var orb_;
};

#endif /* STOCK_FACTORY_LOCATOR_I_H */
