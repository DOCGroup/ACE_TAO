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
                                  PortableServer::POA_ptr poa,
                                  CORBA::Environment &ACE_TRY_ENV)
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
      //CORBA::Exception *exception = new CORBA::OBJECT_NOT_EXIST (CORBA::COMPLETED_NO);
      //env.exception (exception);
      //return 0;
    }
}


void
MyFooServantActivator::etherealize (const PortableServer::ObjectId &,
                                    PortableServer::POA_ptr ,
                                    PortableServer::Servant servant,
                                    CORBA::Boolean ,
                                    CORBA::Boolean remaining_activations,
                                    CORBA::Environment &)
{
  
  // If there are no remaining activations i.e ObjectIds associated
  // with MyFooServant delete it.

  if (remaining_activations == 0)
    delete servant;
}
