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
//     Implementation of MyFooServantActivator , which is used by a
//     POA with a RETAIN policy.
//
// = AUTHOR
//     Irfan Pyarali
//
// ============================================================================

#include "Servant_Activator.h"
#include "MyFooServant.h"

ACE_RCSID(On_Demand_Activation, Servant_Activator, "$Id$")

MyFooServantActivator::MyFooServantActivator (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{
}

PortableServer::Servant
MyFooServantActivator::incarnate (const PortableServer::ObjectId &oid,
                                  PortableServer::POA_ptr poa
                                  TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableServer::ForwardRequest))
{
  // Convert ObjectId to String.

  CORBA::String_var s = PortableServer::ObjectId_to_string (oid);

  // If ObjectId string has a Foo Substring, create and return a
  // MyFooServant.

  if (ACE_OS::strstr (s.in (), "Foo") != 0)
    return new MyFooServant (this->orb_.in (), poa, 27);
  else
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (), 0);
    }
}


void
MyFooServantActivator::etherealize (const PortableServer::ObjectId &,
                                    PortableServer::POA_ptr ,
                                    PortableServer::Servant servant,
                                    CORBA::Boolean ,
                                    CORBA::Boolean remaining_activations
                                    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // If there are no remaining activations i.e ObjectIds associated
  // with MyFooServant delete it.

  if (remaining_activations == 0)
    delete servant;
}
