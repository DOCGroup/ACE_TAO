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
  work_.activate(THR_NEW_LWP | THR_JOINABLE | THR_INHERIT_SCHED,10);
  work_.wait (); // blocks forever as the worker threads never end
}

void
TestCallback::test_call (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - test_call\n"));
}

void
TestCallback::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) client - test completed\n"));
  exit (0);
}
