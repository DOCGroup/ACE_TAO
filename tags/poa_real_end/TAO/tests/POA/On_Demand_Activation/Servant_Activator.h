#include "tao/corba.h"

class MyFooServantActivator : public POA_PortableServer::ServantActivator
{
public:
  virtual PortableServer::Servant incarnate (const PortableServer::ObjectId &oid, 
                                             PortableServer::POA_ptr poa,
                                             CORBA::Environment &env);

  virtual void etherealize (const PortableServer::ObjectId &oid, 
                            PortableServer::POA_ptr adapter, 
                            PortableServer::Servant servant, 
                            CORBA::Boolean cleanup_in_progress, 
                            CORBA::Boolean remaining_activations,  
                            CORBA::Environment &env);
};

