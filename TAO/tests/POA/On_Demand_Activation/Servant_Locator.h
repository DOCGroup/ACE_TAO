#include "tao/corba.h"

class MyFooServantLocator : public POA_PortableServer::ServantLocator
{
public:
  MyFooServantLocator (void);

  virtual PortableServer::Servant preinvoke (const PortableServer::ObjectId &oid, 
                                             PortableServer::POA_ptr adapter, 
                                             const char *operation, 
                                             PortableServer::ServantLocator::Cookie &the_cookie,  
                                             CORBA::Environment &env);
  
  virtual void postinvoke (const PortableServer::ObjectId &oid, 
                           PortableServer::POA_ptr adapter, 
                           const char *operation, 
                           PortableServer::ServantLocator::Cookie the_cookie, 
                           PortableServer::Servant the_servant,  
                           CORBA::Environment &env);
private:
  
  int counter_;
};

