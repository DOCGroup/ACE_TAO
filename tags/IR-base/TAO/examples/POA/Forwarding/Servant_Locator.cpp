// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO/tests/POA/Forwarding/Servant_Locator
//
// = FILENAME
//     Servant_Locator.cpp
//
// = DESCRIPTION
//     Implementation of MyFooServantLocator class , used with a POA
//     having a NON_RETAIN policy.
//
// = AUTHOR
//     Irfan Pyarali
//     Michael Kircher
//
// ============================================================================

#include "Servant_Locator.h"
#include "MyFooServant.h"

ACE_RCSID(Forwarding, Servant_Locator, "$Id$")

MyFooServantLocator::MyFooServantLocator (CORBA::ORB_ptr orb_ptr,
                                          CORBA::Object_ptr forward_to_ptr)
  : orb_var_ (CORBA::ORB::_duplicate (orb_ptr)),
    counter_ (0),
    servant_ptr_ (0),
    forward_to_var_ (CORBA::Object::_duplicate (forward_to_ptr)),
    forwarding_ (0)  // by default do not forward
{
}

MyFooServantLocator::~MyFooServantLocator ()
{
  delete servant_ptr_;
}


PortableServer::Servant
MyFooServantLocator::preinvoke (const PortableServer::ObjectId &oid,
                                PortableServer::POA_ptr /* poa_ptr */,
                                const char *operation,
                                PortableServer::ServantLocator::Cookie & /* cookie */,
                                CORBA::Environment &ACE_TRY_ENV)
{
  ACE_UNUSED_ARG (operation);

  if (this->forwarding_ == 0) // do not forward
    {

      // Convert ObjectID to String.

      CORBA::String_var s = PortableServer::ObjectId_to_string (oid);

      // If ObjectID string has a Foo Substring create and return a
      // MyFooServant.

      this->counter_++;

      if (ACE_OS::strstr (s.in (), "Foo") != 0)
        {
          if (this->servant_ptr_ == 0)
            {
              this->servant_ptr_ = new MySecondFooServant (orb_var_.in(),
                                                           this,
                                                           127);

              // Return the servant as the cookie , used as a check when
              // postinvoke is called on this MyFooServantLocator.
              // cookie = servant;
            }
          // reuse the old servant

          return this->servant_ptr_;
        }
      else
        {
          ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (CORBA::COMPLETED_NO), 0);
        }
    }
  else // now forward, in throwing the ForwardRequest Exception
  {
    // Throw forward exception
    ACE_THROW_RETURN (PortableServer::ForwardRequest (), 0);
  }
}

void
MyFooServantLocator::postinvoke (const PortableServer::ObjectId &,
                                 PortableServer::POA_ptr ,
                                 const char *,
                                 PortableServer::ServantLocator::Cookie ,
                                 PortableServer::Servant ,
                                 CORBA::Environment &)
{
}

void
MyFooServantLocator::forward (CORBA::Environment &)
{
  this->forwarding_ = 1;
}

