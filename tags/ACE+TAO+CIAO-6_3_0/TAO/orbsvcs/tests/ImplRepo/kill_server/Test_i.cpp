/* -*- C++ -*- $Id$ */

#include "Test_i.h"
#include "Terminator.h"
#include "ace/OS_NS_unistd.h"

Test_i::Test_i (CORBA::Short server_num,
                Terminator &terminator,
                PortableServer::POA_ptr pa,
                PortableServer::POA_ptr pb)
  : mgr_a (pa->the_POAManager ()),
    mgr_b (pb->the_POAManager ()),
    server_num_ (server_num),
    terminator_ (terminator)
{
}

Test_i::~Test_i ()
{
  ACE_Message_Block *mb = 0;
  ACE_NEW (mb,
           ACE_Message_Block(0,
                             ACE_Message_Block::MB_HANGUP));
  terminator_.putq (mb);
  terminator_.wait ();
}

CORBA::Short
Test_i::get_server_num (void)
{
  return this->server_num_;
}



