/* -*- C++ -*- $Id$ */

#include "Test_i.h"
#include "ace/OS_NS_unistd.h"

Test_i::Test_i (void)
  : count_ (0)
{
}

Test_i::~Test_i ()
{
}

void
Test_i::foo (void)
{
  ACE_DEBUG ((LM_DEBUG,"foo called\n"));
  ++this->count_;
}

CORBA::Short
Test_i::get_call_count (void)
{
  CORBA::Short r = this->count_;
  this->count_ = 0;
  return r;
}



