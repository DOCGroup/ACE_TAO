// $Id$

//===============================================================================
//
//
// = FILENAME
//     MyFooServant.cpp
//
// = DESCRIPTION
//     This is a simple foo servant implementation
//
// = AUTHOR
//     Irfan Pyarali
//
//==================================================================================

#include "MyFooServant.h"

// Constructor
MyFooServant::MyFooServant (PortableServer::POA_ptr poa,
                            CORBA::Long value,
                            CORBA::Object_ptr forward_to)
  : poa_ (PortableServer::POA::_duplicate (poa)),
    value_ (value),
    forward_to_ (CORBA::Object::_duplicate (forward_to))
{
}

// Destructor
MyFooServant::~MyFooServant (void)
{
}

// Return the Default POA of this Servant
PortableServer::POA_ptr
MyFooServant::_default_POA (CORBA::Environment &/*env*/)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

// Return this->value
CORBA::Long
MyFooServant::doit (CORBA::Environment &/*env*/)
{
  return this->value_++;
}

void 
MyFooServant::forward (CORBA::Environment &env)
{
  if (!CORBA::is_nil (this->forward_to_.in ()))
    {
      PortableServer::ObjectId_var oid = this->poa_->servant_to_id (this, env);

      if (env.exception () != 0)
        return;

      PortableServer::Servant servant = this->poa_->_servant ();
      if (servant == 0)
        {
          CORBA::Exception *exception = new Foo::Cannot_Forward;
          env.exception (exception);
          return;
        }

      void *ptr = servant->_downcast ("IDL:PortableServer/POA:1.0");
      POA_PortableServer::POA *poa = (POA_PortableServer::POA *) ptr;
      TAO_POA *tao_poa = ACE_dynamic_cast (TAO_POA *, poa);

      tao_poa->forward_object (oid.in (),
                               this->forward_to_.in (),
                               env);
    }
  else
    {
      CORBA::Exception *exception = new Foo::Cannot_Forward;
      env.exception (exception);
      return;
    }
}
