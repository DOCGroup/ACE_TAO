// $Id$

//==========================================================================
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
//==========================================================================

#include "tao/corba.h"
#include "MyFooServant.h"

ACE_RCSID(Forwarding, MyFooServant, "$Id$")

// Constructor
MyFirstFooServant::MyFirstFooServant (CORBA::ORB_ptr orb_ptr,
                                      PortableServer::POA_ptr poa_ptr,
                                      CORBA::Long value,
                                      CORBA::Object_ptr forward_to_ptr)
  : orb_var_ (CORBA::ORB::_duplicate (orb_ptr)),
    poa_var_ (PortableServer::POA::_duplicate (poa_ptr)),
    value_ (value),
    forward_to_var_ (CORBA::Object::_duplicate (forward_to_ptr))
{
  if (CORBA::is_nil (this->forward_to_var_.in ()))
    ACE_DEBUG ((LM_DEBUG,
                "POA approach: Forward_to is nil!\n"));
}

// Destructor
MyFirstFooServant::~MyFirstFooServant (void)
{
}

// Return the Default POA of this Servant
PortableServer::POA_ptr
MyFirstFooServant::_default_POA (CORBA::Environment &/*env*/)
{
  return PortableServer::POA::_duplicate (this->poa_var_.in ());
}

// Return this->value
CORBA::Long
MyFirstFooServant::doit (CORBA::Environment &/*env*/)
{
  return this->value_++;
}

void
MyFirstFooServant::shutdown (CORBA::Environment &/*env*/)
{
  this->orb_var_->shutdown();
}


void 
MyFirstFooServant::forward (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_DEBUG ((LM_DEBUG,
              "MyFirstFooServant::forward: being called\n"));
  if (!CORBA::is_nil (this->forward_to_var_.in ()))
    {
      PortableServer::ObjectId_var oid =
        this->poa_var_->servant_to_id (this, ACE_TRY_ENV);
      ACE_CHECK;

      PortableServer::Servant servant = this->poa_var_->_servant ();
      if (servant == 0)
        {
          ACE_THROW (Foo::Cannot_Forward ());
        }
      
      void *ptr = servant->_downcast ("IDL:PortableServer/POA:1.0");
      POA_PortableServer::POA *poa = (POA_PortableServer::POA *) ptr;
      TAO_POA *tao_poa = ACE_dynamic_cast (TAO_POA *, poa);
      
      tao_poa->forward_object (oid.in (),
                               this->forward_to_var_.in (),
                               ACE_TRY_ENV);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "POA approach: Forward_to refenence is nil.\n"));
      ACE_THROW (Foo::Cannot_Forward ());
    }
}

// Second Foo

// Constructor
MySecondFooServant::MySecondFooServant (CORBA::ORB_ptr orb_ptr,
                                        MyFooServantLocator *locator_ptr,
                                        CORBA::Long value)
  : orb_var_ (CORBA::ORB::_duplicate (orb_ptr)),
    locator_ptr_ (locator_ptr),
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
MySecondFooServant::forward (CORBA::Environment &ACE_TRY_ENV)
{
  // forward the forwarding request to the Servant Locator :-) This is
  // kind of a loop back, but it is correct only the IDL interface can
  // be assumed !!
  this->locator_ptr_->forward (ACE_TRY_ENV);
  ACE_CHECK;
}


void
MySecondFooServant::shutdown (CORBA::Environment &/*env*/)
{
  this->orb_var_->shutdown();
}

