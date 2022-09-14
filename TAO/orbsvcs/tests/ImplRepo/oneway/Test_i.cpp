/* -*- C++ -*-  */

#include "Test_i.h"
#include "ace/OS_NS_unistd.h"

Test_i::Test_i ()
  : count_ (0)
{
}

Test_i::~Test_i ()
{
}

void
Test_i::foo ()
{
  ACE_DEBUG ((LM_DEBUG,"foo called\n"));
  ++this->count_;
}

CORBA::Short
Test_i::get_call_count ()
{
  CORBA::Short r = this->count_;
  this->count_ = 0;
  return r;
}


