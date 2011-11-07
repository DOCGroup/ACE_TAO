//
// $Id: TestCallback.cpp
//
#include "TestCallback.h"
#include "ace/OS_NS_unistd.h"

TestCallback::TestCallback (CORBA::ORB_ptr orb)
  : work_ (orb)
{
}

void
TestCallback::start_threads_and_block (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - start_threads_and_block\n"));
  work_.activate(0,10);
  ACE_OS::sleep(-1);
}

void
TestCallback::test_call (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - test_call\n"));
}

void
TestCallback::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) server - test completed\n"));
  exit (0);
}
