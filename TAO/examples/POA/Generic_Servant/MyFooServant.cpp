// $Id$

// ================================================================
//
//
// = FILENAME
//     MyFooServant.cpp
//
// = DESCRIPTION
//     This is a simple foo servant implementation
//
// = AUTHOR
//     Irfan Pyarali
//
// ================================================================
#define ACE_BUILD_SVC_EXPORT

#include "MyFooServant.h"

ACE_RCSID(Generic_Servant, MyFooServant, "$Id$")

// Constructor
MyFooServant::MyFooServant (CORBA::ORB_ptr orb,
			    PortableServer::POA_ptr poa,
                            CORBA::Long value)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    value_ (value)
{
}

// Destructor
MyFooServant::~MyFooServant (void)
{
}

// Return the Default POA of this Servant
PortableServer::POA_ptr
MyFooServant::_default_POA (CORBA::Environment &/*env*/)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

CORBA::Long
MyFooServant::doit (CORBA::Environment &/*env*/)
{
  return this->value_;
}

void
MyFooServant::simply_doit (CORBA::Environment &/*env*/)
{
}

void
MyFooServant::timed_operation (CORBA::ULong timeout,
                               CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "Thread %t starting  MyFooServant::timed_operation with timeout %d\n",
              timeout));

  ACE_OS::sleep (timeout);

  ACE_DEBUG ((LM_DEBUG,
              "Thread %t completes MyFooServant::timed_operation\n"));
}

void
MyFooServant::shutdown (CORBA::Environment &)
{
  this->orb_->shutdown ();
}


// This is the point of entry into this library.

extern "C" GENERIC_SERVANT_Export PortableServer::Servant create_MyFoo (CORBA::ORB_ptr orb,
                                                                        PortableServer::POA_ptr poa,
                                                                        CORBA::Long value);

// The servant pointer is returned which will be of Base class
// type. The binding to the MyFoo servant will happen at run-time.

PortableServer::Servant
create_MyFoo (CORBA::ORB_ptr orb,
              PortableServer::POA_ptr poa,
              CORBA::Long value)
{
  PortableServer::Servant servant;

  ACE_NEW_RETURN (servant,
                  MyFooServant (orb,
                                poa,
                                value),
                  0);
  return servant;
}


extern "C" GENERIC_SERVANT_Export PortableServer::Servant supply_servant (const PortableServer::ObjectId &oid,
                                                                          PortableServer::POA_ptr poa,
                                                                          CORBA::ORB_ptr orb);

// The servant pointer is returned which will be of Base class
// type. The binding to the servant will happen at run-time.

PortableServer::Servant
supply_servant (const PortableServer::ObjectId &oid,
                PortableServer::POA_ptr poa,
                CORBA::ORB_ptr orb)
{
  PortableServer::Servant servant = 0;

  // Convert ObjectId to String.
  CORBA::String_var s = PortableServer::ObjectId_to_string (oid);

  if (ACE_OS::strstr (s.in (), "Foo") != 0)
    ACE_NEW_RETURN (servant,
                    MyFooServant (orb,
                                  poa,
                                  0),
                    0);
  return servant;
}

extern "C" GENERIC_SERVANT_Export void destroy_servant (const PortableServer::ObjectId &oid,
                                                        PortableServer::POA_ptr poa,
                                                        PortableServer::Servant servant);

// This function will take care of the destruction of the servant.

void
destroy_servant (const PortableServer::ObjectId & /* oid */,
                PortableServer::POA_ptr /* poa */,
                 PortableServer::Servant servant)
{
  delete servant;
}
