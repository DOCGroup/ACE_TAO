/* -*- C++ -*- $Id$ */

#include "Test_i.h"
#include "Terminator.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Log_Msg.h"

Test_i::Test_i (CORBA::Short server_num, Terminator &terminator)
  : server_num_ (server_num)
  , terminator_ (terminator)
  , armed_ (false)
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
Test_i::get_server_num (const CORBA::Short delay_secs)
{
  ACE_OS::sleep (delay_secs);
  return this->server_num_;
}

void
Test_i::abort (CORBA::Short delay_secs)
{
  ACE_Message_Block *mb = 0;
  ACE_NEW(mb, ACE_Message_Block(2));
  ACE_OS::sprintf(mb->wr_ptr (), "%d", delay_secs);
  terminator_.putq(mb);
}

void
Test_i::arm (void)
{
  this->armed_ = true;
}

void
Test_i::trigger (void)
{
  if (this->armed_)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P) BOOM!\n")));
      ACE_OS::exit (1);
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P) Click! not armed, returning\n")));
}
