//
// $Id$
//

#ifndef STOCK_FACTORY_LOCATOR_I_H
#define STOCK_FACTORY_LOCATOR_I_H

#include "tao/corba.h"

class Quoter_Stock_Factory_Locator_i : public POA_PortableServer::ServantLocator
{
 public:
  Quoter_Stock_Factory_Locator_i (CORBA::ORB_ptr orb);
  
  // Preinvoke function
  virtual PortableServer::Servant preinvoke (const PortableServer::ObjectId &oid, 
                                             PortableServer::POA_ptr poa, 
                                             const char * operation, 
                                             void * & cookie)
    throw (CORBA::SystemException, PortableServer::ForwardRequest);
  
  // Postinvoke function
  virtual void postinvoke (const PortableServer::ObjectId & oid, 
                           PortableServer::POA_ptr poa,
                           const char * operation, 
                           void * cookie, 
                           PortableServer::Servant servant)
    throw (CORBA::SystemException);

 private:
  CORBA::ORB_var orb_;
  
};

#endif /* STOCK_FACTORY_LOCATOR_I_H */















