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

#include "tao/corba.h"
#include "MyFooServant.h"

// Constructor
MyFirstFooServant::MyFirstFooServant (PortableServer::POA_ptr poa,
                                      CORBA::Long value,
                                      CORBA::Object_ptr forward_to)
  : poa_ (PortableServer::POA::_duplicate (poa)),
    value_ (value),
    forward_to_ (CORBA::Object::_duplicate (forward_to))
{
  if (CORBA::is_nil (forward_to))
    {
     ACE_DEBUG ((LM_DEBUG,"MyFirstFooServant::MyFirstFooServant: forward_to is nil!\n"));
    }
}

// Destructor
MyFirstFooServant::~MyFirstFooServant (void)
{
}

// Return the Default POA of this Servant
PortableServer::POA_ptr
MyFirstFooServant::_default_POA (CORBA::Environment &/*env*/)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

// Return this->value
CORBA::Long
MyFirstFooServant::doit (CORBA::Environment &/*env*/)
{
  return this->value_++;
}

void 
MyFirstFooServant::forward (CORBA::Environment &env)
{
  ACE_DEBUG ((LM_DEBUG,"MyFirstFooServant::forward: being called\n"));
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
      ACE_DEBUG ((LM_DEBUG,"MyFirstFooServant::forward: forward_to refenence is nil.\n"));
      CORBA::Exception *exception = new Foo::Cannot_Forward;
      env.exception (exception);
      return;
    }
}


// Second Foo

// Constructor
MySecondFooServant::MySecondFooServant (MyFooServantLocator *locator_ptr,
                                        CORBA::Long value)
  : locator_ptr_ (locator_ptr),
    value_ (value)
{
}

// Destructor
MySecondFooServant::~MySecondFooServant (void)
{
}


// Return this->value
CORBA::Long
MySecondFooServant::doit (CORBA::Environment &/*env*/)
{
  return this->value_++;
}

void 
MySecondFooServant::forward (CORBA::Environment &env)
{
  // forward the forwarding request to the Servant Locator
  // :-) This is kind of a loop back, but it is correct
  // only the IDL interface can be assumed !!
  locator_ptr_->forward (env);  
}


