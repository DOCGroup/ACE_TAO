
//=============================================================================
/**
 *  @file     Servant_Activator.cpp
 *
 *   Implementation of ServantActivator, which is used by a POA with
 *   a RETAIN policy.
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================

#include "Servant_Activator.h"
#include "test_i.h"

#include <cstring>
#include <memory>

ServantActivator::ServantActivator (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

PortableServer::Servant
ServantActivator::incarnate (PortableServer::ObjectId const &oid,
                             PortableServer::POA_ptr poa)
{
  // Convert ObjectId to String.

  CORBA::String_var s = PortableServer::ObjectId_to_string (oid);

  // If ObjectId string has a test substring, create and return a
  // test_i.

  if (std::strstr (s.in (), "test") != nullptr)
    {
      std::unique_ptr<test_i> servant (new test_i (this->orb_.in (), poa));
      // The POA takes ownership of the newly created servant.
      return servant.release ();
    }

  throw CORBA::OBJECT_NOT_EXIST ();
}

void
ServantActivator::etherealize (PortableServer::ObjectId const &,
                               PortableServer::POA_ptr,
                               PortableServer::Servant servant,
                               CORBA::Boolean,
                               CORBA::Boolean remaining_activations)
{
  // If there are no remaining activations i.e ObjectIds associated
  // with test_i delete it.
  if (!remaining_activations)
    {
      PortableServer::ServantBase_var servant_owner (servant);
    }
}
