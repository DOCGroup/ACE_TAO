#include "Servant_Locator.h"
#include "MyFooServant.h"

MyFooServantLocator::MyFooServantLocator (void)
  : counter_ (0)
{
}


PortableServer::Servant 
MyFooServantLocator::preinvoke (const PortableServer::ObjectId &oid, 
                                PortableServer::POA_ptr poa, 
                                const char *operation, 
                                PortableServer::ServantLocator::Cookie &cookie,  
                                CORBA::Environment &env)
{
  CORBA::String_var s = PortableServer::ObjectId_to_string (oid);
  if (ACE_OS::strstr (s.in (), "Foo") != 0) 
    {
      PortableServer::Servant servant = new MyFooServant (poa, ++this->counter_);
      cookie = servant;
      return servant;
    } 
  else 
    {
      CORBA::Exception *exception = new CORBA::OBJECT_NOT_EXIST (CORBA::COMPLETED_NO);
      env.exception (exception);
      return 0;
    }
}
  
void 
MyFooServantLocator::postinvoke (const PortableServer::ObjectId &oid, 
                                 PortableServer::POA_ptr poa, 
                                 const char *operation, 
                                 PortableServer::ServantLocator::Cookie cookie, 
                                 PortableServer::Servant servant,  
                                 CORBA::Environment &env)
{
  PortableServer::Servant my_servant = (PortableServer::Servant) cookie;
  ACE_ASSERT (servant == my_servant);
  delete servant;
}
