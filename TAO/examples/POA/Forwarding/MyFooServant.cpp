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
  : orb_ (CORBA::ORB::_duplicate (orb_ptr)),
    poa_ (PortableServer::POA::_duplicate (poa_ptr)),
    value_ (value),
    forward_to_var_ (CORBA::Object::_duplicate (forward_to_ptr))
{
  if (CORBA::is_nil (this->forward_to_var_.in ()))
    ACE_DEBUG ((LM_DEBUG,
                "POA approach: Forward_to is nil!\n"));

  // Assign this object a dummy I/O descriptor.  Note that even though
  // we open this file "Write Only" we still need to use the
  // ACE_Event_Handler::NULL_MASK when registering this with the
  // ACE_Reactor (see below).
  this->handle_ = ACE_OS::open (ACE_DEV_NULL, O_WRONLY);
  ACE_ASSERT (this->handle_ != ACE_INVALID_HANDLE);

  // Register handle.  Note that NULL_MASK is used to keep the
  // ACE_Reactor from calling us back on the ACE_DEV_NULL descriptor.
  ACE_Reactor_Mask mask = ACE_Event_Handler::NULL_MASK;
  int result = this->orb_->orb_core ()->reactor ()->
    register_handler (this->handle_, this, mask);
  ACE_ASSERT (result == 0);
}

// Destructor
MyFirstFooServant::~MyFirstFooServant (void)
{
  ACE_Reactor_Mask mask = ACE_Event_Handler::NULL_MASK;
  int result = this->orb_->orb_core ()->reactor ()->
    remove_handler (this->handle_, mask);
  ACE_ASSERT (result == 0);
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
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->value_++;
}

void
MyFirstFooServant::shutdown (CORBA::Environment &/*env*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown();
}


void
MyFirstFooServant::forward (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Foo::Cannot_Forward))
{
  ACE_DEBUG ((LM_DEBUG,
              "MyFirstFooServant::forward: being called\n"));
  if (!CORBA::is_nil (this->forward_to_var_.in ()))
    {
      //
      // This is a *hack*!  We cannot ask the POA to replace us while
      // the upcall is still in progress.  So we ask the Reactor to
      // wake us up as soon as this upcall completes.  At that time
      // (in handle_input), we ask the POA to replace us.
      //

      int result = this->orb_->orb_core ()->reactor ()->ready_ops
	(this->handle_,
         ACE_Event_Handler::READ_MASK,
         ACE_Reactor::ADD_MASK);
      ACE_ASSERT (result != -1);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "POA approach: Forward_to refenence is nil.\n"));
      ACE_THROW (Foo::Cannot_Forward ());
    }
}

int
MyFirstFooServant::handle_input (ACE_HANDLE)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      PortableServer::ObjectId_var oid =
        this->poa_->servant_to_id (this, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::Servant servant = this->poa_->_servant ();
      ACE_ASSERT (servant != 0);

      void *ptr = servant->_downcast ("IDL:omg.org/PortableServer/POA:1.0");
      POA_PortableServer::POA *poa = (POA_PortableServer::POA *) ptr;
      TAO_POA *tao_poa = ACE_dynamic_cast (TAO_POA *, poa);

      tao_poa->forward_object (oid.in (),
                               this->forward_to_var_.in (),
                               ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_ASSERT (1 == 0);
    }
  ACE_ENDTRY;

  return 0;
}

// Second Foo

// Constructor
MySecondFooServant::MySecondFooServant (CORBA::ORB_ptr orb_ptr,
                                        MyFooServantLocator *locator_ptr,
                                        CORBA::Long value)
  : orb_ (CORBA::ORB::_duplicate (orb_ptr)),
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
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return this->value_++;
}

void
MySecondFooServant::forward (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Foo::Cannot_Forward))
{
  // forward the forwarding request to the Servant Locator :-) This is
  // kind of a loop back, but it is correct only the IDL interface can
  // be assumed !!
  this->locator_ptr_->forward (ACE_TRY_ENV);
  ACE_CHECK;
}


void
MySecondFooServant::shutdown (CORBA::Environment &/*env*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown();
}
