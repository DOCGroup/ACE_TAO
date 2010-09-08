// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO/tests/POA/On_Demand_Activation/Servant_Activator
//
// = FILENAME
//     Servant_Activator.cpp
//
// = DESCRIPTION
//     Implementation of ServantActivator, which is used by a POA with
//     a RETAIN policy.
//
// = AUTHOR
//     Irfan Pyarali
//
// ============================================================================

#include "Servant_Activator.h"
#include "test_i.h"
#include "ace/OS_NS_string.h"

ServantActivator::ServantActivator (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

PortableServer::Servant
ServantActivator::incarnate (const PortableServer::ObjectId &oid,
                             PortableServer::POA_ptr poa)
{
  // Convert ObjectId to String.

  CORBA::String_var s = PortableServer::ObjectId_to_string (oid);

  // If ObjectId string has a test substring, create and return a
  // test_i.

  if (ACE_OS::strstr (s.in (), "test") != 0)
    return new test_i (this->orb_.in (), poa);
  else
    {
      throw CORBA::OBJECT_NOT_EXIST ();
    }
}


void
ServantActivator::etherealize (const PortableServer::ObjectId &,
                               PortableServer::POA_ptr ,
                               PortableServer::Servant servant,
                               CORBA::Boolean ,
                               CORBA::Boolean remaining_activations)
{
  // If there are no remaining activations i.e ObjectIds associated
  // with test_i delete it.
  if (!remaining_activations)
    delete servant;
}
