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

MyFooServantLocator::MyFooServantLocator (CORBA::ORB_ptr orb_ptr,
                                          CORBA::Object_ptr forward_to_ptr)
  : orb_var_ (CORBA::ORB::_duplicate (orb_ptr)), 
    counter_ (0),
    forward_to_var_ (CORBA::Object::_duplicate (forward_to_ptr)),
    forwarding_ (0),  // by default do not forward
    servant_ptr_ (0)
{
}

MyFooServantLocator::~MyFooServantLocator ()
{
  delete servant_ptr_;
}


PortableServer::Servant
MyFooServantLocator::preinvoke (const PortableServer::ObjectId &oid,
                                PortableServer::POA_ptr poa_ptr,
                                const char *operation,
                                PortableServer::ServantLocator::Cookie &cookie,
                                CORBA::Environment &env)
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
          CORBA::Exception *exception = new CORBA::OBJECT_NOT_EXIST (CORBA::COMPLETED_NO);
          env.exception (exception);
          return 0;
        }
    }
  else // now forward, in throwing the ForwardRequest Exception
  {
    // Throw forward exception
    env.exception (new PortableServer::ForwardRequest (this->forward_to_var_));

    ACE_DEBUG ((LM_DEBUG,"MyFooServantLocator::preinvoke: Threw the ForwardRequest exception.\n"));
    return 0;
  }
}

void
MyFooServantLocator::postinvoke (const PortableServer::ObjectId &oid,
                                 PortableServer::POA_ptr poa,
                                 const char *operation,
                                 PortableServer::ServantLocator::Cookie cookie,
                                 PortableServer::Servant p_servant,
                                 CORBA::Environment &env)
{
  ACE_UNUSED_ARG (oid);
  ACE_UNUSED_ARG (poa);
  ACE_UNUSED_ARG (operation);
  ACE_UNUSED_ARG (cookie);
  ACE_UNUSED_ARG (p_servant);
  ACE_UNUSED_ARG (env);
}

void 
MyFooServantLocator::forward (CORBA::Environment &env)
{
  this->forwarding_ = 1;
}

