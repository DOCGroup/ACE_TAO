// -*- C++ -*-
// $Id$

#include "test_i.h"

test_i::test_i (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
test_i::client_test (Test::TestScenario scenario)
{
  switch (scenario)
    {
    case 1:
    case 4:
      // Both the client-side and server-side tests throw this
      // exception in these scenarios.
      throw Test::X ();  // Expected test exception
    case 2:
    case 3:
      break;
    default:
      throw Test::UnknownScenario (scenario);  // Unexpected!
    }
}

void
test_i::server_test (Test::TestScenario scenario,
                     CORBA::ULongSeq_out myseq)
{
  myseq = new CORBA::ULongSeq;

  switch (scenario)
    {
    case 1:
    case 4:
      // Both the client-side and server-side tests throw this
      // exception in these scenarios.
      throw Test::X ();  // Expected test exception
    case 2:
    case 3:
      break;
    default:
      throw Test::UnknownScenario (scenario);  // Unexpected!
    }
}

void
test_i::shutdown (void)
{
  ACE_DEBUG ((LM_INFO,
              "Server is shutting down.\n"));

  this->orb_->shutdown (0);
}
