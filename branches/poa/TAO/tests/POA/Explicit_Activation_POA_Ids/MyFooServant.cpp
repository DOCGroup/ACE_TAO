#include "MyFooServant.h"

MyFooServant::MyFooServant (PortableServer::POA_ptr poa, 
                            CORBA::Long value)
  : poa_ (PortableServer::POA::_duplicate (poa)), 
    value_ (value) 
{
}
  
MyFooServant::~MyFooServant (void) 
{ 
}

PortableServer::POA_ptr 
MyFooServant::_default_POA (CORBA::Environment &env)
{ 
  return PortableServer::POA::_duplicate (this->poa_); 
}

CORBA::Long 
MyFooServant::doit (CORBA::Environment &env) 
{ 
  return this->value_; 
}

