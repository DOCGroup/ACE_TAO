// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO/tests/POA/On_Demand_Activation/Servant_Locator
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
//
// ============================================================================

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
  ACE_UNUSED_ARG (operation);

  // Convert ObjectID to String.

  CORBA::String_var s = PortableServer::ObjectId_to_string (oid);
  // If ObjectID string has a Foo Substring create and return a
  // MyFooServant.

  if (ACE_OS::strstr (s.in (), "Foo") != 0)
    {
      PortableServer::Servant servant = new MyFooServant (poa, ++this->counter_);

      // Return the servant as the cookie , used as a check when
      // postinvoke is called on this MyFooServantLocator.

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
  ACE_UNUSED_ARG (oid);
  ACE_UNUSED_ARG (poa);
  ACE_UNUSED_ARG (operation);
  ACE_UNUSED_ARG (env);

  // Check the passed servant with the cookie.

  PortableServer::Servant my_servant = (PortableServer::Servant) cookie;
  ACE_ASSERT (servant == my_servant);
  delete servant;
}
