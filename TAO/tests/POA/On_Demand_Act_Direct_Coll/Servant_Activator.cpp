
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

ServantActivator::ServantActivator (CORBA::ORB_ptr orb,
                                    ACE_thread_t thrid)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    thr_id_ (thrid)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("ServantActivator::ServantActivator\n")));
}

PortableServer::Servant
ServantActivator::incarnate (PortableServer::ObjectId const &oid,
                             PortableServer::POA_ptr poa)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("ServantActivator::incarnate\n")));
  // Convert ObjectId to String.
  CORBA::String_var const s = PortableServer::ObjectId_to_string (oid);

  // If ObjectId string has a test substring, create and return a
  // test_i.

  if (std::strstr (s.in (), "test") == nullptr)
    throw CORBA::OBJECT_NOT_EXIST ();
  return new test_i (this->orb_.in (), poa, this->thr_id_);
}

void
ServantActivator::etherealize (PortableServer::ObjectId const &,
                               PortableServer::POA_ptr,
                               PortableServer::Servant servant,
                               CORBA::Boolean,
                               CORBA::Boolean remaining_activations)
{
  ACE_DEBUG ((LM_DEBUG,
               ACE_TEXT ("ServantActivator::etherealize\n")));
  // If there are no remaining activations i.e ObjectIds associated
  // with test_i delete it.
  if (!remaining_activations)
    delete servant;
}
