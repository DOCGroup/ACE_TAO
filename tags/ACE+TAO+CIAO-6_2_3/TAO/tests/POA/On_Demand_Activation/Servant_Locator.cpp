
//=============================================================================
/**
 *  @file     Servant_Locator.cpp
 *
 *  $Id$
 *
 *   Implementation of ServantLocator class , used with a POA
 *   having a NON_RETAIN policy.
 *
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#include "Servant_Locator.h"
#include "test_i.h"
#include "ace/OS_NS_string.h"

ServantLocator::ServantLocator (CORBA::ORB_ptr orb)
  : counter_ (0),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}


PortableServer::Servant
ServantLocator::preinvoke (const PortableServer::ObjectId &oid,
                           PortableServer::POA_ptr poa,
                           const char * /* operation */,
                           PortableServer::ServantLocator::Cookie &cookie)
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
      throw CORBA::OBJECT_NOT_EXIST ();
    }
}

void
ServantLocator::postinvoke (const PortableServer::ObjectId & /* oid */,
                            PortableServer::POA_ptr /* poa */,
                            const char * /* operation */,
                            PortableServer::ServantLocator::Cookie cookie,
                            PortableServer::Servant servant)
{
  // Check the passed servant with the cookie.

  PortableServer::Servant my_servant = (PortableServer::Servant) cookie;
  ACE_ASSERT (servant == my_servant);
  delete servant;

  // To avoid warning about unused variable with ACE_NDEBUG.
  ACE_UNUSED_ARG (my_servant);
}
