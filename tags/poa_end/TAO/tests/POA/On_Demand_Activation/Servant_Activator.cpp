#include "Servant_Activator.h"
#include "MyFooServant.h"

PortableServer::Servant 
MyFooServantActivator::incarnate (const PortableServer::ObjectId &oid, 
                                  PortableServer::POA_ptr poa,
                                  CORBA::Environment &env)
{
  CORBA::String_var s = PortableServer::ObjectId_to_string (oid);
  if (ACE_OS::strstr (s.in (), "Foo") != 0) 
    {
      return new MyFooServant (poa, 27);
    } 
  else 
    {
      CORBA::Exception *exception = new CORBA::OBJECT_NOT_EXIST (CORBA::COMPLETED_NO);
      env.exception (exception);
      return 0;
    }
}

void 
MyFooServantActivator::etherealize (const PortableServer::ObjectId &oid,
                                    PortableServer::POA_ptr poa,
                                    PortableServer::Servant servant,
                                    CORBA::Boolean cleanup_in_progress, 
                                    CORBA::Boolean remaining_activations,
                                    CORBA::Environment &env)
{
  if (remaining_activations == CORBA::B_FALSE)
    delete servant;
}

