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
//     Implementation of ServantLocator class , used with a POA
//     having a NON_RETAIN policy.
//
// = AUTHOR
//     Irfan Pyarali
//
// ============================================================================

#include "Servant_Locator.h"
#include "test_i.h"

ACE_RCSID(On_Demand_Activation, Servant_Locator, "$Id$")

ServantLocator::ServantLocator (CORBA::ORB_ptr orb)
  : counter_ (0),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}


PortableServer::Servant
ServantLocator::preinvoke (const PortableServer::ObjectId &oid,
                           PortableServer::POA_ptr poa,
                           const char * /* operation */,
                           PortableServer::ServantLocator::Cookie &cookie
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::ForwardRequest))
{
  // Convert ObjectID to String.

  CORBA::String_var s = PortableServer::ObjectId_to_string (oid);
  // If ObjectID string has a test substring, create and return a
  // test_i.

  if (ACE_OS::strstr (s.in (), "test") != 0)
    {
      PortableServer::Servant servant =
        new test_i (this->orb_.in (), poa);

      // Return the servant as the cookie , used as a check when
      // postinvoke is called on this ServantLocator.

      cookie = servant;
      return servant;
    }
  else
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (), 0);
    }
}

void
ServantLocator::postinvoke (const PortableServer::ObjectId & /* oid */,
                            PortableServer::POA_ptr /* poa */,
                            const char * /* operation */,
                            PortableServer::ServantLocator::Cookie cookie,
                            PortableServer::Servant servant
                            ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Check the passed servant with the cookie.

  PortableServer::Servant my_servant = (PortableServer::Servant) cookie;
  ACE_ASSERT (servant == my_servant);
  delete servant;

  // To avoid warning about unused variable with ACE_NDEBUG.
  ACE_UNUSED_ARG (my_servant);
}
