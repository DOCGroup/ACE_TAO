/* -*- C++ -*- $Id$ */

#include "Test_i.h"
#include "ace/OS_NS_unistd.h"

Test_i::Test_i (CORBA::Short server_num,
                CORBA::Short reply_delay_secs)
  : server_num_ (server_num)
  , reply_delay_secs_(reply_delay_secs)
{
}

Test_i::~Test_i ()
{
}

CORBA::Short
Test_i::get_server_num ()
{
  ACE_DEBUG((LM_DEBUG,
             "Server %d Test_i::get_server_num () sleeping for %d seconds\n",
             this->server_num_, this->reply_delay_secs_));
  ACE_OS::sleep (this->reply_delay_secs_);
  return this->server_num_;
}
