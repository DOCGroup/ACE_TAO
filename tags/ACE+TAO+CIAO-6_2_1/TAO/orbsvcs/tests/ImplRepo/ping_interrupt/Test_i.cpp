/* -*- C++ -*- $Id$ */

#include "Test_i.h"
#include "ace/OS_NS_unistd.h"

Test_i::Test_i (CORBA::Short server_num)
  : server_num_ (server_num)
{
}

Test_i::~Test_i ()
{
}

CORBA::Short
Test_i::get_server_num (const CORBA::Short delay_secs)
{
  ACE_OS::sleep (delay_secs);
  return this->server_num_;
}

