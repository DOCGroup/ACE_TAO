// -*- C++ -*-

#include "test_i.h"

ACE_RCSID (Request_Interceptor_Flow,
           test_i,
           "$Id$")

test_i::test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
test_i::client_test (Test::TestScenario scenario
                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((Test::X,
                   Test::UnknownScenario,
                   CORBA::SystemException))
{
  switch (scenario)
    {
    case 1:
    case 4:
      // Both the client-side and server-side tests throw this
      // exception in these scenarios.
      ACE_THROW (Test::X ());  // Expected test exception
    case 2:
    case 3:
      break;
    default:
      ACE_THROW (Test::UnknownScenario (scenario));  // Unexpected!
    }
}

void
test_i::server_test (Test::TestScenario scenario
                     ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((Test::X,
                   Test::UnknownScenario,
                   CORBA::SystemException))
{
  switch (scenario)
    {
    case 1:
    case 4:
      // Both the client-side and server-side tests throw this
      // exception in these scenarios.
      ACE_THROW (Test::X ());  // Expected test exception
    case 2:
    case 3:
      break;
    default:
      ACE_THROW (Test::UnknownScenario (scenario));  // Unexpected!
    }
}

void
test_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_INFO,
              "Server is shutting down.\n"));

  this->orb_->shutdown (0 ACE_ENV_ARG_PARAMETER);
}
