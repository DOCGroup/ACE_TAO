/* -*- C++ -*- $Id$ */

#include "Test_i.h"
#include "ace/OS_NS_unistd.h"

/* static */ bool Test_i::expected_requests_made_ = false;

Test_i::Test_i (CORBA::Short num_requests_expected)
  : num_requests_expected_ (num_requests_expected)
  , num_requests_made_ (0)
{
}

Test_i::~Test_i ()
{
  if (this->num_requests_expected_ == this->num_requests_made_)
    Test_i::expected_requests_made_ = true;
}

CORBA::Short
Test_i::get_num_requests (const CORBA::Short delay_secs)
{
  ACE_OS::sleep (delay_secs);
  return ++this->num_requests_made_;
}

bool
Test_i::expected_requests_made ()
{
  return Test_i::expected_requests_made_;
}
