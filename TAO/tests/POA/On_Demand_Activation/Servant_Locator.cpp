
//=============================================================================
/**
 *  @file     Servant_Locator.cpp
 *
 *   Implementation of ServantLocator class , used with a POA
 *   having a NON_RETAIN policy.
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================

#include "Servant_Locator.h"
#include "test_i.h"

#include <cstring>
#include <memory>

ServantLocator::ServantLocator (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

PortableServer::Servant
ServantLocator::preinvoke (PortableServer::ObjectId const &oid,
                           PortableServer::POA_ptr poa,
                           char const * /* operation */,
                           PortableServer::ServantLocator::Cookie &cookie)
{
  // Convert ObjectID to String.

  CORBA::String_var s = PortableServer::ObjectId_to_string (oid);
  // If ObjectID string has a test substring, create and return a
  // test_i.

  if (std::strstr (s.in (), "test") != nullptr)
    {
      std::unique_ptr<test_i> servant =
        std::make_unique<test_i> (this->orb_.in (), poa);

      // Return the servant as the cookie, used as a check when
      // postinvoke is called on this ServantLocator.

      cookie = static_cast<PortableServer::Servant> (servant.get ());
      // The POA owns the servant until postinvoke receives it.
      return servant.release ();
    }

  throw CORBA::OBJECT_NOT_EXIST ();
}

void
ServantLocator::postinvoke (PortableServer::ObjectId const & /* oid */,
                            PortableServer::POA_ptr /* poa */,
                            char const * /* operation */,
                            PortableServer::ServantLocator::Cookie cookie,
                            PortableServer::Servant servant)
{
  // Check the passed servant with the cookie.

  PortableServer::Servant const my_servant =
    static_cast<PortableServer::Servant> (cookie);
  ACE_ASSERT (servant == my_servant);
  PortableServer::ServantBase_var servant_owner (servant);

  // To avoid warning about unused variable with ACE_NDEBUG.
  ACE_UNUSED_ARG (my_servant);
}
