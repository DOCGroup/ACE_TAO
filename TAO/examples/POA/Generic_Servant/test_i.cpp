// $Id$

#include "test_i.h"

ACE_RCSID(Generic_Servant, test_i, "$Id$")

// Constructor
test_i::test_i (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

PortableServer::POA_ptr
test_i::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

void
test_i::method (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
test_i::oneway_method (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
test_i::sleep (CORBA::ULong timeout,
               const char *method)
{
  ACE_DEBUG ((LM_DEBUG,
              "Thread %t starting %s with timeout %d\n",
              method,
              timeout));

  ACE_OS::sleep (timeout);

  ACE_DEBUG ((LM_DEBUG,
              "Thread %t completes %s\n",
              method));
}

void
test_i::timed_method (CORBA::ULong timeout
                      ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->sleep (timeout,
               "test_i::timed_method");
}

void
test_i::timed_oneway_method (CORBA::ULong timeout
                             ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->sleep (timeout,
               "test_i::timed_oneway_method");
}

void
test_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->orb_->shutdown (0
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


// This is the point of entry into this library.
extern "C" GENERIC_SERVANT_Export PortableServer::Servant create_test_i (CORBA::ORB_ptr orb,
                                                                         PortableServer::POA_ptr poa);

PortableServer::Servant
create_test_i (CORBA::ORB_ptr orb,
               PortableServer::POA_ptr poa)
{
  PortableServer::Servant servant;

  ACE_NEW_RETURN (servant,
                  test_i (orb,
                          poa),
                  0);
  return servant;
}


extern "C" GENERIC_SERVANT_Export PortableServer::Servant supply_servant (const PortableServer::ObjectId &oid,
                                                                          PortableServer::POA_ptr poa,
                                                                          CORBA::ORB_ptr orb);

PortableServer::Servant
supply_servant (const PortableServer::ObjectId &oid,
                PortableServer::POA_ptr poa,
                CORBA::ORB_ptr orb)
{
  PortableServer::Servant servant = 0;

  // Convert ObjectId to string.
  CORBA::String_var s = PortableServer::ObjectId_to_string (oid);

  if (ACE_OS::strstr (s.in (), "test") != 0)
    ACE_NEW_RETURN (servant,
                    test_i (orb,
                            poa),
                    0);
  return servant;
}

extern "C" GENERIC_SERVANT_Export void destroy_servant (const PortableServer::ObjectId &oid,
                                                        PortableServer::POA_ptr poa,
                                                        PortableServer::Servant servant);

void
destroy_servant (const PortableServer::ObjectId & /* oid */,
                 PortableServer::POA_ptr /* poa */,
                 PortableServer::Servant servant)
{
  delete servant;
}
